# Recovery state markers — diagnosis only

The second trace has 495 samples and again no TagProneBackExit or native
rising_from_back=true. It therefore does not demonstrate that the late-entry
candidate fixed recovery. Throw wind-up/playback are false throughout.

This package retains the previous late-entry test's two entry conditions and
adds unique diagnostic Tags to six graph nodes/states. No condition, animation,
blend, duration or camera value differs from that previous candidate. The new
tag names are referenced only by the trace script, not by gameplay conditions.
The purpose is to determine whether the modified graph loads and which cancel
branch actually evaluates; the existing broad ThrowingCancel tag cannot identify
the nested recovery state by itself.

Markers:
- TagHKRecoveryGraphV1: HKCancelRecoverySTM
- TagHKRecoveryOriginal: its Original state
- TagHKRecoveryProneCancel: its ProneCancel state
- TagHKRecoveryBackRise: its BackRise state
- TagHKCancelParent: ThrowSTM.Cancel
- TagHKCancelPoBParent: ThrowSTM.CancelPoB

The updated HK_Trace prints version=3_state_markers and logs tag binding results.
Its interval is 0.01 seconds (limited by the actual command update rate), with a
2400-sample cap. It still only reads player state after the inherited handler.
Tag IDs/bindings are diagnostic evidence, not proof that a branch was rendered.

Replace BOTH included PBOs (Luigi and HK_Trace) on your client/test server, keeping
one copy of each. Back up previous copies outside loaded addons folders. Leave
zoom and movement PBOs unchanged. Use your normal signing workflow if needed;
the changed PBOs are unsigned and old signatures do not authenticate them.

Start a fresh session, wait briefly, then do THREE G -> prone -> look right ->
A/D sequences. Also do one ordinary standing G-cancel. Upload the newest script
log and optionally a short clip. This package is not another animation fix.
Restore the original Luigi PBO if behavior is worse or graph loading fails.

Packaging validation checks: only added Tags distinguish the graph from the
previous late-entry candidate; its entry conditions remain unchanged. Original
AGR/config/archive metadata and every unrelated payload are preserved. Trace
payloads/checksums are verified. Compilation and native graph behavior require
the actual DayZ run; no runtime success is claimed here.
