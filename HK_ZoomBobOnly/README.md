# FOV revert: crouch height only — experimental

This test preserves the original uploaded StormZz_Zoom.pbo entries byte for byte,
including OldZoom.c and its CameraHandler. One small script is added to its
registered Scripts/4_World folder. It subtracts only the speed-dependent crouch
camera height offset after the normal crouch camera update. Raised crouch
inherits that update too.

No FOV values, zoom input/state, camera selection, camera distances, blend timing,
or standing/prone/vehicle camera behavior are changed by the added script.
The real standing-to-crouch height change is retained. This does not remove
first-person walking bob, recoil, damage shake, terrain movement or all possible
sources of third-person movement. The original movement mod already suppresses
sprint camera roll; this patch adds no second sprint override.

## Install

Back up your current PBOs and signatures outside loaded addons folders. Replace
BOTH PBOs from this ZIP's addons/ folder on client and test server and restart:

- StormZz_Zoom.pbo: original zoom script/config plus the one crouch-height script.
- lwp_movement.pbo: byte-identical original uploaded movement PBO, restoring the
  original movement and removing earlier injected zoom experiments.

Disable standalone HK_CrouchZoom/HK_CrouchHandsGuard test addons. Load only one
copy of each PBO and use your original Luigi PBO. Do not combine this test with
the StableCamera or CrouchZoom v1/v2/v3 builds. The modified zoom PBO is unsigned;
sign it with your usual key if your server requires signatures. An old signature
cannot authenticate a changed PBO.

## Test and limits

First confirm that the original snappy camera and FOV feel are restored. Then
crouch and move forward/back/sideways, both empty-handed and holding a rifle;
compare with the original zoom PBO. Test holding/releasing middle mouse, standing,
sprint, leaning, shoulder switching and ADS. If the objectionable motion happens
while crouching in place rather than moving, this speed-dependent offset may not
be its source; report that result before expanding the patch.

The actual crouch-zoom issue is not claimed fixed by this bob-only experiment.
The original CameraHandler still selects cameras exactly as before. Archive
payloads, checksums and preservation of all original entries were verified.
DayZ compilation and visual behavior are unverified; restore the originals if
any error or regression appears.

Rebuild: `python3 tools/repack_zoom_bob_only.py --output /workspace/artifacts`
