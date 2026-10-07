# Native source decomposition evidence

Updated 2026-09-28. This records the integrated refactor that replaced physical line chunks with complete source declarations, helper operations, and ordered data batches while enforcing a 400-line ceiling on project-owned native sources.

The original input checkpoint was `.tmp/source-split-20260927/source-checkpoint.zip` (SHA-256 `c8b388b71fe242305560818c0af9b8b4bd42cfd180a91814cc64904fd88198b3`) with source identity manifest `.tmp/source-split-20260927/source-inputs.json` (SHA-256 `090d691e9c3a8759f7cac35d7038bfa80b12e272510f3e6a4b10c4287a436447`). The temporary archive and scratch root were removed after recording their hashes. The compact original path/hash inventory and affected-source list are retained at `AgentState/source-line-split-integration/source-inputs.json` and `split-paths.json`. The shared splitter and tests are [native_source_layout.py](../scripts/native_source_layout.py) (SHA-256 `a8922bddad1fae04271ccb174aa54f94bfe19ed9eb0e7a50eb82d1cfac66ea09`) and [test_native_source_layout.py](../scripts/test_native_source_layout.py) (SHA-256 `42476103a8769209b16d2ff7e08f396781cd8fd944d8c65509cacde24735fc73`). Oversized arrays use the shared constexpr composer [source_layout_detail.h](../native/include/gain_ground/source_layout_detail.h) (SHA-256 `84a2572e7f8de438a1a5129de3d7761e7902b698bf4a3e1b2f41de1de15d4aac`); generated headers keep their `#pragma once` on the wrapper. The splitter rejects an indivisible declaration over 400 lines instead of cutting through a function.

The final physical-source scan was `cmake -DSOURCE_ROOT=native -P native/CheckSourceLineLimit.cmake`. It passed for 1,177 files under `native/src`, `native/include`, and `native/generated`; the largest file was 399 lines. There are no `.partNNN.inc` files or references left in those source trees. The source package audit passed with zero errors. Its four reported files over 400 lines are existing vendored ymfm sources, outside the project-source limit scan.

Generated outputs retain complete table order through named constexpr batches. The original 641-entry checkpoint mask table now composes five batches through the shared helper; the test suite checks entry order, count, wrapper inclusion, and the trailing-comma edge case. The oversized state-72 callback dispatches are factored into whole PC-case helper switches. Sorted PC-block label inventories exactly match the checkpoint: 6,518 labels in `gground_stage_callbacks.cpp` and 1,313 in `cpu_b_stage_gap_callbacks.cpp`. These inventories establish retained case coverage, not general binary or runtime equivalence. The generated stage callback report remains `implemented-but-unverified`.

The eight generated owners handled were `gground_checkpoints.h`, `gground_corpus_fixture_catalog.h`, `gground_enemy_definitions.cpp`, `gground_fixture_marker_compatibility.cpp`, `gground_functions.h`, `gground_game_definitions.cpp`, `gground_sound_timing.h`, and `gground_stage_callbacks.cpp`. `cpu_b_stage_gap_callbacks.cpp` also received generated helper factoring. Other split files are listed in the retained `AgentState` evidence. CMake registers the runtime host, fixture comparison, Win32 runtime, and CPU-B helper translation units. Private ROM-derived game-definition sources remain ignored and denied by the source-package allowlist: all eight emitted `gground_game_definitions.module*.inc` files were checked individually.

Verification recorded for the current source tree:

