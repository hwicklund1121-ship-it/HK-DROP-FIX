# Left shoulder during HK drop — experimental camera-only overlay

The user confirmed the back-rise clip test restores the spin, but wants the
camera over the left shoulder. This optional PBO uses the native shoulder-input
override during the marked back-cancel phase plus a 0.3-second outgoing-blend
tail. It preserves normal shoulder selection when the per-frame override ends.
The tail is an initial camera-only estimate, not a measured clip duration.

The working LUGU-BackRiseClip-TEST's Luigi and trace PBOs are included byte for
byte. The only new PBO is HK_DropLeftShoulder.pbo. No graph, movement, FOV,
camera distance, clip, timing or original zoom script is modified.

Back up your current files outside loaded addons folders. Put all three PBOs
from addons/ into your test mod's addons folder on client/test server, replacing
previous Luigi/trace copies and adding HK_DropLeftShoulder.pbo. Load one copy of
each. Leave zoom/FOV and ronjam/movement PBOs alone and restart. Use your normal
PBO signing workflow if the server requires signatures; the new PBO is unsigned.

Test the spin from both initial shoulder choices, and verify that the camera
uses the left shoulder during it and releases normally afterward. Native
shoulder smoothing remains enabled, so switching is not instantaneous. The
client log prints [HKLEFTDROP] active=true/false around overrides. If it returns
too soon or late, send a clip with those log lines. Check normal walking, ADS,
shoulder switching, vehicle entry and death. First-person output is not changed
by this script; the shoulder-input override may still be requested there.

Remove ONLY HK_DropLeftShoulder.pbo to undo this camera test while keeping the
spin build. Compilation and in-game behavior remain unverified here. Archive
checks confirm the working Luigi and trace PBOs are unchanged, registered script
paths resolve in the new PBO, and all payload/checksum round-trips pass.
