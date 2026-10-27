# Luigi motorcycle-branch isolation PBO — experimental

This ZIP contains a repacked LUGUINSTADROPFIX.pbo. Its existing text AGR now
references a new local copy of Vehicles.agf with seven motorcycle selectors set
to false. All other original PBO entries, including Luigi's locomotion graph,
remain byte-identical. The base-game vehicle graph is not overwritten.

The original assumption that this specific PBO necessarily needed a separate
graph compilation step was incorrect: inspection showed that Luigi already
ships a text AnimSrcGraph AGR referencing text AGF resources. This package uses
that existing structure. Whether DayZ loads and evaluates the modified graph
correctly still requires an in-game test; no DayZ runtime is available here.

## Install on a test setup

1. Back up your original Luigi PBO and its signature outside every loaded addons
   folder. Keep that backup for rollback.
2. Extract @LUGU_NoMotorbike_TEST. Disable the original Luigi mod and load this
   test mod instead on client and test server. If Luigi is inside a combined mod,
   remove its original PBO from that TEST COPY before adding this replacement.
   There must be only one loaded LUGUINSTADROPFIX.pbo / CfgPatches definition.
3. Disable HK_CrouchHandsGuard for this comparison so only the graph changes.
4. This repack is unsigned. The original .bisign cannot authenticate it. If the
   test server enforces signatures, sign this PBO with your normal private key
   and install the corresponding public .bikey on that server.
5. Restart both client and server. Crouch and stand repeatedly empty-handed,
   stationary and moving. Compare against the original Luigi PBO and check the
   client RPT for graph/resource errors. Check another player's view too.
6. Test car and boat entry, driver/passenger seats, driving and exit. Their
   source branches were not edited, but runtime behavior is unverified.

Motorcycle branch selection is intentionally disabled. Do not use this package
for riding motorcycles: mounting, riding, dismounting or death states may be
incorrect. The test does not establish that motorcycle logic caused the fists.
If the pose remains or any graph/resource errors appear, restore the original
Luigi mod and send the error lines and observed result.

No other uploaded graphs or the uploaded ASI were substituted, keeping this
experiment limited to the motorcycle selectors. The original PBO is not edited
by the build script. Archive checksums, changed AGR reference and all untouched
archive entries were checked during packaging. In-game validation has not run.

Rebuild from repository root:
`python3 tools/repack_lugu_no_motorbike.py --output /workspace/artifacts`
