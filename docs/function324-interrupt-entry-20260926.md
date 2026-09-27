# Controlled IRQ3 entry and return inventory — 2026-09-26

The expanded original-machine observer records all 299 CPU bus accesses from
the selected terminal DBRA through IRQ3 entry, the executed handler path and
RTE continuation. The original microcode predicts the same bus order, addresses,
masks and phase offsets. All 283 opcode words match the retained opcode bank
selected by the observed IRQ-mode flag.

The capture preserves every prior held-control observation, all five reference
outputs and its instruction trace. The instruction trace still has its original
unsupported exception-transition event and incomplete footer. This separate bus
inventory does not relabel that trace complete.

## Observed interval

Times below are CPU clocks relative to the interrupted DBRA's entry.

| Phase | Clock or duration |
| --- | ---: |
| Terminal DBRA completes | 14 |
| Return-PC low word written at old SP minus two | 20 |
| IRQ3 autovector acknowledged | 37 |
| Saved SR written; SP commits to exception frame | 46 |
| Return-PC high word written | 50 |
| Vector data words read at `0x6C` and `0x6E` | 54, 58 |
| Handler opcode prefetches at `0x8034` and `0x8036` | 62, 68 |
| Handler's first instruction begins | 72 |
| RTE at `0x8032` begins | 1464 |
| Original continuation at `0x1B9B4` begins | 1484 |

The exception-entry duration is 58 clocks. Its VPA request occurs at clock 24;
the reconstructed CPU-local phase predicts a 13-clock synchronization wait,
matching acknowledgement at clock 37. The handler including RTE takes another
1,412 clocks across 139 instructions at 12 distinct PCs. These counts describe
the controlled invocation, not a universal interrupt delay.

The inventory contains 283 opcode reads, nine program-data reads, six
program-data writes and one CPU-space interrupt acknowledgement. It is checked
against the generated original instruction handlers, including 127 taken
DBRAs and one terminal DBRA in the handler's delay loop. A second source review
independently confirmed the count and instruction durations.

## State and bus details

The first stack write occurs before IRQ mode activates. IRQ mode is active at
acknowledgement and during the prologue. The `CMP.L #$0300FFFF` instruction at
`0x803C` exits IRQ mode after reading its immediate low word and before fetching
the state-72 continuation at `0x8042`. Saved FD1094 base state remains `0x72`.

The byte test's effective address is `0xFFFF8001`; the word-oriented bus tap
reports aligned address `0xFF8000` with mask `0x00FF`. Its unused high byte is
not the byte consumed by the instruction.

MOVEM at `0x802E` restores D0 and performs an additional read of the saved SR
word. RTE then reads that SR word again. Removing either read would omit an
authoritative bus access. At RTE, SP advances before the third stack read; SR
commits after it. Both resumed opcode fetches are captured, and the following
instruction sees the expected registers, SP, SR and FD1094 mode.

The observer watches all three CPU bus spaces during the bounded interval.
Its own known debugger sentinel reads are excluded, and its controlled timer
MMIO writes are recorded separately. Instruction-entry observations distinguish
executed instructions from prefetched words.

## Remaining implementation gates

Current project audits still report two immediate blockers:

- F99 has an uncatalogued state-changing successor from `0x803C` to `0x8042`.
- F103 lacks the branch outcome from `0x804C` to `0x801E`.

This capture establishes the observed IRQ3 path, not every handler path, IRQ
level, trace/NMI, retry or fault behavior. Native consumption of the captured
fields and the common native/reference starting state remain unproved. No
native behavior, build or review package changed.

Private evidence is in
`analysis/acceptance-function324-irq-entry-contract-current.json`, with the
complete source excerpts and reproducible validator linked there. The current
F324, F99 and F103 packets retain the results and open gates. Full graphics,
audio, timing, fixture/scenario and review-build acceptance remain open.
