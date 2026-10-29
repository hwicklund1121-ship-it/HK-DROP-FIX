# GRZ Movement Only - 17 Degree Test

This PBO contains only config.cpp, GRZMovement.c and the original GRZRunningLean.c. All GRZ camera/FOV/eye-zoom/settings UI/layout code is absent. Native weapon obstruction remains in effect. No movement diagnostic addon is bundled.

Settings: acceleration 0.06/100, strafe 0.14, turn threshold 17 degrees, lean response 6.5. The 45-degree test and experimental sprint-start changes are not present.

Stop server and client, back up the old GRZ PBO outside all loaded addon directories, and replace the whole GRZ_Movement.pbo on BOTH sides with the supplied prebuilt file. Do not merge source folders or keep older GRZ scripts in another loaded PBO. Sign normally if required and restart both. Compare SHA256.txt with both deployed PBOs. Test initially without the separate movement trace addon.

GRZ FOV and render-distance preferences are no longer read/applied; their JSON is left untouched on disk. Other camera mods or native/user settings are not reset by this PBO. Native camera bob, sprint zoom restrictions and FOV settings may look different.

Package checks and client/SERVER preprocessing are not Enforce compilation or runtime validation. This isolates GRZ camera/FOV code, not a confirmed rubber-banding fix.
