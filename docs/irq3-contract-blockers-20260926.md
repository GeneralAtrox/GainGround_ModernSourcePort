# IRQ3 contract blockers — 2026-09-26

F99's observed state-changing handoff is present in the original evidence.
The current audit does not resolve it correctly. Correcting that lookup alone
would still not justify full native readiness: the saved FD1094 base-state
domain and F103's busy-lock branch remain unproved.

## F99: observed owner versus reachable state domain

Aggregate record 33958 explicitly records a kind-6 edge from state `04` at
`0x803C` to state `72` at `0x8042`, and a matching call to F103. The existing
hash-bound transition contract records 1,406 occurrences of this boundary
across four captures. Regenerating the contract from its original capture
inputs with `python -B scripts/generate_gain_ground_fd1094_transition_contract.py
--check` passed without modifying the contract.

`MCP/server.py` resolves the destination under the source descriptor's state
`04`, where no owner exists. Resolving the captured state `72` identifies F103.
The dependency audit also classifies every kind >= 5 as an interruption,
discarding this observed continuation from its dependency list.

Original `fd1094.cpp` gives the important limit. `CMPI.L #$0300FFFF,D0`
invokes the `STATE_RTE` operation, which clears IRQ mode while preserving
`m_state`. The effective IRQ state `04` comes from the key; it does not encode
the saved base state. Thus the instruction restores a previously selected
state rather than selecting `72` itself. The recent controlled IRQ capture
retains base `72`, consistent with this behavior.

A state-aware audit may recognize the exact observed, hash-bound kind-6
edge/call as an F99-to-F103 dependency. Before that can justify full readiness,
the lifecycle contract must prove that every reachable F99 invocation has
saved base `72`, or enumerate and close every reachable restored-state owner.
The 1,406 observed destinations do not establish exclusivity. Neither the
audit nor native behavior was changed by this investigation.

## Saved-state investigation: image lifetime and reset ordering

A complete scan of all 524,288 aligned word positions in the retained 1 MiB
state-72 opcode bank found no `CMPI.L #$xxxxFFFF,D0` state command. The final
two positions, whose immediate would cross the image boundary, were also
checked for the opcode and excluded. This is stronger than a catalog-only
scan: it includes bytes outside the current function inventory without
claiming those bytes are executable code.

The original reference cache reuses each generated state buffer until reset
or reconfiguration. Guest writes to program RAM do not refresh a populated
opcode buffer. The state callback selects this cache, and the driver's opcode
map covers `0x000000-0x0FFFFF`. CMPI obtains its low immediate before calling
the state callback and fetches subsequent instructions afterward. The full
and partial CPU implementations are retained in the evidence report.

Consequently, with initial base `72`, these cache identities and execution
confined to the three established IRQ prefixes in state `04`, base state `72`
is preserved. Those prefixes only exit IRQ mode; IRQ entry and RTE also leave
the base unchanged. Reset, a different cache generation or other state-04
control flow is outside that conditional result. The raw IRQ bank contains
another matching byte pattern at `0x8496` (`0C80 7596 FFFF`); it is not an
established executable owner or evidence of a reachable new state.

Startup lowers SR at `0x84D0` in state `58`, before selecting `dd` and `72`.
Thus SR alone does not exclude pre-72 interrupts. The identified CPU-A
startup sequence writes CPU-B IRQ mask `0x18` before releasing it, excluding
timer IRQ3. The identified `0x1C` write enabling that timer is at state-72
IRQ5 PC `0x813C`. IRQ4/5 remain possible after the early SR lowering; their
startup ordering still needs to be included in a complete lifecycle proof.

The software restart branch at CPU-A `0x803B4` returns to `0x800C0` and the
same startup sequence. CPU-A first halts B at `0x80136`, then clears its IRQ
mask at `0x8015C`. It subsequently sets mask `0x18` at `0x800F0`, before the
release/reset write at `0x800F8`. This establishes the order on that software
path. A bare CPU-only reset neither runs those CPU-A writes nor resets the
board IRQ controller; it must not be mistaken for the software path. No
exhaustive claim about control-register aliases is made here.

These findings narrow the remaining proof to the selected cache lifetime,
actual reset/IRQ-mask producers and asynchronous entry/control ownership.
They do not promote F99's gate or add hypothetical debugger resets to the
gameplay acceptance requirements.

## F103: missing busy-lock outcome

The missing `0x804C -> 0x801E` branch requires both:

- `FFFF8001 != 0` at the gate test, so execution reaches TAS.
- The old byte at `FFFF800A != 0` when TAS reads it. TAS writes old OR `0x80`,
  but its NZVC flags reflect the old byte, not the value written.

Bounded project-MCP inspections validated seven complete F103 records each
from coin-start r12, coin-start r13 and two-player pulsed-assault r1. All 21
take `0x804C -> 0x804E` with a zero lock prestate. These sources overlap in
frame sequence; they are not 21 independent scenarios or newly admitted
fixtures. They neither close the missing outcome nor prove it unreachable.

CPU-A IRQ3 sound service acquires the same byte at `0x80F9E` and clears it at
`0x80FC0`, with no gate test in that body. It is a candidate original producer
for an overlapping busy interval. CPU-A IRQ5 checks the gate first and skips
its TAS when that check sees a nonzero gate; concurrent changes after that
check remain a separate timing question.

The discriminating capture must observe the original successful CPU-A TAS
while the gate is nonzero and a genuine CPU-B timer IRQ3 reaching `0x804C`
before CPU-A's original clear. It must preserve producer and timer evidence.
No overlap has yet been observed, and no RAM-injected case is admitted here.

## Validation and acceptance limits

`analysis/validate-irq3-contract-blockers.py` checks current packet input hashes,
the original transition contract's input hashes and identity, bounded fixture
slices, both catalog lookups, the live dependency classification, all three
source inspections, and the relevant instruction bytes in their captured
images. It also scans all six complete opcode banks for FD1094 command byte
patterns and verifies the relevant startup, mask and reset-order bytes.
Its compact report is
`analysis/acceptance-irq3-contract-blockers-current.json`.

No build, capture, native edit, audit-gate promotion or cleanup was performed.
Graphics provenance, exact audio samples, timing parity and comprehensive
scenario/fixture acceptance remain open.
