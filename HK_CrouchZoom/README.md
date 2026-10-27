# HK Crouch Eye Zoom — experimental

Preserves an already-active normal eye zoom while the zoom input remains held
during crouch/stand changes. This addresses two possible sources of a zoom pop:
the stock HandleView clearing eye zoom when the movement state becomes Raised,
and a third-person standing/crouching camera initializing its FOV on activation.
Neither mechanism has been confirmed as the cause on the user's machine.

The patch calls the inherited HandleView and OnActivate methods. It does not
replace CameraHandler, change global FOV, request a stance, or edit animation
graphs. It restores an existing zoom only when vanilla clears it while crouched
or during a detected transition from crouch to standing, and the input
does not request raised hands or ADS. It does not force zoom on from idle.
Intentional weapon raising and sights/scopes retain vanilla behavior. Camera FOV
continuity is limited to the named vanilla third-person crouch/standing cameras.
Custom camera subclasses are not covered by that part of the patch.

## Install and test

Extract @HK_CrouchZoom and load it with -mod on client and test server. The PBO
is unsigned; use your normal signing workflow on a signature-enforcing server.
Restore the original Luigi PBO and disable the motorcycle-blocking experiment
and HK_CrouchHandsGuard for this test. This patch is separate from the hands-up
pose investigation, which remains unresolved.

Hold normal eye zoom (middle mouse by default), repeatedly crouch and stand,
stationary and moving, and check for zoom loss. Repeat in first and third person,
empty-handed and with a lowered weapon. Release zoom halfway through a transition
and confirm it releases normally. Check starting zoom while already crouched,
intentional raised fists, ironsights/scopes, sprinting, prone, vehicles and death.
Compare with this addon unloaded. If the issue persists, send a clip plus the
client script log and RPT; do not infer success from a successful load alone.

The uploaded StormZz_Zoom PBO replaces CameraHandler using older logic; this patch
does not edit or redistribute that script. Its compatibility still needs testing.

API names/signatures were checked against the published DayZ-Script-Diff sources.
PBO contents/checksum are verified during packaging. No DayZ compiler/runtime is
installed here, so compilation and in-game behavior remain unverified.
