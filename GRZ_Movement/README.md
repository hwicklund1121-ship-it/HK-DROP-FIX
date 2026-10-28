# GRZ Movement 0.1 — compatibility baseline

Fresh movement-response implementation for DayZ 1.30. Uses the supplied LWP response values: 0.01 sprint smoothing, 200 acceleration limit, 0.1 strafe smoothing, 17-degree turn threshold, 6.5 lean speed and unrestricted water movement speed. Initializes through the native implementation first. No animation assets from LUGU are included or changed.

This baseline matches movement response settings, not every behavior of the legacy movement PBO. Native sprint/lean restrictions, stamina, weapon sway, weapon lifting, item stance rules and stance callbacks remain native. FOV and camera behavior remain under the installed zoom mod. In particular, it does not abort weapon events or refresh weapon animations during stance changes. These legacy overrides are possible conflict points, not a confirmed diagnosis.

## Install and test

1. Extract the ZIP and load @GRZ_Movement on the client and server where required. Use the same build on both.
2. Disable lwp_movement, ronjam_fix and other movement replacements, including loose legacy 1337/OPM scripts in another PBO. GRZ cannot undo their overrides while they remain loaded.
3. Use the owner's verified working LUGU PBO. Remove previous experimental LUGU replacements and HK shoulder/camera/stance patches. This ZIP deliberately does not ship another LUGU version.
4. First test LUGU alone with required dependencies. Then add GRZ and repeat G → prone → look right → A/D, on flat ground and a hill. Compare rifle, pistol and empty hands. If LUGU alone fails, GRZ cannot establish that the movement mod caused it.
5. Add the original zoom/FOV mod and other mods one at a time. Check sprint/strafe/lean, crouch, prone, recovery and held middle-mouse zoom. Keep any newly changed RPT/script errors.

Package checks verify PBO checksum, decompression and source round-trip. DayZ script compilation and movement compatibility require in-game validation; they have not been verified here. This is an unsigned test build; signature-enforcing public servers need an appropriate signed release.

Source: GRZ_Movement/scripts/4_World/GRZMovement.c. Rebuild with `python tools/build_grz_movement.py` from the repository root.
