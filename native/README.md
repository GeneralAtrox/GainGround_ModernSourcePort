# Native source layout

`CMakeLists.txt` builds the game and source-only tests. No game assets or
research captures are required for compilation.

| Directory or file | Responsibility |
| --- | --- |
| `src/runtime_win32.cpp` | Window, input, pause, scheduling and audio output |
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
