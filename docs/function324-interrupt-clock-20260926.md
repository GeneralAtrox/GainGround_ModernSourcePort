# CPU-B interrupt clock evidence — 2026-09-26

A read-only reference capture contradicts the assumption that CPU-B's E-clock
phase can be calculated directly from global elapsed time after the modified
loader handoff. This is a timing-contract finding, not a measured native
gameplay failure or an authorized constant-offset repair.

The capture contains 29,916 saved-state snapshots. It exactly preserves the
previous 27,279 architectural snapshots, the complete 43,243-event F141 trace,
and all five reference outputs, including audio. No native behavior changed.

## Clock reconstruction

In the original `diexec.cpp`, `local_time()` and `total_cycles()` add the same
currently executing cycle count to their saved bases. At the observed bus
callbacks, the trace and machine timestamp agree. CPU cycles therefore equal
saved `m_totalcycles` plus the difference between observed time and saved
`m_localtime`, divided by the 100 ns CPU period. Every difference is an exact
integer number of cycles, and reconstructed cycles advance with observed time.

For every snapshot in this bounded interval:

```
floor(global time / 100 ns) - reconstructed CPU cycles = 361846
```

Consequently, the two cycle counts differ by six modulo ten. All 14 observed
interrupt acknowledgements occur at CPU cycle phase zero and global-time phase
six. At every acknowledgement, reconstructed cycles equal the CPU's saved
`m_last_vpa_time`, independently checking the reconstruction at those events.

## Natural interrupt entry

Seven IRQ4 and seven IRQ5 entries have the source-prescribed bus order:

1. Write the return PC's low word at old SP minus two.
2. Read the CPU-space autovector after VPA synchronization.
3. Write saved SR at old SP minus six, nine clocks after acknowledgement.
4. Write the return PC's high word four clocks later.
5. Read the vector's high and low words at acknowledgement plus 17 and 21 clocks.

The observed handlers are `0x806E` and `0x80A2`. For each entry, the VPA wait
calculated from reconstructed CPU cycles matches the captured acknowledgement.
Applying the existing native helper's global-time expression at the same
reference request boundary instead changes that wait by plus four or minus six
clocks. This is a counterfactual comparison of the formula with reference
observations; it is not a native timing replay.

## What remains unresolved

The measured constant must not become a fitted correction. A valid replacement
requires accounting for CPU reset, halt/release, clock scale and elapsed cycles
from a proven starting state. Full exception prefetch, handler and RTE behavior
also remain unverified by this capture. Saved FD1094 `m_state` does not include
the unobserved IRQ-mode flag, so it does not prove effective decryption state.

Source inspection narrows the required startup accounting. The scheduler adds
cycles while a CPU runs or is suspended with `m_eatcycles`; HALT and RESET use
that suspension mode. Device reset clears `m_totalcycles` without clearing local
time. The board's CPU-B release also pulses RESET and temporarily reduces its
clock to 0.7. The retained modified loader explicitly restores the clock scale
to 1.0 using saved state and post-load. The startup capture below measures their
combined effect; elapsed time since HALT release alone is not the accounting rule.

All ordinary F324 pending-interrupt fields were zero. A separate discriminating
test is still needed for terminal DBRA's samples at clocks six and ten. Clearing
the real timer through its MMIO register queues the CPU input-line update;
delivery is not synchronous. Any controlled pulse experiment must observe that
the pending level actually became low before the second sample. Writing the
CPU's internal pending fields would not establish this hardware behavior.

## Startup observation resolves this reference origin

A subsequent read-only capture recorded 1,156 startup and later snapshots.
The first reset-vector fetches show a zero saved cycle count and a local-time
base of `36,183,199,999,999,998` attoseconds. CPU-B then counts 34 cycles at
7 MHz before the diagnostic loader restores 10 MHz. The loader preserves
both the count of 34 and local time through that restoration.

Deriving the later origin solely from those startup observations gives:

```
origin = reset base + 34 * (floor(10^18 / 7,000,000) - 100,000,000,000)
       = 36,184,657,142,857,136 attoseconds
CPU cycles at 10 MHz = (CPU local time - origin) / 100,000,000,000
```

This prediction matches all 29,916 saved cycle/local-time pairs in the earlier
interrupt capture. It explains the previously observed global-time phase
difference without fitting the late samples. All five reference outputs remain
byte-identical, as do all timing events and the footer. Only the trace header's
source digest changes because the new observer also binds scheduler sources.

The reset base is two attoseconds before the CPU-A release-register write;
individual queued reset/clock-change callbacks were not directly instrumented.
The observer also sees same-time opcode reads made by the loader's debugger PC
imports; these are not additional executed CPU cycles. The 34-cycle count is
established by the saved scheduler count before the loader intervention.

This is a property of the retained diagnostic handoff. It does not establish a
native gameplay defect. Native/reference comparisons still require a common
handoff contract that includes cycle origin and the intentional loader policy;
copying the derived constant into native code would not satisfy that contract.
Startup evidence and its validator are
`analysis/acceptance-cpu-b-clock-origin-current.json` and
`analysis/validate-cpu-b-clock-origin.py`.

The interrupt evidence and validator are
`analysis/acceptance-function324-irq-clock-current.json` and
`analysis/validate-function324-irq-clock.py`. The acceptance report and canonical
F324 packet link this evidence. Graphics, audio, timing, scenario/fixture and
matched-handoff acceptance remain open.
