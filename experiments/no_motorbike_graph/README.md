# Motorcycle graph isolation experiment — SOURCE ONLY

This is not an installable mod or a compiled animation graph. Putting this AGF
in an addons folder or wrapping it alone in a PBO does not establish a runtime
override. No DayZ graph compiler or complete parent graph is available here.

Changed exactly seven motorcycle selectors in the uploaded vehicles.agf:
five blend conditions for vehicle types 10/11, and the two start conditions for
Jawa_05/Jawa_Bitrak, are set to false. Other vehicle selectors, nodes and clip
references are unchanged. Motorcycle animations remain defined but these
selectors no longer choose their branches. This intentionally disables normal
motorcycle branch selection; riding motorcycles is outside the test's supported
behavior. Keeping car and boat source unchanged does not prove they work at runtime.

Keep the original file backed up. In a COPY of the matching complete player graph
project, substitute this file at the original vehicles.agf include path, then
compile/export the parent graph using the matching DayZ Tools workflow. A new
addon must point the player at that compiled graph and retain its required
animation instance, assets and references. The parent graph and its original
resource location are needed before a reliable deployable PBO can be prepared.
Do not change the live base-game PBOs or replace the original backup.

For testing, remove HK_CrouchHandsGuard so the experiment has one variable. Compare
empty-handed crouching/standing with and without the compiled graph override,
then test car/boat entry, driving, passenger seats and exit. Restart the client
and test server between variants. Sign any test PBO using your normal workflow.
If the fists still rise, the motorcycle selectors in this sheet are not enough
to explain the bug; do not treat this experiment as a confirmed fix.

Validation here: exactly seven selector lines changed; all remaining source
lines are identical to the upload. Compilation and in-game tests have not run.
