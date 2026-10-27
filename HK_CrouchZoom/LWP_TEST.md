# Movement PBO with crouch eye-zoom patch v2 — experimental

V1 excluded raised-weapon input. The reported clip shows a shoulder-held rifle,
so V2 permits held eye zoom during shoulder aim as well. It does not require
zoom to have been active on a preceding frame. This deliberately permits eye
zoom while raised, where vanilla normally clears it. Sights/scopes, active
fighting, prone, vehicles and standing sprint keep inherited behavior. This
revision is unverified in-game.

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

The patch restores normal eye zoom while requested if the default view handler
clears it during standing/crouched movement, including raised stances. It carries
previous FOV between named vanilla third-person standing/crouching cameras and
their raised variants. Custom camera classes and the existing StormZz camera
selection override require testing. The client script log prints
`[HKCROUCHZOOM] version=2 loaded` and, when restoring zoom,
`[HKCROUCHZOOM] version=2 preserving held eye zoom`. These show execution, not
visual success. If the view still pulls back while these messages appear, send
the matching script log/RPT so camera position/blending can be investigated.

Archive contents and checksums were verified. There is no DayZ compiler/runtime
here: compilation and the actual zoom fix are UNVERIFIED. If the game reports a
script error or the zoom issue remains, restore the original movement PBO and
send the script log/RPT and a short clip.

Rebuild from the repository root:
`python3 tools/repack_lwp_crouch_zoom.py --output /workspace/artifacts`
