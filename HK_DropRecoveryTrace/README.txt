HK drop recovery trace — diagnostic only

This is NOT an animation fix. Both included PBOs are byte-identical to the
original repository uploads: LUGUINSTADROPFIX.pbo and HK_Trace.pbo. No graph,
throwing rule, input, camera or movement values were edited.

Back up your current files outside loaded addons folders. Use the ORIGINAL
Luigi PBO supplied here and place HK_Trace.pbo alongside it in your loaded mod's
addons folder. If HK_Trace is already loaded, replace that copy rather than
loading a duplicate. Keep your original movement and FOV-revert PBOs; disable
the experimental camera/zoom addons. Use your normal matching signature setup
on a server requiring signatures. This package contains no keys/signatures.

Start a fresh DayZ session. Reproduce exactly the recorded sequence 3 times:
G -> prone -> look right -> A/D quick recovery. Leave a couple of seconds between
attempts. Do not deliberately throw your held item just for the test. Recording
a short clip at the same time helps match the visible snap to the logged state.

Close DayZ, then upload the newest script_*.log from %LOCALAPPDATA%\DayZ.
The log should contain [HKTRACE] ready version=2 followed by samples showing
throw_mode, throw_windup, throw_playing, stance, changing, on_back,
rising_from_back, input movement and active graph tags. If no [HKTRACE] entries
appear, the trace did not run; say that rather than treating it as a fix result.
Sampling is bounded (1200 samples at 0.05 s while relevant tags/transitions are
active). It calls the inherited command handler and observes the local player.

Why: the recorded pre-1.30 recovery includes feet-up movement and a slower rise,
whereas the post-1.30 side straightens much sooner. The original Luigi graph
includes HKCancelRecoverySTM with a tagged BackRise path, but that branch is
entered through throw cancellation. The G/prone/look-right/A-D sequence needs
to be observed to establish whether it reaches that branch and when it exits.
The official DayZ-Script-Diff throwing script has no changes between builds
1.29.163047 and 1.30.164200, so a new throwing-script cancellation check is not
established as the cause. Graph/native animation behavior remains under diagnosis.

This ZIP changes nothing about the requested movement. No repaired behavior is
claimed. Do not mix it with motorcycle-blocking graph experiments.
