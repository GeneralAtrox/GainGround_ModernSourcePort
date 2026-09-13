# Source-only packaging audit — 2026-09-13

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
