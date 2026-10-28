# Parent-exit recovery experiment

The latest state-marker trace binds all six custom tags, confirming the modified
graph is registered. Across three attempts it samples Cancel -> ProneCancel,
but never the custom BackRise marker. The parent Cancel marker disappears after
about 0.13–0.14 s per attempt. TagProneBackExit later appears without the custom
BackRise marker, so that broad tag alone cannot prove this recovery path ran.
These observations do not expose the graph's complete internal evaluation order.

This test starts from the state-marker candidate. It appends a guard to the five
ordinary exits from the outer Throwing state: IdleLow, MovementLow, MovementRas,
IdleRas and PneOnBack. The guard prevents those exits only while the nested
ProneCancel marker AND parent Cancel marker are present and AimX is outside
[-90,90]. This targets the sampled backwards/right-facing cancel window. It is
an early-exit hypothesis test, not a confirmed native-animation fix.

No animation clips, rates, durations, cameras, zoom, movement scripts or existing
recovery transitions change from the preceding diagnostic build. The existing
late-entry candidate and unique state markers remain present. The updated trace
PBO is identical to the previous state-marker package.

Install BOTH PBOs from addons/ on client/test server, replacing the previous test
copies. Back up your originals outside loaded addons folders; keep one loaded
copy of each. Leave zoom and ronjam/movement files alone. Re-sign changed PBOs
normally where server signatures are required. Restart before testing.

Repeat the G/prone/look-right/A-D sequence three times and send the new script
log and a short clip. We need to see whether TagHKRecoveryBackRise activates and
whether its visible recovery approaches the pre-1.30 reference.

Also test ordinary standing, crouched and prone G-cancel, and on-back cancellation
without trying to stand. A known possible regression is retaining the cancel
pose while facing behind without a subsequent stance change. Turning the aim
back within [-90,90] removes the new guard. If movement/cancellation gets stuck
or behavior worsens, stop and restore the original Luigi PBO. No input is locked
by this script, but graph state retention can still affect animation/movement.

Validation here: exactly five transition-condition lines differ from the prior
diagnostic graph; all other original archive entries/metadata and the trace PBO
are preserved. PBO and ZIP checksums verified. No DayZ runtime is installed here,
so compilation, visual behavior and ordinary-cancel regressions remain untested.
