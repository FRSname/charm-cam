# Charm-Cam

<p align="center">
  <img src="Charm-Cam-reveal.gif" alt="Charm-Cam in action" width="600" />
</p>

[![Built with Claude](https://img.shields.io/badge/Built_with-Claude-D97757?logo=anthropic&logoColor=white)](https://claude.com)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> Built in collaboration with [Claude](https://claude.com)

A Wi-Fi enabled point-and-shoot camera built for the AI-Thinker ESP32-CAM.

Charm-Cam turns the ESP32-CAM into a small standalone camera with a built-in web UI, SD card storage, analog film-roll simulation, digital mode, physical shutter support, and OTA firmware updates — designed to feel like a keychain charm you carry around and snap with.

## Features

- Access Point mode with configurable SSID and password
- Web UI for shooting, gallery viewing, archive browsing, and settings
- Two camera modes:
  - **Analog mode**: load a film, shoot up to 36 frames, then develop the roll
  - **Digital mode**: unlimited direct-to-gallery captures
- Physical shutter button support on GPIO13
- SD card image storage
- Developed roll archive storage by roll folder, with auto-incrementing roll numbers preserved across reboots
- Rename developed rolls from the web UI
- Delete individual saved photos and entire developed rolls from the web UI
- Toggleable flash for captures
- EXIF orientation tag injected at save time (zero-cost rotation, no re-encode)
- Persistent settings and roll state using ESP32 Preferences
- OTA firmware update page
- LED feedback for status, warnings, roll full, and new roll loaded

## Sample photos

Frames captured with Charm-Cam, straight from the SD card with no post-processing.

<table>
  <tr>
    <td><img src="Gallery/ilford_7.jpg"  alt="Ilford roll — frame 7"  width="220" /></td>
    <td><img src="Gallery/ilford_8.jpg"  alt="Ilford roll — frame 8"  width="220" /></td>
    <td><img src="Gallery/ilford_12.jpg" alt="Ilford roll — frame 12" width="220" /></td>
    <td><img src="Gallery/ilford_19.jpg" alt="Ilford roll — frame 19" width="220" /></td>
  </tr>
  <tr>
    <td><img src="Gallery/ilford_24.jpg" alt="Ilford roll — frame 24" width="220" /></td>
    <td><img src="Gallery/ilford_25.jpg" alt="Ilford roll — frame 25" width="220" /></td>
    <td><img src="Gallery/ilford_36.jpg" alt="Ilford roll — frame 36" width="220" /></td>
    <td><img src="Gallery/kodak_16.jpg"  alt="Kodak roll — frame 16"  width="220" /></td>
  </tr>
</table>

## App

The whole interface runs on the ESP32-CAM itself — no cloud, no companion app, no internet required. Connect to the camera's Wi-Fi hotspot, open `http://192.168.4.1` in any browser (phone, tablet, laptop), and you get a self-contained film-camera UI styled in amber-on-black.

<table>
  <tr>
    <td width="280" valign="top"><img src="Gallery/screenshots/cam.PNG" alt="Capture screen" width="260" /></td>
    <td valign="top">
      <h3>Capture</h3>
      <p>The main shooting screen. Shows the live viewfinder, the loaded film stock (here: <em>Ilford</em>), the frame counter, frames remaining, and the big amber shutter button. The <strong>Develop Roll</strong> action below moves all exposures into the archive and resets the roll. Top bar carries the <em>Charm-Cam / OV2640</em> wordmark, a <em>READY</em> status pill, and quick access to gallery and settings.</p>
    </td>
  </tr>
  <tr>
    <td width="280" valign="top"><img src="Gallery/screenshots/rolls.PNG" alt="Load Film screen" width="260" /></td>
    <td valign="top">
      <h3>Load Film</h3>
      <p>Pick the film stock for the next roll. Eight presets are included — <em>Natural, Kodak, Ilford, Velvia, Faded, Cyanotype, Redscale, X-Pro</em> — each shown as a film canister with a short personality description. Picking one starts a fresh 36-exposure roll.</p>
    </td>
  </tr>
  <tr>
    <td width="280" valign="top"><img src="Gallery/screenshots/gallery.PNG" alt="Roll Archive screen" width="260" /></td>
    <td valign="top">
      <h3>Roll Archive</h3>
      <p>Every developed roll, with its frame count and folder name (<code>rNN_filmname</code>). Each row has <strong>Rename</strong> and <strong>Delete</strong> actions so you can curate the archive without touching the SD card. The top buttons jump back to the camera or load a new roll.</p>
    </td>
  </tr>
  <tr>
    <td width="280" valign="top"><img src="Gallery/screenshots/images.PNG" alt="Developed Roll view" width="260" /></td>
    <td valign="top">
      <h3>Developed Roll</h3>
      <p>The contact-sheet view for a single developed roll. Browse all frames, tap any thumbnail to view full size, or remove the whole roll with <strong>Delete Roll</strong>. <strong>Rename</strong> lets you replace the auto-generated <code>rNN_filmname</code> name with something meaningful (e.g. <em>"Prague Saturday"</em>).</p>
    </td>
  </tr>
  <tr>
    <td width="280" valign="top"><img src="Gallery/screenshots/settings.PNG" alt="Settings screen" width="260" /></td>
    <td valign="top">
      <h3>Settings</h3>
      <p>Switch between <strong>Analog</strong> (36-shot rolls + developing) and <strong>Digital</strong> (direct-to-gallery) modes, toggle the flash LED, and change the AP name and password. Settings persist across reboots in NVS. <strong>Firmware Update</strong> opens the OTA upload page.</p>
    </td>
  </tr>
</table>

<p align="center">
  <img src="Gallery/printed.jpg" alt="Charm-Cam printed and assembled" width="600" />
</p>

## Hardware

Designed for:

- AI-Thinker ESP32-CAM with OV2640 camera module — [buy on AliExpress](https://www.aliexpress.com/item/1005006299363624.html)  
  <a href="https://www.aliexpress.com/item/1005006299363624.html"><img src="Gallery/ESP.png" alt="AI-Thinker ESP32-CAM with OV2640" width="250" /></a>
- Battery — 3.7 V Li-Po cell, optimal dimensions **39 × 26 × 4.5 mm** to fit the enclosure. The prototype uses a **502040 350 mAh** cell (40 × 20 × 5 mm) — [buy on AliExpress](https://www.aliexpress.com/item/1005011928866829.html)  
  <a href="https://www.aliexpress.com/item/1005011928866829.html"><img src="Gallery/Battery.png" alt="502040 3.7V 350mAh Li-Po battery" width="250" /></a>
- USB C Charging board — [buy on AliExpress](https://www.aliexpress.com/item/1005009974716402.html)  
  <a href="https://www.aliexpress.com/item/1005009974716402.html"><img src="Gallery/Batteryboard.png" alt="USB-C lithium battery charging module" width="250" /></a>
- Flat on/off switch — **ESP1010 side-actuated slide switch** — [buy on AliExpress](https://www.aliexpress.com/item/1005007537444508.html)  
  <a href="https://www.aliexpress.com/item/1005007537444508.html"><img src="Gallery/Switch.png" alt="ESP1010 side-actuated slide switch" width="250" /></a>
- Momentary shutter button — **6 × 6 × 6 mm SMD tact switch**, connected between GPIO13 and GND — [buy on AliExpress](https://www.aliexpress.com/item/1005007046105681.html)  
  <a href="https://www.aliexpress.com/item/1005007046105681.html"><img src="Gallery/Button.png" alt="6x6x6 mm SMD tact switch" width="250" /></a>
- microSD card

### 3D printable enclosure

STL/3MF files for the Charm-Cam case are in [3D Files/](3D%20Files/):

- `Charm-Cam-Front-v1.stl` / `Charm-Cam-Back-v1.stl` — main body halves
- `Charm-Cam-Cage-v1.stl` — internal frame holding the ESP32-CAM and battery
- `Charm-Cam-ESP-Cover-v1.stl` — module cover
- `Charm-Cam-Lens-v1.stl` — lens bezel
- `Charm-Cam-Parts-Holder-v1.stl` — print-bed jig for small parts
- `Charm-Cam-v1-PrintFILE.3mf` — pre-arranged print plate (PrusaSlicer / Bambu Studio)

<table>
  <tr>
    <td><img src="Gallery/Body.png"     alt="Charm-Cam assembled body"      width="360" /></td>
    <td><img src="Gallery/exploded.png" alt="Charm-Cam exploded parts view" width="360" /></td>
  </tr>
</table>

### Wiring

<p align="center">
  <img src="Gallery/wiring.png" alt="Charm-Cam wiring diagram" width="700" />
</p>

- **USB-C charging board** → ESP32-CAM `5V` / `GND` (power in, with the slide switch breaking the 5V line)
- **Li-Po battery** → charging board `B+` / `B-`
- **ESP1010 slide switch** → in-line on the 5V rail between charging board and ESP32-CAM (cuts power when off)
- **6×6×6 tact button** → ESP32-CAM `GPIO13` ↔ `GND` (the firmware uses `INPUT_PULLUP`, no external resistor needed)
- Onboard **flash LED** (GPIO4) and **status LED** (GPIO33) are already on the ESP32-CAM board

### Pin usage

- **GPIO13**: shutter button
- **GPIO4**: flash LED
- **GPIO33**: red status LED (active LOW)

## How it works

### Analog mode

- Load a film roll from the web UI
- Capture up to 36 photos
- When the roll is full, the flash LED blinks quickly 3 times
- Develop the roll from the UI or by a long press of the shutter button
- Developed rolls are stored in archive folders on the SD card

### Digital mode

- No film loading required
- No 36-frame limit
- Captured images are saved directly to the gallery

### Physical shutter behavior

- **Short press**: capture immediately
- **Long press (2 seconds)** in analog mode: develop the current roll and automatically load a new roll of the same film
- If no film is loaded in analog mode, the status LED blinks 3 times as a warning

## Storage layout

Images are saved on the microSD card.

Typical folder structure:

- `/roll/` - current undeveloped analog roll
- `/photos/` - developed rolls and digital captures
- `/photos/rNN_filmname/` - one developed analog roll archive folder (NN is an auto-incrementing roll counter persisted in NVS)

## Web interface

After boot, the ESP32-CAM creates its own Wi-Fi hotspot.

Default settings:

- **SSID**: `Charm-Cam`
- **Password**: `camera123`

Connect to the hotspot and open the device IP shown in Serial Monitor, typically:

- `http://192.168.4.1`

From the web UI you can:

- capture photos
- load film
- develop rolls
- browse gallery and roll archive
- rename developed rolls
- delete saved photos or whole developed rolls
- switch between analog and digital mode
- toggle the flash on or off
- change hotspot name and password
- open the OTA firmware update page

## Build and upload

### Arduino IDE setup

Use Arduino IDE with ESP32 board support installed.

Recommended board settings for OTA:

- **Board**: AI Thinker ESP32-CAM
- **Partition Scheme**: Minimal SPIFFS (1.9MB APP with OTA / 128KB SPIFFS)

That partition scheme is important if you want OTA updates to work.

### First upload

1. Connect the ESP32-CAM over USB/serial programmer
2. Select the correct board and port
3. Upload the sketch normally from Arduino IDE
4. Open Serial Monitor at `115200` baud
5. Wait for the AP name and IP address to appear

## OTA updates

OTA is built into the firmware and available from the settings area.

Important notes:

- Use an OTA-capable partition scheme
- Upload only the main compiled firmware binary: the `.ino.bin` file
- Do **not** upload the bootloader, partition, or merged files through the OTA page

If OTA fails, the most common causes are:

- wrong partition scheme
- firmware binary too large for the OTA app slot
- uploading the wrong exported file

## Power notes

ESP32-CAM boards are sensitive to weak power supplies.

For stable behavior:

- use a good USB cable
- use a solid 5V power source
- avoid unstable or low-current adapters

Brownout or boot issues are often power related. The firmware also lowers Wi-Fi TX power to `8.5 dBm` to reduce brownout risk on marginal supplies.

## Project files

The current firmware lives in [esp32cam_camera-v1.2/](esp32cam_camera-v1.2/) and is split across:

- `esp32cam_camera-v1.2.ino` - main sketch (setup, loop, globals, shutter button handling)
- `config.h` - pin definitions, default credentials, roll size, button timing, capture rotation
- `html_main.h` - main web UI (PROGMEM)
- `html_ota.h` - OTA update page (PROGMEM)
- `camera.ino` - OV2640 camera init
- `storage.ino` - SD card, file ops, roll logic, capture, EXIF rotation
- `persistence.ino` - NVS load/save for settings and roll state
- `handlers.ino` - HTTP request handlers and route registration
- `led.ino` - status and flash LED helpers

A separate `diag_test/` sketch is included for hardware diagnostics.

## Notes

- Settings and roll state are saved in non-volatile storage and survive reboot
- The SD card is required for saving photos and roll archives
- The UI is designed around a film-camera workflow, even when digital mode is enabled

## Trademarks & brand assets

The film canister SVGs in [Filmrolls/](Filmrolls/) reproduce visual elements (logos, color palettes, typography) inspired by real film stocks (Kodak, Ilford, Velvia, etc.). These are included for aesthetic purposes only. All trademarks and brand identities belong to their respective owners. This project is not affiliated with, endorsed by, or sponsored by any film manufacturer.

## License

Released under the [MIT License](LICENSE) — free to use, modify, and distribute. This is a free hobby project; no warranty.
