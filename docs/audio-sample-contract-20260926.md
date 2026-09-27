# Audio sample contract investigation — 26 September 2026

The current audio implementation uses a different sample-update rule from the
pinned MAME reference. This difference is isolated for the 14-second neutral
modified-loader diagnostic. Production behavior is unchanged; full audio and
scene acceptance remain open.

## Observed taps and provenance

The pinned MAME executable was run with read-only Lua hooks for `:ymsnd`, `:dac`
and `:speaker`. The 15-second, 62,500 Hz reference retained exactly the same
sound-bus CSV and WAV bytes as the preceding unhooked run. The five YMFM source
files used by native and MAME have identical SHA-256 hashes. All 788 native
build-source hashes still match the fresh acceptance baseline.

All 2,250 tap spans are contiguous. At every observation, the cumulative sample
count is `floor(machine_time * sample_rate) + 1`: 937,501 YM/speaker frames and
23,040,001 DAC frames. The DAC stream runs at 1,536,000 Hz; the YM and speaker
streams run at 62,500 Hz in this diagnostic.

Converting the captured speaker floats with the pinned
`sound_manager::output_push` rule,
`clamp(trunc(sample * 32768), -32768, 32767)`, matches every WAV channel sample
over all 937,501 frames. This establishes the observed WAV tap and conversion.

Speaker floats equal half the YM output except for 88 channel samples in frame
indices 2–45. The captured DAC is nonzero only in its first 1,014 frames. This
is the reference startup transient: native deliberately preloads the DAC's
unsigned midpoint before enabling output under the previously approved startup
policy. It is separate from the later YM waveform mismatch.

The first observer attempt used the unsupported Lua `register_stop` function.
That attempt is not acceptance evidence. The corrected observer uses the
source-documented `add_machine_stop_notifier` and completed without Lua errors.

## Controlled sample-rule comparison

The unchanged production audio class was first fed the recorded native YM
writes. Its 875,000 sample frames exactly reproduce the live native PCM. This
checks that the replay supplies the audio inputs needed for this interval.

The same production class was then fed the original MAME writes at their
captured timestamps. It still differs from half the captured MAME YM output in
658,942 channel samples. Substituting original CPU write timing therefore does
not remove the synthesis mismatch.

The pinned `sound_stream::update()` and `update_nodeps()` generate through the
current sample index before a register write. At positive time, their target
count is `floor(time_ns / 16000) + 1`; they return without updating at time zero.
`ymfm_device_base::write()` calls that stream update before writing the chip.
Native currently generates only completed 16-microsecond intervals, beginning
at 16,000 ns.

A diagnostic replay using the identical YMFM core, the original register
writes and MAME's source-derived update rule matches **all 875,001 captured
raw YM frames through 14 seconds, with zero differing channel samples**. No
sample shifting, fitted origin, event removal or approximate waveform was used.

A negative control changes only the target count to completed intervals,
`floor(time_ns / 16000)`. It produces 661,690 raw-channel mismatches against
MAME. After the production half-gain conversion, it exactly reproduces the
production class fed those same original writes. This isolates the sample-rule
difference as the cause of that controlled replay's mismatch.

Every original attosecond timestamp was checked to retain its sample slot when
floored to nanoseconds. The replay rejects CSM mode; its observed sample match
does not establish mode-write scheduler, IRQ or timer parity. The measured
MAME tap remains authoritative; the replay is a diagnostic, not a replacement
reference for game execution.

## Acceptance boundary

This closes the tap/conversion question and identifies the sample-update defect
for the stated interval. It does not establish complete live audio parity.
Native CPU write timing still differs. Common machine-state handoff, the full
DAC/resampling contract, longer title/attract/return and coin/start scenarios,
and the graphics owner/state/clock contract remain open.

No production repair has been applied. The binding exact-parity instructions
require complete authoritative contracts before native implementation; the
bounded sample proof does not waive those requirements. T/G/A/F/H remain open.

Detailed private evidence is in `analysis/acceptance-audio-current.json`, linked
from `analysis/acceptance-current.json`. It includes source and executable
hashes, tap counts and identities, exact comparison results, negative-control
source, observer sources, full timing observations and the rejected first
attempt. Reproduction sources are `analysis/acceptance-audio-tap-capture.py`,
`analysis/acceptance-audio-replay.cpp`,
`analysis/acceptance-ym-stream-replay.cpp` and
`analysis/retain-acceptance-audio-contract.py`.

Automatic approval review rejected the guarded removal of
`.tmp/acceptance-audio-taps-20260926` and
`.tmp/acceptance-audio-taps-20260926-r2` with "blocked by policy" and no more
specific reason. The command did not execute. Both directories remain and
cleanup is incomplete; no alternate deletion was attempted. The earlier
rejected `.tmp/acceptance-20260926` deletion was not retried.
