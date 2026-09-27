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
