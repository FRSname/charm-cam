// ============================================================
// HTTP HANDLERS
// ============================================================

void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleUpdatePage() {
  server.send_P(200, "text/html", OTA_HTML);
}

void handleUpdateUpload() {
  HTTPUpload& upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("OTA update start: %s\n", upload.filename.c_str());
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
      Serial.printf("OTA begin failed, error=%d\n", Update.getError());
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Serial.printf("OTA write failed, error=%d\n", Update.getError());
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) {
      Serial.printf("OTA update success: %u bytes\n", upload.totalSize);
    } else {
      Serial.printf("OTA end failed, error=%d\n", Update.getError());
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_ABORTED) {
    Update.end();
    Serial.println("OTA update aborted");
  }
  delay(0);
}

void handleCapture() {
  if (!digitalMode) {
    if (rollFilm == "")          { server.send(400, "application/json", "{\"ok\":false,\"error\":\"no_roll\"}");  return; }
    if (rollDeveloped)           { server.send(400, "application/json", "{\"ok\":false,\"error\":\"developed\"}"); return; }
    if (rollFrames >= ROLL_SIZE) { server.send(400, "application/json", "{\"ok\":false,\"error\":\"full\"}");      return; }
  }
  String path = captureAndSave();
  if (path.length() > 0) {
    int frames = digitalMode ? photoCount : rollFrames;
    int total  = digitalMode ? 0 : ROLL_SIZE;
    String mode = digitalMode ? "digital" : "analog";
    String resp = "{\"ok\":true,\"path\":\"" + path + "\",\"frames\":" + frames
                + ",\"total\":" + total + ",\"mode\":\"" + mode + "\"}";
    server.send(200, "application/json", resp);
  } else {
    server.send(500, "application/json", "{\"ok\":false,\"error\":\"capture_failed\"}");
  }
}

void handleList() {
  if (!sdCardOK) { server.send(200, "application/json", "[]"); return; }
  String dirPath = "/photos";
  if (server.hasArg("dir")) {
    dirPath = "/photos/" + sanitizePath(server.arg("dir"));
  }
  File dir = SD_MMC.open(dirPath);
  String json = "[";
  bool first = true;
  if (dir) {
    File file = dir.openNextFile();
    while (file) {
      if (!file.isDirectory()) {
        if (!first) json += ",";
        json += "{\"name\":\"" + String(file.name()) + "\"}";
        first = false;
      }
      file = dir.openNextFile();
    }
  }
  json += "]";
  server.send(200, "application/json", json);
}

void handlePhoto() {
  if (!sdCardOK || !server.hasArg("name")) { server.send(404, "text/plain", "Not found"); return; }
  String name = sanitizePath(server.arg("name"));

  String path;
  if (server.hasArg("dir")) {
    path = "/photos/" + sanitizePath(server.arg("dir")) + "/" + name;
  } else {
    path = "/photos/" + name;
  }

  File file = SD_MMC.open(path, FILE_READ);
  if (!file) {
    path = "/roll/" + name;
    file = SD_MMC.open(path, FILE_READ);
  }
  if (!file) { server.send(404, "text/plain", "File not found"); return; }

  if (server.hasArg("dl"))
    server.sendHeader("Content-Disposition", "attachment; filename=" + name);
  server.streamFile(file, "image/jpeg");
  file.close();
}

void handleSetRoll() {
  if (digitalMode) { server.send(200, "application/json", "{\"ok\":true,\"digital\":true}"); return; }
  if (!server.hasArg("film")) { server.send(400, "application/json", "{\"ok\":false}"); return; }
  startRoll(server.arg("film"));
  Serial.printf("Film loaded: %s\n", rollFilm.c_str());
  server.send(200, "application/json", "{\"ok\":true}");
}

void handleDevelop() {
  String rollDirName, developed, error;
  if (!developCurrentRoll(rollDirName, developed, error)) {
    server.send(error == "no_sd" ? 500 : 400, "application/json",
      "{\"ok\":false,\"error\":\"" + error + "\"}");
    return;
  }
  server.send(200, "application/json",
    "{\"ok\":true,\"film\":\"" + rollFilm + "\",\"dir\":\"" + rollDirName
    + "\",\"photos\":" + developed + "}");
}

