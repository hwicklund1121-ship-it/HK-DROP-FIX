# Luigi recovery late-entry experiment

The supplied trace contains 470 samples. Throw wind-up and throw playback are
false in every sample. There are repeated brief prone/on-back cancellations,
followed by raised-standing indices and ordinary standing. No sample reports
rising_from_back=true or TagProneBackExit. This establishes that the intended
back-rise state was not observed, not that no sub-sample activation occurred.

The original Luigi HKCancelRecoverySTM chooses Original when Stance != 2, and
BackRise has an empty start condition. Thus cancellation entering this machine
after the graph stance has already become standing selects Original rather than
the existing back-rise clip. The trace is consistent with that race, but does
not expose the graph's internal evaluation order or prove it.

This test changes only two start conditions in that one state machine:

- Original: crouched entry, or standing without TagThrowingPneTransition.
- BackRise: standing AND TagThrowingPneTransition.

ProneCancel keeps Stance == 2. Existing transitions, clips, durations, rates,
camera references, AGR, config, movement scripts and FOV are untouched. The
three entry conditions now cover crouch/prone/standing without overlap. A
standing throw cancellation without that transition tag keeps the normal entry.
The trace also shows the transition tag persisting after the glitch, so ordinary
standing G-cancel after a drop still needs a regression check. This
is a diagnostic candidate, not a completed restoration of pre-1.30 behavior.

Back up the original Luigi PBO and signature outside loaded addons folders.
Replace only LUGUINSTADROPFIX.pbo with this test on client and test server, with
one loaded copy. Leave your current original zoom and ronjam/movement PBOs alone.
Keep one copy of HK_Trace loaded (the unchanged trace PBO is included for
convenience). The repacked Luigi PBO is unsigned and requires your usual signing
workflow where signatures are enforced. Restart after replacing the PBO.

Reproduce G -> prone -> look right -> A/D recovery three times, then test normal
G-cancel while standing, crouched and prone, ordinary prone-to-stand movement,
and throwing an expendable item if practical. Stop and restore the original if
normal cancellation incorrectly triggers a back-rise, movement gets stuck, or
the graph fails to load. Upload the new script log and a short clip, whether
improved or unchanged. TagProneBackExit should become observable if this path
is selected and the graph binds it correctly. Do not infer success from loading.

The archive builder verifies the original graph differs by these two conditions
only, retains the original compression method and metadata, and preserves every
other PBO entry byte-for-byte. No DayZ runtime/graph visualization is available
here; runtime behavior remains unverified.
