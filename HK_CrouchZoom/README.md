# HK Crouch Eye Zoom v2 — experimental

Preserves held normal eye zoom during standing/crouched movement, including
shoulder aim. The first candidate excluded raised-weapon input and required an
already-active eye zoom. V2 removes those restrictions while keeping ironsights,
scopes, fighting, prone, vehicles and standing sprint on inherited behavior.
The stock HandleView normally clears eye zoom for Raised movement states.

The patch calls inherited HandleView and OnActivate methods. It restores normal
eye zoom when still requested if the inherited handler clears it in eligible
movement. It preserves the FOV between named vanilla third-person standing/
crouching cameras and their raised variants. It does not change animation graphs,
camera position, or the existing CameraHandler override in StormZz_Zoom.
Custom camera subclasses are not covered by the FOV activation patch.
The actual cause of the user's zoom pop and effectiveness of V2 are unverified.

## Install and test

Extract @HK_CrouchZoom and load it with -mod on client and test server. The PBO
is unsigned; use your normal signing workflow on a signature-enforcing server.
Restore the original Luigi PBO and disable the motorcycle-blocking experiment
and HK_CrouchHandsGuard for this test. This patch is separate from the hands-up
pose investigation, which remains unresolved.

Hold normal eye zoom (middle mouse by default), repeatedly crouch and stand,
stationary and moving, and check for zoom loss. Repeat in first and third person,
empty-handed and with lowered/shoulder-held weapons. Release zoom halfway through a transition
and confirm it releases normally. Check starting zoom while already crouched,
intentional raised fists, ironsights/scopes, sprinting, prone, vehicles and death.
Compare with this addon unloaded. If the issue persists, send a clip plus the
client script log and RPT; do not infer success from a successful load alone.

The uploaded StormZz_Zoom PBO replaces CameraHandler using older logic; this patch
does not edit or redistribute that script. Its compatibility still needs testing.

API names/signatures were checked against the published DayZ-Script-Diff sources.
PBO contents/checksum are verified during packaging. No DayZ compiler/runtime is
installed here, so compilation and in-game behavior remain unverified.
