# GRZ DayZ 1.30 crash investigation

## Assessment

Root cause remains unproven. The old camera stack is a reasonable isolation target, with StormZz_Zoom the highest-priority available source to remove from the baseline: it replaces CameraHandler, executes the native handler but discards its return, then reimplements camera selection with older logic. That can conflict with newer native camera rules and downstream overrides. Neither the code pattern nor the native crash address proves that this PBO caused an access violation.

GRZ replacement deployment is not confirmed for either reported incident. The supplied older RPT explicitly loads the four legacy camera PBOs; this is stronger evidence than their presence on disk. An absent name does not exclude renamed or repacked scripts.

## Directly verified from supplied files

Input: 1793213243.zip, plus the local original StormZz_Zoom.pbo and GRZ/LWP/Ronjam source resources. Raw server logs and player identifiers are not redistributed.

- RPT: DayZServer version 1.30.164200.56.
- RPT PBO-load lines 545, 551, 713 and 716 load GRZ_Zoom, HerosperNo3rdPersonSprintShake, StormZz_Script and StormZz_Zoom from the same Workshop pack, @3826418564/addons. The StormZz_Zoom oldzoom.c warning also identifies compiled source from that addon.
- No named PBO-load entry for OPM_Scripts, GRZ_Movement, LUGUINSTADROPFIX, lwp_movement or ronjam_fix in that RPT. This does not prove their logic was absent from repacks. Ordinary item names containing OPM are not evidence that OPM_Scripts loaded.
- The available native minidump exception stream records code 0xC0000005, read operation, attempted address 0x38, module DayZServer_x64.exe, module-relative instruction offset 0xA30EB9. See available-minidump-summary.json.
- RPT ends in networked-player simulation before the access violation. It does not identify the responsible player or mod.
- Earlier TimerQueue.Tick null exceptions are present. Their update hook chain does not identify the timer callback owner. They are not proven to cause the later native access violation.
- The local original StormZz_Zoom calls super.CameraHandler but ignores its returned mode and follows it with a full legacy selection tree. It lacks the newer handler's obstruction/ADS fallback handling in that replacement tree. It has no dedicated-server exclusion.

## Reported, not independently verified here

The newly pasted investigation reports a second dump with offset +0xA3387A, read 0x104 and RAX=0, plus nearby memory containing CameraHandler. The raw ProcDump full dumps and the matching server executable/private symbols are not present in this workspace. Thus this work cannot independently reproduce its register, vtable or latest-session deployment findings.

The listed instruction chain loads a member after a virtual dispatch and then faults on a subsequent member access. It does not, by itself, show which script or PBO created the invalid state. A CameraHandler string near a vtable is supporting context, not an address-to-script mapping. Likewise CDPInitServer+offset is not a reliable private function name.

The historical improvement after removing OPM is an important operational observation. It is not a controlled comparison: uptime alone does not account for player-hours, load, triggers, other deployment changes or competing crash causes.

## Scope of the supplied fixes

Existing full GRZ v0.32 excludes GRZ camera/UI overrides from dedicated SERVER compilation and removes GRZ unlimited stamina. That does not remove other mods' camera overrides, prove the client-side camera replacements correct, or establish production stability.

The separate staged package goes further for isolation: its core includes only current GRZ movement-response initialization. Lean, no-object weapon lift, eye zoom, sprint roll and crouch-height correction each get their own optional PBO. No stage overrides CameraHandler or HandleView, changes camera IDs, changes camera bones, introduces FOV UI, or replaces player stance/recovery callbacks. Camera/eye-zoom additions are excluded from SERVER builds. Stamina stays native throughout.

These are controlled-test builds, not a proven native-engine crash fix. No production server, executable or third-party PBO was modified remotely.

## Exact deployment evidence still needed

For each incident/test session preserve the launch command, RPT PBO-load records, client/server file hashes, deployed PBO names and any repacked script inventory. The supplied logs identify one older session, not the live server or the newer crash session. The actual deployed StormZz_Script, GRZ_Zoom, Herosper and OPM PBO/source are needed to verify the reported override details and hidden duplicates. The local StormZz_Zoom copy must also be hash-compared to production before treating it as identical.

The remaining native comparison requires both raw dumps (or reproducible debugger exports), the matching DayZServer executable build/module identity and the latest same-session logs. Compare process IDs/start times, PE identity and exception addresses; dump filename times and log times differ and should not be assumed to align.

Suggested WinDbg export, for each dump: .ecxr; !analyze -v; kv; r; lmvm DayZServer_x64; u @rip-30 @rip+30. Capture pointer reads only where memory is present. For the reported second dump also export dq @rbx-10 L2; dq poi(@rbx-10)+0B38 L1; dq poi(poi(@rbx-10)+0B38) L8; dps poi(poi(poi(@rbx-10)+0B38)) L24. Correlate the faulting instruction and enclosing module-relative range, not misleading nearest public-symbol labels. A minidump may not contain the required heap pages.

## Confidence

High: four legacy PBOs loaded in the supplied older session; available minidump's exception/address/relative offset; local original StormZz_Zoom source behavior; staged package composition and preprocessor results.

Moderate: old camera overrides are a useful fault-isolation boundary, particularly the full legacy CameraHandler replacement. Their involvement as a group remains a hypothesis.

Low/unestablished: which individual PBO caused either crash, whether both crashes share one cause, precise script-to-native object lifetime failure, or whether any proposed build is production-stable. No available evidence supports declaring a particular author/mod responsible.
