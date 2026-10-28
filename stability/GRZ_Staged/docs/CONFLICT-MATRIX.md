# Camera and movement override matrix

"Loaded" refers only to the supplied older RPT. "Reported" means the newly pasted investigation, not inspected source. Unknowns must not be treated as verified overrides.

| Owner | Override/API | Verification | Potential overlap |
|---|---|---|---|
| StormZz_Zoom.pbo | DayZPlayerImplement.CameraHandler; super return discarded | Local original source inspected; named PBO loaded in RPT; deployed hash unknown | Full GRZ legacy camera, native ADS/obstruction/vehicle/stance-camera selection |
| StormZz_Script.pbo | DayZPlayerCamera3rdPerson.OnUpdate | PBO loaded; override reported; actual source missing | Camera transform, native/GRZ update chain, zoom/distance outputs |
| GRZ_Zoom.pbo | DayZPlayerImplement.HandleView | PBO loaded; override reported; source missing | Native eye-zoom state, scope/view state, GRZ GetEyeZoomLevel policy |
| HerosperNo3rdPersonSprintShake.pbo | Sprint camera bone/reference change | PBO loaded; behavior reported; source missing | Sprint roll and any shared camera-bone/transform changes |
| OPM_Scripts.pbo | Movement, stance, roll, aiming, weapon lift | Reported; no named load entry in this RPT; exact PBO missing | Shared PlayerBase callbacks, animation/weapon state, other movement replacements |
| lwp_movement.pbo | Type creation/init; item behavior; movement/sprint callbacks; aiming weight; stance-change weapon abort/refresh; stance permissions; rolling; weapon lift; sprint roll | Local PBO source inspected; no named load entry | OPM/Ronjam/GRZ movement; stance and weapon animation lifecycle |
| ronjam_fix.pbo | Type creation/init; item behavior; empty movement/sprint callbacks; PlayerBase movement/raised predicates | Local PBO source inspected; no named load entry | LWP/OPM/GRZ; not evidence of current deployment |
| Full GRZ v0.32 GRZLegacyCamera | CameraHandler; native result selectively replaced with stance camera modes | Local source inspected; client-only; deployment unconfirmed | StormZz_Zoom and native camera selection |
| Full GRZ GRZSprintEyeZoom | GetEyeZoomLevel plus sprint camera constructor/OnUpdate offsets | Local source inspected; client-only | GRZ_Zoom/native eye zoom; sprint camera updates |
| Full GRZ GRZCameraDistance | Shared player distance state; third-person StdFovUpdate distance substitution | Local source inspected; client-only | Third-person output modifiers and stance switching |
| Full GRZ GRZZoomContinuity | Third-person OnActivate, FOV state carry-over | Local source inspected; client-only | Camera activation, zoom/blend state |
| Full GRZ GRZSteadyCamera | Sprint GetCurrentRoll; crouch OnUpdate height correction | Local source inspected; client-only | Herosper and other third-person update chains |
| Full GRZ GRZZoomTrace | Third-person OnUpdate diagnostics | Local source inspected; client-only | Adds update chaining/logging; omitted from staged package |
| Full GRZ preferences/menu | DayZGame.GetUserFOV; OptionsMenu/Game lifecycle; MissionGameplay.OnMissionStart | Local source inspected; client-only | Other settings extensions, saved FOV precedence; omitted from staged package |
| Staged core | DayZPlayerTypeUtils.DayZPlayerTypeInitMovement; native super first | Newly packaged from existing GRZ preset | Other movement initialization replacements; keep only one |
| Staged lean | DayZPlayerImplement.OnSprintStart | Existing GRZ logic, separate optional stage | Other sprint-start input/callback policies |
| Staged weapon lift | PlayerBase.CheckLiftWeapon | Existing GRZ logic, separate optional stage | Other obstruction/lift/network policies |
| Staged eye zoom | DayZPlayerImplement.GetEyeZoomLevel only | Existing GRZ getter; sprint camera constructor/update removed | Other eye-zoom handlers; server excludes this stage's overrides |
| Staged sprint roll | Sprint camera.GetCurrentRoll only | Isolated existing correction | Other roll overrides; no bone reassignment |
| Staged crouch height | Crouch camera.OnUpdate only; super first after null checks | Isolated existing correction | Other OnUpdate corrections; introduce last |

Modded override chaining/load order can change behavior. A matching method name is a conflict opportunity, not proof of incompatibility or crash causation. Config CfgPatches dependencies help load order but do not make duplicate overrides compatible.
