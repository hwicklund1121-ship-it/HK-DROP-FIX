# Shared on-back cancellation clip test

The supplied rifle and pistol parent instances both define separate Throw.Pne.Cancel
and Throw.PoB.Cancel clips. The original Luigi AnimNodeBlendT3 chooses only the
face-down ThrowCancelAnimPne whenever Stance == 2, even though the trace samples
native on_back=true during that cancellation. The existing separate CancelPoB
entry depends on the earlier TagProneBack pose tag, which was not sampled before
the cancellation. This supports testing clip selection, rather than retaining
states until they stall movement. It is not proof of the complete visual cause.

This build starts from the ORIGINAL uploaded Luigi PBO, not the failed parent-exit
experiment. It changes one child reference and adds a zero-duration selector and
one on-back cancel source. For prone cancellation, ThrowingOnBack OR an AimX
outside [-90,90] selects Throw.PoB.Cancel; otherwise it uses the original
Throw.Pne.Cancel. The on-back source carries the same TagProneBack marker as the
original on-back cancel node, plus a diagnostic marker.

All pre-existing transition conditions, durations, clips, native controls,
camera/FOV settings and movement scripts remain as in the original. There are
NO added parent exit guards or persistent input locks. The pre-existing custom
HKCancelRecoverySTM is untouched, including its original entry conditions.

Weapon coverage: the shared graph resolves Throw.PoB.Cancel through the held
item's animation instance. The supplied rifle and pistol parent mappings exist,
so the graph edit is not SKS-specific and does not remap each gun's resources.
Compatibility across every stock/custom weapon is not proven. Other item-family
parents were not supplied; do not assume coverage for every tool or custom mod.

## Install and compare

Back up the original PBOs outside loaded addons folders. Replace BOTH included
PBOs (Luigi and HK_Trace) on client and test server with one loaded copy of each.
Leave your zoom/FOV and ronjam/movement PBOs alone. Remove the prior diagnostic/
parent-exit test copies so they do not override this build. Re-sign modified
PBOs normally if your server requires signatures. Restart the game/server.

Test G -> prone -> look right -> A/D three times with SKS, another rifle, and a
pistol if available. Compare a short clip with the pre-1.30 reference. Check
normal standing/crouched/prone G-cancel, both forward- and back-facing, and
ordinary movement. If this causes a wrong pose, resource error or inability to
recover, restore the original Luigi PBO rather than stacking patches.

Upload the newest script log and a clip. The trace prints version=4_back_cancel
and binds TagHKBackCancelSelected. That marker appearing shows the new source
was evaluated; it does not prove that the final rendered animation is correct.
Graph blend-out tags can linger, so visual confirmation is required.

Packaging checks verify that removing the added selector/source and restoring
the single child reference reproduces the original graph exactly, all unrelated
PBO entries/metadata are identical, and new payload/checksum round-trips pass.
No DayZ runtime is available here. This is a clip-selection candidate, not a
confirmed restoration of the old glitch or a verified all-guns release.
