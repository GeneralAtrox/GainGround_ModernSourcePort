# Gain Ground native source port

A Windows C++20 source port of the Sega System 24 arcade game. This repository
contains the native implementation and supporting source. It does not supply
ROMs, disk images, extracted graphics, music, game icons or captured game memory.

This is a reverse-engineered translation, **not a clean-room implementation**.
Removing game data is not legal clearance of the translated code. See
[NOTICE.md](NOTICE.md) for provenance and licensing limits.

## Improvements
New pathfinding model stops characters getting stuck on each other, on geometry and on their restricted play area
Added ability to change white noise title screen music

## Build

Requirements: Windows, CMake 3.24+, Ninja, a C++20 compiler and zlib development
files. With the MSYS2 UCRT64 toolchain installed at `C:\msys64\ucrt64`:

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
`zlib1.dll` beside the executable, and retain the third-party notices. No binary
release is bundled here.

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

The default icon is Windows' application icon. Optional user-provided
`%LOCALAPPDATA%\GainGround\game.ico` replaces it. Optional
`%LOCALAPPDATA%\GainGround\title_music.pcm` replaces the title cue once, without
looping. Its format is raw signed 16-bit little-endian stereo PCM at 62,500 Hz,
at most 64 MiB. Neither file is shipped. Without a replacement, audio comes
from the original ROM-driven synthesis.

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
