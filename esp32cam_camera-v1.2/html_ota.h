#ifndef HTML_OTA_H
#define HTML_OTA_H

const char OTA_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32 OTA Update</title>
<style>
body{margin:0;background:#141210;color:#f0e8d8;font-family:Arial,sans-serif;display:flex;min-height:100vh;align-items:center;justify-content:center;padding:20px}
.card{width:min(100%,420px);background:#1e1b18;border:1px solid #332e28;border-radius:10px;padding:24px}
h1{margin:0 0 10px;color:#e8b84b;font-size:28px}
p{color:#a89b8b;line-height:1.5;font-size:14px}
.warn{margin-top:12px;padding:10px 12px;border:1px solid #6a4d20;border-radius:8px;background:#2b2113;color:#f3d08a;font-size:12px;line-height:1.5}
input,button{width:100%;margin-top:12px}
input[type=file]{padding:10px;border:1px solid #332e28;border-radius:8px;background:#272320;color:#f0e8d8}
button{padding:14px;border:none;border-radius:8px;background:#c8923a;color:#141210;font-weight:700;cursor:pointer}
button:disabled{opacity:.5;cursor:not-allowed}
.muted{font-size:12px;color:#8a7e70;margin-top:12px}
#status{margin-top:12px;font-size:13px;color:#e8b84b;word-break:break-word}
progress{width:100%;margin-top:12px;height:12px}
a{color:#e8b84b}
</style>
</head>
<body>
  <div class="card">
    <h1>Firmware Update</h1>
    <p>Select the compiled <strong>.bin</strong> file for this sketch and upload it over Wiâ€‘Fi. The ESP32 will reboot automatically when finished.</p>
    <div class="warn">Use only the main sketch file ending in <strong>.ino.bin</strong>. OTA also requires an ESP32 partition scheme with OTA support; <strong>Huge APP / No OTA</strong> will fail.</div>
    <form id="ota-form">
      <input id="firmware" type="file" accept=".bin" required>
      <button id="upload-btn" type="submit">Upload Firmware</button>
      <progress id="progress" value="0" max="100" style="display:none"></progress>
      <div id="status"></div>
    </form>
    <div class="muted">Build the binary in Arduino IDE, then upload it here while connected to the ESP32 access point.</div>
    <div class="muted"><a href="/">Back to camera</a></div>
  </div>
<script>
document.getElementById('ota-form').addEventListener('submit', async (e) => {
  e.preventDefault();
  const file = document.getElementById('firmware').files[0];
  const status = document.getElementById('status');
  const button = document.getElementById('upload-btn');
  const progress = document.getElementById('progress');
  if (!file) return;
  const formData = new FormData();
  formData.append('update', file);
  button.disabled = true;
  progress.style.display = 'block';
  status.textContent = 'Uploading...';
  const xhr = new XMLHttpRequest();
  xhr.open('POST', '/update', true);
  xhr.upload.onprogress = (event) => {
    if (event.lengthComputable) progress.value = Math.round((event.loaded / event.total) * 100);
  };
  xhr.onload = () => {
    status.textContent = xhr.status === 200 ? 'Upload complete. Rebooting...' : 'Update failed: ' + xhr.responseText;
    button.disabled = false;
  };
  xhr.onerror = () => {
    status.textContent = 'Network error during upload';
    button.disabled = false;
  };
  xhr.send(formData);
});
</script>
</body>
</html>
)rawhtml";

#endif
