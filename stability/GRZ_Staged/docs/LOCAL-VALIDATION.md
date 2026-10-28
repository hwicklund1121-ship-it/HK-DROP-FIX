# Local validation

- 01-movement: PASS client/SERVER preprocessing and forbidden-override checks
- 02-lean: PASS client/SERVER preprocessing and forbidden-override checks
- 03-weapon-lift: PASS client/SERVER preprocessing and forbidden-override checks
- 04-eye-zoom: PASS client/SERVER preprocessing and forbidden-override checks
- 05-sprint-roll: PASS client/SERVER preprocessing and forbidden-override checks
- 06-crouch-height: PASS client/SERVER preprocessing and forbidden-override checks

- Core movement source matches the current tuned GRZ preset exactly.
- Every PBO checksum and source payload is checked by the builder.
- Three camera/eye-zoom stages produce no SERVER script body.
- No native layouts, client UI, CameraHandler, HandleView, player stance callbacks or stamina overrides are packaged.

These checks use the C preprocessor to inspect conditional script contents; they do not compile Enforce Script or test runtime behavior. No server was deployed or observed.
