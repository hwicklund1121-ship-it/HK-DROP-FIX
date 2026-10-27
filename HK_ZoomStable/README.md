# Zoom PBO with stable third-person eye zoom — test build

This build targets the third-person behavior shown in the uploaded clip. It
replaces Scripts/4_World/OldZoom.c in the uploaded StormZz_Zoom.pbo with an
independently authored script. Its original config.bin and archive prefix are
unchanged. The old full CameraHandler override is removed, allowing the current
DayZ camera selection and its safety/stance handling to run normally.

The replacement:

- Retains requested eye zoom while standing/crouched, including shoulder aim.
- Applies normal eye-zoom FOV and holds camera boom distance across the named
  third-person standing/crouch camera changes while middle mouse remains held.
- Removes pelvis-driven sprint camera roll through GetCurrentRoll().
- Subtracts the native speed-dependent crouch camera-height addition after the
  normal update, without cancelling the actual standing/crouch height change.

This does not freeze mouse looking or remove first-person walking bob, recoil,
damage shake, terrain motion, prone movement, or every possible camera effect.
Scope/ironsight cameras are excluded from the FOV enforcement. Custom camera
classes outside the named stock classes are not covered by the held-zoom patch.
Holding zoom applies the target FOV immediately, changing normal zoom-in easing.

## Install both supplied PBOs

Back up the installed movement and zoom PBOs and their signatures OUTSIDE loaded
addons folders. Put both files from this ZIP's addons/ into your test mod's
addons folder on client and server, replacing the existing copies:

- StormZz_Zoom.pbo: revised camera script.
- lwp_movement.pbo: byte-identical original uploaded movement PBO, included to
  remove the earlier injected zoom experiments and prevent duplicate patches.

Load only one copy of each. Disable the standalone HK_CrouchZoom and
HK_CrouchHandsGuard addons. Use the original Luigi PBO, not the unsuccessful
motorcycle-blocking graph test. Restart client and server after replacement.

The modified zoom PBO is unsigned. Its old signature is invalid for this build.
If your test server requires signatures, sign it using your normal key and
install the matching public key. The included movement PBO was not modified.

Hold middle mouse while walking, crouching and standing, both empty-handed and
with a shoulder-held rifle. Release zoom mid-transition and verify normal release.
Test sprint roll, crouch movement-height changes, first-person view, optics,
recoil, shoulder switching, lean, collisions and vehicle entry/exit. If the bob
persists, identify which of these movements produces it rather than disabling
unrelated animations. The client log should print [HKZOOMSTABLE] version=1 loaded
and bounded camera samples while zoom is held. Send a new clip and matching script
log/RPT if it still pulls back.

Validation here covers the registered script path, inherited API signatures,
archive payloads, compression and checksums. DayZ compilation and in-game behavior
are not verified. Restore both backed-up PBOs if any error or regression occurs.

Rebuild from the repository root:
`python3 tools/repack_zoom_stable.py --output /workspace/artifacts`
