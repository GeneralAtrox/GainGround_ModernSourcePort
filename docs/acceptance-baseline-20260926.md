# Acceptance baseline — 26 September 2026

Subsequent investigation: [audio sample contract](audio-sample-contract-20260926.md)
identifies a sample-update-rule defect with an exact original-chip comparison
and negative control. The baseline below retains its original scope and results;
no full acceptance gate has been closed.

Subsequent [graphics inventory investigation](graphics-inventory-investigation-20260926.md)
matches all 589 selected original draws under authoritative RAM and identifies
unwritten palette-state differences. It does not close live graphics provenance.

The [live handoff investigation](live-handoff-investigation-20260926.md) establishes
matching CPU-A entry registers/RAM and identifies the reference adapter's later
CPU-B reset interval. A complete common-machine checkpoint remains unproven.

The [live render-clock investigation](live-render-clock-investigation-20260926.md)
retains native frame RAM/output and measures rendering at VBLANK start, while
the authoritative reference renders 1.640 ms later at VBLANK end.

The [live owner and reference-boundary investigation](live-owner-boundary-investigation-20260926.md)
finds differing live owners despite pixel equality, and observes render/IRQ
ordering on the retained timing reference. That variant matches all 13 frozen
review presentations and visual RAM snapshots, but does not reproduce the
missing original executable's identity or close the full graphics contract.

The [native visual-writer investigation](native-visual-writer-investigation-20260926.md)
reconstructs every retained native render from its actual memory mutations. It
identifies a missed 15.2-microsecond title tilemap state, eight cleared visible
BIOS palette entries, and a 48-write tile sequence with differing timing.

The [selected render-state source audit](render-state-contract-audit-20260926.md)
resolves all 15 declared original source hashes across retained trees and adds
source dispositions for 44 selected-stage contract fields. Its overall coverage
check still fails; source recovery does not recover the original executable.

The [presentation contract audit](presentation-contract-audit-20260926.md)
reconstructs all 13 captured presentations from their indexed pixels, palette
and rotation. It distinguishes matching RGB from unequal stored top bytes,
adds 30 presentation dispositions and keeps the complete-coverage gate false.

The subsequent [fixture census](fixture-census-investigation-20260926.md)
attempts all 95,169 eligible records and retains 95,168 returned comparisons
plus one timeout. Its 35,387 passes and 59,781 divergences include a separately
labelled temporary fixture-host correction for 31,971 F115/F116 records.
This completes the diagnostic record inventory, not the production-suite gate.
The baseline counts below retain their original stop-at-first-failure scope.

The [enclosing-loop comparator repair](fixture-loop-comparator-repair-20260926.md)
then integrates that correction without changing game functions or compatibility
rules. Its fresh research build passes 35/36 tests, including all 16 real-child
tests, and reproduces every affected diagnostic result. The full fixture gate
still fails. Current build-source bindings are in that repair's followup report;
the baseline source hashes and results below remain historical evidence.

The T/G/A/F/H gates in the opening/title completion audit remain open. This
baseline covers the current working sources, including the pre-existing local
stage-selection, stage-callback and performance changes. It does not change
native behavior or promote a review executable. The goal remains complete live
graphics provenance, matched audio samples, timing comparisons, and complete
scenario/fixture validation; these checks are not substitutes for that goal.

## Fresh build and fixture results

A separate Release build with `GAIN_GROUND_RESEARCH_TESTS=ON` completed all
751 build steps. All 15 real-child integration tests passed. Of the remaining
19 CTest tests, 18 passed and the all-implemented fixture test failed at
function 1, record 1: an additional call at sequence 7,359,792. Thus the combined
result is **33/34 tests passed**, not full acceptance.

Each of the 641 catalogued functions was then attempted independently using
`gain_ground_fixture_compare <bundle> --function <id> --all`. No process timed
out. Each invocation stops at its own first failure:

| Result | Functions |
| --- | ---: |
| All selected eligible fixtures passed | 489 |
| No replay-eligible fixtures | 120 |
| Translation contract violation | 7 |
| Call sequence/count/identity divergence | 22 |
| Memory divergence | 2 |
| Compatibility provenance rejection | 1 |

The 32 replay-divergent functions are 1, 55, 62, 63, 71, 99, 100, 101, 104,
111, 115, 116, 121, 122, 124, 140, 148, 169, 170, 171, 173, 174, 175, 178,
328, 340, 353, 497, 520, 558, 628 and 629. These are failures of the comparison;
their causes must be established before classifying them as native-code bugs.

