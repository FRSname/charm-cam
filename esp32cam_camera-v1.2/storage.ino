// ============================================================
// SD CARD INIT
// ============================================================

bool initSD() {
  // AI-Thinker uses 1-bit SD_MMC mode to free up GPIO13
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("SD Card mount failed");
    return false;
  }
  if (SD_MMC.cardType() == CARD_NONE) {
    Serial.println("No SD card attached");
    return false;
  }
  Serial.printf("SD card OK — %lluMB\n", SD_MMC.cardSize() / (1024 * 1024));

  // Count existing photos to avoid overwriting
  File root = SD_MMC.open("/photos");
  if (!root) SD_MMC.mkdir("/photos");
  else {
    File file = root.openNextFile();
    while (file) {
      if (!file.isDirectory()) photoCount++;
      file = root.openNextFile();
    }
    root.close();
  }
  return true;
}

// ============================================================
// PATH HELPERS
// ============================================================

static String sanitizePath(String input) {
  input.replace("..", "");
  input.replace("/", "");
  return input;
}

// ============================================================
// FILE OPERATIONS
// ============================================================

void clearRollFolder() {
  if (!sdCardOK) return;
  SD_MMC.mkdir("/roll");
  File dir = SD_MMC.open("/roll");
  if (!dir) return;
  File f = dir.openNextFile();
  while (f) {
    SD_MMC.remove(String("/roll/") + f.name());
    f = dir.openNextFile();
  }
}

bool deleteFolderContents(const String& dirPath) {
  File dir = SD_MMC.open(dirPath);
  if (!dir || !dir.isDirectory()) return false;

  File entry = dir.openNextFile();
  while (entry) {
    String entryName = String(entry.name());
    String fullPath = dirPath + "/" + entryName;
    bool ok = entry.isDirectory() ? (deleteFolderContents(fullPath) && SD_MMC.rmdir(fullPath.c_str()))
                                  : SD_MMC.remove(fullPath.c_str());
    entry.close();
    if (!ok) {
      dir.close();
      return false;
    }
    entry = dir.openNextFile();
  }

  dir.close();
  return true;
}

bool deleteDevelopedRoll(const String& dirName) {
  if (!sdCardOK || dirName.length() == 0) return false;
  String safeDir = sanitizePath(dirName);
  if (safeDir.length() == 0) return false;

  String path = "/photos/" + safeDir;
  if (!deleteFolderContents(path)) return false;
  return SD_MMC.rmdir(path.c_str());
}

bool deleteDevelopedPhoto(const String& fileName, const String& dirName) {
  if (!sdCardOK || fileName.length() == 0) return false;
  String safeName = sanitizePath(fileName);
  if (safeName.length() == 0) return false;

  String path = "/photos/" + safeName;
  if (dirName.length() > 0) {
    String safeDir = sanitizePath(dirName);
    if (safeDir.length() == 0) return false;
    path = "/photos/" + safeDir + "/" + safeName;
  }

  return SD_MMC.remove(path.c_str());
}

bool renameRoll(const String& oldDir, const String& newName) {
  if (!sdCardOK || oldDir.length() == 0 || newName.length() == 0) return false;
  String safeOld = sanitizePath(oldDir);
  String safeNew = sanitizePath(newName);
  if (safeOld.length() == 0 || safeNew.length() == 0) return false;
  String oldPath = "/photos/" + safeOld;
  String newPath = "/photos/" + safeNew;
  return SD_MMC.rename(oldPath.c_str(), newPath.c_str());
}

// ============================================================
// ROLL MANAGEMENT
// ============================================================

void startRoll(const String& film) {
  rollFilm      = film;
  rollDeveloped = false;
  rollFrames    = 0;
  clearRollFolder();
  saveRollState();
  signalNewRollLoaded();
}

