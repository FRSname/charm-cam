#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// CONFIGURATION — edit these defaults as needed
// ============================================================

// Default Access Point credentials
#define DEFAULT_AP_SSID     "Charm-Cam"
#define DEFAULT_AP_PASSWORD "camera123"    // min 8 chars, or "" for open

// GPIO pins (AI-Thinker ESP32-CAM pinout)
#define SHUTTER_BTN_PIN  GPIO_NUM_13      // Button between GPIO13 and GND
#define LED_FLASH_PIN    GPIO_NUM_4       // Onboard flash LED
#define STATUS_LED_PIN   GPIO_NUM_33      // Onboard red status LED (active LOW)

// Camera model: AI-THINKER
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM       5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

// Roll / button constants
#define ROLL_SIZE       36
#define DEBOUNCE_MS     50      // ignore bounces shorter than this (ms)
#define LONG_PRESS_MS   2000    // hold >= this to trigger develop

// JPEG save rotation (EXIF orientation tag injected at save time — no decode overhead)
//   1 = no rotation (default)
//   3 = 180°
//   6 = 90° CW  (rotate right)
//   8 = 90° CCW (rotate left)
#define CAPTURE_ROTATION  6

#endif
