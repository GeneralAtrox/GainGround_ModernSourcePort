# Descriptor initialization timing — 2026-09-26

The retained original-game trace identifies F324's descriptor clearing loop as
the largest part of the F141 initialization interval. No production code changed,
and no new build or capture was run for this analysis.

| Original execution within F323 | Clocks | Duration |
| --- | ---: | ---: |
| F323's own instructions | 720 | 0.072 ms |
| F324: clear descriptor pool | 86,056 | 8.6056 ms |
| Six F325 descriptor initializations | 3,780 | 0.378 ms |
| Total, including children | 90,556 | 9.0556 ms |

These intervals use actual completed calls and returns. F324 starts at
`1B99C`, clock 15,282, and completes its `1B9B8` RTS at clock 101,338, returning
to `1B95A`. Its 7,812 dynamic instructions cover all nine static instructions
and both outcomes of both loops. All 7,424 word writes match the exact ordered
addresses and zero values for 128 slots, each containing 29 cleared longwords.

Each `MOVE.L D1,(A6)+` takes 12 clocks: high-word write at offset 0, low-word
write at offset 4, then program prefetch at offset 8. Both DBRA instructions
take 10 clocks when looping and 14 on termination, including the branch-target
prefetch on the terminal path. The retained analysis contains each observed
phase pattern, occurrence count and first/last bus examples.

## What still prevents a timing repair

F324's current path/ownership audit is ready, but this is separate from its
timing contract. The original microcode updates low-word flags before the
second write, then A6 and high-word flags before the final prefetch; it also
selects the pending interrupt state there. The native implementation performs
the writes before updating flags, defers architectural A6/D0/D2 updates until
the loops finish, and has no per-instruction timing or interrupt boundary.
Matching final memory alone does not validate these intermediate phases.

The trace does not contain register/flag commit events or interrupt assertion,
sampling and acceptance state. It covers one uninterrupted invocation with no
matched native starting-state identity. Before editing timing, the remaining
source phase contract and interrupt behavior must be established and the
affected native boundary validation made concrete. An aggregate 86,056-clock
delay would not reproduce the observed behavior.

## F323's separate path gaps

F323 still lacks the negative descriptor-count branch and the nonzero stage
callback path. Bounded inspection of six records from the stage-1 and stage-10
sources supplies neither missing outcome. The function-179 stage-39 captures
do not include F323, so their interventions cannot prove its callback behavior.

The baseline data image supplies a discriminating candidate: stage 39's three
descriptor pointers reference header `0001 FFFF`, and its callback is `1845A`.
Other nonzero callback entries occur at stages 7, 9, 19 and 29. These are
candidate input cases, not a closed target domain: the callback lookup uses the
raw stage word with 16-bit offset arithmetic, and CPU-B's low address window
is writable RAM. Earlier descriptions of this table as immutable are not
established by the current memory map. The stage and player-selector domains,
runtime table contents, actual indirect call and return remain proof obligations.

Private evidence: `analysis/acceptance-descriptor-timing-current.json`,
`analysis/function-work/function323.json`, and
`analysis/function-work/function324.json`. The public predecessor is
[F141 timing evidence](function141-timing-evidence-20260926.md).
Graphics, audio, timing, scenario/fixture and handoff acceptance remain open.
