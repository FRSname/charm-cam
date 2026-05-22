#ifndef HTML_MAIN_H
#define HTML_MAIN_H

const char INDEX_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
<title>Charm-Cam</title>
<link rel="icon" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 32 32'><rect width='32' height='32' rx='6' fill='%23141210'/><rect x='3' y='11' width='26' height='17' rx='3' fill='%23c8923a'/><circle cx='16' cy='19' r='6' fill='%23141210'/><circle cx='16' cy='19' r='3.5' fill='%23e8b84b'/><rect x='10' y='7' width='9' height='5' rx='2' fill='%23c8923a'/><circle cx='25' cy='14' r='1.5' fill='%23e8b84b'/></svg>">
<style>
:root{--bg:#141210;--surface:#1e1b18;--raised:#272320;--border:#332e28;--gold:#c8923a;--amber:#e8b84b;--cream:#f0e8d8;--muted:#6a5f52;--red:#c43828;--green:#4a8a5a;
  --ff-display:'Trebuchet MS','Arial Narrow',Arial,sans-serif;
  --ff-mono:'Courier New',Courier,monospace;
  --ff-body:Georgia,'Times New Roman',serif}
*{box-sizing:border-box;margin:0;padding:0;-webkit-tap-highlight-color:transparent}
body{background:var(--bg);color:var(--cream);font-family:var(--ff-body);min-height:100vh;overflow-x:hidden}
body::before{content:'';position:fixed;inset:0;z-index:999;pointer-events:none;opacity:.03;background-image:url("data:image/svg+xml,%3Csvg viewBox='0 0 256 256' xmlns='http://www.w3.org/2000/svg'%3E%3Cfilter id='n'%3E%3CfeTurbulence type='fractalNoise' baseFrequency='0.9' numOctaves='4' stitchTiles='stitch'/%3E%3C/filter%3E%3Crect width='100%25' height='100%25' filter='url(%23n)'/%3E%3C/svg%3E");background-size:200px}
header{display:flex;align-items:center;justify-content:space-between;padding:16px 20px;border-bottom:1px solid var(--border);background:var(--surface)}
.wordmark{font-family:var(--ff-display);font-size:22px;letter-spacing:.12em;color:var(--amber)}
.wordmark span{color:var(--muted);font-size:14px}
.header-right{display:flex;align-items:center;gap:10px}
.status-pill{display:flex;align-items:center;gap:8px;font-family:var(--ff-mono);font-size:10px;color:var(--muted);letter-spacing:.1em}
.status-dot{width:7px;height:7px;border-radius:50%;background:var(--green);box-shadow:0 0 6px var(--green);animation:pulse 2.5s ease-in-out infinite}
.icon-btn{width:36px;height:36px;border-radius:50%;border:1px solid var(--border);background:transparent;color:var(--cream);cursor:pointer;font-size:16px}
.icon-btn:hover{border-color:var(--gold);color:var(--amber)}
@keyframes pulse{0%,100%{opacity:1}50%{opacity:.4}}
.screen{display:none;flex-direction:column;max-width:520px;margin:0 auto;padding:24px 20px 48px}
.screen.active{display:flex}

/* â”€â”€ FILM SELECTION â”€â”€ */
.screen-title{font-family:var(--ff-display);font-size:32px;letter-spacing:.1em;color:var(--amber);margin-bottom:4px}
.screen-sub{font-family:var(--ff-mono);font-size:10px;color:var(--muted);letter-spacing:.15em;margin-bottom:28px}
.roll-grid{display:grid;grid-template-columns:repeat(2,1fr);gap:10px}
.roll-card{background:var(--raised);border:1px solid var(--border);border-radius:8px;overflow:hidden;cursor:pointer;transition:border-color .2s,transform .15s;outline:none;padding:0}
.roll-card:hover{border-color:var(--gold);transform:translateY(-2px)}
.roll-card.selected{border-color:var(--amber);box-shadow:0 0 0 1px var(--amber)}
.roll-swatch{height:110px;overflow:hidden}
.roll-name{padding:8px 12px 4px;font-family:var(--ff-display);font-size:18px;letter-spacing:.08em;color:var(--cream)}
.roll-desc{padding:0 12px 10px;font-family:var(--ff-mono);font-size:8px;color:var(--muted);letter-spacing:.08em;line-height:1.5}
.load-btn{margin-top:24px;background:var(--gold);color:var(--bg);border:none;border-radius:6px;padding:14px;font-family:var(--ff-display);font-size:20px;letter-spacing:.12em;cursor:pointer;width:100%;transition:opacity .2s}
.load-btn:disabled{opacity:.35;cursor:not-allowed}

