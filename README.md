# Gain Ground native source port

A Windows C++20 source port of the Sega System 24 arcade game. This repository
contains the native implementation and supporting source. It does not supply
ROMs, disk images, extracted graphics, music, game icons or captured game memory.

This is a reverse-engineered translation, **not a clean-room implementation**.
Removing game data is not legal clearance of the translated code. See
[NOTICE.md](NOTICE.md) for provenance and licensing limits.

## Improvements
- New pathfinding model stops characters getting stuck on each other, on geometry and on their restricted play area.
- Added ability to change white noise title screen music
- Steady foreground drawing replaces alternate-frame wall flicker while keeping characters opaque.
- Player 1 keyboard controls use WASD/Q/E/F, with an optional unlimited-credits mode.
- Fixed secondary-attack projectile crashes affecting five character IDs.
- Single native game loop replaces independently scheduled CPU-A/CPU-B workers.
- Bounded iteration prevents the reported 0x1F0DA call-depth crash.

## Build

For a ready-to-run Windows x64 build, download the ZIP from
[GitHub Releases](https://github.com/GeneralAtrox/GainGround_ModernSourcePort/releases/latest).
Extract the whole folder and run `gain_ground_runtime.exe`, keeping the included
DLLs beside it. Supply your own supported ROM set on first launch.

Requirements: Windows, CMake 3.24+, Ninja, a C++20 compiler and zlib development
files. With the MSYS2 UCRT64 toolchain installed at `C:\msys64\ucrt64`:

Double-click **`launch.bat`** at the project root to build the game and open it.
It reuses `build/launcher` for incremental builds and leaves errors visible if
the build fails. Set `GAIN_GROUND_TOOLCHAIN_ROOT` if MSYS2 UCRT64 is installed
elsewhere. CMake and Ninja must be on PATH.

For a manual build and test run:

```powershell
$env:Path = 'C:\msys64\ucrt64\bin;' + $env:Path
cmake -S native -B build -G Ninja -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DZLIB_ROOT=C:/msys64/ucrt64
cmake --build build --parallel
ctest --test-dir build --output-on-failure
.\build\gain_ground_runtime.exe
```

Committed C++ files suffice to build; private research generators are not build
prerequisites. For a portable local build, keep the compiler runtime DLLs and
`zlib1.dll` beside the executable, and retain the third-party notices. Binary
downloads are published as release assets rather than committed to the source tree.

## First run

Launch without arguments. **Find your ROM to extract** opens a file picker.
Select your arcade ROM ZIP or a member of an extracted folder containing:

- `epr-12187.ic2`
- `epr-12186.ic1`
- `ds3-5000-03d-rev-a.img`

The supported set is `gground`, disk DS3-5000-03D Rev A. Console versions, other
revisions, encrypted ZIPs, split archives and ZIP64 are not supported. The program
checks the complete BIOS and disk identities and every extracted payload before
using them. It does not download ROMs or supply the game's decryption key.

Data is cached under `%LOCALAPPDATA%\GainGround\rom-v1-*`. Later launches validate
and reuse the cache. A damaged cache prompts for the ROM again and creates a new
generation. Cancel closes the application. Original ROM files are never modified.

Advanced local research can still use:

```powershell
.\build\gain_ground_runtime.exe --bios C:\local-data\cpu_a_program.bin --assets C:\local-data\assets
```

## Controls and local presentation

Three players share the stage. Keyboard controls player 1; Xbox controllers 1-3
control the matching slots. Press **P** or use the Pause menu to pause; the title
changes to **Gain Ground - Paused**.

Keyboard controls are exclusively for player 1. By default **WASD** moves, **Q**
uses the small attack, **E** uses the bigger attack, and **F** adds a credit.
Press **Enter** (or an attack button) to join/start. **Esc** exits. Left/right
mouse buttons also perform the two attacks. The old arrow, Z/X and number-key
bindings are disabled; additional players use controllers.

**Settings > Controls...** remaps the keyboard and the controller layout (shared
by all three controllers). Click a binding, then press the new key or button; a
key already in use swaps with it. The game pauses while the dialog is open, and
the choices are saved to `%LOCALAPPDATA%\GainGround\controls.ini`. Esc and the
Pause key stay fixed, the left stick always moves, and Enter stops joining once
it is bound to another action.

Toggle **Settings > Unlimited credits** to start and continue without spending
credits. It is off at launch and applies to all players for the current session.
While it is on, the menu item is checked and the window title shows
"Unlimited credits". Switch it off to return to the normal credit balance.

The **Stage** menu jumps to a chosen stage (Round 1-4, Stage 1-10). During play
the current stage ends at once through the game's own stage-clear sequence and
the chosen stage loads with its round's graphics bank. Chosen at the title, it
applies on the first frame of the next game. The title bar shows the pending
choice until it applies. It applies once, and the attract demo is unaffected.
Pick **Continue normally** to withdraw the choice before it applies.

The default icon is Windows' application icon. Optional user-provided
`%LOCALAPPDATA%\GainGround\game.ico` replaces it. Optional
`%LOCALAPPDATA%\GainGround\title_music.pcm` replaces the title cue once, without
looping. Its format is raw signed 16-bit little-endian stereo PCM at 62,500 Hz,
at most 64 MiB. Neither file is shipped. Without a replacement, audio comes
from the original ROM-driven synthesis.

## Performance

`GAIN_GROUND_SAMPLE=<path>` starts an in-process statistical sampler of the
runtime thread; on exit it writes a histogram that
`scripts/Resolve-Samples.py build/gain_ground_runtime.exe <path>` ranks by
function (build with `-g` for symbols; samples in system DLLs are attributed
by module and export). `GAIN_GROUND_NAV_LOG` adds a `progress` line every 600
frames with execution counts and audio queue diagnostics.

The playable runtime uses one native game loop and one monotonic device clock.
Input, actor updates, sprite construction, asset loading and timed sound services
execute serially. Original CPU/state tags and register layouts remain as call
adapters for translated routines and fixture research; they do not identify
independently running CPUs. The Windows game thread has one resumable call stack
so long operations can yield every presentation slice for input and audio delivery.
Board events remain pending until their serial service consumes them.

The old `GAIN_GROUND_QUANTUM` and `GAIN_GROUND_DIRECT_WAIT` settings no longer
select a scheduler in the playable executable. Rendering remains limited to
display rate above real speed. See [migration and checks](docs/native-game-loop-20261007.md)
for the tested scope and the remaining arcade-parity limits.

The migration check passed 30 CTest checks, all 40 stage smoke cases and all 60
character movement/attack cases. Live original/replacement audio checks reported
no starvation, dropped buffers or clipping after pacing was corrected. Historical
contract generation and full fixture parity remain blocked as detailed above.

## Source and checks

See [native/README.md](native/README.md) for implementation layout and
[docs/source-release.md](docs/source-release.md) for the packaging audit.
Default tests need no original game assets. Additional capture-backed tests
require private research data and `-DGAIN_GROUND_RESEARCH_TESTS=ON`.

To check extraction with your own set:

```powershell
python scripts/Test-RomImport.py build/gain_ground_rom_import.exe C:\my-roms\gground
python scripts/Audit-SourcePackage.py
```

To drive the built game through every stage with the Stage menu, spawning a
character and playing a few seconds on each, and report which stages load and
run without a runtime stop:

```powershell
.\scripts\Test-StageSelect.ps1 -From 0 -To 39
```

Add `-Sweep` for the exhaustive variant: the game runs as fast as the host
allows with an invulnerable player, and on each stage the runtime moves player 1
to every 8-pixel grid cell whose footprint the terrain probes accept, a few
frames apart (`-SweepFrames`), so the character's update, contacts and the
enemies' reactions run from every reachable position. The sweep holds the
stage's time-up byte, skips cells with any terrain attribute, and by default
stays below the exit strip at the top of the field, because the original ends a
player who reaches the exit with an empty roster. The test aids behind it are
environment variables read at startup and otherwise inert:
`GAIN_GROUND_TEST_SPEED` (audio is dropped above 1; the current emulation
sustains about 3x on one core), `GAIN_GROUND_INVULNERABLE` (hits are ignored
without marking the attacker), `GAIN_GROUND_SWEEP_FRAMES`,
`GAIN_GROUND_SWEEP_BOUNDS=x0,x1,y0,y1`, and the sweep window commands the
script posts. The script also sets `GAIN_GROUND_DEFAULT_CONTROLS=1`, so the
runtime uses the default key bindings it presses (F credit, Q start/attack) and
neither reads nor overwrites the controls saved from Settings > Controls.

It needs the imported ROM cache and the MSYS2 runtime DLLs, takes about ten
seconds per stage plus the play time, and restarts the game after a runtime stop
so the remaining stages are still covered. The per-stage fault summaries name the
untranslated routine when a stage's objects or enemies reach code no captured
run executed. Such a routine is translated with
`scripts/Generate-GapTranslations.py`, which reads the privately retained
CPU-B opcode bank, follows the routine's control flow and unregistered callees,
and regenerates `native/src/translated/cpu_b_stage_gap_callbacks.cpp` with its
registry header. Its output is implementation-first translation, not fixture
evidence.