bool developCurrentRoll(String& rollDirName, String& developedJson, String& error) {
  if (digitalMode)      { error = "digital_mode"; return false; }
  if (rollFilm == "")   { error = "no_roll";      return false; }
  if (rollFrames == 0)  { error = "empty";         return false; }
  if (rollDeveloped)    { error = "already";       return false; }
  if (!sdCardOK)        { error = "no_sd";         return false; }

  rollCount++;
  rollDirName = "r" + String(rollCount < 10 ? "0" : "") + String(rollCount) + "_" + rollFilm;
  String rollDirPath = "/photos/" + rollDirName;
  SD_MMC.mkdir("/photos");
  SD_MMC.mkdir(rollDirPath.c_str());

  developedJson = "[";
  bool first = true;
  File dir = SD_MMC.open("/roll");
  if (dir) {
    File f = dir.openNextFile();
    while (f) {
      String fname = String(f.name());
      String src   = "/roll/"    + fname;
      String dst   = rollDirPath + "/" + fname;
      f.close();
      File in = SD_MMC.open(src, FILE_READ);
      if (in) {
        File out = SD_MMC.open(dst, FILE_WRITE);
        if (out) {
          uint8_t buf[512];
          size_t n;
          while ((n = in.read(buf, sizeof(buf))) > 0) out.write(buf, n);
          out.close();
          SD_MMC.remove(src);
          if (!first) developedJson += ",";
          developedJson += "{\"name\":\"" + fname + "\"}";
          first = false;
        }
        in.close();
      }
      f = dir.openNextFile();
    }
  }
  developedJson += "]";
  rollDeveloped = true;
  saveRollState();
  Serial.printf("Roll #%d developed: %s, %d frames -> /photos/%s\n",
    rollCount, rollFilm.c_str(), rollFrames, rollDirName.c_str());
  return true;
}

// ============================================================
// JPEG EXIF ROTATION HELPER
// Prepends a minimal EXIF APP1 segment with an orientation tag
// directly into the open File, then writes the rest of the JPEG.
// This costs nothing at runtime — no decode, no re-encode.
// ============================================================

static void writeJpegWithExif(File& file, const uint8_t* buf, size_t len, uint8_t orientation) {
  // APP1 segment: EXIF header + one IFD entry (Orientation tag)
  // Length = 34 (0x0022): includes 2-byte length field itself
  const uint8_t exif[] = {
    0xFF, 0xE1,                          // APP1 marker
    0x00, 0x22,                          // APP1 length = 34
    0x45, 0x78, 0x69, 0x66, 0x00, 0x00, // "Exif\0\0"
    0x49, 0x49,                          // TIFF: little-endian byte order
    0x2A, 0x00,                          // TIFF magic number
    0x08, 0x00, 0x00, 0x00,             // Offset to first IFD = 8
    0x01, 0x00,                          // IFD entry count = 1
    0x12, 0x01,                          // Tag 0x0112 = Orientation (274)
    0x03, 0x00,                          // Type = SHORT
    0x01, 0x00, 0x00, 0x00,             // Count = 1
    orientation, 0x00, 0x00, 0x00,      // Value = orientation
    0x00, 0x00, 0x00, 0x00,             // Next IFD offset = 0 (none)
  };
  file.write(buf, 2);                   // SOI marker (FF D8) — must come first
  file.write(exif, sizeof(exif));        // EXIF APP1 segment
  file.write(buf + 2, len - 2);          // Rest of JPEG (everything after SOI)
}

// ============================================================
// CAPTURE & SAVE
// ============================================================

String captureAndSave() {
  if (!digitalMode && (rollFilm == "" || rollDeveloped || rollFrames >= ROLL_SIZE)) return "";

  // Flush stale frames from DMA buffer — the OV2640 fills buffers
  // continuously; without this the image can lag by 2-3 frames.
  for (int i = 0; i < 3; i++) {
    camera_fb_t* stale = esp_camera_fb_get();
    if (stale) esp_camera_fb_return(stale);
  }

  if (flashEnabled) digitalWrite(LED_FLASH_PIN, HIGH);
  delay(100);
  camera_fb_t* fb = esp_camera_fb_get();
  if (flashEnabled) digitalWrite(LED_FLASH_PIN, LOW);

  if (!fb) {
    Serial.println("Frame capture failed");
    return "";
  }

  photoCount++;
  String path;
  if (digitalMode) {
    path = "/photos/digital_" + String(photoCount) + ".jpg";
  } else {
    rollFrames++;
    saveRollState();
    path = "/roll/" + rollFilm + "_" + String(rollFrames) + ".jpg";
  }

  if (sdCardOK) {
    File file = SD_MMC.open(path, FILE_WRITE);
    if (file) {
#if CAPTURE_ROTATION != 1
      writeJpegWithExif(file, fb->buf, fb->len, CAPTURE_ROTATION);
#else
      file.write(fb->buf, fb->len);
#endif
      file.close();
      Serial.printf("Exposed %s (%d bytes)\n", path.c_str(), fb->len);
    } else {
      Serial.println("Failed to open file for writing");
    }
  }

  esp_camera_fb_return(fb);

  if (!digitalMode && rollFrames >= ROLL_SIZE) {
    blinkFlashFast(3, 80, 80);
  }

  return path;
}
