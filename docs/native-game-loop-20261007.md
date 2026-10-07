# Native game loop, crash repair verification and sound check

The user requested repair of the screenshot's `Native call depth exceeds runtime
capacity` stop at source address `0x1F0DA`, a game and sound check, and explicitly
chose replacement of the remaining dual-CPU scheduling architecture now.

## Result

The playable Win32 runtime now uses `NativeGameLoop`. It owns the frame sequence:
frame wait, input sampling, active actor callbacks, then sprite construction.
System services for asset loading, rowscroll and sound execute synchronously on
the same monotonic device timeline. The old endless service and actor-dispatch
entries return to this native owner at an explicit host boundary.

There are no independently scheduled CPU-A/CPU-B fibers, local clocks or
shared-memory rendezvous in the playable path. One Windows fiber retains the
single native call stack so it can yield to input, rendering and audio delivery
during long operations. It does not represent an emulated CPU. Original CPU/state
tags, registers, memory layouts, interrupt adapters and translated routines remain
as compatibility ABIs. The research host retains its legacy scheduling interfaces
for existing fixture and timing tests. This is a scheduler migration, not a rewrite
of every translated routine into ordinary C++ game objects or an arcade timing
parity claim.

The native event mode latches frame/rowscroll events until their service consumes
them. This prevents a service from missing another event merely because its work
outlives the board's short scanline pulse. Each is acknowledged once. Legacy device
mode remains available to the reference tests.

## Reported crash

The workspace already contained uncommitted iterative repairs for functions 609
(`0x1F0DA`) and 610 (`0x1F0E6`). They were preserved and included in the tested
runtime. The former implemented `DBF` as repeated native self-calls; the register
word can require 65,536 iterations, far exceeding the host's 512-frame capacity.
This is a translation recursion defect; the screenshot alone does not establish
a two-CPU race.

The new `native_loop_depth` regression executes real children and checks the
outer return PC/SP, arithmetic result, retained register halves, stack guard and
bounded depth. Counts 0, 1, 512, 511, 65,280 and 65,535 all pass at depth 2. Linking
that same regression against the pre-repair function from Git HEAD reproduces
`Native call depth exceeds runtime capacity` at count 512. The screenshot's
complete input history was not supplied, so this is a focused reproduction of its
failing loop rather than a replay of the exact user session.

## Verification

- Release CMake/Ninja build passed with the MSYS2 UCRT64 compiler.
- 30/30 CTest checks passed: 15 required real-child boundary tests and 15 other
  tests. Sound IRQ tests now also run on the single native clock and check real
  child returns, preserved state and exact synthetic-case durations.
- All 40 stages loaded and ran with a live player and primary/secondary attacks
  through the new loop: 15,100 game updates and 60,321 system sound services.
  This bounded run used unlimited credits and invulnerability; it is not a full
  unaided playthrough or an exhaustive enemy-position sweep.
- All 60 character checks passed: four-direction movement, primary attack and
  secondary attack for each of the 20 characters, using a snapshot created by the
  new loop and real translated children.
- A rendered Round 1 / Stage 1 frame was visually inspected: terrain, actors,
  player, stage overlay and HUD were present.
- The hidden Win32 runtime ran a scripted 35-second check including startup,
  credit, join, both attacks and pause/resume. It remained running without a stop.
  Progress at device frames 600/1200/1800 reported zero audio starvation and zero
  dropped buffers. Queue ranges were 44.0–101.5, 38.1–70.1 and 49.6–69.8 ms.
- Original synthesized audio: a 900-step live WaveOut probe reached 33.627 seconds
  of device time, submitted 2,104,157 stereo frames including preroll, and observed
  zero starvation, dropped buffers or clipped samples. Peak absolute sample: 5,648.
  The queue stayed between 40 and 100 ms in the final probe.
- The user's existing title replacement was checked through startup, title,
  credit, gameplay and both attacks: zero starvation, dropped buffers or clipped
  samples; peak 30,938 and queue 44–148 ms. These are measured synthesis/output
  checks, not a subjective listening or speaker-volume certification.

The first synchronous-frame presentation attempt produced audio bursts during long
operations. Its playback check observed one starvation and three dropped buffers.
The single resumable game stack fixes that by delivering audio within presentation
slices. A later standalone probe also exposed a test-harness pacing error: its
wall-clock epoch began before opening WaveOut. The probe now starts its epoch after
device open and uses the runtime's thread priority and timer precision. The actual
Win32 runtime already starts its epoch after device open; its live check passed.

## Existing evidence limits

Function 610's 11 complete fixtures passed. Function 609 has no complete eligible
fixtures in the retained aggregate bundle, so its fixture gate reports zero
eligible records rather than a pass. The real-child regression provides the
bounded crash check without inventing missing fixture evidence.

The native generation `--check` still fails with `Function contract identity
differs without a compatibility proof`, as documented before this task in
`source-line-split-20260927.md`. The aggregate fixture comparison stops at function
1 (`cpu_a_reset_entry`), record 1, with an additional-call mismatch. That translation
was already modified when this task began and was not changed here. The migration
does not waive either blocker or establish complete original-machine parity.

## Rebuild and cleanup

Use the repository's `launch.bat` to build and run these sources. Previously
downloaded or retained executables are not updated automatically.

The build, old-body comparison executable, RAM snapshots, rendered frame and
intermediate logs were created under `.tmp/native-loop-20261007`. They are
disposable under the workspace rules. Their hashes and compact results are in
`native-game-loop-20261007.json`; cleanup status is recorded there. No ROM, user
settings, title music or pre-existing build directory was removed. No shutdown
was issued.
