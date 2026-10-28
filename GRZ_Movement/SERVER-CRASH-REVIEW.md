# Server crash review and v0.32 changes

Reviewed the uploaded 1793213243.zip logs locally. No raw server logs, account identifiers, network addresses or minidump are included in this release.

## Evidence

- DayZ Server version 1.30.164200.56.
- Two earlier script exceptions are NULL-pointer errors in TimerQueue.Tick, followed by the game update hook chain. The stacks do not identify the scheduled callback owner.
- The final crash is a native access violation at 14:46:49 during networked player simulation. The provided stack is not symbolized enough to identify an offending script or mod.
- No CameraHandler or GRZ_Movement name appears in the supplied RPT. This does not establish that GRZ was absent: the installed code may be repacked/renamed. The exact deployed GRZ PBO/version is not verified by this archive.
- These observations do not prove that GRZ camera code caused the crash or that the earlier timer exceptions caused the final native crash.

## Change

Wrap the following in #ifndef SERVER: GRZLegacyCamera, GRZCameraDistance, GRZSprintEyeZoom, GRZSteadyCamera, GRZZoomContinuity, GRZZoomTrace, the Game-layer camera preferences and Mission-layer camera menu. Dedicated builds therefore compile none of these GRZ overrides. Retain shared movement, running-lean and weapon-lift policies. Remove GRZSelectiveStamina entirely at the user request; stamina returns to engine/server/other-mod behavior. Add client callback null guards before inherited updates/accesses. Keep existing client tuning and settings ranges.

## Validation and deployment

Local validation checks PBO checksums/source round-trip and preprocessed client/SERVER script sets. It does not compile Enforce Script or run a DayZ server. Replace the existing PBO with the new same-name PBO; remove duplicate older/repacked GRZ camera scripts. Use matching builds and appropriate signatures. Verify client Settings, held eye zoom, running lean, prone recovery, normal stamina and weapon obstruction. Reproduce the server scenario on a test server before general rollout. If another crash occurs, collect the new same-session RPT/crash/script logs and the exact deployed PBO; native symbols/engine debugging may be needed for the final access violation.