/* â”€â”€ SHOOTING â”€â”€ */
.viewfinder-wrap{position:relative;aspect-ratio:4/3;background:#0a0806;overflow:hidden;border-radius:4px;border:1px solid var(--border)}
.viewfinder-wrap::after{content:'';position:absolute;inset:0;pointer-events:none;background:radial-gradient(ellipse at center,transparent 60%,rgba(0,0,0,.7) 100%);z-index:2}
#preview-img{width:100%;height:100%;object-fit:cover;display:block;transition:filter .4s}
.vf-empty{position:absolute;inset:0;display:flex;flex-direction:column;align-items:center;justify-content:center;gap:10px;z-index:1}
.vf-empty .icon{font-size:40px;opacity:.12}
.vf-empty p{font-size:12px;color:var(--muted);font-family:var(--ff-mono);letter-spacing:.08em}
.flash-overlay{position:absolute;inset:0;background:white;z-index:10;opacity:0;pointer-events:none;transition:opacity .05s}
.flash-overlay.active{opacity:1}
.bracket{position:absolute;z-index:3;width:18px;height:18px}
.bracket.tl{top:12px;left:12px;border-top:2px solid var(--gold);border-left:2px solid var(--gold)}
.bracket.tr{top:12px;right:12px;border-top:2px solid var(--gold);border-right:2px solid var(--gold)}
.bracket.bl{bottom:12px;left:12px;border-bottom:2px solid var(--gold);border-left:2px solid var(--gold)}
.bracket.br{bottom:12px;right:12px;border-bottom:2px solid var(--gold);border-right:2px solid var(--gold)}
.film-badge{position:absolute;bottom:12px;left:50%;transform:translateX(-50%);background:rgba(0,0,0,.65);backdrop-filter:blur(4px);font-family:var(--ff-mono);font-size:10px;letter-spacing:.14em;color:var(--amber);padding:4px 12px;border-radius:20px;border:1px solid rgba(200,146,58,.3);z-index:4;text-transform:uppercase}
/* sprocket-hole film strip */
.film-strip-wrap{margin:12px 0 8px;position:relative}
.film-strip{background:#0d0b09;border:1px solid var(--border);border-radius:3px;height:28px;display:flex;align-items:center;position:relative;overflow:hidden}
.sprocket-col{display:flex;flex-direction:column;justify-content:space-around;height:100%;padding:3px 0;width:18px;flex-shrink:0;position:absolute;z-index:3}
.sprocket-col.left{left:0;border-right:1px solid var(--border);background:var(--bg)}
.sprocket-col.right{right:0;border-left:1px solid var(--border);background:var(--bg)}
.sprocket-hole{width:8px;height:6px;border-radius:1px;background:var(--surface);border:1px solid #1a1714;margin:0 auto}
.frames-track{flex:1;display:flex;align-items:center;gap:2px;padding:0 22px;overflow:hidden;height:100%}
.frame-pip{flex:1;height:12px;border-radius:1px;background:var(--raised);border:1px solid #1a1714;transition:background .3s,box-shadow .3s;min-width:3px}
.frame-pip.exposed{background:var(--gold);border-color:var(--gold)}
.frame-pip.current{background:var(--amber);border-color:var(--amber);box-shadow:0 0 5px var(--amber)}
.shutter-row{display:flex;align-items:center;justify-content:space-between;padding:10px 0 0}
.counter{font-family:var(--ff-display);font-size:48px;color:var(--gold);line-height:1;letter-spacing:.05em;min-width:70px}
.counter-label{font-family:var(--ff-mono);font-size:9px;color:var(--muted);letter-spacing:.12em;margin-top:2px}
.shutter-btn{width:72px;height:72px;border-radius:50%;background:var(--raised);border:2px solid var(--gold);cursor:pointer;position:relative;outline:none;transition:transform .12s,box-shadow .12s;box-shadow:0 0 0 6px rgba(200,146,58,.08),inset 0 2px 4px rgba(0,0,0,.5)}
.shutter-btn::before{content:'';position:absolute;inset:7px;border-radius:50%;background:radial-gradient(circle at 38% 35%,#e8b84b,#c8923a 60%,#9a6a1a);box-shadow:inset 0 2px 6px rgba(0,0,0,.4)}
.shutter-btn:hover{box-shadow:0 0 0 10px rgba(200,146,58,.12),inset 0 2px 4px rgba(0,0,0,.5)}
.shutter-btn:active{transform:scale(.93)}
.shutter-btn.firing::before{background:radial-gradient(circle at 38% 35%,#fff8e0,#e8b84b 60%,#c8923a)}
.shutter-btn:disabled{opacity:.3;cursor:not-allowed}
.frames-info{min-width:70px;text-align:right;font-family:var(--ff-mono);font-size:9px;color:var(--muted);letter-spacing:.1em;line-height:2}
.frames-info .val{color:var(--cream);font-size:16px;font-family:var(--ff-display)}
.develop-btn{margin-top:18px;background:transparent;color:var(--amber);border:1px solid var(--amber);border-radius:6px;padding:12px;font-family:var(--ff-display);font-size:18px;letter-spacing:.12em;cursor:pointer;width:100%;transition:background .2s,color .2s}
.develop-btn:hover{background:var(--amber);color:var(--bg)}
.develop-btn:disabled{opacity:.3;cursor:not-allowed}

/* â”€â”€ DARKROOM / DEVELOPING â”€â”€ */
.darkroom{align-items:center;text-align:center;padding-top:60px}
.darkroom .icon{font-size:64px;animation:spin 3s linear infinite;display:inline-block;margin-bottom:24px}
@keyframes spin{from{transform:rotate(0deg)}to{transform:rotate(360deg)}}
.darkroom h2{font-family:var(--ff-display);font-size:36px;color:var(--amber);letter-spacing:.1em;margin-bottom:8px}
.darkroom p{font-family:var(--ff-mono);font-size:11px;color:var(--muted);letter-spacing:.1em;line-height:1.8}

/* â”€â”€ GALLERY â”€â”€ */
.gallery-header{display:flex;align-items:center;justify-content:space-between;margin-bottom:18px}
.gallery-header h2{font-family:var(--ff-display);font-size:28px;color:var(--amber);letter-spacing:.1em}
.new-roll-btn{font-family:var(--ff-mono);font-size:9px;letter-spacing:.12em;padding:8px 14px;border-radius:4px;cursor:pointer;border:1px solid var(--border);background:transparent;color:var(--cream);text-transform:uppercase}
.new-roll-btn:hover{border-color:var(--gold)}
.film-label{font-family:var(--ff-mono);font-size:9px;color:var(--muted);letter-spacing:.14em;text-transform:uppercase;margin-bottom:14px}
.gallery-grid{display:grid;grid-template-columns:repeat(3,1fr);gap:6px}
.thumb-card{aspect-ratio:4/3;background:var(--raised);border-radius:4px;overflow:hidden;cursor:pointer;position:relative;border:1px solid var(--border);transition:border-color .2s;animation:fadeIn .4s ease both}
@keyframes fadeIn{from{opacity:0;transform:scale(.96)}to{opacity:1;transform:scale(1)}}
.thumb-card:hover{border-color:var(--gold)}
.thumb-card img{width:100%;height:100%;object-fit:cover;display:block}
.thumb-card .dl-overlay{position:absolute;inset:0;background:rgba(0,0,0,.6);display:flex;align-items:center;justify-content:center;opacity:0;transition:opacity .2s;font-family:var(--ff-mono);font-size:10px;color:var(--amber);letter-spacing:.1em}
.thumb-card:hover .dl-overlay{opacity:1}
.danger-btn{font-family:var(--ff-mono);font-size:9px;letter-spacing:.12em;padding:8px 14px;border-radius:4px;cursor:pointer;border:1px solid #8c3b30;background:transparent;color:#ffb1a4;text-transform:uppercase}
.danger-btn:hover{background:#8c3b30;color:#fff4ef}

/* â”€â”€ ARCHIVE â”€â”€ */
.archive-grid{display:grid;grid-template-columns:repeat(2,1fr);gap:10px;margin-top:14px}
.archive-card{background:var(--raised);border:1px solid var(--border);border-radius:8px;overflow:hidden;cursor:pointer;transition:border-color .2s,transform .15s}
.archive-card:hover{border-color:var(--gold);transform:translateY(-2px)}
.archive-swatch{height:56px;overflow:hidden}
.archive-film{font-family:var(--ff-display);font-size:16px;letter-spacing:.08em;color:var(--cream);padding:8px 12px 2px}
.archive-meta{font-family:var(--ff-mono);font-size:8px;color:var(--muted);letter-spacing:.08em;padding:0 12px 10px}
.archive-actions{display:flex;justify-content:flex-end;gap:6px;padding:0 12px 12px}

/* â”€â”€ SETTINGS â”€â”€ */
.settings-card{background:var(--raised);border:1px solid var(--border);border-radius:8px;padding:16px;margin-bottom:14px}
.settings-card h3{font-family:var(--ff-display);font-size:22px;color:var(--amber);letter-spacing:.08em;margin-bottom:10px}
.settings-label{display:block;font-family:var(--ff-mono);font-size:10px;color:var(--muted);letter-spacing:.12em;margin:10px 0 6px;text-transform:uppercase}
.settings-input,.settings-select{width:100%;padding:12px;border-radius:6px;border:1px solid var(--border);background:var(--surface);color:var(--cream);font-family:var(--ff-mono);font-size:12px}
.settings-help{font-family:var(--ff-mono);font-size:10px;color:var(--muted);line-height:1.6;margin-top:8px}

/* â”€â”€ LIGHTBOX â”€â”€ */
#lightbox{position:fixed;inset:0;background:rgba(10,8,6,.96);z-index:100;display:none;flex-direction:column;align-items:center;justify-content:center;padding:20px;backdrop-filter:blur(8px)}
#lightbox.open{display:flex}
#lightbox img{max-width:100%;max-height:70vh;object-fit:contain;border:1px solid var(--border);border-radius:4px}
.lb-actions{display:flex;gap:12px;margin-top:18px}
.lb-btn{font-family:var(--ff-mono);font-size:11px;letter-spacing:.12em;padding:10px 24px;border-radius:4px;cursor:pointer;border:1px solid;text-decoration:none;text-transform:uppercase}
.lb-btn.primary{background:var(--gold);color:var(--bg);border-color:var(--gold)}
.lb-btn.ghost{background:transparent;color:var(--cream);border-color:var(--border)}
.lb-name{font-family:var(--ff-mono);font-size:11px;color:var(--muted);margin-top:12px}

/* â”€â”€ TOAST â”€â”€ */
#toast{position:fixed;bottom:24px;left:50%;transform:translateX(-50%) translateY(80px);background:var(--surface);border:1px solid var(--gold);color:var(--amber);font-family:var(--ff-mono);font-size:11px;padding:12px 22px;border-radius:4px;letter-spacing:.1em;transition:transform .3s cubic-bezier(.34,1.56,.64,1),opacity .3s;opacity:0;z-index:200;white-space:nowrap}
#toast.show{transform:translateX(-50%) translateY(0);opacity:1}
</style>
</head>
<body>
<header>
  <div class="wordmark">Charm-Cam <span>/ OV2640</span></div>
  <div class="header-right">
    <div class="status-pill">
      <div class="status-dot" id="status-dot"></div>
      <span id="status-text">READY</span>
    </div>
    <button class="icon-btn" onclick="goToGalleryHeader()" title="Gallery">&#128444;</button>
    <button class="icon-btn" onclick="openSettings()" title="Settings">&#9881;</button>
  </div>
</header>

<!-- SCREEN 1: FILM SELECTION -->
<div class="screen active" id="screen-load">
  <div style="padding-top:20px">
    <div class="screen-title" id="load-title">Load Film</div>
    <div class="screen-sub" id="load-sub">&#8212; Choose your roll &#183; 36 exposures</div>
    <div class="roll-grid" id="roll-grid"></div>
    <button class="load-btn" id="load-btn" disabled onclick="loadRoll()">Load Roll</button>
    <button class="develop-btn" id="archive-btn" onclick="goToArchive()" style="margin-top:10px">&#128247; Roll Archive</button>
  </div>
</div>

<!-- SCREEN 2: SHOOTING -->
<div class="screen" id="screen-shoot">
  <div class="viewfinder-wrap" id="viewfinder" onclick="handleVfClick()">
    <div class="vf-empty" id="vf-empty">
      <svg width="72" height="56" viewBox="0 0 72 56" fill="none" xmlns="http://www.w3.org/2000/svg">
        <rect x="1" y="14" width="70" height="40" rx="5" fill="#1e1b18" stroke="#c8923a" stroke-width="1.5"/>
        <rect x="22" y="8" width="20" height="10" rx="3" fill="#1e1b18" stroke="#c8923a" stroke-width="1.5"/>
        <circle cx="36" cy="34" r="13" fill="#141210" stroke="#c8923a" stroke-width="1.5"/>
        <circle cx="36" cy="34" r="8" fill="#141210" stroke="#6a5f52" stroke-width="1"/>
        <circle cx="36" cy="34" r="4" fill="#272320"/>
        <circle cx="36" cy="34" r="1.5" fill="#e8b84b" opacity=".6"/>
        <rect x="8" y="22" width="8" height="5" rx="1.5" fill="#272320" stroke="#6a5f52" stroke-width="1"/>
        <circle cx="60" cy="22" r="3" fill="#e8b84b" opacity=".4"/>
      </svg>
      <p>NO EXPOSURE YET</p>
    </div>
    <img id="preview-img" src="" alt="" style="display:none">
    <div class="flash-overlay" id="flash"></div>
    <div class="bracket tl"></div><div class="bracket tr"></div>
    <div class="bracket bl"></div><div class="bracket br"></div>
    <div class="film-badge" id="film-badge">&#8212;</div>
  </div>
  <div class="film-strip-wrap" id="strip-wrap">
    <div class="film-strip" id="strip">
      <div class="sprocket-col left"><div class="sprocket-hole"></div><div class="sprocket-hole"></div></div>
      <div class="frames-track" id="frames-track"></div>
      <div class="sprocket-col right"><div class="sprocket-hole"></div><div class="sprocket-hole"></div></div>
    </div>
  </div>
  <div class="shutter-row">
    <div>
      <div class="counter" id="frames-taken">00</div>
      <div class="counter-label">EXPOSED</div>
    </div>
    <button class="shutter-btn" id="shutter-btn" onclick="capture()"></button>
    <div class="frames-info">
      <div class="val" id="frames-left">36</div>
      <div>FRAMES LEFT</div>
    </div>
  </div>
  <button class="develop-btn" id="develop-btn" disabled onclick="goToDevelop()">Develop Roll</button>
</div>

<!-- SCREEN 3: DARKROOM -->
<div class="screen darkroom" id="screen-dark">
  <div class="icon">&#9711;</div>
  <h2>Developing&#8230;</h2>
  <p>Your film is being processed<br>in the darkroom.<br><br>Please wait.</p>
</div>

<!-- SCREEN 4: GALLERY -->
<div class="screen" id="screen-gallery">
  <div class="gallery-header">
    <h2>Developed Roll</h2>
    <div style="display:flex;gap:6px">
      <button class="danger-btn" id="delete-roll-btn" onclick="deleteCurrentRoll()" style="display:none">Delete Roll</button>
      <button class="new-roll-btn" id="rename-roll-btn" onclick="openRenameModal(currentGalleryDir)" style="display:none">Rename</button>
      <button class="new-roll-btn" id="back-cam-gallery" onclick="goBack()" style="display:none">&#128247; Camera</button>
      <button class="new-roll-btn" onclick="goToArchive()">All Rolls</button>
      <button class="new-roll-btn" onclick="goToLoadScreen()">&#43; New Roll</button>
    </div>
  </div>
  <div class="film-label" id="gallery-film-label"></div>
  <div class="gallery-grid" id="gallery"></div>
</div>

<!-- SCREEN 5: ROLL ARCHIVE -->
<div class="screen" id="screen-archive">
  <div class="gallery-header">
    <h2>Roll Archive</h2>
    <div style="display:flex;gap:6px">
      <button class="new-roll-btn" id="back-cam-archive" onclick="goBack()" style="display:none">&#128247; Camera</button>
      <button class="new-roll-btn" onclick="goToLoadScreen()">&#43; New Roll</button>
    </div>
  </div>
  <div class="screen-sub">&#8212; Previously developed rolls</div>
  <div id="archive-grid" class="archive-grid"></div>
  <div id="archive-empty" style="display:none;color:var(--muted);font-family:var(--ff-mono);font-size:11px;padding:30px 0;text-align:center">NO DEVELOPED ROLLS YET</div>
</div>

<!-- SCREEN 6: SETTINGS -->
<div class="screen" id="screen-settings">
  <div class="gallery-header">
    <h2>Settings</h2>
    <button class="new-roll-btn" onclick="goBack()">Back</button>
  </div>
  <div class="settings-card">
    <h3>Camera Mode</h3>
    <label class="settings-label" for="mode-select">Mode</label>
    <select id="mode-select" class="settings-select">
      <option value="analog">Analog</option>
      <option value="digital">Digital</option>
    </select>
    <div class="settings-help">Analog uses 36-shot film rolls and developing. Digital saves each shot directly to the gallery.</div>
  </div>
  <div class="settings-card">
    <h3>Flash</h3>
    <label class="settings-label" for="flash-select">Flash LED</label>
    <select id="flash-select" class="settings-select">
      <option value="1">Enabled</option>
      <option value="0">Disabled</option>
    </select>
    <div class="settings-help">When disabled the flash LED stays off during capture. Useful in low-contrast scenes or to preserve night vision.</div>
  </div>
  <div class="settings-card">
    <h3>Access Point</h3>
    <label class="settings-label" for="ssid-input">AP Name</label>
    <input id="ssid-input" class="settings-input" maxlength="32" type="text" placeholder="ESP32-Camera">
    <label class="settings-label" for="password-input">Password</label>
    <input id="password-input" class="settings-input" maxlength="63" type="text" placeholder="camera123">
    <div class="settings-help">Leave password empty to create an open network. Otherwise use at least 8 characters.</div>
  </div>
  <button class="load-btn" onclick="saveSettings()">Save Settings &amp; Reboot</button>
  <button class="develop-btn" onclick="window.open('/update','_blank')">Firmware Update</button>
</div>

<!-- RENAME MODAL -->
<div id="rename-modal" style="position:fixed;inset:0;background:rgba(10,8,6,.96);z-index:150;display:none;align-items:center;justify-content:center;padding:20px;backdrop-filter:blur(8px)">
  <div style="background:var(--surface);border:1px solid var(--border);border-radius:8px;padding:24px;width:100%;max-width:380px">
    <div style="font-family:var(--ff-display);font-size:24px;color:var(--amber);letter-spacing:.1em;margin-bottom:6px">Rename Roll</div>
    <div id="rename-current" style="font-family:var(--ff-mono);font-size:9px;color:var(--muted);letter-spacing:.12em;margin-bottom:16px"></div>
    <input id="rename-input" class="settings-input" type="text" maxlength="31" placeholder="my_summer_roll" autocomplete="off" autocapitalize="off" spellcheck="false" onkeydown="if(event.key==='Enter')submitRename()">
    <div style="font-family:var(--ff-mono);font-size:9px;color:var(--muted);margin-top:8px;line-height:1.6">Letters, numbers, underscores and hyphens only. Spaces become underscores.</div>
    <div style="display:flex;gap:8px;margin-top:18px">
      <button class="load-btn" style="margin:0;flex:1;font-size:16px;padding:11px" onclick="submitRename()">Rename</button>
      <button class="new-roll-btn" onclick="closeRenameModal()">Cancel</button>
    </div>
  </div>
</div>

<!-- LIGHTBOX -->
<div id="lightbox">
  <img id="lb-img" src="" alt="">
  <div class="lb-name" id="lb-name"></div>
  <div class="lb-actions">
    <a id="lb-dl" href="#" class="lb-btn primary" download>Download</a>
    <button id="lb-delete" class="lb-btn ghost" onclick="deleteCurrentPhoto()">Delete</button>
    <button class="lb-btn ghost" onclick="closeLightbox()">Close</button>
  </div>
</div>
<div id="toast"></div>

<script>
// ============================================================
// FILM DEFINITIONS
// Look is fully baked by the OV2640 sensor via special_effect.
// effect: 0=none 1=negative 2=grayscale 3=red 4=green 5=blue 6=sepia
// ============================================================
const FILMS = [
  { id:'natural',  name:'Natural',   desc:'True to life, no processing',
    swatch:'linear-gradient(135deg,#8a7060,#c4a880 40%,#e8c898 70%,#a08060)',
    sensor:{brightness:1,saturation:0,contrast:0,effect:0} },
  { id:'kodak',    name:'Kodak',     desc:'Warm sepia tones',
    swatch:'linear-gradient(135deg,#9a7050,#d4a870 40%,#f0d090 70%,#b08050)',
    sensor:{brightness:1,saturation:2,contrast:1,effect:6} },
  { id:'ilford',   name:'Ilford',    desc:'True grayscale, high contrast',
    swatch:'linear-gradient(135deg,#2a2a2a,#888 40%,#ccc 70%,#555)',
    sensor:{brightness:0,saturation:-2,contrast:2,effect:2} },
  { id:'velvia',   name:'Velvia',    desc:'Vivid, punchy, saturated',
    swatch:'linear-gradient(135deg,#6a3020,#d06030 40%,#e8b040 70%,#205050)',
    sensor:{brightness:0,saturation:2,contrast:2,effect:0} },
  { id:'faded',    name:'Faded',     desc:'Light, washed sepia look',
    swatch:'linear-gradient(135deg,#b09880,#d8c4a8 40%,#ede0c8 70%,#c0a888)',
    sensor:{brightness:2,saturation:-1,contrast:-1,effect:6} },
  { id:'cyano',    name:'Cyanotype', desc:'Blueprint blue toning',
    swatch:'linear-gradient(135deg,#102040,#204880 40%,#4080c0 70%,#183060)',
    sensor:{brightness:0,saturation:1,contrast:1,effect:5} },
  { id:'redscale', name:'Redscale',  desc:'Hot reds, expired film look',
    swatch:'linear-gradient(135deg,#6a1008,#c82010 40%,#e86020 70%,#902008)',
    sensor:{brightness:0,saturation:2,contrast:1,effect:3} },
  { id:'xpro',     name:'X-Pro',    desc:'Cross-process green cast',
    swatch:'linear-gradient(135deg,#203828,#408050 40%,#80c878 70%,#305840)',
    sensor:{brightness:-1,saturation:2,contrast:2,effect:4} },
];

// ============================================================
// STATE
// ============================================================
let selectedFilm = null;  // on load screen
let activeFilm   = null;  // loaded roll
let rollDeveloped = false; // true after developing, false while shooting
let framesTaken  = 0;
let lastFilename = null;
let currentGalleryDir = null;
let currentLightboxFile = null;
let currentLightboxIsPreview = false;
let appMode      = 'analog';
let currentSettings = { mode:'analog', ssid:'ESP32-Camera', password:'camera123' };

// ============================================================
// INIT
// ============================================================
document.addEventListener('DOMContentLoaded', () => {
  Promise.all([
    fetch('/settings').then(r=>r.json()).catch(() => ({ mode:'analog', ssid:'ESP32-Camera', password:'camera123' })),
    fetch('/status').then(r=>r.json()).catch(() => ({ film:'', frames:0, developed:false, mode:'analog' }))
  ]).then(([settings, d]) => {
    currentSettings = settings;
    appMode = settings.mode || d.mode || 'analog';
    rollDeveloped = d.developed || false;
    renderMainScreen();
    if (appMode === 'digital') {
      activeFilm = FILMS[0];
      framesTaken = d.frames || 0;
      startDigitalShoot(false);
    } else if (d.film && !d.developed && d.frames > 0) {
      renderRolls();
      activeFilm  = FILMS.find(f => f.id === d.film) || null;
      framesTaken = d.frames;
      resumeShoot();
    } else if (d.film && d.developed && d.lastDir) {
      renderRolls();
      activeFilm = FILMS.find(f => f.id === d.film) || null;
      fetch('/list?dir=' + encodeURIComponent(d.lastDir)).then(r=>r.json()).then(photos => renderGallery(photos, d.lastDir));
      showScreen('screen-gallery');
    } else {
      renderRolls();
    }
  }).catch(()=>{ renderRolls(); renderMainScreen(); });
});

// ============================================================
// SCREEN HELPERS
// ============================================================
function showScreen(id) {
  document.querySelectorAll('.screen').forEach(s => s.classList.remove('active'));
  document.getElementById(id).classList.add('active');
}

function renderMainScreen() {
  const title = document.getElementById('load-title');
  const sub = document.getElementById('load-sub');
  const grid = document.getElementById('roll-grid');
  const loadBtn = document.getElementById('load-btn');
  const archiveBtn = document.getElementById('archive-btn');
  if (appMode === 'digital') {
    title.textContent = 'Digital Camera';
    sub.innerHTML = '&#8212; Unlimited shots &#183; instant gallery';
    grid.style.display = 'none';
    loadBtn.disabled = false;
    loadBtn.textContent = 'Open Camera';
    archiveBtn.style.display = 'none';
  } else {
    title.textContent = 'Load Film';
    sub.innerHTML = '&#8212; Choose your roll &#183; 36 exposures';
    grid.style.display = 'grid';
    loadBtn.textContent = 'Load Roll';
    loadBtn.disabled = !selectedFilm;
    archiveBtn.style.display = 'block';
  }
}

function openSettings() {
  document.getElementById('mode-select').value  = currentSettings.mode || appMode;
  document.getElementById('flash-select').value = currentSettings.flash === false ? '0' : '1';
  document.getElementById('ssid-input').value   = currentSettings.ssid || '';
  document.getElementById('password-input').value = currentSettings.password || '';
  showScreen('screen-settings');
}

function goToGalleryHeader() {
  if (appMode === 'digital') { goToDevelop(); }
  else { goToArchive(); }
}

function goToLoadScreen() {
  selectedFilm = null;
  document.querySelectorAll('.roll-card').forEach(c => c.classList.remove('selected'));
  renderMainScreen();
  showScreen('screen-load');
}

function goBack() {
  if (appMode === 'digital') { showScreen('screen-shoot'); return; }
  // Only return to shoot screen if a roll is actually in progress (not yet developed)
  if (activeFilm && !rollDeveloped && framesTaken < 36) { showScreen('screen-shoot'); return; }
  goToLoadScreen();
}

async function saveSettings() {
  const body = new URLSearchParams({
    mode:     document.getElementById('mode-select').value,
    flash:    document.getElementById('flash-select').value,
    ssid:     document.getElementById('ssid-input').value.trim(),
    password: document.getElementById('password-input').value.trim()
  });
  try {
    const r = await fetch('/settings', { method:'POST', headers:{'Content-Type':'application/x-www-form-urlencoded'}, body });
    const d = await r.json();
    if (!d.ok) { showToast('SETTINGS SAVE FAILED'); return; }
    showToast('SETTINGS SAVED â€” REBOOTING');
  } catch(e) {
    showToast('SETTINGS SAVE FAILED');
  }
}

// ============================================================
// ROLL SELECTION
// ============================================================
const FILM_SVGS = {
  natural:  `<svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" id="film_natural" version="1.1" viewBox="0 0 800 800"><path d="m381.34 115.97 84.35 39.09 189.28 3.92 128.2-36.84 4.12 52.8-2.75 442.97-.68 64.46L739 729.69 595.29 762.6l-166.63-30.17-47.32-46.63z" style="fill:#fff"/><ellipse cx="581.23" cy="105.03" style="fill:#353535" rx="199.89" ry="66.55"/><path d="m375.86 510.94-292.8 3.43-30.17-36.34 2.74-187.2 27.43-37.72 284.28-2.05 14 10.89z" style="fill:#1e1e1e"/><path d="M360.09 141.34H11.06v489.6l364.11 6.17V532.2l-4.8-13.28H92.1l-27.71-21.49-4.65-44.81-.68-169.23 19.19-30.28 71.32-2.05h231.77V141.34z" style="fill:#353535"/><path d="M583.67 771.16c-121.3 0-216.33-45.69-216.33-104.02V644.7H11.54C5.17 644.7 0 639.54 0 633.16V137.7c0-6.37 5.16-11.54 11.54-11.54h355.8v-25.5c0-47.15 108.83-71.82 216.33-71.82S800 53.51 800 100.66v566.47c0 58.33-95.02 104.02-216.33 104.02Zm-193.25-636.3v532.27c0 39.09 77.65 80.95 193.25 80.95s193.25-41.85 193.25-80.95V134.86c-37.62 24.8-115.78 37.62-193.25 37.62s-155.63-12.81-193.25-37.62M23.08 621.62h344.27v-93.36H95.32c-27.63 0-50.11-22.48-50.11-50.11V288.99c0-27.63 22.48-50.11 50.11-50.11h272.03v-89.65H23.08v472.38Zm72.24-359.67c-14.9 0-27.03 12.12-27.03 27.03v189.16c0 14.91 12.13 27.03 27.03 27.03h272.03V261.95zM583.67 51.92c-117.97 0-193.25 28.87-193.25 48.74s75.28 48.74 193.25 48.74 193.25-28.87 193.25-48.74-75.28-48.74-193.25-48.74M422.39 657.08c-6.37 0-11.54-5.16-11.54-11.54V183.12c0-6.37 5.16-11.54 11.54-11.54s11.54 5.16 11.54 11.54v462.43c0 6.37-5.17 11.54-11.54 11.54Zm-84.83-47.69h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1V564.8H64.39zm273.17-359.66h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1v-21.51H64.39zm519.28-70.85c-23.47 0-100.05-2.31-100.05-32.06S560.2 68.6 583.67 68.6s100.05 2.31 100.05 32.06-76.58 32.06-100.05 32.06m-71.3-32.06c11.44 4.16 36.01 8.98 71.3 8.98s59.87-4.82 71.3-8.98c-11.44-4.16-36.01-8.98-71.3-8.98s-59.87 4.82-71.3 8.98"/></svg>`,
  kodak:    `<svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" id="film_kodak" version="1.1" viewBox="0 0 800 800"><path d="m381.34 115.97 84.35 39.09 189.28 3.92 128.2-36.84 4.12 52.8-2.75 442.97-.68 64.46L739 729.69 595.29 762.6l-166.63-30.17-47.32-46.63z" style="fill:#feb827"/><ellipse cx="581.23" cy="105.03" style="fill:#353535" rx="199.89" ry="66.55"/><path d="m375.86 510.94-292.8 3.43-30.17-36.34 2.74-187.2 27.43-37.72 284.28-2.05 14 10.89z" style="fill:#1e1e1e"/><path d="M360.09 141.34H11.06v489.6l364.11 6.17V532.2l-4.8-13.28H92.1l-27.71-21.49-4.65-44.81-.68-169.23 19.19-30.28 71.32-2.05h231.77V141.34z" style="fill:#353535"/><path d="M583.67 771.16c-121.3 0-216.33-45.69-216.33-104.02V644.7H11.54C5.17 644.7 0 639.54 0 633.16V137.7c0-6.37 5.16-11.54 11.54-11.54h355.8v-25.5c0-47.15 108.83-71.82 216.33-71.82S800 53.51 800 100.66v566.47c0 58.33-95.02 104.02-216.33 104.02Zm-193.25-636.3v532.27c0 39.09 77.65 80.95 193.25 80.95s193.25-41.85 193.25-80.95V134.86c-37.62 24.8-115.78 37.62-193.25 37.62s-155.63-12.81-193.25-37.62M23.08 621.62h344.27v-93.36H95.32c-27.63 0-50.11-22.48-50.11-50.11V288.99c0-27.63 22.48-50.11 50.11-50.11h272.03v-89.65H23.08v472.38Zm72.24-359.67c-14.9 0-27.03 12.12-27.03 27.03v189.16c0 14.91 12.13 27.03 27.03 27.03h272.03V261.95zM583.67 51.92c-117.97 0-193.25 28.87-193.25 48.74s75.28 48.74 193.25 48.74 193.25-28.87 193.25-48.74-75.28-48.74-193.25-48.74M422.39 657.08c-6.37 0-11.54-5.16-11.54-11.54V183.12c0-6.37 5.16-11.54 11.54-11.54s11.54 5.16 11.54 11.54v462.43c0 6.37-5.17 11.54-11.54 11.54Zm-84.83-47.69h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1V564.8H64.39zm273.17-359.66h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1v-21.51H64.39zm519.28-70.85c-23.47 0-100.05-2.31-100.05-32.06S560.2 68.6 583.67 68.6s100.05 2.31 100.05 32.06-76.58 32.06-100.05 32.06m-71.3-32.06c11.44 4.16 36.01 8.98 71.3 8.98s59.87-4.82 71.3-8.98c-11.44-4.16-36.01-8.98-71.3-8.98s-59.87 4.82-71.3 8.98"/><path d="M471.06 380.57s0 5.8 5.81 13.27l125.25 144.33s3.33 4.15 6.65 0c3.32-3.32 121.12-140.2 121.12-140.2s9.1-9.13 9.1-21.57v-84.6c0-14.93-13.26-20.76-23.22-20.76H492.62c-11.61 0-21.57 9.15-21.57 20.76v88.76Zm.78 175.52c.36 10.66 9.07 19.42 19.96 19.42h224.83c11.61.83 21.55-8.29 22.38-20.73V468.5c-1.66-9.95-11.59-4.97-11.59-4.97s-106.2 77.14-114.5 82.95c-9.12 7.47-16.6-.84-16.6-.84s-104.51-77.16-114.47-81.31c-9.95-4.15-9.94 6.65-9.94 6.65v82.95c-.05.73-.09 1.44-.07 2.15Z" style="fill:#ed1b2d"/><path d="M578.09 497.43h52.24v-14.95h-26.54l26.54-16.6v-17.4l-29.03 19.89-23.22-18.24v16.6l21.54 15.76h-21.54v14.95Zm.8-116.14H593c-1.66 2.49-1.65 5.79-1.65 9.1 0 14.1 9.11 18.28 19.89 18.28s20.73-5 20.73-18.28c0-4.15-1.64-7.47-4.13-10.78l2.49-.81v-10.78h-51.44zm0-58.9h51.44v-13.27h-19.08l19.08-11.62v-14.11l-24.06 14.11-14.92-12.43v13.27l13.27 10.78h-25.74v13.27Zm11.63 109.5c0 14.1 6.65 20.73 19.92 20.73 11.61 0 21.54-3.31 21.54-20.73 0-14.93-9.09-20.73-21.54-20.73-14.93 0-19.92 7.46-19.92 20.73m.84-70.52h11.59v-9.94c-2.49 0-4.13-2.5-4.13-5.81 0-4.98.82-5.79 4.97-6.62l3.33 14.92c1.66 6.64 7.45 10.78 12.43 10.78 5.81 0 12.43-4.14 12.43-12.43 0-6.64-4.12-12.45-5.78-14.11h4.13v-11.62H601.3c-7.47 0-9.94 7.49-9.94 14.95v19.89Zm7.46 70.52c0-6.64 8.31-6.65 11.62-6.65 4.15 0 12.43.01 12.43 6.65 0 7.47-8.28 7.46-12.43 7.46-2.49 0-11.62 0-11.62-7.46m.8-43.98c0-5.81 4.16-6.62 11.62-6.62 2.49 0 12.46-.02 12.46 6.62 0 7.47-9.98 7.49-12.46 7.49-2.49 0-11.62-.03-11.62-7.49m10.82-48.11s4.13-.83 6.62 0c3.32 0 5.81 3.33 5.81 6.65s-1.65 5.81-4.97 5.81-4.97-3.31-4.97-4.97c-.83-.83-2.49-5.83-2.49-7.49" style="fill:#feb827"/></svg>`,
  ilford:   `<svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" id="film_ilford" version="1.1" viewBox="0 0 800 800"><path d="m381.34 115.97 84.35 39.09 189.28 3.92 128.2-36.84 4.12 52.8-2.75 442.97-.68 64.46L739 729.69 595.29 762.6l-166.63-30.17-47.32-46.63z" style="fill:#fff"/><ellipse cx="581.23" cy="105.03" style="fill:#353535" rx="199.89" ry="66.55"/><path d="m375.86 510.94-292.8 3.43-30.17-36.34 2.74-187.2 27.43-37.72 284.28-2.05 14 10.89z" style="fill:#1e1e1e"/><path d="M360.09 141.34H11.06v489.6l364.11 6.17V532.2l-4.8-13.28H92.1l-27.71-21.49-4.65-44.81-.68-169.23 19.19-30.28 71.32-2.05h231.77V141.34z" style="fill:#353535"/><path d="M583.67 771.16c-121.3 0-216.33-45.69-216.33-104.02V644.7H11.54C5.17 644.7 0 639.54 0 633.16V137.7c0-6.37 5.16-11.54 11.54-11.54h355.8v-25.5c0-47.15 108.83-71.82 216.33-71.82S800 53.51 800 100.66v566.47c0 58.33-95.02 104.02-216.33 104.02Zm-193.25-636.3v532.27c0 39.09 77.65 80.95 193.25 80.95s193.25-41.85 193.25-80.95V134.86c-37.62 24.8-115.78 37.62-193.25 37.62s-155.63-12.81-193.25-37.62M23.08 621.62h344.27v-93.36H95.32c-27.63 0-50.11-22.48-50.11-50.11V288.99c0-27.63 22.48-50.11 50.11-50.11h272.03v-89.65H23.08v472.38Zm72.24-359.67c-14.9 0-27.03 12.12-27.03 27.03v189.16c0 14.91 12.13 27.03 27.03 27.03h272.03V261.95zM583.67 51.92c-117.97 0-193.25 28.87-193.25 48.74s75.28 48.74 193.25 48.74 193.25-28.87 193.25-48.74-75.28-48.74-193.25-48.74M422.39 657.08c-6.37 0-11.54-5.16-11.54-11.54V183.12c0-6.37 5.16-11.54 11.54-11.54s11.54 5.16 11.54 11.54v462.43c0 6.37-5.17 11.54-11.54 11.54Zm-84.83-47.69h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1V564.8H64.39zm273.17-359.66h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1v-21.51H64.39zm519.28-70.85c-23.47 0-100.05-2.31-100.05-32.06S560.2 68.6 583.67 68.6s100.05 2.31 100.05 32.06-76.58 32.06-100.05 32.06m-71.3-32.06c11.44 4.16 36.01 8.98 71.3 8.98s59.87-4.82 71.3-8.98c-11.44-4.16-36.01-8.98-71.3-8.98s-59.87 4.82-71.3 8.98"/><path id="ILFORD" d="M552.45 444.9c-.23 9.18 1.94 18.59 6.14 25.87 6.04 12.07 18.24 21.33 30.57 24.95 4.48 1.38 9.28 2.04 14.11 1.96 7.94 0 15.91-1.95 22.47-5.23 12.16-5.61 22.47-17.37 26.52-29.79 5.08-13.88 2.75-31.17-4.83-43.24-9.75-15.53-26.05-24.22-44.16-24.3h-1.05c-9.75.08-18.19 3.1-25.87 8.1-9.66 6.12-17.28 15.6-20.9 25.87-1.84 4.85-2.87 10.3-3 15.81m3.26 221.57h94.98v-34.49h-94.98zm0-39.58h94.98v-61.66h-28.48v27.7h-66.5v33.97Zm0-64.93h94.98v-34.49h-32.92v-27.04h-24.69v27.04h-12.67v-30.31h-24.69v64.8Zm0-169.84h94.98v-32.4h-32.92l32.92-17.64v-76.43c-.17-7.93-1.07-15.88-3.92-22.86-5.77-15.87-22.72-24.58-38.67-25.87-17.07-1.99-33.33 2.36-44.03 15.81-4.83 6.39-7.93 15.76-8.36 24.04v49.64h91.58l-35.67 21.95-.78.39c-.08-.86-.22-1.36-.39-1.96-1.72-7.33-8.17-14.09-15.42-16.33-7.77-2.5-17.88-2.1-24.95 1.7-9.23 4.66-13.5 14.81-14.11 24.56l-.26 14.5v40.89Zm24.83-32.4c0-3.11-.04-6.43.13-9.54.17-2.84 2.34-5.67 5.1-6.53 3.37-1.12 7.25-.1 9.67 2.48 1.21 1.55 1.96 3.16 1.96 5.23v8.36h-16.85Zm3.52-87.66s.3-.7.13-1.05c-.26-7.33 1.64-14.49 8.36-18.03 7.5-3.88 18.09-2.74 24.56 2.61 4.48 4.14 5.53 10.11 5.1 15.81v.65h-38.15Zm.13 175.19c-.03-.76-.06-1.58 0-2.35.52-4.39 3.05-8.48 7.19-10.97 3.28-2.16 7.58-3.4 11.89-3.4 4.57-.09 9.27 1.07 12.8 3.4 4.91 3.54 7.09 9.04 6.14 15.15-.52 4.92-3.84 9.65-8.49 11.63-3.02 1.47-6.74 2.22-10.45 2.22-4.14 0-8.35-.98-11.63-2.87-4.83-2.64-7.26-7.46-7.45-12.8Z"/></svg>`,
  velvia:   `<svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" id="film_velvia" version="1.1" viewBox="0 0 800 800"><path d="m381.34 115.97 84.35 39.09 189.28 3.92 128.2-36.84 4.12 52.8-2.75 442.97-.68 64.46L739 729.69 595.29 762.6l-166.63-30.17-47.32-46.63z" style="fill:#73c67f"/><ellipse cx="581.23" cy="105.03" style="fill:#353535" rx="199.89" ry="66.55"/><path d="m375.86 510.94-292.8 3.43-30.17-36.34 2.74-187.2 27.43-37.72 284.28-2.05 14 10.89z" style="fill:#1e1e1e"/><path d="M360.09 141.34H11.06v489.6l364.11 6.17V532.2l-4.8-13.28H92.1l-27.71-21.49-4.65-44.81-.68-169.23 19.19-30.28 71.32-2.05h231.77V141.34z" style="fill:#353535"/><path d="M583.67 771.16c-121.3 0-216.33-45.69-216.33-104.02V644.7H11.54C5.17 644.7 0 639.54 0 633.16V137.7c0-6.37 5.16-11.54 11.54-11.54h355.8v-25.5c0-47.15 108.83-71.82 216.33-71.82S800 53.51 800 100.66v566.47c0 58.33-95.02 104.02-216.33 104.02Zm-193.25-636.3v532.27c0 39.09 77.65 80.95 193.25 80.95s193.25-41.85 193.25-80.95V134.86c-37.62 24.8-115.78 37.62-193.25 37.62s-155.63-12.81-193.25-37.62M23.08 621.62h344.27v-93.36H95.32c-27.63 0-50.11-22.48-50.11-50.11V288.99c0-27.63 22.48-50.11 50.11-50.11h272.03v-89.65H23.08v472.38Zm72.24-359.67c-14.9 0-27.03 12.12-27.03 27.03v189.16c0 14.91 12.13 27.03 27.03 27.03h272.03V261.95zM583.67 51.92c-117.97 0-193.25 28.87-193.25 48.74s75.28 48.74 193.25 48.74 193.25-28.87 193.25-48.74-75.28-48.74-193.25-48.74M422.39 657.08c-6.37 0-11.54-5.16-11.54-11.54V183.12c0-6.37 5.16-11.54 11.54-11.54s11.54 5.16 11.54 11.54v462.43c0 6.37-5.17 11.54-11.54 11.54Zm-84.83-47.69h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1V564.8H64.39zm273.17-359.66h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1v-21.51H64.39zm519.28-70.85c-23.47 0-100.05-2.31-100.05-32.06S560.2 68.6 583.67 68.6s100.05 2.31 100.05 32.06-76.58 32.06-100.05 32.06m-71.3-32.06c11.44 4.16 36.01 8.98 71.3 8.98s59.87-4.82 71.3-8.98c-11.44-4.16-36.01-8.98-71.3-8.98s-59.87 4.82-71.3 8.98"/><path d="m455.08 504.3 175.71-209.8 7.81-9.31-5.48 241.94-78.37 93.76-99.66-.02z" style="fill:#fff"/><path d="M519.1 576.61v-10.73h17.96c1.11 0 2.23-.07 3.33-.22a7.77 7.77 0 0 0 5.13-2.99 7.8 7.8 0 0 0 1.51-3.65c.12-.87.19-1.74.22-2.62.17-5.93.17-11.86 0-17.78-.03-.87-.1-1.75-.22-2.62a7.74 7.74 0 0 0-2.99-5.13 7.66 7.66 0 0 0-3.65-1.51c-1.1-.15-2.22-.23-3.33-.23h-29.93v13.2h25.62c.45 0 .9.04 1.34.13a2.834 2.834 0 0 1 2.27 2.49c.17 1.7.17 3.41 0 5.11a2.823 2.823 0 0 1-2.27 2.49c-.44.09-.89.13-1.34.13h-25.62v23.94h-7.65V483.3l-20.39 24.31v80.48c0 1.03.09 2.05.26 3.07a7.49 7.49 0 0 0 6.14 6.14c1.01.17 2.04.26 3.07.26h58.81v-20.93h-28.26Z" style="fill:#e53517"/><path d="M507.13 521.63h30.28c1.62 0 3.24.1 4.85.29 2.5.31 4.87 1.29 6.86 2.84 2.89 2.26 4.77 5.58 5.21 9.23q.3 2.415.3 4.86v50.03l12.43-14.82v-38.69c0-2.91-.34-5.8-1-8.63a23.21 23.21 0 0 0-17.31-17.32c-2.83-.66-5.73-1-8.63-1h-32.97v13.21ZM507.14 501.08v-13.2h40.23v13.2z" style="fill:#e53517"/><path d="M574.32 480.55v84.87l19.77-23.56V470.5c0-1.32-.11-2.63-.34-3.94a7.49 7.49 0 0 0-5.52-5.97c-.44-.11-.88-.21-1.33-.28q-1.83-.3-3.69-.3h-64.17l-17.24 20.54z" style="fill:#e53517"/><path d="M455.08 262.06V504.3l178.04-212.58v235.41l-78.37 93.76h187.24V262.14z" style="fill:#05a45c"/><path d="M712.69 597.98h-42.73c-3.95 0-7.66.07-7.66-6.83v-27.98h7.57v19.68h13.18v-18.66h7.7v18.66h21.94zM662.3 462.59v-14.64h50.39v14.64zM697.26 510.57h-3.37v-14.08h7.21c1.72 0 5.65.1 5.72-4.83.06-3.98-1.54-5.29-5.41-5.3H662.3v-14.65h33.38c6.39 0 12.14.91 15.93 6.84 4.18 6.53 3.11 20.42.03 25.47-3.98 6.51-9.56 6.54-14.38 6.54ZM712.69 342.98v22.72c0 5.27-.16 8.54-6.88 8.53H662.3v-14.62h42.87v-16.62h7.52ZM712.69 339.11H662.3v-16.29l33.08-8.68-33.08-8.83v-20.12h50.39v14.2l-35.67-.02 35.67 9.6v12.23l-35.41 9.84h35.41zM662.3 558.34v-14.42h37.79c3.25 0 4.73-.52 5.62-2.07 1.5-2.59 1.5-7.66.04-10.26-.81-1.44-1.94-2.24-5.43-2.25-3.88-.01-38.03 0-38.03 0v-14.37h33c2.5 0 8.12-.09 11.34 1.84 2.62 1.57 5.09 4.15 6.38 8.76 1.77 6.32 1.73 15.93-.04 21.82-1.92 6.38-5.72 10.94-13.97 10.96-8.25.01-36.71 0-36.71 0ZM712.69 438.44h-42.73c-3.95 0-7.66.07-7.66-6.83v-27.98h7.57v19.68h13.18v-18.66h7.7v18.66h21.94zM662.3 398.4v-14.64h50.39v14.64z" style="fill:#fff"/><path d="M419.16 298.01h358.78v286.93H419.16z" style="fill:none;stroke:#000;stroke-width:.5px" transform="rotate(-90 598.545 441.475)"/></svg>`,
  faded:    `<svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" id="film_faded" version="1.1" viewBox="0 0 800 800"><path d="m381.34 115.97 84.35 39.09 189.28 3.92 128.2-36.84 4.12 52.8-2.75 442.97-.68 64.46L739 729.69 595.29 762.6l-166.63-30.17-47.32-46.63z" style="fill:#fff"/><ellipse cx="581.23" cy="105.03" style="fill:#353535" rx="199.89" ry="66.55"/><path d="m375.86 510.94-292.8 3.43-30.17-36.34 2.74-187.2 27.43-37.72 284.28-2.05 14 10.89z" style="fill:#1e1e1e"/><path d="M360.09 141.34H11.06v489.6l364.11 6.17V532.2l-4.8-13.28H92.1l-27.71-21.49-4.65-44.81-.68-169.23 19.19-30.28 71.32-2.05h231.77V141.34z" style="fill:#353535"/><path d="M583.67 771.16c-121.3 0-216.33-45.69-216.33-104.02V644.7H11.54C5.17 644.7 0 639.54 0 633.16V137.7c0-6.37 5.16-11.54 11.54-11.54h355.8v-25.5c0-47.15 108.83-71.82 216.33-71.82S800 53.51 800 100.66v566.47c0 58.33-95.02 104.02-216.33 104.02Zm-193.25-636.3v532.27c0 39.09 77.65 80.95 193.25 80.95s193.25-41.85 193.25-80.95V134.86c-37.62 24.8-115.78 37.62-193.25 37.62s-155.63-12.81-193.25-37.62M23.08 621.62h344.27v-93.36H95.32c-27.63 0-50.11-22.48-50.11-50.11V288.99c0-27.63 22.48-50.11 50.11-50.11h272.03v-89.65H23.08v472.38Zm72.24-359.67c-14.9 0-27.03 12.12-27.03 27.03v189.16c0 14.91 12.13 27.03 27.03 27.03h272.03V261.95zM583.67 51.92c-117.97 0-193.25 28.87-193.25 48.74s75.28 48.74 193.25 48.74 193.25-28.87 193.25-48.74-75.28-48.74-193.25-48.74M422.39 657.08c-6.37 0-11.54-5.16-11.54-11.54V183.12c0-6.37 5.16-11.54 11.54-11.54s11.54 5.16 11.54 11.54v462.43c0 6.37-5.17 11.54-11.54 11.54Zm-84.83-47.69h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1V564.8H64.39zm273.17-359.66h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1v-21.51H64.39zm519.28-70.85c-23.47 0-100.05-2.31-100.05-32.06S560.2 68.6 583.67 68.6s100.05 2.31 100.05 32.06-76.58 32.06-100.05 32.06m-71.3-32.06c11.44 4.16 36.01 8.98 71.3 8.98s59.87-4.82 71.3-8.98c-11.44-4.16-36.01-8.98-71.3-8.98s-59.87 4.82-71.3 8.98"/><g id="text2995"><path id="path3000" d="m557.66 670.05.36-90.68h81.24v23.56h-63.8v43.39h36.49c9.28 0 14.92 2.45 18.69 5.06 3.72 2.61 5.66 5.62 6.83 8.79s1.89 7.11 1.89 14.13c0 4.16.05 8-.23 16.49l-18.65-.26q0-.84.15-4.38c.15-2.73.23-4.84.23-6.33 0-3.8-.62-7.48-2.6-8.56-1.94-.81-4.92-1.21-13.64-1.21h-46.97" style="fill:#0091ca"/><path id="path3002" d="M597.49 567.34c-7.14 0-14.05-2.2-20.73-6.59s-11.78-10.6-15.3-18.62c-3.52-8.08-5.28-17.09-5.28-27.02 0-15.34 4-27.91 12.01-37.71 7.96-9.8 18.03-14.7 30.22-14.7 12.29 0 22.49 4.96 30.6 14.89 8.06 9.86 12.09 22.31 12.09 37.33 0 9.29-1.68 18.17-5.05 26.64-3.37 8.4-8.65 14.8-15.12 19.19-6.53 4.39-14.1 6.59-23.43 6.59m1.14-24.25c8.06 0 15.31-3.11 19.59-7.88s8.23-13.19 8.23-20.19-3.24-15.19-7.51-19.91c-4.3-4.76-12.31-7.88-20.47-7.88-7.96 0-16.24 2.57-20.52 7.34-4.28 4.71-8.23 13.45-8.23 20.45s4.49 15.41 8.77 20.19c4.28 4.77 12.08 7.88 20.14 7.88" style="fill:#0091ca"/><path id="path3004" d="M558.02 449.58v-34.41l64.69-24.59-64.69-24.78v-33.95h81.24v23.89h-62.15l62.15 24.13v22.36l-62.15 23.01h62.15v24.34z" style="fill:#0091ca"/><path id="path3008" d="M597.49 320.63c-7.14 0-14.05-2.2-20.73-6.59s-11.78-10.6-15.3-18.62c-3.52-8.08-5.28-17.09-5.28-27.02 0-15.34 4-27.91 12.01-37.71 7.96-9.8 18.03-14.7 30.22-14.7 12.29 0 22.49 4.96 30.6 14.89 8.06 9.86 12.09 22.31 12.09 37.33 0 9.29-1.68 18.17-5.05 26.64-3.37 8.4-8.65 14.8-15.12 19.19-6.53 4.39-14.1 6.59-23.43 6.59m1.14-24.25c8.06 0 15.31-3.11 19.59-7.88s8.23-13.19 8.23-20.19-3.24-15.19-7.51-19.91c-4.3-4.76-12.31-7.88-20.47-7.88-7.96 0-16.24 2.57-20.52 7.34-4.28 4.71-8.23 13.45-8.23 20.45s4.49 15.41 8.77 20.19c4.28 4.77 12.08 7.88 20.14 7.88" style="fill:#0091ca"/></g></svg>`,
  cyano:    `<svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" id="film_cyano" version="1.1" viewBox="0 0 800 800"><path d="m381.34 115.97 84.35 39.09 189.28 3.92 128.2-36.84 4.12 52.8-2.75 442.97-.68 64.46L739 729.69 595.29 762.6l-166.63-30.17-47.32-46.63z" style="fill:#3a4eaf"/><ellipse cx="581.23" cy="105.03" style="fill:#353535" rx="199.89" ry="66.55"/><path d="m375.86 510.94-292.8 3.43-30.17-36.34 2.74-187.2 27.43-37.72 284.28-2.05 14 10.89z" style="fill:#1e1e1e"/><path d="M360.09 141.34H11.06v489.6l364.11 6.17V532.2l-4.8-13.28H92.1l-27.71-21.49-4.65-44.81-.68-169.23 19.19-30.28 71.32-2.05h231.77V141.34z" style="fill:#353535"/><path d="M583.67 771.16c-121.3 0-216.33-45.69-216.33-104.02V644.7H11.54C5.17 644.7 0 639.54 0 633.16V137.7c0-6.37 5.16-11.54 11.54-11.54h355.8v-25.5c0-47.15 108.83-71.82 216.33-71.82S800 53.51 800 100.66v566.47c0 58.33-95.02 104.02-216.33 104.02Zm-193.25-636.3v532.27c0 39.09 77.65 80.95 193.25 80.95s193.25-41.85 193.25-80.95V134.86c-37.62 24.8-115.78 37.62-193.25 37.62s-155.63-12.81-193.25-37.62M23.08 621.62h344.27v-93.36H95.32c-27.63 0-50.11-22.48-50.11-50.11V288.99c0-27.63 22.48-50.11 50.11-50.11h272.03v-89.65H23.08v472.38Zm72.24-359.67c-14.9 0-27.03 12.12-27.03 27.03v189.16c0 14.91 12.13 27.03 27.03 27.03h272.03V261.95zM583.67 51.92c-117.97 0-193.25 28.87-193.25 48.74s75.28 48.74 193.25 48.74 193.25-28.87 193.25-48.74-75.28-48.74-193.25-48.74M422.39 657.08c-6.37 0-11.54-5.16-11.54-11.54V183.12c0-6.37 5.16-11.54 11.54-11.54s11.54 5.16 11.54 11.54v462.43c0 6.37-5.17 11.54-11.54 11.54Zm-84.83-47.69h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1V564.8H64.39zm273.17-359.66h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1v-21.51H64.39zm519.28-70.85c-23.47 0-100.05-2.31-100.05-32.06S560.2 68.6 583.67 68.6s100.05 2.31 100.05 32.06-76.58 32.06-100.05 32.06m-71.3-32.06c11.44 4.16 36.01 8.98 71.3 8.98s59.87-4.82 71.3-8.98c-11.44-4.16-36.01-8.98-71.3-8.98s-59.87 4.82-71.3 8.98"/></svg>`,
  redscale: `<svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" id="film_redscale" version="1.1" viewBox="0 0 800 800"><path d="m381.34 115.97 84.35 39.09 189.28 3.92 128.2-36.84 4.12 52.8-2.75 442.97-.68 64.46L739 729.69 595.29 762.6l-166.63-30.17-47.32-46.63z" style="fill:#ed3535"/><ellipse cx="581.23" cy="105.03" style="fill:#353535" rx="199.89" ry="66.55"/><path d="m375.86 510.94-292.8 3.43-30.17-36.34 2.74-187.2 27.43-37.72 284.28-2.05 14 10.89z" style="fill:#1e1e1e"/><path d="M360.09 141.34H11.06v489.6l364.11 6.17V532.2l-4.8-13.28H92.1l-27.71-21.49-4.65-44.81-.68-169.23 19.19-30.28 71.32-2.05h231.77V141.34z" style="fill:#353535"/><path d="M583.67 771.16c-121.3 0-216.33-45.69-216.33-104.02V644.7H11.54C5.17 644.7 0 639.54 0 633.16V137.7c0-6.37 5.16-11.54 11.54-11.54h355.8v-25.5c0-47.15 108.83-71.82 216.33-71.82S800 53.51 800 100.66v566.47c0 58.33-95.02 104.02-216.33 104.02Zm-193.25-636.3v532.27c0 39.09 77.65 80.95 193.25 80.95s193.25-41.85 193.25-80.95V134.86c-37.62 24.8-115.78 37.62-193.25 37.62s-155.63-12.81-193.25-37.62M23.08 621.62h344.27v-93.36H95.32c-27.63 0-50.11-22.48-50.11-50.11V288.99c0-27.63 22.48-50.11 50.11-50.11h272.03v-89.65H23.08v472.38Zm72.24-359.67c-14.9 0-27.03 12.12-27.03 27.03v189.16c0 14.91 12.13 27.03 27.03 27.03h272.03V261.95zM583.67 51.92c-117.97 0-193.25 28.87-193.25 48.74s75.28 48.74 193.25 48.74 193.25-28.87 193.25-48.74-75.28-48.74-193.25-48.74M422.39 657.08c-6.37 0-11.54-5.16-11.54-11.54V183.12c0-6.37 5.16-11.54 11.54-11.54s11.54 5.16 11.54 11.54v462.43c0 6.37-5.17 11.54-11.54 11.54Zm-84.83-47.69h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1V564.8H64.39zm273.17-359.66h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1v-21.51H64.39zm519.28-70.85c-23.47 0-100.05-2.31-100.05-32.06S560.2 68.6 583.67 68.6s100.05 2.31 100.05 32.06-76.58 32.06-100.05 32.06m-71.3-32.06c11.44 4.16 36.01 8.98 71.3 8.98s59.87-4.82 71.3-8.98c-11.44-4.16-36.01-8.98-71.3-8.98s-59.87 4.82-71.3 8.98"/></svg>`,
  xpro:     `<svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" id="film_xpro" version="1.1" viewBox="0 0 800 800"><path d="m381.34 115.97 84.35 39.09 189.28 3.92 128.2-36.84 4.12 52.8-2.75 442.97-.68 64.46L739 729.69 595.29 762.6l-166.63-30.17-47.32-46.63z" style="fill:#e9f91e"/><ellipse cx="581.23" cy="105.03" style="fill:#353535" rx="199.89" ry="66.55"/><path d="m375.86 510.94-292.8 3.43-30.17-36.34 2.74-187.2 27.43-37.72 284.28-2.05 14 10.89z" style="fill:#1e1e1e"/><path d="M360.09 141.34H11.06v489.6l364.11 6.17V532.2l-4.8-13.28H92.1l-27.71-21.49-4.65-44.81-.68-169.23 19.19-30.28 71.32-2.05h231.77V141.34z" style="fill:#353535"/><path d="M583.67 771.16c-121.3 0-216.33-45.69-216.33-104.02V644.7H11.54C5.17 644.7 0 639.54 0 633.16V137.7c0-6.37 5.16-11.54 11.54-11.54h355.8v-25.5c0-47.15 108.83-71.82 216.33-71.82S800 53.51 800 100.66v566.47c0 58.33-95.02 104.02-216.33 104.02Zm-193.25-636.3v532.27c0 39.09 77.65 80.95 193.25 80.95s193.25-41.85 193.25-80.95V134.86c-37.62 24.8-115.78 37.62-193.25 37.62s-155.63-12.81-193.25-37.62M23.08 621.62h344.27v-93.36H95.32c-27.63 0-50.11-22.48-50.11-50.11V288.99c0-27.63 22.48-50.11 50.11-50.11h272.03v-89.65H23.08v472.38Zm72.24-359.67c-14.9 0-27.03 12.12-27.03 27.03v189.16c0 14.91 12.13 27.03 27.03 27.03h272.03V261.95zM583.67 51.92c-117.97 0-193.25 28.87-193.25 48.74s75.28 48.74 193.25 48.74 193.25-28.87 193.25-48.74-75.28-48.74-193.25-48.74M422.39 657.08c-6.37 0-11.54-5.16-11.54-11.54V183.12c0-6.37 5.16-11.54 11.54-11.54s11.54 5.16 11.54 11.54v462.43c0 6.37-5.17 11.54-11.54 11.54Zm-84.83-47.69h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71V564.8h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1V564.8H64.39zm273.17-359.66h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08h-50.78c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08h27.71v-21.51h-27.71zm-77.71 23.08H52.86c-6.37 0-11.54-5.16-11.54-11.54v-44.59c0-6.37 5.16-11.54 11.54-11.54h50.78c6.37 0 11.54 5.16 11.54 11.54v44.59c0 6.37-5.16 11.54-11.54 11.54m-39.25-23.08H92.1v-21.51H64.39zm519.28-70.85c-23.47 0-100.05-2.31-100.05-32.06S560.2 68.6 583.67 68.6s100.05 2.31 100.05 32.06-76.58 32.06-100.05 32.06m-71.3-32.06c11.44 4.16 36.01 8.98 71.3 8.98s59.87-4.82 71.3-8.98c-11.44-4.16-36.01-8.98-71.3-8.98s-59.87 4.82-71.3 8.98"/><path d="M547.69 684.29v-433.9h81.41v433.9z" style="fill:#e2091a"/><path d="M563.87 667.61v-14.04h42.2v14.04h-42.2z" style="fill:#fff;stroke-miterlimit:3.86;stroke:#fff"/><path d="M584.93 632.52v-7.1h14.09v7.1zm-7.04 13.97v-35.05h28.19v35.05z" style="fill:#fff;fill-rule:evenodd"/><path d="M584.93 632.52v-7.1h14.09v7.1zm-7.04 13.97v-35.05h28.19v35.05z" style="stroke-miterlimit:3.86;fill:none;stroke:#fff"/><path d="M584.93 526.9v-7.1h14.09v7.1zm-7.04 13.98v-35.05h28.19v35.05z" style="fill:#fff;fill-rule:evenodd"/><path d="M584.93 526.9v-7.1h14.09v7.1zm-7.04 13.98v-35.05h28.19v35.05z" style="stroke-miterlimit:3.86;fill:none;stroke:#fff"/><path d="M577.89 386.09v-35.05h28.19v21.08h7.03v13.97zm7.04-13.98v-7.1h14.09v7.1z" style="fill:#fff;fill-rule:evenodd"/><path d="M577.89 386.09v-35.05h28.19v21.08h7.03v13.97zm7.04-13.98v-7.1h14.09v7.1z" style="stroke-miterlimit:3.86;fill:none;stroke:#fff"/><path d="M577.89 604.36v-49.27h7.07v-7.05h21.11v14.09H585v7.04h21.07v14.07H585v7.04h21.07v14.08h-28.18z" style="fill:#fff;stroke-miterlimit:3.86;stroke:#fff"/><path d="M577.89 498.77v-35.18h28.19v7h7.03v28.23h-7.03V477.7H599v21.12zm7.08-14.08v-7.08h7.02v7.08z" style="fill:#fff;fill-rule:evenodd"/><path d="M577.89 498.77v-35.18h28.19v7h7.03v28.23h-7.03V477.7H599v21.12l-21.11-.05Zm7.08-14.08v-7.08h7.02v7.08z" style="stroke-miterlimit:3.86;fill:none;stroke:#fff"/><path d="M577.89 456.56h28.18v-14.1h-21.11v-7.05h-7.07v21.15z" style="fill:#fff;stroke-miterlimit:3.86;stroke:#fff"/><path d="M584.93 414.34h14.09v-7.1h-14.09zm-7.04 7.02v-28.09h28.19v28.09h-7.03v6.96h-14.08v-6.96z" style="fill:#fff;fill-rule:evenodd"/><path d="M584.93 414.34h14.09v-7.1h-14.09v7.1Zm-7.04 7.02v-28.09h28.19v28.09h-7.03v6.96h-14.08v-6.96z" style="stroke-miterlimit:3.86;fill:none;stroke:#fff"/><path d="M577.89 308.74h28.18v14.07h-21.15v7.06h21.15v14.14h-42.23v-14.14h14.05v-21.13zM577.89 301.72v-14.06h14.09v-6.99h-14.09v-14.18h28.18v14.14h6.95v20.91h-6.95v-13.78h-6.98v13.82l-21.2.14z" style="fill:#fff;stroke-miterlimit:3.86;stroke:#fff"/><path d="M629.1 684.29v-433.9l19.96-15.24v427.34l-19.96 21.8z" style="fill:#cb0f16;stroke:#cb0f16;stroke-miterlimit:3.86"/><path d="m629.1 250.39 19.96-15.24h-81.19l-20.18 15.24z" style="fill:#e83336"/></svg>`
};

function filmCanisterSvg(id) {
  return FILM_SVGS[id] || FILM_SVGS.natural;
}

function renderRolls() {
  const g = document.getElementById('roll-grid');
  g.innerHTML = FILMS.map(f => `<button class="roll-card" onclick="selectRoll('${f.id}')" id="rc-${f.id}">
      <div class="roll-swatch">${filmCanisterSvg(f.id)}</div>
      <div class="roll-name">${f.name}</div>
      <div class="roll-desc">${f.desc}</div>
    </button>`).join('');
}

function selectRoll(id) {
  if (appMode === 'digital') return;
  selectedFilm = FILMS.find(f => f.id === id);
  document.querySelectorAll('.roll-card').forEach(c => c.classList.remove('selected'));
  document.getElementById('rc-' + id).classList.add('selected');
  document.getElementById('load-btn').disabled = false;
}

async function loadRoll() {
  if (appMode === 'digital') { startDigitalShoot(true); return; }
  if (!selectedFilm) return;
  setStatus('LOADING', '#e8b84b');
  try {
    const r = await fetch('/setroll?film=' + selectedFilm.id);
    const d = await r.json();
    if (!d.ok) { showToast('ERROR LOADING ROLL'); return; }
    rollDeveloped = false;
    activeFilm  = selectedFilm;
    framesTaken = 0;
    lastFilename = null;
    // Apply sensor settings for this film
    const p = activeFilm.sensor;
    await fetch(`/filter?brightness=${p.brightness}&saturation=${p.saturation}&contrast=${p.contrast}&effect=${p.effect}`).catch(()=>{});
    startShoot();
  } catch(e) { showToast('CONNECTION ERROR'); }
  setStatus('READY', 'var(--green)');
}

async function startDigitalShoot(resetCount) {
  activeFilm = FILMS[0];
  if (resetCount) framesTaken = 0;
  lastFilename = null;
  const p = activeFilm.sensor;
  await fetch(`/filter?brightness=${p.brightness}&saturation=${p.saturation}&contrast=${p.contrast}&effect=${p.effect}`).catch(()=>{});
  document.getElementById('preview-img').style.display = 'none';
  document.getElementById('vf-empty').style.display    = 'flex';
  resumeShoot();
}

// ============================================================
// SHOOTING SCREEN
// ============================================================
function startShoot() {
  framesTaken = 0;
  lastFilename = null;
  document.getElementById('preview-img').style.display = 'none';
  document.getElementById('vf-empty').style.display    = 'flex';
  resumeShoot();
}

function resumeShoot() {
  document.getElementById('film-badge').textContent = appMode === 'digital' ? 'DIGITAL' : (activeFilm ? activeFilm.name.toUpperCase() : '');
  updateShootUI();
  showScreen('screen-shoot');
  startStatusPoll();
}

function updateShootUI() {
  const left = appMode === 'digital' ? '&#8734;' : (36 - framesTaken);
  document.getElementById('frames-taken').textContent = String(framesTaken).padStart(2,'0');
  document.getElementById('frames-left').innerHTML  = left;
  const btn = document.getElementById('shutter-btn');
  btn.disabled = false;
  const developBtn = document.getElementById('develop-btn');
  developBtn.textContent = appMode === 'digital' ? 'Open Gallery' : 'Develop Roll';
  developBtn.disabled = (framesTaken === 0);
  // Film strip pips
  const strip = document.getElementById('strip-wrap');
  const track = document.getElementById('frames-track');
  if (appMode === 'digital') {
    strip.style.display = 'none';
    return;
  }
  strip.style.display = 'block';
  let html = '';
  for (let i = 0; i < 36; i++) {
    if (i < framesTaken)        html += '<div class="frame-pip exposed"></div>';
    else if (i === framesTaken) html += '<div class="frame-pip current"></div>';
    else                        html += '<div class="frame-pip"></div>';
  }
  track.innerHTML = html;
  if (framesTaken >= 36) showToast('ROLL FULL \u2014 DEVELOP NOW');
}

async function capture() {
  if (appMode !== 'digital' && framesTaken >= 36) return;
  const btn   = document.getElementById('shutter-btn');
  const flash = document.getElementById('flash');
  btn.disabled = true;
  btn.classList.add('firing');
  flash.classList.add('active');
  setStatus('CAPTURING', '#e8b84b');
  setTimeout(() => { flash.classList.remove('active'); btn.classList.remove('firing'); }, 180);
  try {
    const r = await fetch('/capture');
    const d = await r.json();
    if (d.ok) {
      framesTaken = d.frames;
      lastFilename = d.path.split('/').pop();
      showPreview('/photo?name=' + encodeURIComponent(lastFilename) + '&_=' + Date.now());
      updateShootUI();
      showToast(appMode === 'digital' ? 'PHOTO SAVED' : ('FRAME ' + String(framesTaken).padStart(2,'0') + ' / 36'));
    } else {
      showToast(d.error === 'full' ? 'ROLL FULL' : 'CAPTURE FAILED');
    }
  } catch(e) { showToast('CONNECTION ERROR'); }
  btn.disabled = false;
  setStatus('READY', 'var(--green)');
}

function showPreview(src) {
  const img   = document.getElementById('preview-img');
  const empty = document.getElementById('vf-empty');
  img.onload  = () => { img.style.display = 'block'; empty.style.display = 'none'; };
  img.onerror = null;
  img.src = src;
}

function handleVfClick() {
  if (lastFilename) openLightbox(lastFilename, true);
}

// ============================================================
// DEVELOP
// ============================================================
async function goToDevelop() {
  if (appMode === 'digital') {
    try {
      const photos = await fetch('/list').then(r=>r.json());
      renderGallery(photos, null);
      showScreen('screen-gallery');
    } catch(e) { showToast('CONNECTION ERROR'); }
    return;
  }
  if (framesTaken === 0) return;
  stopStatusPoll();
  showScreen('screen-dark');
  setStatus('DEVELOPING', '#e8b84b');
  try {
    // Small delay for dramatic effect
    await new Promise(res => setTimeout(res, 2200));
    const r = await fetch('/develop');
    const d = await r.json();
    if (d.ok) {
      rollDeveloped = true;
      renderGallery(d.photos, d.dir);
      showScreen('screen-gallery');
      setStatus('READY', 'var(--green)');
    } else {
      showToast('DEVELOP FAILED: ' + (d.error||''));
      showScreen('screen-shoot');
      setStatus('READY', 'var(--green)');
    }
  } catch(e) {
    showToast('CONNECTION ERROR');
    showScreen('screen-shoot');
    setStatus('READY', 'var(--green)');
  }
}

// ============================================================
// GALLERY
// ============================================================
function renderGallery(photos, dir) {
  currentGalleryDir = dir || null;
  const filmName = appMode === 'digital' ? 'DIGITAL' : (activeFilm ? activeFilm.name.toUpperCase() : '');
  document.getElementById('gallery-film-label').textContent = filmName + ' \u2014 ' + photos.length + ' ' + (appMode === 'digital' ? 'PHOTOS' : 'FRAMES');
  const showRollBtns = currentGalleryDir ? 'inline-block' : 'none';
  document.getElementById('delete-roll-btn').style.display = showRollBtns;
  document.getElementById('rename-roll-btn').style.display = showRollBtns;
  document.getElementById('back-cam-gallery').style.display = appMode === 'digital' ? 'inline-block' : 'none';
  const g = document.getElementById('gallery');
  const dirParam = currentGalleryDir ? '&dir=' + encodeURIComponent(currentGalleryDir) : '';
  if (!photos.length) { g.innerHTML = '<div style="color:var(--muted);font-family:var(--ff-mono);font-size:11px;padding:30px 0">NO PHOTOS</div>'; return; }
  g.innerHTML = photos.map((p, i) => {
    const name = p.name || p;
    return `<div class="thumb-card" style="animation-delay:${i*0.06}s" onclick="openLightbox('${name}',false)">
      <img src="/photo?name=${encodeURIComponent(name)}${dirParam}&_=${Date.now()}" alt="${name}" loading="lazy">
      <div class="dl-overlay">&#8595; SAVE</div>
    </div>`;
  }).join('');
}

// ============================================================
// LIGHTBOX
// ============================================================
function openLightbox(filename, isPreview) {
  const lb  = document.getElementById('lightbox');
  const img = document.getElementById('lb-img');
  const dl  = document.getElementById('lb-dl');
  const del = document.getElementById('lb-delete');
  const dirParam = (!isPreview && currentGalleryDir) ? '&dir=' + encodeURIComponent(currentGalleryDir) : '';
  const srcUrl  = '/photo?name=' + encodeURIComponent(filename) + dirParam + (isPreview ? '&_=' + Date.now() : '');
  const dlUrl   = '/photo?name=' + encodeURIComponent(filename) + dirParam + '&dl=1';
  currentLightboxFile = filename;
  currentLightboxIsPreview = !!isPreview;
  document.getElementById('lb-name').textContent = filename;
  img.style.filter = '';
  img.src  = srcUrl;
  dl.href  = dlUrl;
  dl.download = filename;
  del.style.display = isPreview ? 'none' : 'inline-block';
  lb.classList.add('open');
}
function closeLightbox() {
  currentLightboxFile = null;
  currentLightboxIsPreview = false;
  document.getElementById('lightbox').classList.remove('open');
}
document.getElementById('lightbox').addEventListener('click', e => {
  if (e.target === document.getElementById('lightbox')) closeLightbox();
});

// ============================================================
// ARCHIVE
// ============================================================
async function goToArchive() {
  if (appMode === 'digital') {
    goToDevelop();
    return;
  }
  document.getElementById('back-cam-archive').style.display =
    (activeFilm && framesTaken < 36) ? 'inline-block' : 'none';
  showScreen('screen-archive');
  loadArchive();
}

async function loadArchive() {
  try {
    const r = await fetch('/rolls');
    const rolls = await r.json();
    const grid = document.getElementById('archive-grid');
    const empty = document.getElementById('archive-empty');
    if (!rolls.length) { grid.innerHTML = ''; empty.style.display = 'block'; return; }
    empty.style.display = 'none';
    grid.innerHTML = rolls.map(r => {
      const film = FILMS.find(f => f.id === r.film);
      const swatch = film ? film.swatch : 'linear-gradient(135deg,#333,#666)';
      const canisterSvg = filmCanisterSvg(r.film);
      const name = film ? film.name : r.film;
      return `<div class="archive-card" onclick="viewArchiveRoll('${r.dir}','${r.film}')">
        <div class="archive-swatch">${canisterSvg}</div>
        <div class="archive-film">${name}</div>
        <div class="archive-meta">${r.count} FRAMES &#183; ${r.dir.toUpperCase()}</div>
        <div class="archive-actions">
          <button class="new-roll-btn" onclick="event.stopPropagation(); openRenameModal('${r.dir}')">Rename</button>
          <button class="danger-btn" onclick="event.stopPropagation(); deleteRoll('${r.dir}')">Delete</button>
        </div>
      </div>`;
    }).join('');
  } catch(e) { showToast('CONNECTION ERROR'); }
}

async function viewArchiveRoll(dir, filmId) {
  activeFilm = FILMS.find(f => f.id === filmId) || null;
  try {
    const r = await fetch('/list?dir=' + encodeURIComponent(dir));
    const photos = await r.json();
    renderGallery(photos, dir);
    showScreen('screen-gallery');
  } catch(e) { showToast('CONNECTION ERROR'); }
}

async function deleteCurrentPhoto() {
  if (!currentLightboxFile || currentLightboxIsPreview) return;
  const target = currentLightboxFile;
  if (!confirm('Delete this developed photo?')) return;

  try {
    const body = new URLSearchParams({ type:'photo', name:target, dir: currentGalleryDir || '' });
    const r = await fetch('/delete', { method:'POST', headers:{'Content-Type':'application/x-www-form-urlencoded'}, body });
    const d = await r.json();
    if (!d.ok) { showToast('DELETE FAILED'); return; }

    closeLightbox();
    const query = currentGalleryDir ? ('?dir=' + encodeURIComponent(currentGalleryDir)) : '';
    const refreshed = await fetch('/list' + query).then(x => x.json());
    renderGallery(refreshed, currentGalleryDir);
    showToast('PHOTO DELETED');
  } catch(e) {
    showToast('CONNECTION ERROR');
  }
}

async function deleteRoll(dir) {
  if (!dir) return;
  if (!confirm('Delete this developed roll and all its photos?')) return;

  try {
    const body = new URLSearchParams({ type:'roll', dir });
    const r = await fetch('/delete', { method:'POST', headers:{'Content-Type':'application/x-www-form-urlencoded'}, body });
    const d = await r.json();
    if (!d.ok) { showToast('DELETE FAILED'); return; }

    if (currentGalleryDir === dir) {
      closeLightbox();
      currentGalleryDir = null;
      showScreen('screen-archive');
    }
    loadArchive();
    showToast('ROLL DELETED');
  } catch(e) {
    showToast('CONNECTION ERROR');
  }
}

function deleteCurrentRoll() {
  if (!currentGalleryDir) return;
  deleteRoll(currentGalleryDir);
}

// ============================================================
// RENAME ROLL
// ============================================================
let renameTargetDir = null;

function openRenameModal(dir) {
  if (!dir) return;
  renameTargetDir = dir;
  document.getElementById('rename-current').textContent = 'CURRENT: ' + dir.toUpperCase();
  document.getElementById('rename-input').value = dir;
  document.getElementById('rename-modal').style.display = 'flex';
  setTimeout(() => document.getElementById('rename-input').focus(), 60);
}

function closeRenameModal() {
  renameTargetDir = null;
  document.getElementById('rename-modal').style.display = 'none';
}

async function submitRename() {
  if (!renameTargetDir) return;
  const newName = document.getElementById('rename-input').value.trim();
  if (!newName) { showToast('ENTER A NAME'); return; }
  const oldDir = renameTargetDir;
  try {
    const body = new URLSearchParams({ dir: oldDir, name: newName });
    const r = await fetch('/rename', { method:'POST', headers:{'Content-Type':'application/x-www-form-urlencoded'}, body });
    const d = await r.json();
    if (!d.ok) { showToast('RENAME FAILED'); return; }
    if (currentGalleryDir === oldDir) currentGalleryDir = d.newDir;
    closeRenameModal();
    showToast('ROLL RENAMED');
    if (document.getElementById('screen-archive').classList.contains('active')) loadArchive();
  } catch(e) { showToast('CONNECTION ERROR'); }
}

// ============================================================
// UTIL
// ============================================================
function setStatus(text, color) {
  document.getElementById('status-text').textContent = text;
  const dot = document.getElementById('status-dot');
  dot.style.background  = color || 'var(--green)';
  dot.style.boxShadow   = '0 0 6px ' + (color || 'var(--green)');
}
let toastTimer;
function showToast(msg) {
  const t = document.getElementById('toast');
  t.textContent = msg;
  t.classList.add('show');
  clearTimeout(toastTimer);
  toastTimer = setTimeout(() => t.classList.remove('show'), 2400);
}

// â”€â”€ FAST STATUS POLL (runs only while on shooting screen) â”€â”€
// Syncs frame count when physical shutter button is used
let statusPollTimer = null;
function startStatusPoll() {
  stopStatusPoll();
  statusPollTimer = setInterval(async () => {
    if (!document.getElementById('screen-shoot').classList.contains('active')) {
      stopStatusPoll(); return;
    }
    try {
      const d = await fetch('/status').then(r => r.json());
      if (d.frames !== framesTaken) {
        framesTaken = d.frames;
        updateShootUI();
        // If a new frame was taken but we have no preview, fetch the latest
        if (d.frames > 0 && !lastFilename) {
          fetch('/list').then(r=>r.json()).then(photos => {
            if (photos.length > 0) {
              lastFilename = (photos[photos.length-1].name || photos[photos.length-1]);
              showPreview('/photo?name=' + encodeURIComponent(lastFilename) + '&_=' + Date.now());
            }
          }).catch(()=>{});
        }
        if (appMode !== 'digital' && d.frames >= 36) showToast('ROLL FULL â€” DEVELOP NOW');
      }
    } catch(e) {}
  }, 2000);
}
function stopStatusPoll() {
  if (statusPollTimer) { clearInterval(statusPollTimer); statusPollTimer = null; }
}
</script>
</body>
</html>
)rawhtml";

#endif
