# Source-only packaging audit - 2026-09-13

This package contains native implementation source, address/hash/timing metadata,
source-only tests, build/import tools and third-party notices. It excludes ROMs,
disk images, keys, extracted graphics/audio, the supplied icon and title track,
original-data JSON, memory captures, fixtures, disassembly exports, opcode
inventories and private research output directories.

The source build no longer embeds original character, level or enemy-template
byte arrays. Default data comes from the user's disk image. The enemy callback
registry is translated executable behavior and remains in source. This is a
reverse-engineered translation, not a clean-room implementation or legal sign-off.

The prior main tree contained 5,639 files. The selected tree contains 792 files, including 780 native C/C++ files.
36 native files exceed 400 physical lines. The largest is the generated stage
callback translation (59,386 lines); stripping assets does not modularize it.

## Validation

- A fresh export containing only selected source files completed all 746 Release
  build steps. All 23 default tests passed from that export, with no ROM/capture
  files in its source tree. The 90-case private attack-direction regression also
  passed with its separately supplied data.
- Importer: 11 checks covering folder/member selection, stored/deflated ZIPs,
  cache reuse, missing members, wrong hashes, duplicates, CRC failure, truncated
  archives and corrupt-cache protection. All passed; rejected imports wrote no cache.
- Win32 first-run message and Cancel were exercised; cancellation returned 0.
- The real file picker imported a ZIP to local application data successfully.
- Six board-startup schedules using newly imported data passed with identical
  RAM/registers and main-loop handoff time of 36,185,600 ns.
- A 60-second replay launched without arguments, reused the imported cache, and
  completed 282,240,661 execution checkpoints without a runtime fault.
- Default tests require no ROMs. Capture-backed tests are explicitly opt-in.
- One pre-existing sound test failed against the previous runtime library too:
  it attempted YM writes without releasing reset. Its setup now releases the
  documented I/O reset line before checking busy status. Runtime code was unchanged.

## Original-behavior acceptance

The 26 September 2026 private acceptance baseline passes 33 of 34 research
tests. A subsequent [fixture-host repair](fixture-loop-comparator-repair-20260926.md)
passes 35 of 36 tests, including all 16 real-child tests; complete graphics,
audio, timing and scenario/fixture parity remains unproved. The native
visual-writer trace reconstructs all 805 observed renders,
but exposes missed title-frame timing and cleared visible BIOS palette entries.
The source audit resolves all 15 declared original source identities and maps
44 selected-stage contract fields; its complete-coverage gate still fails.
The presentation audit adds 30 dispositions and verifies composition of all 13
captured review frames, while retaining the stored top-byte mismatch separately
from RGB display equivalence.
A subsequent [fixture census](fixture-census-investigation-20260926.md) attempts
all 95,169 eligible records: 95,168 comparisons return and one times out.
The 35,387 passes and 59,781 divergences explicitly include 31,971 records run
with a temporary fixture-host loop correction; this is not a production-suite
pass. The integrated correction reproduces all 31,971 affected results. Game
functions, RuntimeHost behavior and compatibility rules remained unchanged in
that loop repair. The later [input-sampling repair](input-sampling-repair-20260926.md)
passes **37/38** research tests, including all **17** real-child tests. It fixes
the live I/O read upper byte and integrates the evidenced F119/F120 fixture
boundary. The affected 31,971 comparisons retain all previous passes and add
94 passes. Translated functions and compatibility rules are unchanged; complete
parity and the remaining acceptance gates are still open.
See the [current acceptance baseline](acceptance-baseline-20260926.md) and
[presentation audit](presentation-contract-audit-20260926.md). Source-only build
success and function-body coverage do not establish original-behavior parity.

The subsequent F498 fixture investigation verifies a separate current capture
executable against all 641 function descriptors, 748 base ranges and 41 body
extensions. Historical capture executable and manifest identities are unchanged.
Thirteen new secondary-selector captures pass source integrity, pre-entry
intervention, original branch and enclosing-return checks. One F498 record per
capture is admitted as controlled path evidence; 78 repeated records are excluded.
F498 now has 97 observed instruction entries and 77 missing control outcomes,
with two secondary targets and the exception/ownership gaps still open. This
grants no native replay admission or natural-reachability claim. See
[the fixture investigation](function103-lock-lifetime-20260926.md). Graphics,
matched audio, timing and comprehensive scenario acceptance remain open.
The complete MCP suite passed **97/97** after correcting stale catalog/path
expectations and an audit that omitted 17 already-proven body ranges. The audit
retains exact owner/range equality across all 41 extensions; unresolved producer
contracts still block readiness. These are tooling checks, not native-game parity.
The selector12 follow-up also passed 97/97. Selector13 has independently verified
indexed-byte effects, flags and an actual enclosing return. The complete MCP
suite passes 97/97 against the final catalog; native corpus acceptance remains
open and no native replay eligibility was added.

