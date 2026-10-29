# GRZ Movement - Camera / FOV / No Weapon Lift / Selective Stamina

As requested after the user reported the rubber-banding stopped when Schana autorun was removed. This package does not include autorun or diagnostics. That report does not certify every restored feature's runtime behavior.

Features: original snappy 0.06/100 acceleration, 0.14 strafe, 17-degree turn threshold and 6.5 lean response; original sprint lean; legacy GRZ movement-camera selection; zero sprint camera roll and crouch speed-height bob correction; FOV continuity on eye-zoom camera changes; focused crouch/sprint eye-zoom getter; FOV live preview with Apply/Cancel plus render-distance settings; no automatic weapon lift/obstruction response; selective movement stamina.

Movement stamina drains are removed for sprint, swimming sprint, fast ladders, jumping, vaulting, climbing, rolling and melee evasion. Weight does not shrink the 100 stamina capacity. Punches/weapon melee and holding breath retain normal native costs and cooldowns. Native stamina recovery, sync and activation restrictions remain; paid-action exhaustion can temporarily prevent movement actions. No instant refill or global stamina disable. Drowning and car-pushing drains remain native.

Earlier user-requested exclusions remain: HK-drop/prone recovery distance floor, custom sprint distance/pivot/shoulder offsets, duplicate old eye-zoom policy, sprint-start experiments and camera diagnostic trace. All restored camera/UI scripts are excluded from SERVER builds. The obstruction override deliberately changes weapon pose/ADS obstruction policy, not physical collisions or projectile geometry.

INSTALL: Stop client and server. Back up old GRZ_Movement.pbo outside loaded addon folders. Replace the whole PBO on BOTH sides with the supplied prebuilt file. Do not merge old extracted sources. Ensure no duplicate/repacked GRZ scripts and keep the conflicting autorun disabled. Sign using your usual key if required. Restart both; check SHA256.txt against deployed files. Main addon name remains GRZ_Movement.

Use the provided mod-controlled cfggameplay.json if desired; no separate stamina JSON is required by this PBO. Keep other stamina-disable mods/settings from overriding paid action costs.

TEST: FOV preview/Apply/Cancel/reopen; crouch/sprint eye zoom and release; ADS/scopes; both lean directions; walls/doorways and native weapon switching; extended sprint and then punching/holding breath to confirm those still drain. Compare normal movement and moonwalk on the server without autorun.

Verified: strict package inventory, one eye-zoom override, HK/distance/experimental source absence, client/SERVER preprocessing, PBO checksum/source round-trips, ZIP integrity. No Enforce compiler or in-game execution available here; report any compilation/runtime errors.
