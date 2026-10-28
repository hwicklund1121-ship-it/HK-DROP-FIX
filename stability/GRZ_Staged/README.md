# GRZ staged stability package

This is a separate isolation build, not a feature-complete upgrade to full GRZ. Do not install every stage at once. It deliberately removes full camera-selection replacements and the FOV menu during testing. Stamina remains native. Nothing is automatically deployed to a server.

## Baseline configuration

On a test deployment matching production, disable only the identified legacy camera PBOs: StormZz_Zoom.pbo, StormZz_Script.pbo, GRZ_Zoom.pbo and HerosperNo3rdPersonSprintShake.pbo. Keep OPM_Scripts disabled and verify that its overrides are not repacked elsewhere. Do not introduce full GRZ_Movement v0.32 or the staged core in this same first test. Keep unrelated gameplay mods and LUGU at their existing, recorded versions initially. Remove any duplicate earlier experimental camera/movement patches. If those names are packed into a shared mod, make a test copy of that pack rather than deleting unrelated gameplay PBOs.

Verify absence in the new RPT's PBO-load records, plus a repacked-source inventory. Verify client/server hashes and signatures. A file sitting on disk or a launch-argument Workshop folder is not sufficient proof of actual PBO/script loading. These baseline changes have not been performed on production.

## Restore one feature per stage

After a sufficiently observed baseline, add one directory's PBO at a time and retain only previously accepted stages:

| Stage | Add | Behavior |
|---|---|---|
| 00 | Nothing from this ZIP | Native camera/movement baseline, other recorded gameplay mods retained |
| 01-movement | @GRZ_Movement/addons/GRZ_Movement.pbo | Existing GRZ acceleration/strafe/turn preset; native initialization first |
| 02-lean | @GRZ_RunningLean/addons/GRZ_RunningLean.pbo | Existing running/sprint lean permission |
| 03-weapon-lift | @GRZ_NoObjectLift/addons/GRZ_NoObjectLift.pbo | Existing no automatic lift/obstruction response |
| 04-eye-zoom | @GRZ_SprintZoom/addons/GRZ_SprintZoom.pbo | Full eye zoom through sprint/crouch; getter only, client camera selection remains native |
| 05-sprint-roll | @GRZ_SprintRoll/addons/GRZ_SprintRoll.pbo | Sprint roll correction only, no bone changes |
| 06-crouch-height | @GRZ_CrouchHeight/addons/GRZ_CrouchHeight.pbo | Crouch speed-height correction only; added separately because it edits camera output |

Stage 01 reuses the GRZ_Movement name/prefix. It REPLACES full GRZ_Movement.pbo; never load the full and staged versions together. Optional stages require the staged core. Load matching shared packages on server and clients; camera-only source is #ifndef SERVER. Existing signature enforcement requires the owner to sign these new test PBOs; this ZIP is unsigned.

The three last stages are optional experiments, not assurances of stability. Full legacy CameraHandler, FOV UI, camera-distance substitution, sprint pivot offsets, FOV activation carry-over and trace logging are absent from every stage. No native executable is patched. Read docs/INVESTIGATION.md and docs/CONFLICT-MATRIX.md before deploying.

## Repeatable validation

For each stage record server build/PID/start time, launch parameters, PBO hashes/load records, player count/player-hours, exact test actions, crashes and elapsed exposure. Repeat with similar population and activity. Use at least the previously observed 10–12-hour window under comparable load as an initial observation target; no fixed duration or single crash-free hour proves stability.

1. Spawn and reconnect with existing/new characters, then respawn repeatedly. Include empty hands, pistol, long rifle and scoped rifle.
2. Repeat standing/jogging/sprinting, forward/backward strafe, alternating lean and sprint lean. Hold/release eye zoom, then test ADS and scope entry/exit.
3. Repeat standing/crouch/prone and raised variants, normal roll, and the LUGU G → prone → look right → A/D sequence on flat and sloped ground. Watch local and remote player views.
4. Test normal weapon lift near walls before stage 03, then no-lift behavior after it. Confirm manual raise/lower, inventory swaps, reloads and shooting states.
5. Test death while aiming/moving, respawn, unconsciousness, disconnect/reconnect and deletion/cleanup of the old player object. Include two players watching each other and a reconnect while another player is sprinting/rolling.
6. Verify jump/punch costs and ordinary stamina, vehicles, ladders, swimming, normal camera collision and server-side damage. Native restrictions should remain except each explicitly introduced feature.
7. Preserve complete same-session RPT, script, error, crash and dump outputs for every failure. Compare instruction/module offsets and exception addresses; do not assign blame from the final player line.

If a stage regresses, remove that stage alone, return to the recorded prior configuration and repeat the same actions/exposure. If the baseline still crashes, keep the result and inspect other loaded overrides or an engine regression; do not declare the four camera mods the cause. Reintroducing a removed suspect to reproduce a matching signature should be done only on the test deployment.

## Packaging and checks

Each numbered directory contains its own addon PBO and matching editable source. The root source tree also includes the investigation and validation documents. Build from the repository with `python tools/build_grz_staged.py`.

Checks: PBO SHA1 checksum, compressed payload round-trip, module/prefix paths, SERVER/client preprocessing, no CameraHandler/HandleView in any staged source, no camera/UI classes in core and no stamina overrides. No Enforce compiler or running DayZ server was available. The package is ready to pack/test, not certified production-safe.

## Missing evidence

Upload the exact deployed four legacy camera PBOs (and OPM if present/repacked), the newer crash session's load records/manifest and matching hashes. The two full ProcDump dumps/matching executable or debugger exports are needed to independently compare native CameraHandler state. None of these are supplied by the pasted summary alone. The older available minidump has been inspected; the newer full dump has not.
