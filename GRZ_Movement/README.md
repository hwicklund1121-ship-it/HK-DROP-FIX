# GRZ clean FOV / eye zoom with selective movement stamina

Based on GRZ-Live-FOV-EyeZoom-Test.zip, as selected by the user. All original eight runtime files remain byte-identical. Adds GRZStaminaPolicy.c (shared Game-layer stamina getters) and GRZFreeMovementActions.c (free movement action drains).

No stamina drain for standing/crouched sprint, sprint swimming, fast ladders, jumping, vaulting, climbing, rolling or melee evasion. Carried weight does not shrink stamina capacity. Capacity is 100, minimum-cap parameter is 5. Punches and weapon melee retain native stamina costs (multiplier 1), as does holding breath. Native recovery/cooldowns, synchronization, stamina consumers and other restrictions are retained. No constant refill or global stamina disable is used. Drowning and car-pushing drains remain native. Free actions may still be unavailable at exhaustion because native activation checks are intentionally retained.

The code sets these stamina getters on both client and server; there is no separate JSON requirement. Your existing server gameplay JSON is not rewritten. Other stamina mods/global stamina disabling can conflict and prevent paid costs; do not use them for this comparison. MovementData and its inertia flag are not changed.

Clean FOV preview/settings and focused crouch/sprint eye zoom are restored; old HK recovery distance, legacy camera selection, bob/pivot/shoulder overrides and duplicate zoom handlers remain absent. Original 17-degree movement preset and running lean remain unchanged. This restores the selected feature build, not the mixed server PBO.

Back up the previous PBO outside loaded addons. Replace the entire GRZ_Movement.pbo on BOTH client and server. Sign through your normal workflow where required. Restart both, check SHA256.txt, and test sprint/swim/ladder plus jump/roll, punches/weapon melee and prolonged holding breath. Confirm the stamina bar drains during paid actions and does not refill instantly. Test FOV drag/Apply/Cancel and crouch/sprint zoom.

This is a feature test, not a confirmed rubber-banding fix. Native APIs checked against the available DayZ source reference; package checks and client/SERVER preprocessing passed. No Enforce compiler or DayZ runtime validation is available here.
