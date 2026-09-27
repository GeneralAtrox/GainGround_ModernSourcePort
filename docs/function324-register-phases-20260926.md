# F324 register and bus phases — 2026-09-26

A read-only reference capture now verifies F324's ordinary register updates at
bus and instruction boundaries. It recorded 27,279 snapshots, matched all
19,466 bus accesses to the retained instruction trace by exact timestamp, PC,
address, value and mask, and checked 463,726 register-field values across 7,812
instructions. Another 27,278 checks verify the exported pipeline PC against
the source phase rules. All nine static instructions are covered.

The complete F141 instruction trace remains byte-identical, as do all five
reference outputs: device writes, audio, sampled inputs, input reads and the
input-observer completion record. No game input, translated implementation or
review build changed.

## Source mapping and observed register updates

Clocks below are relative to each instruction's beginning. Bus access occurs
at the start of its four-clock cycle.

| Instruction | Register/flag update | Pending interrupt sampling | Completion |
| --- | --- | --- | ---: |
| `1B99C LEA` | A6 at 0 | 4 | 8 |
| `1B9A0 MOVE.W #127,D0` | D0 low word and NZVC at 4 | 4 | 8 |
| `1B9A4 MOVEQ #0,D1` | D1 and NZVC at 0 | 0 | 4 |
| `1B9A6 ADDA.W #12,A6` | Low half at 4; high half at 10 | 10 | 12 |
| `1B9AA MOVE.W #28,D2` | D2 low word and NZVC at 4 | 4 | 8 |
| `1B9AE MOVE.L D1,(A6)+` | Low-word NZVC at 4; A6 and high-word NZ at 8 | 8 | 12 |
| `1B9B0 DBRA D2`, looping | Counter low word at 6 | 6 | 10 |
| `1B9B0 DBRA D2`, terminal | Counter low word at 10 | 6, then replacement at 10 | 14 |
| `1B9B4 DBRA D0` | Same taken/terminal phases as D2 | Same as D2 | 10 / 14 |
| `1B9B8 RTS` | SP advances four at 8 | 12 | 16 |

The source supplies internal phases that have no coincident bus observation,
including ADDA's high-half commit at clock 10. Each observed register vector
was checked against that mapping, including preserved registers and status
bits. Exported pipeline PC values were also checked separately against the
source's internal PC updates; they are distinct from the current instruction PC.

Terminal DBRA samples pending interrupt state twice. For ordinary interrupts,
the second sample replaces the first; it does not accumulate it. A pulse that
is present at clock 6 and gone by clock 10 can therefore be cancelled. Trace
state and level-7 NMI have separate rules. Both full and partial-resume original
microcode retain this ordering.

## Remaining acceptance requirements

These observations cover one uninterrupted modified-loader invocation. They
do not observe IRQ assertion/acceptance, exception entry, bus retry or address
error. Interrupt sample times above are source-derived, not dynamically
validated. A matched native/reference starting state and exact native timing
comparison are also still absent.

The existing initialization timing helper samples interrupts at instruction
completion and emits incomplete-observation markers. Reusing it unchanged
would not consume this phase contract. Timing implementation remains gated
until the affected interrupt and continuation contract is established.

Two observer attempts were rejected as evidence: an invalid inclusive endpoint
for an opcode tap, then use of an unavailable Lua CPU-time property yielding
zero snapshots. The corrected capture uses the established machine-time API.
Both failed attempts are retained as diagnostic history, not passes.

Private evidence is in `analysis/acceptance-function324-phases-current.json`;
the canonical F324 packet and acceptance report reference it. Full graphics,
audio, timing, fixture/scenario and handoff acceptance remain open.
