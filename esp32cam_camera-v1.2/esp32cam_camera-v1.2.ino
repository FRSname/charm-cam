/**
 * Charm-Cam — ESP32-CAM Point & Shoot Camera  v1.2
 * Hardware: AI-Thinker ESP32-CAM + OV2640 + Baseboard
 * Features: Shutter button, SD card save, WiFi web server,
 *           analog/digital modes, OTA updates
 *
 * File layout:
 *   config.h        — Pin definitions, constants
 *   html_main.h     — Main UI (PROGMEM)
 *   html_ota.h      — OTA update page (PROGMEM)
 *   persistence.ino  — NVS load / save (settings + roll state)
 *   led.ino          — Status & flash LED helpers
 *   camera.ino       — OV2640 camera init
 *   storage.ino      — SD card, file ops, roll logic, capture
 *   handlers.ino     — HTTP request handlers & route registration
 */

#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"
#include "SD_MMC.h"
#include "WiFi.h"
#include "WebServer.h"
#include <Preferences.h>
#include <Update.h>

#include "config.h"
#include "html_main.h"
#include "html_ota.h"

// ============================================================
// FORWARD DECLARATIONS
// Required because Arduino IDE's auto-prototype generation fails
// for functions with default parameters defined in other .ino files.
// ============================================================

// led.ino
void blinkStep(int count, int onMs = 120, int offMs = 120);
void blinkStatusWarning(int count = 3, int onMs = 100, int offMs = 100);
void blinkFlashFast(int count = 3, int onMs = 80, int offMs = 80);
void signalNewRollLoaded();

// persistence.ino
void loadAppSettings();
void saveAppSettings();
void saveRollState();
void loadRollState();

// camera.ino
bool initCamera();

// storage.ino
bool initSD();
void startRoll(const String& film);
bool developCurrentRoll(String& rollDirName, String& developedJson, String& error);
String captureAndSave();

// handlers.ino
void registerRoutes();

// ============================================================
// GLOBALS
// ============================================================
WebServer server(80);
Preferences prefs;

// Hardware state
int  photoCount       = 0;
bool sdCardOK         = false;
bool wifiConnected    = false;

// Shutter button state
unsigned long lastShutterTime   = 0;
unsigned long shutterPressStart = 0;
bool longPressHandled = false;

// App settings (persisted in NVS)
bool   digitalMode  = false;
bool   flashEnabled = true;
String apSsid      = DEFAULT_AP_SSID;
String apPassword   = DEFAULT_AP_PASSWORD;

// Film roll state (persisted in NVS)
String rollFilm      = "";
bool   rollDeveloped = false;
int    rollFrames    = 0;
int    rollCount     = 0;

// ============================================================
// SETUP
// ============================================================
void setup() {
  // GPIO
  pinMode(LED_FLASH_PIN, OUTPUT);
  digitalWrite(LED_FLASH_PIN, LOW);
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, HIGH);
  pinMode(SHUTTER_BTN_PIN, INPUT_PULLUP);

  blinkStep(1, 250, 250);  // setup entered

  Serial.begin(115200);
  delay(500);
  Serial.println("\n\nCharm-Cam starting...");
  blinkStep(2);            // serial ready

  // Load persisted settings
  loadAppSettings();
  Serial.printf("Mode: %s | AP: %s\n", digitalMode ? "digital" : "analog", apSsid.c_str());

  // WiFi AP — start immediately so it's always reachable
  WiFi.mode(WIFI_AP);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);  // lower power = less brownout risk
  Serial.println(">> STEP 1: WiFi AP...");
  if (apPassword.length() == 0) WiFi.softAP(apSsid.c_str());
  else WiFi.softAP(apSsid.c_str(), apPassword.c_str());
  IPAddress ip = WiFi.softAPIP();
  Serial.printf("Hotspot: %s — connect and go to http://%s\n", apSsid.c_str(), ip.toString().c_str());
  wifiConnected = true;
  Serial.println(">> STEP 1 OK");
  blinkStep(3);            // WiFi AP started

  // Camera
  Serial.println(">> STEP 2: Camera...");
  if (!initCamera()) {
    Serial.println("WARNING: Camera init failed — continuing without camera");
  }
  Serial.println(">> STEP 2 OK");
  blinkStep(4);            // camera init finished

  // SD card
  Serial.println(">> STEP 3: SD...");
  sdCardOK = initSD();
  loadRollState();
  Serial.println(">> STEP 3 OK");
  blinkStep(5);            // SD init finished

  // Web server
  Serial.println(">> STEP 4: Web server...");
  registerRoutes();
  server.begin();
  Serial.println(">> STEP 4 OK");
  blinkStep(6);            // web server started

  // Ready signal — 3x flash blink
  blinkFlashFast(3, 80, 80);

  Serial.println("Ready. Press shutter button to capture.");
}

// ============================================================
// LOOP
// ============================================================
void loop() {
  if (wifiConnected) server.handleClient();

  // Physical shutter button (active LOW with pull-up)
  bool pressed = (digitalRead(SHUTTER_BTN_PIN) == LOW);
  unsigned long now = millis();

  if (pressed) {
    if (shutterPressStart == 0) {
      // Record the moment the button went down
      shutterPressStart = now;
      longPressHandled  = false;
    }
  } else if (shutterPressStart > 0) {
    // Button released — decide short vs long press
    unsigned long pressDuration = now - shutterPressStart;
    shutterPressStart = 0;

    // Ignore bounces shorter than DEBOUNCE_MS
    if (pressDuration < DEBOUNCE_MS) {
      longPressHandled = false;
      return;
    }

    if (!digitalMode && pressDuration >= LONG_PRESS_MS) {
      // ── LONG PRESS: develop current roll and reload same film,
      //               OR load a random film if no roll is loaded ──
      Serial.println("Long press detected!");
      lastShutterTime = now;
      if (rollFilm == "") {
        // No roll loaded — load a random one
        static const char* const FILM_IDS[] = {
          "natural", "kodak", "ilford", "velvia",
          "faded",   "cyano", "redscale", "xpro"
        };
        const int FILM_COUNT = sizeof(FILM_IDS) / sizeof(FILM_IDS[0]);
        String pick = FILM_IDS[esp_random() % FILM_COUNT];
        startRoll(pick);
        Serial.printf("Long press: no roll loaded, started random %s roll\n", pick.c_str());
      } else if (!rollDeveloped && rollFrames > 0) {
        String previousFilm = rollFilm;
        String rollDirName, developed, error;
        if (developCurrentRoll(rollDirName, developed, error)) {
          startRoll(previousFilm);
          Serial.printf("Long press: developed roll, loaded new %s roll\n", previousFilm.c_str());
        } else {
          Serial.printf("Long press develop failed: %s\n", error.c_str());
          blinkStatusWarning(3, 100, 100);
        }
      } else {
        // Nothing to develop — warn with LED
        blinkStatusWarning(3, 100, 100);
      }
    } else {
      // ── SHORT PRESS: capture ──
      if (now - lastShutterTime >= DEBOUNCE_MS) {
        lastShutterTime = now;
        Serial.println("Shutter pressed!");
        if (!digitalMode && rollFilm == "") {
          blinkStatusWarning(3, 100, 100);   // no film loaded warning
        } else {
          captureAndSave();
        }
      }
    }

    longPressHandled = false;
  }
}