void handleStatus() {
  String lastDir = "";
  if (rollCount > 0 && rollDeveloped) {
    lastDir = "r" + String(rollCount < 10 ? "0" : "") + String(rollCount) + "_" + rollFilm;
  }
  String resp = "{\"film\":\"" + (digitalMode ? String("digital") : rollFilm)
              + "\",\"frames\":" + (digitalMode ? String(photoCount) : String(rollFrames))
              + ",\"total\":" + (digitalMode ? String(0) : String(ROLL_SIZE))
              + ",\"developed\":" + (digitalMode ? "false" : (rollDeveloped ? "true" : "false"))
              + ",\"rollCount\":" + rollCount
              + ",\"mode\":\"" + (digitalMode ? String("digital") : String("analog")) + "\""
              + ",\"lastDir\":\"" + lastDir + "\"}";
  server.send(200, "application/json", resp);
}

void handleSettingsGet() {
  String resp = "{\"mode\":\"" + String(digitalMode ? "digital" : "analog") + "\""              + ",\"flash\":" + (flashEnabled ? "true" : "false")              + ",\"ssid\":\"" + apSsid + "\""
              + ",\"password\":\"" + apPassword + "\"}";
  server.send(200, "application/json", resp);
}

void handleSettingsSave() {
  String newMode  = server.hasArg("mode")     ? server.arg("mode")     : String(digitalMode ? "digital" : "analog");
  String newSsid  = server.hasArg("ssid")     ? server.arg("ssid")     : apSsid;
  String newPass  = server.hasArg("password") ? server.arg("password") : apPassword;
  bool   newFlash = server.hasArg("flash")    ? (server.arg("flash") == "1" || server.arg("flash") == "true") : flashEnabled;
  newSsid.trim();
  newPass.trim();
  newMode.toLowerCase();

  if (newSsid.length() == 0 || newSsid.length() > 32) {
    server.send(400, "application/json", "{\"ok\":false,\"error\":\"bad_ssid\"}");
    return;
  }
  if (!(newPass.length() == 0 || newPass.length() >= 8) || newPass.length() > 63) {
    server.send(400, "application/json", "{\"ok\":false,\"error\":\"bad_password\"}");
    return;
  }

  bool changed = (digitalMode != (newMode == "digital")) || (apSsid != newSsid) || (apPassword != newPass);
  digitalMode  = (newMode == "digital");
  flashEnabled = newFlash;
  apSsid       = newSsid;
  apPassword   = newPass;
  saveAppSettings();

  server.send(200, "application/json", "{\"ok\":true,\"reboot\":" + String(changed ? "true" : "false") + "}");
  if (changed) {
    delay(500);
    ESP.restart();
  }
}

void handleDelete() {
  if (!sdCardOK || server.method() != HTTP_POST || !server.hasArg("type")) {
    server.send(400, "application/json", "{\"ok\":false,\"error\":\"bad_request\"}");
    return;
  }

  String type = server.arg("type");
  bool ok = false;

  if (type == "photo" && server.hasArg("name")) {
    ok = deleteDevelopedPhoto(server.arg("name"), server.hasArg("dir") ? server.arg("dir") : String(""));
  } else if (type == "roll" && server.hasArg("dir")) {
    ok = deleteDevelopedRoll(server.arg("dir"));
  }

  if (!ok) {
    server.send(400, "application/json", "{\"ok\":false,\"error\":\"delete_failed\"}");
    return;
  }

  server.send(200, "application/json", "{\"ok\":true}");
}

