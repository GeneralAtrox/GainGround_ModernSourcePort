# Native source layout

`CMakeLists.txt` builds the game and source-only tests. No game assets or
research captures are required for compilation.

| Directory or file | Responsibility |
| --- | --- |
| `src/runtime_win32.cpp` | Window, input, pause, scheduling and audio output |
| `src/native_game_loop.cpp` | Single native frame loop and synchronous system/sound services |
| `src/runtime_startup_win32.cpp` | First-run ROM picker and cache selection |
| `src/rom_archive.cpp`, `src/rom_import.cpp` | ZIP/folder import and validated extraction |
| `src/runtime_host.cpp` | Native dispatch, guest memory and runtime hooks |
| `src/system24_*.cpp` | Board devices, video and synthesized audio |
| `src/gameplay/` | Character/enemy hierarchies, attacks, navigation and bridges |
| `src/translated/` | Translated routines and CPU helpers |
| `generated/` | Committed callbacks, registries and address/hash metadata |
| `include/gain_ground/` | Runtime and gameplay interfaces |
| `tests/` | Controlled tests and optional private capture readers |
| `third_party/` | Licensed MAME adaptations and ymfm with notices |

Character statistics, enemy templates, layouts, placements and rescue tables
come from the user's ROM data. `gground_enemy_definitions.cpp` retains translated
enemy callback dispatch without embedding those data tables. The optional
character override loader remains available through `--characters`.

The translation retains operand widths, flags, guest addresses and control flow.
This implementation provenance is not a clean-room or copyright-clearance claim.

The small gameplay modules meet a 400-line target, but some generated routines,
timing helpers, runtime modules and licensed vendor files are larger. There is
no universal 400-line enforcement.

## Quick all-character smoke test

Configure an existing build with a private runtime failure snapshot, then run:

```powershell
cmake -S native -B build -DGAIN_GROUND_CHARACTER_SNAPSHOT=C:/Projects/GainGround/run/last-native-failure
cmake --build build --target check_characters
```

The snapshot must contain `cpu-b-ram.bin` and `cpu-a-shared-ram.bin` from an
active game with player 1 initialized. These private ROM-derived files are not
distributed. For a fresh build, also supply the normal compiler and ZLIB
configuration described by `scripts/Build-Source.ps1`.

The 60 isolated cases cover all 20 character IDs: four directional movement
preparations, primary attack, and secondary attack. Each attack runs 180 native
updates, including spawned projectiles and their later callbacks. Each case has
a five-second timeout; failures do not prevent the other cases from running.
An attack that never creates an actor fails instead of passing vacuously.

This is a bounded smoke test seeded from live RAM, with controlled character
profiles and inputs. It does not simulate a complete match, CPU A, stage
progression, rendering/audio, or every collision. CTest prints the character,
attack, frame and callback for a failure and exits nonzero. Without a configured
snapshot these tests are not registered. The initial 2026-09-27 run took 0.52
seconds after compilation: 55 passed and 5 secondary attacks failed (IDs 12,
14, 15, 18 and 19). The last two reproduce the reported missing `0x12742`
callback; the others reach missing callbacks `0x1249C`, `0x12598`, `0x1264A`.
The subsequent repair adds those callbacks and their required helper and handles
the collision helper's nonlocal return. All 60 character cases now pass; the
separate saved-crash replay passes as well. The combined 61-test run took 1.14
seconds. These cases carry the `native-child-boundary` label for the integration
suite when the snapshot is configured.

## Stage 6 rampart archer flicker

The 2026-09-27 investigation reproduced alternating upper-body occlusion in
57 consecutive native frames (1439–1495) on stage index 5. Actor activation
remained stable, and the affected sprite retained the same submitted geometry,
graphic and palette across the first two frames. Mixer priorities 1 and 3
alternated from 5/3 to 2/2; an unmasked foreground tile (entry 0x8B84) therefore
covered part of the archer on alternate frames. The replay ended without a fault.

Retained CPU-B instructions at 0x80FC and 0x810A explicitly toggle these mixer
bytes, gated by stage flag 0xD0E. Stage index 5 selects 0x80 from table 0xABB0,
enabling both toggles. The native IRQ implementation preserves those operations.
This identifies the priority-alternation effect, not a confirmed translation
defect or full original-machine parity.

The first user-authorized modernization replaced this effect with steady
50% transparency. The renderer composes both priority orders from the same
current scene and blends their RGB results. It uses no previous-frame images,
so moving objects leave no trails. Unaffected pixels retain their exact colors;
game RAM, actor behavior and the IRQ code remain unchanged. Both flag modes
(0x80 and 0x81) are covered, including the second foreground-priority channel.
The stage table enables this mechanism across all 40 stages; the new presentation
applies wherever it is used. This is intentionally modernized presentation,
not an original-hardware parity claim.

The ROM-free `gain_ground_video_transparency` test checks both modes, opposite
phase equality, actual red/blue 50% blending, fully opaque unobscured sprites,
motion without trails, unchanged mixer RAM and blanking. It passes. Re-rendering
the same 57 captured Stage 6 scenes changes the affected upper-body crop only
on the four normal animation updates, instead of all 56 frame transitions.
Measured rendering averaged 6.26 ms/frame on the development machine; this is
a renderer-only measurement, not a full-game performance guarantee. Release
compilation also passed.

The local playable package is `run/modern-transparency-20260927`; its executable
SHA-256 is `b38202a6a150397767494ed0694bc473c32fe64998026e454d7590f95129af07`.
Root `launch.bat` also builds the updated source. This update is included in the
`build-2026-09-27-r2` GitHub release.

A focused review found that shadows already use direct palette shading rather
than alternate-frame visibility. No additional rendering workaround was
confirmed by this review; it was not an exhaustive audit of every game effect.

Automatic approval review rejected guarded cleanup of the diagnostic directory
`.tmp/archer-flicker-20260927` as "blocked by policy". Its temporary executable
and captures remain; cleanup was not retried.
The same review rejected the guarded removal of the subsequent replay directory
`.tmp/modern-transparency-20260927` as "blocked by policy". Its replay executable
and comparison frames also remain; no alternate removal was attempted.

### Opaque archer correction

The user then reported that revision 2's blending made an inner rampart archer
look transparent. That was a defect in the modernization: averaging the two
compositions faded the character wherever the wall had covered it.

The current renderer instead keeps the revealing priority phase for the two
alternating foreground categories. It renders once, with no RGB averaging, so
characters remain fully opaque. The effect-enable flag and its per-category
selection still apply; other foreground priorities and game RAM are unchanged.
This intentionally replaces the old visibility effect rather than reproducing
original-machine presentation.

The updated regression passes for both modes, phase stability, fully opaque
characters, unrelated foreground occlusion, motion, unchanged mixer RAM and
blanking. All 57 retained Stage 6 scenes were re-rendered: the affected crop
changes only at the four animation updates, and all 28 original revealing-phase
crops match exactly. Visual inspection confirms the faded upper body is gone.
Rendering averaged 3.41 ms/frame in this bounded replay. Release build passed.

The corrected local package is `run/opaque-archers-20260927`; `launch.bat` also
builds this correction. It is included in GitHub release `build-2026-09-27-r3`.
Validation reused the existing temporary directories whose cleanup was already
rejected; those removals were not retried.
