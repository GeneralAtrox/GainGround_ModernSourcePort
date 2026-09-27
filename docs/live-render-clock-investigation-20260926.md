# Live render-clock investigation — 26 September 2026

The running native renderer normally samples visual memory at VBLANK start.
The authoritative reference renders after VBLANK, **1,640,000 ns later in the
same hardware period**. This is a measured producer-phase mismatch. No native
render scheduling, game behavior or reference behavior changed in this pass.
The full T/G/A/F/H gates remain open.

## Reference clock

All seven frozen graphics-package source identities were verified again.
Every one of its 1,845 frame presentations, covering frame ordinals 0–1844,
has the exact time:

`19,024,000 ns + frameOrdinal * 17,384,000 ns`.

Each links to a newly completed frame; all have `validFrame = 1` and
`repeated = 0`. This statement covers that recorded interval, not earlier
presentations without a completed frame.

The pinned driver selects `VIDEO_UPDATE_AFTER_VBLANK` and configures 656
pixel clocks per scanline, 424 total lines and 384 visible lines, at a 16 MHz
pixel clock. Its frame period is therefore 17.384 ms and VBLANK spans 40 lines,
or 1.640 ms. The pinned `screen_device::vblank_end` calls the frame update
under that flag. The retained modified-loader reference's frame callbacks
also follow this same grid after its initial time-zero callback. Loading
policy does not explain this observed graphics-clock difference.

## Actual native renders

A read-only observer recorded every invocation of the production renderer in
the same 14-second neutral configuration as the acceptance baseline. Each
record includes its actual device time/frame number, host time variable,
checkpoint count, all seven live visual memory regions, and the resulting
384 × 496 pixel buffer. The observer supplies no expected memory or pixels.

There are **805 records**: one loading-logo render at device time zero and
804 game renders. The first game render is at **36,183,200 ns**, device frame
2. Native's initial CPU fiber advances the device clock through earlier
periods before returning to the host's rendering check. The host's separate
`emulated_ns` variable is still zero at that first render, so it must not be
used as its authoritative render timestamp.

The game-render frame numbers are contiguous from 2 through 805. Their phases
within the 17.384 ms hardware period are:

| Device-frame phase | Render count |
| --- | ---: |
| Exactly VBLANK start | 802 |
| VBLANK start + 100 ns | 1 |
| VBLANK start + 1,415,200 ns, first game render | 1 |

The 100 ns case is device frame 758 at 13,177,072,100 ns. The first render is
224,800 ns before that hardware period's reference VBLANK end. The remaining
renders are 1,640,000 ns early, except frame 758 which is 1,639,900 ns early.
These comparisons use the shared hardware clock period; they do not assert
that native and original game contents correspond at those periods.

The source explains the observation: `System24Devices::frame()` divides the
elapsed scanline count by 424, while `RuntimeWindow::advance()` renders after
running both CPU fibers whenever that number changes. It does not schedule
the render at the reference's VBLANK-end callback.

## Validation and retained provenance

The observed run still ends at exactly 14 seconds, device frame 805, with no
fault or error and **62,251,303 execution checkpoints**. Its entire recorded
sound/timer bus, all 875,000 PCM frames, and final status bytes equal the prior
native baseline. All 788 baseline build-source hashes remain unchanged.

All 805 records are retained losslessly in the private report. It contains
995,804,320 decoded bytes of visual RAM and pixel data, stored as approximately
5.87 MB of compressed XOR deltas. Every retained record was decoded and checked
against the raw observation before retention. Per-region and pixel SHA-256
identities bind each sampled input state to its actual native render output.
These are render-time resource observations, not a record of every producer
write or a complete draw-owner trace.

There are 321 consecutive pairs of equal game pixel buffers. Pixel equality
does not establish a repeated presentation or permit collapsing their frame
identities. The hidden test window does not provide evidence about Windows
paint/compositor timing; this investigation measures render production.

## Remaining contract

The render-trigger change remains behind the exact-parity contract gate.
The required contract must also resolve ordering at the VBLANK-end
boundary relative to IRQ assertion and CPU execution, the live handoff mismatch,
per-write resource provenance, the complete draw/state consumption ledger,
repeated presentations and full scene/input scenarios. In particular, the
13 original review frames report `irqSprite = 0` at frame begin; reproducing a
timestamp alone would not establish that the native callback order is correct.

Reproduction: `analysis/acceptance-live-graphics-clock.py` and
`analysis/retain-live-graphics-clock.py`. The private report is
`analysis/acceptance-live-graphics-clock-current.json`, linked from the current
acceptance report. The initial observer compile rejected use of `size_bytes()`
on the vector-backed pixel buffer; this was corrected to its byte count before
any observation run. The failed compile is not runtime evidence.

## Cleanup

The observation and retention processes terminated successfully. Automatic
approval review rejected the guarded removal of
`.tmp/acceptance-live-graphics-clock-20260926` with "blocked by policy" and no
more specific reason. The command did not execute; the temporary executable,
raw capture and intermediate logs remain. Cleanup is incomplete. No alternate
deletion was attempted and none of the earlier rejected deletions was retried.
