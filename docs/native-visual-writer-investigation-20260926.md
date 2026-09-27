# Native visual-writer investigation — 26 September 2026

The missing first title image has two demonstrated contributors: native
rendering misses a brief matching tilemap state, and direct startup has cleared
eight BIOS-written palette entries that remain visible in original frame 1209.
The subsequent tile-writing sequence also has different timing. These are
measured defects in correspondence, not a completed repair contract. No
production behavior changed, and all acceptance gates remain open.

## Complete reconstruction of the observed native renders

A temporary observer records visual-memory mutations from the existing native
runtime: masked word writes, bulk asset loads, full-region clears and completed
render boundaries. It records global event order, device time/frame, checkpoint
count, CPU/state, active context PC, function owner and native timing position.
The native memory is never supplied with expected reference values.

The 14-second neutral run contains:

| Observation | Count |
| --- | ---: |
| Masked visual word writes | 362,991 |
| Bulk visual loads | 112 |
| Visual-region clears | 21 |
| Completed render records | 805 |
| Total ordered events | 363,929 |

Replaying every mutation validates its old word and write mask, and reconstructs
all seven visual RAM regions at all 805 render boundaries: **zero differing
region hashes across 5,635 comparisons**. This includes initialization and
direct loads, so those changes cannot silently disappear from the writer trace.

The entire render capture, sound/timer bus, PCM output and final status are
byte-identical to the preceding live baseline. The run completes at 14 seconds,
device frame 805, with no fault and 62,251,303 checkpoints. All 788 baseline
native build-source hashes remain unchanged.

This proves mutation-to-render reconstruction for this run. The active context
PC is not always the executed instruction PC: F291 reports its entry PC 0x15fe6
at multiple writes. Its saved native timing ordinal is also not a global
instruction provenance guarantee. These fields must not be relabelled as exact
instruction provenance or used to claim complete original/native parity.

## Title tilemap and writer timing

Reference review frame 1209 and native's first nonblack render, ordinal 129,
have matching observed tile submission metadata but differ in 48 tile words
(94 bytes). The original frame still contains zeroes at those addresses,
written by CPU B's tile/window clear at PC 0x85fe. Native has already replaced
them through F291, `cpu_b_write_incrementing_word_pair`.

The complete native tile RAM equals frame 1209's tile RAM during exactly one
recorded interval:

- It becomes equal at native event 100175, time **2,244,050,100 ns**.
- The next tile write, event 100176 at **2,244,065,300 ns**, ends equality.
- The mixer is not blank, but **no native render occurs in this 15.2 µs interval**.
- All eight investigated palette words still differ at its beginning.

The corresponding first 48 nonzero writes to the differing addresses match
between native and original in address, old value, data, mask, new value and
order. Their spans differ:

| Producer | First-to-last span of those 48 writes |
| --- | ---: |
| Native F291 | 15,200 ns |
| Original CPU B, PCs 0x15fea/0x15fee | 161,400 ns |

Original first/last timestamps are retained at attosecond precision. Native
context PCs identify function ownership, not a captured instruction-by-instruction
match. The native function source performs these writes without explicit
instruction timing around each write; the measured span includes surrounding
execution. The report does not assign the entire span difference to one
instruction or prove that adjusting this function alone would fix presentation.
No absolute time offset or source-frame alignment was fitted.

## Visible startup palette difference

Reference frame 1209 uses palette entries 977, 982, 984, 987, 993, 998, 1000 and
1003 for **558 pixels**. Their original raw palette values are 0x0fff, 0x0fea,
0x0ccc and 0x0777, repeated across the two groups. The original writer is CPU A,
PC 0x5fa, during BIOS initialization; its exact write events remain in the
frozen capture.

At native title render 129, all eight raw words are zero. Their last native
producer is the full palette clear at trace event 964, device time zero. Source
inspection binds that clear to `prepare_direct_boot`, called again when the
approved logo display transitions into direct-loaded game startup. The zeroes
are therefore explained by startup state, rather than missing palette assets or
an unknown final color conversion.

Across the complete retained native trace, the full reference tile RAM and all
palette RAM words used by either title image never match simultaneously. A
render-trigger change alone cannot establish this resource contract. The
approved loading-policy change does not itself prove the resulting title palette
equivalent; required exceptions and original state must remain explicit.

## Evidence and limits

`analysis/acceptance-native-visual-writers-current.json` retains the complete
compressed binary trace, format/field definitions, build and process records,
observer sources, all render-boundary checks, 808 sprite-root writes, and the
selected native/original title write histories. The title analysis independently
decodes and checks the retained trace. Original source evidence remains bound to
its existing immutable package and capture hashes.

Reproduction uses `analysis/acceptance-native-visual-writers.py`,
`analysis/retain-native-visual-writers.py` and
`analysis/analyze-native-title-writers.py`. A retention-script iterator call was
corrected before successful analysis; it did not cause another native run.

Full original/native handoff, per-instruction producer provenance, captured
draw/state consumption, repeated presentations, full scenarios, audio and
fixture acceptance remain unresolved. No native function or render timing is
edited without the binding complete-contract and readiness gates.

Automatic approval review rejected the single guarded removal attempt for
`.tmp/acceptance-native-visual-writers-20260926` with "blocked by policy" and no
more specific reason. The command did not execute; the temporary executable,
raw captures, build output and intermediate logs remain. Cleanup is incomplete.
No alternate deletion was attempted and previous rejected removals were not
retried.
