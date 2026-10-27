# Movement PBO with crouch eye-zoom patch — experimental

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
Try first and third person, empty hands and a lowered weapon. Release zoom during
the transition to ensure it releases normally. Check scopes/ironsights, raised
fists, prone, sprinting, vehicles and item actions for regressions. This patch
preserves an existing eye zoom; it does not force weapon ADS or modify animation
graphs. It does not fix the unresolved fists-up animation itself.

The patch preserves previous eye zoom while it is still requested if the default
view handler clears it during crouching or a detected crouch-to-stand transition,
provided the input does not request raised hands/ADS. It also carries previous
FOV across named vanilla third-person standing/crouching cameras. Custom camera
classes and the existing StormZz camera selection override require in-game tests.

Archive contents and checksums were verified. There is no DayZ compiler/runtime
here: compilation and the actual zoom fix are UNVERIFIED. If the game reports a
script error or the zoom issue remains, restore the original movement PBO and
send the script log/RPT and a short clip.

Rebuild from the repository root:
`python3 tools/repack_lwp_crouch_zoom.py --output /workspace/artifacts`
