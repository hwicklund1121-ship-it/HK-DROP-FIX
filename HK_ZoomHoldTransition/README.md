# Original FOV revert: hold shoulder camera through crouch — test

This retains the original zoom script with two insertions: private state fields
and a camera-choice guard immediately before its normal movement camera choices.
Removing those insertions reproduces the original script byte for byte, which
the builder checks. There are no new FOV, camera-distance or zoom-level writes.
The previous bob-only crouch-height script is included unchanged.

When normal zoom input is active (IsZoom or IsZoomToggle), the guard remembers
whether hands/weapon were raised when that zoom hold began. If so, it keeps the
matching raised standing/crouch camera for that hold instead of selecting the
farther lowered camera during a transient stance-state change. The original
optics, vehicle, first-person, jump/climb and prone selections remain outside this
guard. Releasing zoom clears the memory. While zoom remains held, intentionally
lowering a weapon will retain the raised camera framing until zoom is released.

This targets the shoulder-held rifle shown in the clip. It does not force a
raised camera when zoom starts with lowered hands. The mechanism still needs
in-game verification: no input/camera trace from the affected session has been
supplied. It is not a confirmed fix for every eye-zoom or camera-blending issue.

Back up existing files outside loaded addons folders. Replace both PBOs in your
test mod's addons folder from this ZIP and restart client/server:

- StormZz_Zoom.pbo: original script with the small hold guard, plus the unchanged
  previous bob-only script.
- lwp_movement.pbo: byte-identical original movement PBO, removing earlier broad
  zoom modifications. Keep your original Luigi PBO.

Do not load duplicate zoom/movement PBOs or standalone test addons. The changed
zoom PBO is unsigned and needs your normal signing workflow where required.

Test the original snappy feel first, then hold middle mouse with the rifle at the
shoulder and repeatedly crouch/stand. Release during crouching and confirm the
camera returns to its original selection. Test lowered weapons, scope/ironsight,
first-person, sprint, prone, vehicles and shoulder switching. The log prints
`[HKZOOMHOLD] captured_raised=` when a zoom hold is detected and
`[HKZOOMHOLD] preserving_crouch_camera=1` if the guard replaces a lowered crouch
camera choice. If it still zooms out, send that session's script log/RPT rather
than stacking more test patches. Those messages show code execution, not visual
success. Compilation/visual behavior remain unverified here.