- `python -m unittest test_native_source_layout -v` from `scripts`: 9 tests, 8 passed and the optional standalone `g++` test skipped because `g++` is unavailable in that shell. The integrated MSYS2 build below compiled the generated C++.
- `python scripts/generate_gain_ground_fixture_marker_compatibility.py --check`: passed.
- `python scripts/Audit-SourcePackage.py`: passed, zero errors; 832 tracked package files and 792 native code files.
- CPU-A rebuilt all 9 owned translation units with `-Wall -Wextra -Wpedantic -Werror`; CPU-B independently compiled all 55 owned translation units and preserved exact PC-case label multisets for its 13 owners.
- Full CMake build linked `gain_ground_runtime.exe`; full CTest passed 28/28. The exact command, MSYS2 PATH setup, source manifest, and cleanup record are in [CPU-B evidence](../AgentState/cpu-b-luna6-w3/EVIDENCE.md), whose SHA-256 at final validation was `635ce174e63ba772939acc9018b11dde70dcc597c32c58df7683bb58c483e19f`. CPU-A source and runtime evidence is in [CPU-A evidence](../AgentState/luna6-cpu-a-source-splits/EVIDENCE.md) and [runtime evidence](../AgentState/luna6-runtime-modules/EVIDENCE.md).

The standard native code-generation `--check` still stops at `Function contract identity differs without a compatibility proof`, exactly matching the pre-existing result (baseline log SHA-256 `d3b7aa135d98bc0b0d8585298c46521fcddca68eb9308229f5c85c3ef92d4c2b1`). The scratch log was removed after recording the error and hash. This refactor did not change contract metadata or waive that failure. No new parity capture campaign was run. The full CMake build and CTest tree were disposable; CPU-B recorded its removal after saving the result above.

The preserved baseline evidence recorded a successful 759-step build and 27/27 tests. Its code-generation check already produced the same contract-identity error (baseline log SHA-256 above). The final suite has 28 tests because the source-line-limit test was added. Cleanup verified that `C:\Projects\GainGround\.tmp\source-split-20260927` was the exact target inside the workspace, contained no reparse points, and had no active build processes; it was removed without `-Force`, and `Test-Path` returned false.

## Runtime regressions found after the split (2026-09-28)

Label inventories matched, but three CPU-B translations stopped playing correctly. Each broke a Stage Select run with "Native function rejected execution at PC ...":

- `cpu_b_advance_phase_palette_overlay_dispatch.cpp` (function 148, PC 0xD3A0) and `cpu_b_mode_dispatch_ecdc.cpp` (function 169, PC 0xED36). The instruction helpers were called from the post-instruction `switch (next)` default instead of the `switch (pc)` default. So an instruction owned by a helper was never executed, and a transfer to it was not dispatched out. Fix: moved the helper calls back into the `switch (pc)` default and restored `default: return m.dispatch(...)` in the transfer switch.
- `cpu_b_initials_entry.cpp` (Round 1 Stage 9, PC 0xF19C). A shared `rts` case group (`0xf10c`, `0xf19c`, `0xf1c2`, `0xf316`, `0xf33c`, `0xf35a`, `0xf374`, `0xf3a4`, `0xf3aa`) was split across files. Only `0xf3aa` kept the `m.ret()` body. The other labels landed in region helpers directly above `default: return false`, so every return through them became a contract violation. Fix: moved the labels back onto the `m.ret()` case in the main switch.

Check used afterwards: map every case label to its whitespace-normalised body in HEAD and in the split files (owner plus helper files), then compare the multisets. After the fixes, every remaining difference in `native/src/translated`, `native/src` and `native/generated` was one of three mechanical rewrites:

- a child result carried in an `outcome`/`step.result` that the caller returns;
- `break` rewritten as `return true` in a `contains_next`/`begin_*` predicate;
- a renamed timing parameter.

Fixture comparisons for functions 148, 169 and 175 diverge identically in a HEAD build and in the split build (same record, same first divergence), so those divergences predate the split. 595/597/610/611 pass. Rebuilt tree: CTest 28/28, `scripts/Test-StageSelect.ps1 -From 0 -To 39` 40/40 (run in chunks of 10).

When splitting an interpreter-style translation, check case bodies, not just labels. Keep instruction helpers inside `switch (pc)`. Keep every label of a shared-body group in the file that holds the body.
