# F141 path and timing evidence — 2026-09-26

F141's local path contract is now complete, but its transitive implementation
gate and timing contract remain open. No native behavior changed in this pass.

## Local path admission

The already registered controlled original capture `next15-stage-music-r1`
contains three complete F141 records. Record 5571, frame 2670, enters with stage
word `$0C02=10` and supplies the remaining `A73A -> A73E` branch outcome. The
debugger set stage 9 before the transition; original code incremented it to 10.
All four captured debugger actions occur outside these F141 instruction
intervals. This establishes controlled original behavior, not natural stage-ten
reachability or native replay eligibility.

All 73 local instructions and required local branch outcomes are now observed.
The full readiness audit covers 20 functions and still blocks on F266, F267,
F277, F323 and F325. Historical stage-two reports retain their earlier gate;
the canonical F141 packet records the current gate.

## Original instruction and bus observation

A neutral modified-loader run of the pinned research executable completed
successfully. Its retained trace contains 9,552 complete instructions and 24,139
bus events, from F141 `A65C` entry at 13.215681957142857136 seconds through
`A772` RTS completion at 13.226241957142857136 seconds, returning to `A63C`.
This is **105,600 clocks / 10.56 ms, including descendants**.

| Owner | Boundary clocks | Clocks | Duration |
| --- | --- | ---: | ---: |
| F141 exclusive | Before, between and after child calls | 11,134 | 1.1134 ms |
| F266 subtree | 11,026–14,852 | 3,826 | 0.3826 ms |
| F277 subtree | 14,872–14,956 | 84 | 0.0084 ms |
| F323 subtree | 14,976–105,532 | 90,556 | 9.0556 ms |

The parent owns each JSR's clocks; each child interval includes its final RTS.
Actual child returns resume at `A70E`, `A714` and `A71A`. The trace has no
inter-instruction clock gaps. Global sequence, instruction ordinals, bus phases,
next-PC linkage, masks, root opcodes and root program fetches were checked.
Every integer timestamp agrees exactly with the 10 MHz clock count. All 225
selected memory operations match the earlier independent observer by absolute
timestamp, original PC, address, value, mask and order, without alignment.
The five retained reference outputs are byte-identical to the preceding run.

## Limits and next required evidence

This invocation observes 51 of F141's 73 static instructions. Stage-dependent
DIVU and sound paths are absent, and no interrupt or exception occurs in the
interval. The diagnostic header intentionally lacks an initial-state hash;
the unchanged strict comparator rejects it as a matched-state trace. The
source digest identifies current observer files, not a reproducible build proof.

The native observer records entries and selected memory accesses, without an
F141 return event. It does not establish native full-return duration. The
10.56 ms reference duration therefore cannot be called a measured native timing
deficit or an explanation of the entire approximately 19.789 ms wait-arrival
difference.

Timing implementation still requires the five child path blockers to be
resolved, coverage of the missing timing paths and interrupt behavior, and a
matched-state native/reference instruction comparison. The F323 subtree is the
largest measured interval to investigate. No fitted delay is justified.

Private evidence: `analysis/acceptance-function141-stage10-current.json`,
`analysis/acceptance-function141-timing-current.json` (lossless trace and
per-instruction measurements), and `analysis/function-work/function141.json`.
The canonical acceptance report links both follow-ups. Graphics, audio, timing,
scenario/fixture and handoff acceptance remain open.
