// ============================================================
// PERSISTENCE — App settings & roll state (NVS)
// ============================================================

void loadAppSettings() {
  prefs.begin("cfg", true);
  digitalMode  = prefs.getBool("digital", false);
  flashEnabled = prefs.getBool("flash", true);
  apSsid       = prefs.getString("ssid", DEFAULT_AP_SSID);
  apPassword   = prefs.getString("pass", DEFAULT_AP_PASSWORD);
  prefs.end();

  if (apSsid.length() == 0) apSsid = DEFAULT_AP_SSID;
  if (apSsid.length() > 32) apSsid = apSsid.substring(0, 32);
  if (!(apPassword.length() == 0 || apPassword.length() >= 8)) apPassword = DEFAULT_AP_PASSWORD;
  if (apPassword.length() > 63) apPassword = apPassword.substring(0, 63);
}

void saveAppSettings() {
  prefs.begin("cfg", false);
  prefs.putBool("digital", digitalMode);
  prefs.putBool("flash",   flashEnabled);
  prefs.putString("ssid", apSsid);
  prefs.putString("pass", apPassword);
  prefs.end();
}

void saveRollState() {
  prefs.begin("roll", false);
  prefs.putString("film",  rollFilm);
  prefs.putInt("frames",   rollFrames);
  prefs.putBool("developed", rollDeveloped);
  prefs.putInt("count",    rollCount);
  prefs.end();
}

void loadRollState() {
  prefs.begin("roll", true);
  rollFilm      = prefs.getString("film", "");
  rollFrames    = prefs.getInt("frames", 0);
  rollDeveloped = prefs.getBool("developed", false);
  rollCount     = prefs.getInt("count", 0);
  prefs.end();
  if (rollFilm.length() > 0) {
    Serial.printf("Restored roll: %s, %d/%d frames, %s (rolls: %d)\n",
      rollFilm.c_str(), rollFrames, ROLL_SIZE,
      rollDeveloped ? "developed" : "shooting", rollCount);
  }
}
