/**
 * ESP32-CAM Diagnostic Test
 * 
 * This MINIMAL sketch does only 2 things:
 *   1) Blinks the red LED on GPIO 33
 *   2) Starts a WiFi AP called "ESP32-DIAG"
 * 
 * Flash this first. If LED blinks and AP appears, the board is fine.
 * If not, the issue is board selection or upload settings in Arduino IDE.
 * 
 * Arduino IDE settings for AI-Thinker ESP32-CAM:
 *   Board:           "AI Thinker ESP32-CAM"
 *   Partition Scheme: "Huge APP (3MB No OTA / 1MB SPIFFS)"
 *   Upload Speed:     115200
 *   Flash Mode:       QIO
 *   Flash Frequency:  80MHz
 */

#include "WiFi.h"

#define LED_PIN 33  // Red onboard LED, active LOW

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n=== DIAG TEST START ===");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // off

  // Blink 10 times so it's obvious
  Serial.println("Blinking LED...");
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_PIN, LOW);   // ON
    delay(200);
    digitalWrite(LED_PIN, HIGH);  // OFF
    delay(200);
  }
  Serial.println("Blink done.");

  // Start AP
  Serial.println("Starting AP...");
  WiFi.softAP("ESP32-DIAG", "12345678");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("=== DIAG TEST READY ===");
}

void loop() {
  // Slow heartbeat blink
  digitalWrite(LED_PIN, LOW);
  delay(100);
  digitalWrite(LED_PIN, HIGH);
  delay(2000);
}
