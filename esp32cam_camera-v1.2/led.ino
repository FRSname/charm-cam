// ============================================================
// LED HELPERS
// ============================================================

void blinkStep(int count, int onMs, int offMs) {
  for (int i = 0; i < count; i++) {
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(onMs);
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(offMs);
  }
  delay(250);
}

void blinkStatusWarning(int count, int onMs, int offMs) {
  for (int i = 0; i < count; i++) {
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(onMs);
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(offMs);
  }
}

void blinkFlashFast(int count, int onMs, int offMs) {
  for (int i = 0; i < count; i++) {
    digitalWrite(LED_FLASH_PIN, HIGH);
    delay(onMs);
    digitalWrite(LED_FLASH_PIN, LOW);
    delay(offMs);
  }
}

void signalNewRollLoaded() {
  digitalWrite(LED_FLASH_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_FLASH_PIN, LOW);
}
