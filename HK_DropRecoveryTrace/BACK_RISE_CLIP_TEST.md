# Back-rise clip comparison — single-line test

The user reports SharedBackCancel is better and its camera behavior is acceptable,
but the old roll/flick is missing. The supplied current clip shows a short
on-back cancellation followed by a rise, whereas the pre-1.30 reference shows a
more pronounced feet-up recovery. The native trace never records its
standing-from-back flag; tags alone cannot establish the rendered pose.

This test starts from the IMPROVED SharedBackCancel PBO. It changes exactly one
graph line: the added HKBackCancelSource uses Throw.PoB.ToErect instead of
Throw.PoB.Cancel. The rifle parent resolves that to
p_rfl_pne_onBack_trans_erc_low.anm; the pistol parent resolves it to
p_1hd_pne_onBack_trans_erc_low.anm. These are actual supplied bindings. They are
current-version assets, not recovered 1.29 animation binaries; an exact visual
match to the old glitch is not guaranteed.

Selection conditions, every graph transition/blend/time, the on-back tag and
movement/camera files remain as in the improved build. No state retention or
input lock is added. The trace changes only its ready-version string to
5_back_rise_clip. Its existing TagHKBackCancelSelected marker now refers to this
stand-up clip source. The improved previous ZIP remains available for rollback.

Replace BOTH included PBOs on client/test server, keeping one loaded Luigi and
one HK_Trace. Back up current copies outside loaded addons folders. Leave zoom,
FOV and ronjam/movement PBOs unchanged. Re-sign modified PBOs normally where
required; this build is unsigned. Restart after replacement.

Compare three G/prone/look-right/A-D attempts against the improved previous build
and old reference, and send a short clip plus new script log. Check whether the
feet-up recovery now appears, whether it gets cut short, and whether camera feel
stays acceptable. Also check ordinary on-back cancellation without standing:
using a get-up clip there may be visually inappropriate or mismatch the native
stance, so restore the improved build if that happens. This is a clip comparison,
not a confirmed restored glitch or a production release.

Validation: exactly one graph line differs from the improved PBO; all unrelated
payloads/metadata are preserved. Trace ready-version change is verified. PBO/ZIP
checksums and decoded payloads pass. DayZ visual/native behavior is untested here.
