# Movement PBO with crouch eye-zoom patch v3 — experimental

V1 and V2 did not resolve the reported zoom change. V3 directly applies the
configured normal eye-zoom FOV in eligible third-person camera updates while
zoom is held. It also preserves the camera boom distance from the preceding
standing/crouch camera. This tests both FOV loss and the native switch from a
raised rifle camera (0.8 m) to a lowered crouch camera (1.4 m). That switch is a
concrete source-level difference, not yet a confirmed runtime diagnosis.

V3 intentionally applies full eye zoom immediately while held, without the
normal entry easing. It also keeps the standing/crouch boom distance constant
until zoom is released. Vertical stance offsets and collision handling are not
replaced. This can change the usual third-person framing during held zoom.

The addons/lwp_movement.pbo in this ZIP is based on the uploaded movement PBO.
All four existing entries are unchanged, including its config, movement script,
mission script and sprint camera script. One script was added to its existing
registered scripts/4_World folder. This is not the Luigi motorcycle-blocking PBO.

Back up your existing movement PBO and signature outside loaded addons folders.
Replace lwp_movement.pbo in your mod's addons folder with this ZIP's version on
client and test server, and restart both. Load only one movement PBO. Do not also
load the separate HK_CrouchZoom addon: its script would duplicate this patch.
The repacked PBO is unsigned; sign it with your normal key if the server enforces
signatures. The original signature cannot be reused for changed PBO contents.

Use the original Luigi PBO, not the unsuccessful motorcycle-blocking experiment,
and disable HK_CrouchHandsGuard for a controlled comparison.

Hold middle-mouse eye zoom and crouch/stand repeatedly, stationary and moving.
Try first and third person, empty hands and lowered/shoulder-held weapons. Release zoom during
the transition to ensure it releases normally. Check scopes/ironsights, raised
fists, prone, sprinting, vehicles and item actions for regressions. This patch
preserves an existing eye zoom; it does not force weapon ADS or modify animation
graphs. It does not fix the unresolved fists-up animation itself.

The player-level logic retains held eye zoom, including shoulder aim. The new
camera-level enforcement applies only to the named vanilla third-person standing/
crouching cameras and their raised variants. First-person still uses the prior
player-level patch. Actual ADS/scopes, melee, standing sprint, prone and vehicles
are excluded. Custom camera classes are not covered.

The client script log prints `[HKCROUCHZOOM] version=3 loaded`. Camera-level
samples include camera name, eye level, FOV, applied distance and native distance
while zoom is held (limited to 40 samples per camera instance, 0.25 s apart).
If the zoom still changes, send that session's script log/RPT; the values show
whether this exact patch is executing and what it outputs. They do not prove
what collision or final native blending renders.

Archive contents and checksums were verified. There is no DayZ compiler/runtime
here: compilation and the actual zoom fix are UNVERIFIED. If the game reports a
script error or the zoom issue remains, restore the original movement PBO and
send the script log/RPT and a short clip.

Rebuild from the repository root:
`python3 tools/repack_lwp_crouch_zoom.py --output /workspace/artifacts`
