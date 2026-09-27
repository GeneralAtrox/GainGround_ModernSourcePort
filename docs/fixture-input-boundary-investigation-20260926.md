# Input-sampling fixture boundary investigation — 26 September 2026

A source-bound comparator diagnostic executes the fourth input sample inside
enclosing F116 captures, while preserving standalone F119's earlier capture
boundary. It produces 94 additional passing comparisons without changing any
game function or the production comparator. This is diagnostic evidence, not
an integrated repair or full acceptance.

## Original boundary evidence

F116 calls F119 with a saved return to its dispatch loop. F119 makes three
subroutine calls to F120, advances the input address, then falls through to
F120 for the fourth sample. The final F120 return consumes F116's saved return.

Original record 67106 ends at that sequential owner boundary. Record 67115
starts at the same instruction index with exactly the same registers, reads
the saved outer return, and finishes with the expected PC and stack pointer.
Both belong to source 0. Enclosing record 35286 contains the sequential marker,
fourth hardware input read, final return edge and subsequent dispatch calls.
Thus stopping standalone F119 is correct; stopping the enclosing record there
omits behavior that its authoritative capture includes.

The diagnostic changes only one comparator condition: root F116, child F120,
CPU B/state 72, sequential kind 0, callsite 85CE and target 85D0. Existing
sequence, identity, state, callsite, target and hardware checks remain active.
It adds no compatibility rule and changes no fixture admission.

## Results

All six F119 and five F120 standalone fixtures still pass. The complete F116
diagnostic returns all 31,787 eligible records with no process failure or
timeout in 94.344 seconds:

| Result | Current production comparator | Diagnostic |
| --- | ---: | ---: |
| Passed | 10,660 | 10,754 |
| Diverged | 21,127 | 21,033 |

All 10,660 existing passes remain passes. Of the former call-count failures,
94 now pass, 17,191 expose a later call-sequence failure, 77 expose a later
compatibility-provenance rejection, and 3,739 remain call-count failures.
The other 22 call-sequence and four memory failures remain in their original
categories. Later failures are failures, not partial passes; this result does
not reclassify the whole-corpus diagnostic as a production-suite pass.

Record 35286 now reaches the later F147/F148 sequential boundary before
stopping. Record 35288 reaches a captured self-continuation marker at its loop
end but reports a sequence discrepancy. Those distinct boundaries require
their own source/capture investigation; they are not covered by this narrow
diagnostic. No broad sequential-marker exception was introduced.

## Current authority and limits

Fresh F119/F120 work packets have clear translation gates. F116's gate remains
blocked by its unresolved indirect call target domain. No F116 native behavior
was edited. Production integration also remains pending targeted regression,
real-child return validation and the required integrated checks.

Private evidence is retained in
`analysis/acceptance-fixture-input-boundary-current.json` and
`analysis/acceptance-fixture-input-census-current.json`: bounded capture slices,
source identities, original record identities, comparator source, build output,
every returned comparison and before/after classification. Canonical packets
are `analysis/function-work/function116.json`, `function119.json` and
`function120.json`. All prior 790 build-source identities remain unchanged.

Live graphics provenance, matched audio/timing, comprehensive scenarios and
review-build acceptance remain open.