## Publication boundary

The source-only main history replaces the old history containing game data.
A complete verified Git bundle and working changes were preserved locally first;
research and media files remain local and excluded from the source repository.
Only the source tree is intended for GitHub. The repository remains private
pending a separate decision about the translated code's redistribution rights.

Replacing branch history removes the old commits from normal branch ancestry;
it does not erase other people's clones, forks or hosting-provider cached views.
No blanket license over original game expression is asserted. Licensed MAME and
ymfm attributions are retained. Run `scripts/Audit-SourcePackage.py` for current
file counts and packaging boundaries; it is not an infringement detector.

## Local temporary cleanup

Automatic approval review rejected the guarded removal of the temporary
`public-source-20260913` test directory with "blocked by policy", without a more
specific reason. It remains local and excluded from Git. No alternate deletion
or retry was attempted. The playable build and private history backup are retained
intentionally.

The later F498 capture build and its temporary test-output log also remain local:
automatic approval review rejected their respective guarded cleanup actions as
"blocked by policy". No alternate deletion or parent-directory removal was
attempted. Conclusions are retained in the fixture investigation and manifests.
# Windows binary release — 2026-09-27

Published [Windows x64 build](https://github.com/GeneralAtrox/GainGround_ModernSourcePort/releases/tag/build-2026-09-27)
from source commit `59f105cfffc6abba6073355cf3966c03dbc9dd8e`.
A clean `git archive` export completed all 686 steps for the Release
`gain_ground_runtime` target with GNU C++ 16.2.0 and private research tests off.
No new gameplay or full-parity tests were run for this upload.

The 4,475,279-byte ZIP contains the EXE, four required runtime DLLs, startup
instructions, third-party notices, and a build manifest recording file hashes
and imports. Recursive inspection found no missing non-system DLL import.
Every archived file matches its packaged input and the ZIP integrity check
passes. No ROMs, extracted assets, local game data, or research captures are
included. GitHub reports the asset uploaded and its SHA-256 matches locally:

- ZIP: `9cbdce96157a5eb2bb2df87b240cee33abec41597a3375be368785ed6d0d40b1`
- EXE: `ca5c52dc486eb470d33ff70fa47c285bbc651bc25b98e942d96207385d6e017d`

The user-requested release package and manifest are retained under
`run/releases/windows-2026-09-27-59f105c/`. This publication does not promote
the open original-behavior acceptance gates.

Automatic approval review rejected the guarded cleanup of
`.tmp/github-release-20260927-59f105c` as "blocked by policy". The temporary
source export and build remain local and excluded from Git. No alternate
deletion or retry was attempted.

## Local controls update — 2026-09-27

The user requested player-1-only WASD movement, Q/E attacks, F credits, and an
unlimited-credits option. Enter remains a player 1 join/primary-action alias.
Legacy arrow, Z/X and number-key bindings are disabled. Controllers keep their
three independent slots. Unlimited credits is a session menu toggle, off by
default, using the existing credit eligibility paths and preserving the credit
balance and partial coins during activation. This is an intentional gameplay
customization, including the explicit opt-in to skip F184 credit consumption.

The Release runtime built successfully with GNU C++ 16.2.0. Both focused CTest
targets passed: `gain_ground_controller_input` and `gain_ground_player_start_input`
(24 combinations of player, button, balance and unlimited-credit mode). These
cover keyboard isolation, removed bindings, mixed input sources, controller
pause/focus handling, activation with zero credits, ordinary spending, and
restoration of the underlying setting when disabled. No full gameplay or
parity campaign was run. The local playable package is retained at
`run/keyboard-credits-20260927/`; this update is not yet published to GitHub.

Automatic approval review rejected guarded removal of
`.tmp/keyboard-credits-20260927` as "blocked by policy". Build intermediates
remain local and ignored; no retry or alternate deletion was attempted.

## All-character crash smoke test — 2026-09-27

The reported CPU-B stop at `0x12742` is reproducible by the saved-actor replay:
the runtime has no exact native entry for that projectile callback. The new
`check_characters` target tests all 20 characters through four-direction
movement preparation and 180-update primary/secondary attack cases. It runs
spawned actors through the real registry, reports the failing character and
callback, times out individual cases after five seconds, and continues after
failures. Setup and coverage limits are in `native/README.md`.

The initial run completed in 0.52 seconds after compilation: **55/60 passed**.
The final `check_characters` target reproduced the same results in 0.73 seconds
and correctly returned a failing exit status.
All movement and primary-attack cases passed. Secondary attacks failed for:

| Character ID | Missing callback | First failing update |
| --- | --- | --- |
| 12 | `0x1249C` | 5 |
| 14 | `0x12598` | 5 |
| 15 | `0x1264A` | 5 |
| 18 | `0x12742` | 4 |
| 19 | `0x12742` | 4 |

This change adds tests and diagnosis; these four missing callback bodies remain
unfixed. No production native code was changed for this request. The controlled
test uses the existing private `run/last-native-failure` snapshot, whose hashes
at validation were:

- CPU-B RAM: `5d36088118ec3452496e84f1be14cad8e54ebbb27b376fb3f54927c509486572`
- Shared RAM: `15574eb293a8674ece27d0361d78690a4707079829f5a02c868921282446b791`

It is not a full-game or parity validation. The earlier keyboard/credit changes
remain in the working tree and were included in the linked test library.

Automatic approval review rejected guarded cleanup of
`.tmp/character-smoke-20260927` as "blocked by policy". Its test binaries,
intermediates and temporary logs remain local and ignored. No retry or alternate
deletion was attempted.

## Character projectile crash repair — 2026-09-27

The user authorized repairing the five failures found by the character smoke
test. Added original CPU-B callbacks `0x1249C`, `0x12598`, `0x1264A`, `0x12742`
and the required child `0x12AD2` as native IDs 955–959. The original descriptor
pointers are at `0x11D06`, `0x11D42`, `0x11D70`, `0x11DD6` and `0x11DE6`.
The last two select the same callback for characters 18 and 19.

The retained opcode generator emits 209 instructions across the five entries,
including the shared `0x12610`/`0x1263A` tails and all three original `0x1275C`
branch-table slots. It validates the descriptor pointers and source-image hashes.
The memory arithmetic shift at `0x127A0` retains the sign and updates X/C from
the shifted-out bit. The projectile collision calls preserve the existing
nonlocal-return convention: when F257/F258 have already returned to the actor
scheduler, complete that invocation without popping its stack again.
All 312 previously generated callback bodies remain text-identical.

Validation: Release compilation/link succeeded. The 60 character smoke cases
now pass, including all five previously failing secondary attacks. The saved
`0x12742` crash invocation also passes with PC `0x8594`, SP `0x7FFE`, control 1
and no fault. The final combined **61/61** CTest run took **1.14 seconds**.
The first repaired run exposed the character-14 nonlocal return at update 29;
that regression now passes with the actual child and exact outer PC/SP checks.
Tests are included under `native-child-boundary` when the private snapshot is
configured. This is bounded crash validation, not a full-game or timing-parity
claim; no fixture captures exist for the new callback IDs.

The earlier rejected cleanup of `.tmp/character-smoke-20260927` remains in
effect. Its existing build was reused for this repair; no cleanup retry was
attempted. A playable package is retained at `run/projectile-crash-fix-20260927/`.
The root `launch.bat` also builds the repaired source. Nothing has been pushed
or published to GitHub as part of this repair.

Playable EXE SHA-256:
`8c0575acc51cd7cc922e556ce1b120c8207e801cdda1a850b98ab73c8ddb04ca`.

## Windows release revision 2 — 2026-09-27

Release `build-2026-09-27-r2` includes the player-1 keyboard controls,
unlimited-credits option, root `launch.bat`, character projectile crash repairs,
and steady foreground transparency. The previous release remains available.
The release tag identifies the corresponding source on `main`; the attached
Windows ZIP contains the executable, four runtime DLLs, third-party notices,
launch instructions and a manifest identifying that source commit.

The packaged Release executable is the validated modern-transparency build,
SHA-256 `b38202a6a150397767494ed0694bc473c32fe64998026e454d7590f95129af07`.
Validation for these changes consists of the two input/credit targets, all 60
character smoke cases plus the saved crash, the focused transparency regression,
and re-rendering 57 captured Stage 6 scenes. The latter removes all alternate-frame
upper-body disappearance, retaining the four normal animation transitions.
No full-game or exact-parity claim is made. ROMs and private captures are excluded
from both the source commit and release package.

## Windows release revision 3 — 2026-09-27

Release `build-2026-09-27-r3` corrects the faded archer introduced by revision 2.
The renderer holds the revealing foreground-priority phase instead of averaging
two compositions. Characters remain opaque and steady; unrelated foreground
occlusion still applies. All earlier controls and projectile crash fixes remain.

The packaged Release executable SHA-256 is
`f0d500e60344ae4afa2c9452564023f17b0080a66bc36693f605294e8c946b0e`.
The updated focused renderer test passes. Re-rendering 57 Stage 6 scenes leaves
only the four normal animation changes; all 28 original revealing-phase
upper-body crops match exactly. This is bounded validation, not full-game parity.
The release ZIP includes the executable, required DLLs, notices and source-commit
manifest. No ROMs or private captures are included.
