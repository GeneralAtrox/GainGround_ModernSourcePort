# F324 terminal DBRA interrupt sampling — 2026-09-26

A controlled original-device test now distinguishes terminal DBRA's two pending
interrupt samples. Clearing the timer interrupt after its clock-six sample
allows the clock-ten sample to replace the pending interrupt with zero. Holding
the same interrupt instead produces one acknowledgement and a real interrupt
return to the following instruction with preserved architectural state.

No native function or review build changed. These results establish a bounded
original behavior contract, not native timing parity or natural game cadence.

## Experiment and result

The test operates only the board's real timer MMIO registers. At F324 entry it
masks CPU-A's timer interrupt and prepares the stopped counter at `0xFFF` using
mode-start/stop writes. The source increments the counter once when stopping;
the observer checks each increment and the final counter/mode. At the first
terminal inner DBRA (`0x1B9B0`, D2 low word zero), it enables CPU-B's timer IRQ3
and starts the 8 MHz timer. No internal CPU or board state field is written.

| Observation | Clear after clock-six sample | Hold until acknowledgement |
| --- | --- | --- |
| Clock 6 pending level | 3 | 3 |
| Clock 6 pending and latched state | `0x03000004` | `0x03000004` |
| Clock 10 pending and latched state | zero | `0x03000004` |
| IRQ3 acknowledgements | zero | one, at clock 37 |
| Next instruction `0x1B9B4` begins | clock 14 | clock 1484 |
| Continuation registers, SP, SR | expected values | same expected values |

In the clear case, the fields remain high immediately after the MMIO write,
then become low before clock ten. This directly observes the asynchronous
delivery identified in the scheduler source. D2 becomes `0xFFFF` at clock ten
in both cases. The cancelled pulse preserves the entire baseline F141 timing
trace byte for byte.

The held control reads autovector 27 and shows FD1094 IRQ mode active at
acknowledgement. The real RTE at `0x8032` reads the saved SR and return PC from
the stack. At the following `0x1B9B4` boundary, all 17 observed architectural
register fields, including SP and SR, match the cancellation case, with saved
FD1094 state `0x72` and IRQ mode clear. F324 subsequently returns to `0x1B95A`
in both runs. The additional elapsed time is observed, not used as a fitted
native delay.

An initial attempt produced no stimulus markers. The follow-up observed that
CPU-A allowed the timer interrupt (`0x1C`); the successful setup explicitly
masks its timer bit before proceeding. The initial attempt is retained as
invalid pulse evidence, regardless of its successful process exit.

## Remaining timing and handoff limits

The held-control instruction tracer explicitly stops at the exception state
transition with an unsupported event and an incomplete footer. Independent
register/bus taps establish the acknowledgement and real return, but not a
complete exception, prefetch or handler timing inventory. Existing stack/vector
taps also omit initial exception accesses while CURPC still names DBRA.
Neither limitation is converted into a passing full-timing claim.

This test discriminates one terminal inner DBRA under an ordinary level-three
interrupt. Other sampling sites, trace/NMI and error/retry behavior remain
outside its scope.

Native startup currently injects DD/SR `0x2000`/SP `0x7FFE`/PC `0x84DA` when
CPU-B becomes enabled. It has no corresponding reset-cycle/local-time origin
at that boundary, and its DD selector is untimed. The retained diagnostic
reference instead executes 34 reset cycles at 7 MHz before restoring 10 MHz.
The native helper's global-time E-clock expression therefore cannot be treated
as a proved common phase. This source difference is not itself an observed
native gameplay bug. A matched handoff contract remains necessary.

Evidence and validation are in
`analysis/acceptance-function324-irq-sampling-current.json` and
`analysis/validate-function324-irq-sampling.py`. Full graphics, audio, timing,
scenario/fixture and review-build acceptance remain open.
