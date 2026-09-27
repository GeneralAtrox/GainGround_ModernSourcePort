# Live loading-handoff investigation — 26 September 2026

The live native and modified-loader reference agree at CPU-A game entry in
time, architectural register values and all nine writable memory regions.
Their CPU-B starting boundaries differ because the existing reference adapter
allows the original CPU-B reset sequence to execute before applying the direct
entry policy. The complete common-machine contract remains open. No production
behavior or reference loading intervention changed in this investigation.

## Captured boundaries

Read-only observers sampled the running current native runtime and the pinned
MAME executable using the existing modified-loader adapter. Native ran through
100 ms, reaching frame 5 without fault or error and 422,772 checkpoints. The
reference ran through one second. These are startup investigations, not full
scene or input-scenario validation.

At `audio-done`, immediately before CPU A enters `0x800c0`:

- Both machine times are exactly **832,600 ns**.
- All 18 CPU-A architectural register fields match. The 18 stored fields for
  inactive CPU B also match, but its FD1094 state is not equivalent: native's
  unused context has marker `0xff`, while the held reference CPU has state 0.
- All **999,456 bytes** across native regions 2, 3 and 5–11 match exactly.
- Eleven selected board values match: timer data/value/mode, both IRQ enable
  and pending fields, VBLANK, sprite IRQ, free-counter mode and YM IRQ.

The CPU program-counter comparison uses MAME's `CURPC`. Source inspection of
`m68000_device::state_import` establishes that its exposed `PC` becomes
`CURPC + 2` when importing a new instruction address; comparing `PC` directly
would invent a two-byte discrepancy.

At the CPU-B release intervention:

- Both CPUs' traces record the enabling write to `0x80001c` with value 6 and
  mask `0xffff` at exactly **36,183,200 ns**, from CPU-A instruction `0x800f8`.
- Native prepares CPU B at `0x84da`, SR `0x2000`, SP `0x7ffe`, FD1094 state
  `0xdd` at that same time.
- The reference applies those same CPU-B register/state values at
  **36,188,057.142857136 ns**, **4,857.142857136 ns later**.
- The reference's pre-intervention snapshot reports **34 CPU-B cycles**, a
  clock scale of **0.7**, and `CURPC = 0x8436`. The pinned `cnt1` implementation
  releases HALT, pulses RESET and applies the 0.7 scale. The normal reset path
  in `state_reset_df` consumes 34 cycles. The adapter restores scale 1.0 only
  after reaching this later boundary. This identifies the source and observed
  interval of the handoff mismatch; it does not establish the cause of every
  later timing offset.

At that later reference snapshot, CPU A has already advanced to `0x8034e`
with local scheduler time 36,203,000 ns. Native's suspended CPU-A observation
is at `0x800f8`, with its next CPU deadline at 36,183,600 ns. CPU-A PC, SR and
active SP differ; its other compared registers match. All compared CPU-B
registers and its selected FD1094 state match after the intervention. The
eleven selected board values still match, but this is not a complete timer or
scheduler-phase comparison.

## Memory difference at CPU-B release

Eight shared-memory bytes differ; every other compared byte matches. Six are
the reference adapter's RTE instruction patches at bus addresses `0x80080`,
`0x8008c` and `0x80098`. Native implements those direct transfers through its
host ABI and leaves the corresponding bytes unchanged. The remaining two are
at shared offsets `0x3fffe`–`0x3ffff`: the CPUs have different active stack and
resume positions. These are retained as distinct observations, not silently
excluded from a claim that all RAM matches.

The seven visual memory regions match at both sampled boundaries. Neither
boundary is a substitute for comparing the scene's later draw owners, pixels
and producer clocks.

## Observer validation and remaining contract

The observers preserve the recorded native sound/timer bus prefix exactly
(98 rows through 100 ms) and the reference prefix exactly (447 rows through
one second). Native's first 6,250 PCM frames and reference's first 62,501 PCM
frames are respectively byte-identical to their preceding captures. These
checks compare each observed run to its own baseline; they do not claim that
native and reference PCM match each other. All 788 baseline native source
hashes remain unchanged.

The existing reference adapter therefore cannot yet certify the declared
native CPU-B loading handoff. The complete contract still needs the release
and scheduler/resume semantics, explicit accounting for the adapter's shared
code patches, full CPU/device state, YM/DAC/mixer history and sample cursors,
producer and presentation phase, and full scene/input scenarios. A constant
time subtraction would not make the two captured CPU states equivalent.
T/G/A/F/H remain open.

Reproduction and retention sources are
`analysis/acceptance-handoff-observer.py` and
`analysis/retain-acceptance-handoff.py`. The private report
`analysis/acceptance-handoff-current.json` retains source/executable hashes,
both boundary comparisons, all memory differences, and lossless compressed
snapshots of RAM, registers, logs and observer sources. It is linked from the
canonical current acceptance report.

## Cleanup

The observation processes terminated successfully. Automatic approval review
rejected the guarded removal of `.tmp/acceptance-handoff-20260926` with
"blocked by policy" and no more specific reason. The command did not execute;
the temporary executable, observer copies, raw snapshots and intermediate
logs remain. Cleanup is incomplete. No alternate deletion was attempted and
none of the earlier rejected deletions was retried.