void handleRename() {
  if (!sdCardOK || server.method() != HTTP_POST ||
      !server.hasArg("dir") || !server.hasArg("name")) {
    server.send(400, "application/json", "{\"ok\":false,\"error\":\"bad_request\"}");
    return;
  }
  String oldDir  = server.arg("dir");
  String rawName = server.arg("name");

  // Build safe name: alphanumeric, underscore, hyphen; spaces become underscores
  String safeName = "";
  for (int i = 0; i < (int)rawName.length() && (int)safeName.length() < 31; i++) {
    char c = rawName.charAt(i);
    if (isAlphaNumeric(c) || c == '_' || c == '-') safeName += c;
    else if (c == ' ') safeName += '_';
  }
  safeName.toLowerCase();

  if (safeName.length() == 0) {
    server.send(400, "application/json", "{\"ok\":false,\"error\":\"bad_name\"}");
    return;
  }
  if (!renameRoll(oldDir, safeName)) {
    server.send(500, "application/json", "{\"ok\":false,\"error\":\"rename_failed\"}");
    return;
  }
  server.send(200, "application/json", "{\"ok\":true,\"newDir\":\"" + safeName + "\"}");
}

void handleRolls() {
  if (!sdCardOK) { server.send(200, "application/json", "[]"); return; }
  File root = SD_MMC.open("/photos");
  if (!root) { server.send(200, "application/json", "[]"); return; }

  String dirs[32];
  int nDirs = 0;
  File f = root.openNextFile();
  while (f && nDirs < 32) {
    if (f.isDirectory()) { dirs[nDirs++] = String(f.name()); }
    f = root.openNextFile();
  }
  root.close();

  String json = "[";
  for (int i = 0; i < nDirs; i++) {
    int count = 0;
    File sub = SD_MMC.open("/photos/" + dirs[i]);
    if (sub) {
      File sf = sub.openNextFile();
      while (sf) { count++; sf = sub.openNextFile(); }
      sub.close();
    }
    String filmId = dirs[i];
    int upos = dirs[i].indexOf('_');
    if (upos >= 0) filmId = dirs[i].substring(upos + 1);
    if (i > 0) json += ",";
    json += "{\"dir\":\"" + dirs[i] + "\",\"film\":\"" + filmId + "\",\"count\":" + String(count) + "}";
  }
  json += "]";
  server.send(200, "application/json", json);
}

void handleFilter() {
  sensor_t* s = esp_camera_sensor_get();
  if (!s) { server.send(500, "application/json", "{\"ok\":false}"); return; }

  if (server.hasArg("brightness")) s->set_brightness(s, server.arg("brightness").toInt());
  if (server.hasArg("saturation")) s->set_saturation(s, server.arg("saturation").toInt());
  if (server.hasArg("contrast"))   s->set_contrast(s,   server.arg("contrast").toInt());
  if (server.hasArg("effect"))     s->set_special_effect(s, server.arg("effect").toInt());

  server.send(200, "application/json", "{\"ok\":true}");
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

// ============================================================
// ROUTE REGISTRATION
// ============================================================

void registerRoutes() {
  server.on("/",        handleRoot);
  server.on("/capture", handleCapture);
  server.on("/list",    handleList);
  server.on("/photo",   handlePhoto);
  server.on("/filter",  handleFilter);
  server.on("/setroll", handleSetRoll);
  server.on("/develop", handleDevelop);
  server.on("/status",  handleStatus);
  server.on("/settings", HTTP_GET, handleSettingsGet);
  server.on("/settings", HTTP_POST, handleSettingsSave);
  server.on("/delete", HTTP_POST, handleDelete);
  server.on("/rename", HTTP_POST, handleRename);
  server.on("/rolls",   handleRolls);
  server.on("/update", HTTP_GET, handleUpdatePage);
  server.on("/update", HTTP_POST,
    []() {
      bool ok = !Update.hasError();
      server.sendHeader("Connection", "close");
      String msg = ok ? "OK" : ("FAIL: Update error " + String(Update.getError())
                   + ". Check OTA partition scheme and upload only .ino.bin");
      server.send(ok ? 200 : 500, "text/plain", msg);
      delay(200);
      if (ok) ESP.restart();
    },
    handleUpdateUpload);
  server.onNotFound(handleNotFound);
}
