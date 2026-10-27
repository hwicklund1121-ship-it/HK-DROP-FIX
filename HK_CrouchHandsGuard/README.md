# HK Crouch Hands Guard — experimental

Requests lowered hands while an empty-handed local player changes stance into
or out of crouch. Uses DayZ's `OverrideRaise(ONE_FRAME, false)` and preserves the
normal command-handler chain. It does not replace Luigi, player graphs, camera
code, or movement tuning. No edits to the supplied PBOs are required.

This is an **unverified in-game workaround**, not a confirmed fix. A crouching
animation can move the arms even when the player is not in the raised state.
An input override cannot remove that motion from an animation clip. No specific
motorcycle animation has been identified as the cause.

## Build and test

Pack this directory with DayZ Tools Addon Builder, with PBO prefix
`HK_CrouchHandsGuard`. Put the resulting PBO in
`@HK_CrouchHandsGuard/addons/`. Load `@HK_CrouchHandsGuard` using `-mod` on
both the client and your test server. For a server enforcing signatures, sign
the PBO using your normal DayZ signing workflow and install its public key.

Test first with vanilla plus this addon. Repeatedly crouch and stand with empty
hands, both stationary and moving. Check third-person and another player's view.
Compare with the addon unloaded. The client's script log should contain
`[HKCROUCHHANDS] lower_request` during eligible transitions, followed by
`lower_request_end`. If no request is logged, the guard did not engage; do not
interpret an unchanged pose as a failed animation fix in that case.

Also verify weapon aiming, item use, empty-handed attacks, throwing, emotes and
vehicle entry/exit. These states are excluded from the guard. Empty-handed guard
raising without an attack is intentionally suppressed during a crouch transition;
it is available again when the transition ends. The override expires after one
frame, so it does not leave an enabled input lock on exit.

If the hands still lift while requests are logged, report the clip and matching
`HKCROUCHHANDS` lines. `input_raised` and `graph_raised` are captured **before**
the request; they do not prove what the graph did afterwards. A pose with both
false points away from a raised-hands state and toward the stance animation.
That requires examining the matching game-version animation assets rather than
adding guessed animation names or changing global animation speed.

## Validation limits

API names were checked against Bohemia Interactive's published DayZ-Script-Diff
`main` player and human scripts. A DayZ compiler/runtime is not installed here;
compilation, visual behavior, multiplayer replication and compatibility with your
exact game build still require the test above.
