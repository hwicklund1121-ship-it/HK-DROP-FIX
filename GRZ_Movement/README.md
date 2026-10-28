# GRZ Movement 0.4 — steady third-person camera

Fresh movement-response implementation for DayZ 1.30. Uses the supplied LWP response values: 0.01 sprint smoothing, 200 acceleration limit, 0.1 strafe smoothing, 17-degree turn threshold, 6.5 lean speed and unrestricted water movement speed. Initializes through the native implementation first. No animation assets from LUGU are included or changed.

This baseline matches movement response settings, not every behavior of the legacy movement PBO. Lean inputs remain available when sprint starts. Stamina, weapon sway, weapon lifting, item stance rules, movement-change callbacks, sprint-end cleanup and stance callbacks remain native. The built-in legacy camera policy selects raised stance cameras directly during transitions and restores sprint-camera selection from the movement index. It uses native camera FOV values and your FOV setting; it does not set a fixed FOV angle or force zoom input. Exact equivalence to a third-party FOV mod and held-zoom persistence are not established. In particular, it does not abort weapon events or refresh weapon animations during stance changes. These legacy overrides are possible conflict points, not a confirmed diagnosis.

## Install and test

1. Extract the ZIP and load @GRZ_Movement on the client and server where required. Use the same build on both.
2. Disable lwp_movement, ronjam_fix and other movement replacements, including loose legacy 1337/OPM scripts in another PBO. GRZ cannot undo their overrides while they remain loaded.
3. Use the owner's verified working LUGU PBO. Remove previous experimental LUGU replacements and HK shoulder/camera/stance patches. This ZIP deliberately does not ship another LUGU version.
4. First test LUGU alone with required dependencies. Then add GRZ and repeat G → prone → look right → A/D, on flat ground and a hill. Compare rifle, pistol and empty hands. If LUGU alone fails, GRZ cannot establish that the movement mod caused it.
5. Disable StormZz_Zoom and other camera/FOV replacements when testing this build; GRZ now includes its own camera policy. Add other mods one at a time. Check sprint/strafe/lean, crouch, prone, recovery and held middle-mouse zoom. Keep any newly changed RPT/script errors.

Package checks verify PBO checksum, decompression and source round-trip. DayZ script compilation and movement compatibility require in-game validation; they have not been verified here. This is an unsigned test build; signature-enforcing public servers need an appropriate signed release.

Source: GRZ_Movement/scripts/4_World/GRZMovement.c. Rebuild with `python tools/build_grz_movement.py` from the repository root.

See PROVENANCE.md for package contents and rights limitations. No guarantee against copyright claims is made.

Version 0.3 adds only GRZRunningLean.c to the runtime payload. It skips the native sprint-start lean-input locks. Movement tuning and camera code are byte-identical to version 0.2. Test left/right lean while jogging and sprinting, hold lean while entering/leaving sprint, release lean to check return to center, and repeat the LUGU sequence. In-game validation is required.

Version 0.4 adds GRZSteadyCamera.c: zero sprint camera roll and removal of the crouch speed-height contribution after the native update. The raised crouch camera inherits this correction. Existing movement, running lean, FOV policy and LUGU resources are unchanged. This addresses third-person locomotion bob; first-person head-bone motion, intentional lean, recoil, impact shake, stance transitions and LUGU prone motion are not removed. Test sprint and crouch movement, both shoulders, held zoom, raised weapons and the drop sequence. Runtime validation is required.
