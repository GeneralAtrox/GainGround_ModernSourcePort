# Input-sampling device and fixture repairs — 26 September 2026

The native I/O device now returns zero in the unmapped upper byte of its
full-word reads. The fixture comparator also executes the fourth input sample
inside enclosing F116 captures while preserving standalone F119's earlier
boundary. No translated game function or `RuntimeHost` source changed.

## Why the live-device correction was needed

The required real-child regression disproved the assumption that live input
reads already preserved the original register result. Its return PC, stack,
four samples and byte-level input transitions were correct, but D0 ended as
`1234FF55` instead of the source-derived `12340055`.

The original map connects the 8-bit I/O chip to the low byte of the 16-bit bus.
The address space's unmapped upper lane defaults to zero. The device handler
returns an 8-bit value, and the memory handler combines it with that zero upper
lane. Original F120 captures record full-word idle input reads of `00FF` and
retain zero in D0 bits 15:8 after the byte operations. Native reads incorrectly
filled those bits with ones. The one-line correction changes only the I/O
bank's read projection; port values, writes and instruction timing are unchanged.

This was a retained-register discrepancy. It is not evidence that the user's
playtest had a visible input fault. Complete scene and timing parity remain open.

## Captured partition correction

The [earlier investigation](fixture-input-boundary-investigation-20260926.md)
proved that standalone F119 ends before the fourth sample, while enclosing
F116 captures include it and its return. The integrated comparator uses exactly
the diagnostic condition: root F116, F120, CPU B/state 72, kind 0, and the
original callsite/target. It preserves marker and hardware checks and adds no
compatibility rule. F116's unresolved indirect-target contract remains open;
its native body was not edited.

## Validation

The fresh Release build completed 759 build steps, and native/compatibility
generation checks passed. All 17 required real-child tests pass. The remaining
research tests pass 20/21, for **37/38 overall**. The only remaining suite failure
is the known F1 record-1 capture omission at sequence 7,359,792. No compatibility
exception was added for it.

Both new regressions failed against the previous libraries before the repairs.
The captured-record test uses original records from four independent sources,
rejects 48 damaged marker/input contracts, and preserves all 11 standalone
F119/F120 fixtures. The real-child test executes all four F120 invocations
through `RuntimeHost`, including the final fallthrough and outer RTS. It checks
press/release/held states, four child executions, store-instruction counts,
retained registers, SR/state, return PC/SP and stack guards. Separate mask and
mirror checks cover the I/O bank's upper-lane projection.

The initial negative-test draft expected a changed recorded input value to
fail as a hardware-value mismatch. Because a recorded read defines the test
input, the comparator correctly rejects the resulting memory output instead.
That assertion was corrected, verified on all four original records, and rerun
against both the previous and repaired libraries. This was a test expectation
correction; the production comparator's rejection was retained.

All 31,971 F115/F116 comparisons complete: **10,925 pass and 21,046 diverge**.
Every per-record report exactly matches the earlier scoped diagnostic, including
the unchanged F115 results. That retains all previous passes and adds 94 F116
passes. Later partition, self-continuation and memory discrepancies remain open;
the full-corpus and complete scenario gates still fail.

The instruction observations used by the child test are explicitly incomplete
timing observations. They do not certify complete bus traces or timing parity.

Private evidence is retained in
`analysis/acceptance-input-read-lane-contract-current.json`,
`analysis/acceptance-fixture-input-repair-before.json` and
`analysis/acceptance-fixture-input-repair-current.json`. Original source files
are retained by hash with the before-build evidence. The canonical acceptance
report identifies the current build-source binding separately from historical
baselines. The playable review package has not been replaced.
