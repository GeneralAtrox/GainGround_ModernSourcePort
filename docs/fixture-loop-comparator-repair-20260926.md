# Enclosing-loop fixture comparator repair — 26 September 2026

The fixture comparator now consumes captured F118 branch-loop markers inside
F115/F116 without recursively invoking another F118 host call. It preserves
the eventual original interrupt boundary. This integrates the source-backed
correction investigated in the [fixture census](fixture-census-investigation-20260926.md).
No translated game function, `RuntimeHost` behavior or compatibility rule changed.

## Problem and correction

Original F118 executes `TST.B $502.w` and `BPL.B $85B0` while waiting for the
frame flag. Enclosing captures contain thousands of kind-1 branch markers,
followed by an interrupt. The previous fixture host recursively entered F118
for those markers. Its default stack overflowed; a larger stack exposed an
incorrect conversion of interrupt control 5 into continuation control 4.

The existing enclosing-F118 handling for F628 now also applies to roots F115
and F116. Every next marker must still match sequence, owner, CPU, state, kind,
callsite and target, with no unconsumed hardware event before it. After consuming
that marker, execution continues in the same loop. No marker is omitted and no
new compatibility exception is added.

## Validation

The new captured-record regression fails before the repair with Windows stack
overflow (`0xC00000FD`). Afterward, both digest-bound original records pass all
comparison checks, and twenty damaged identity/order/hardware cases are rejected
at the expected boundary. All 31 standalone F118 fixtures also pass.

A separate `RuntimeHost` test invokes the real F118 child with both caller return
addresses, `8556` and `857A`. After three actual loop backedges, it asserts the
IRQ5 input and executes the real 101/105 handlers, RTE and final RTS. It checks
PC, SP, retained registers, restored FD1094 state, resulting SR, stack guards,
one interrupt entry and one frame commit. This covers the child path; it does
not claim complete startup or runtime-loop scenario validation.

The fresh Release research build completed all 755 build steps. The required
real-child suite passed **16/16**. The remaining research suite passed **19/20**,
for **35/36 overall**. The sole failing test is still
`gain_ground_all_implemented_fixtures`: F1 record 1 encounters the previously
documented additional call at sequence 7,359,792. That evidence defect was not
hidden or changed by this repair.

The integrated comparator returned all **31,971 F115/F116 comparisons**, with
10,831 passes and 21,140 divergences. Every per-record result exactly matches
the prior temporary diagnostic, including source identities, first divergences
and reported compatibility counts. There were no timeouts or process failures
in this affected-record run. The prior whole-corpus diagnostic's separate F1
record-2 timeout remains unresolved.

The required generation check initially found 51 pre-existing implementation
hashes stale in `native-codegen-manifest.json`. Only that metadata was refreshed:
all seven generated code files already matched, and no implementation content
or verification status changed. Native and compatibility generation checks pass.

## Evidence and remaining acceptance

The current source/test binding is
`analysis/acceptance-fixture-loop-repair-current.json`; the original baseline
source hashes remain historical evidence. The selected live work packet is
`analysis/function-work/function118.json`. The source-bound record census,
before/after regression results, complete build/test output and cleanup outcome
are retained with those reports.

This repair closes the identified recursive-marker comparator defect. It does
not close the full fixture gate, compatibility histogram, missing eligible
fixtures, live graphics provenance, matched audio/timing, complete scene/input
scenarios or review-build acceptance. All T/G/A/F/H gates remain open.