Across these invocations, 95,169 eligible records were selected, 23,703 were
executed and 23,671 passed. There were 1,901 compatibility hits. The unexecuted
records remain unverified. This is a complete function-attempt census, not a
complete record scan or a passing global compatibility-histogram check.

The private manifest's 617 functions with complete fixtures / 24 without is a
different measure. It does not prove every function has an eligible replay
record. The actual replay selector found 120 without one. Additional stage
callbacks outside the 641-entry catalogue are not covered by this census.

The live function-1 work packet still fails its translation gate: 22 unobserved
instruction addresses, ten missing control outcomes and two uncatalogued
external targets. Retained original-capture hashes were rechecked for its
extra-call investigation. The newer original recording contains the F40 call
at sequence 7,359,792; the older recording contains the corresponding control
edge but omits it from its ordered calls. Removing the native call or adding a
passing exception would not repair that evidence defect. Source admission and
the broader path contract remain unresolved.

## Timing and audio

A temporary observer compiled against the fresh libraries ran the current
native runtime to exactly 14,000,000,000 ns, frame 805, with no runtime fault
or error and 62,251,303 execution checkpoints. Inputs were neutral; the window
was hidden; optional user title music was disabled for original-synthesis
measurement. Samples were captured instead of sent to WaveOut. The observer
retained current guest execution, device timing and CPU scheduling code.

Against the identity-bound retained modified-loader MAME diagnostic:

- All 76 BIOS sound-port writes match absolute timestamps and values.
- Both sides contain 3,932 sound-port writes and 1,692 selected game
  register/data pairs; all selected pairs match values and ordinal order.
- Game-pair absolute offsets range from **-117,800 ns to +19,000 ns**.
- The first-game-write difference is **-657 ns**. Applying it diagnostically
  gives offsets from -117,143 ns to +19,657 ns; this is not an acceptance
  alignment. Original attosecond timestamps remain in the retained evidence.

This reproduces the later September 12 timing finding, superseding the older
1.195 ms and 369.301 microsecond summaries for this comparison configuration.
The modified loader is still a diagnostic adapter. A complete common
CPU/device/producer starting-state contract has not been established.

A fresh run of the hash-verified pinned MAME executable used the retained
modified-loader adapter at **62,500 Hz**, stereo signed 16-bit WAV output. Its
sound-bus CSV reproduces the retained reference exactly. Native captured
875,000 sample frames; MAME captured 937,501. The first 875,000 frames were
compared by unshifted sample ordinal: **659,037 channel samples differ**, with
maximum absolute error 16,700. The first difference is sample frame 2, left
channel (native 0, reference 176).

This is an audio diagnostic, not a matched-tap acceptance comparison. The
reference still includes the startup transient omitted by the approved native
startup policy. Tap equivalence, sample origin, mixer/resampler state and
machine-state equivalence remain to be proved. No fitted sample shift, omitted
sample interval or waveform approximation was used to produce a pass.

## Live graphics and scenario coverage

All seven source identities bound by the frozen v3 graphics package match,
including the capture, its identity/validation documents, schema, reader and
validator. For all 13 selected review frames, the 589 original draw submissions
have contiguous global submission ordinals, matching frame owners and matching
frame-end counts. The continuous inventory contains 1,845 frames.

The package still has **635 ledger entries and zero recorded native-consumption
or irrelevance closures**. This is missing proof, not 635 missing rendering
features. No current native per-frame draw inventory was produced or compared;
zero omitted/invented draws and one-to-one native provenance are not established.
Historical selected-frame raster equality does not close this gap.

The new live run is neutral and bounded to 14 seconds. Full title/attract/return
and coin/start reference scenarios remain open. H (review build and sign-off)
also remains open until T/G/A/F pass.

## Evidence and cleanup

Private detailed evidence is in `analysis/acceptance-current.json`: hashes for
788 build-source files, fixture and executable identities, every function
result, original review-frame ordinals, timing/audio statistics, raw timing
observations, observer sources and test/build logs. All 788 source hashes were
unchanged between collection and retention. Function 1's one canonical packet
is refreshed at `analysis/function-work/function1.json`; historical research
is preserved without treating it as a current pass.

Automatic approval review rejected the guarded removal of the owned temporary
directory `.tmp/acceptance-20260926` with "blocked by policy" and no more specific
reason. The command did not execute; its build products, observer executables,
object files, raw PCM/WAV and intermediate logs remain. Cleanup is incomplete.
No alternate deletion method was attempted. Pre-existing builds, captures and
local changes are outside this cleanup.
