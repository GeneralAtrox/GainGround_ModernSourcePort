# Live owner and reference-boundary investigation — 26 September 2026

The live native run can produce identical pixels with different sprite owners,
clip-list links, or tile submission order. A separate reference diagnostic now
observes the ordering of completed rendering, sprite IRQ assertion and CPU
interrupt-state updates. Neither result closes the full graphics contract.
Production behavior is unchanged; all T/G/A/F/H acceptance gates remain open.

## Live owner and resource comparisons

The complete frozen reference frame inventory was compared against every
exact-RGB candidate in the retained 804 native game renders. Pixel equality
selects candidates only; it does not establish a shared frame or producer clock.
Each comparison uses one complete reference frame, without mixing resources
from different frames or remapping sprite owners.

The original capture's 1,581,881 visual writes were checked while reconstructing
reference RAM. All 152 distinct native candidate probes reproduced their
retained live RGB output. The actual native tile, sprite and dispatch fields
were compared with the captured reference inventory.

| Reference frame | Native render candidates with identical RGB | Candidates with all observed inventory fields matching |
| --- | --- | --- |
| 1208, 1835, 1842, blank | 135 candidates each | 15 zero-draw candidates each |
| 1209, title entry | None | None |
| 1230 | 148, 149 | None |
| 1260 | 179 | None |
| 1320 | 239 | None |
| 1350 | 269 | None |
| 1800 | 623, 624, 719, 720 | None |
| 1833 | 497, 498, 593, 594, 753 | 498, 594 |
| 1834 | 497, 498, 593, 594, 753 | None |
| 1843 | 761, 762 | 761 |
| 1844 | 761, 762 | 762 |

Several comparisons use the opposite sprite-owner bank, separated by 0x400
owner slots. Others preserve owners but differ in root clip-list links. Frames
1843 and 1844 have identical images but different tile submission metadata;
selecting the wrong native candidate produces 20 field differences. A black
image also does not prove that the authoritative frame submitted zero draws.

For all nonblank exact-RGB candidates, indexed pixels, final palette lookups,
submitted tilemap bytes, referenced glyphs, complete reference-assigned sprite
patterns (including clipped cells), and indirect palettes match. Pattern checks
use the reference's assigned ranges; they do not compensate for differing native
metadata. Whole character, palette and sprite RAM still differ. The checked
subsets do not prove all remaining memory irrelevant.

The first native nonblack render, ordinal 129, was additionally checked against
reference title-entry frame 1209. Its 12 tile-call metadata records match, but
3,072 indexed pixels, eight final palette entries and 94 tilemap bytes differ.
No identical title-entry image exists anywhere in the retained 14-second live
run. There is no established temporal alignment for this diagnostic pair.

## Original sprite producer

The immutable capture identifies CPU B, FD state 0x72, PC 0x80d8, as the first
sprite-root writer following every selected review frame. The six root words
were reconstructed and retained for all 1,845 frames, together with preceding
write provenance. Selected following root writes occur approximately 30–33
microseconds after frame completion. Exact attosecond timestamps and global
event ordinals remain in the private report.

The capture's CPU control-flow events are not an IRQ-line event stream. This
reconstruction alone cannot prove when the sprite interrupt was asserted.

## Reference boundary diagnostic

The original capture executable, `gground_research_v3.exe`, is no longer at its
recorded path, and no matching executable was found in the workspace search.
The retained `gground_timing.exe` has a different identity. The current source
tree matches 12 of the original identity's 15 source files; `m68000.cpp`,
`m68000.h` and `segas24.cpp` differ. This prevents claiming a rerun of the
original executable or closing its contract from this diagnostic alone.

The timing executable was run through normal boot using the original neutral
replay and semantic configuration, bounded to 33 seconds, with read-only Lua
observation. No modified-loader adapter, CPU patch, native behavior change or
new MAME build was used. Both observation runs exited successfully without Lua
errors. Executable, replay, configuration, observer and source identities are
retained separately from the frozen capture identity.

At each of 1,899 boundaries the observer records three phases at exactly the
same machine time:

1. The frame-completion callback sees sprite IRQ flag 0 and screen frame N.
2. A zero-delay callback sees sprite IRQ flag 1 and screen frame N+1; CPU
   interrupt levels have not yet changed.
3. A second zero-delay callback sees level 5 on each CPU whose sprite interrupt
   is enabled, or level 0 where disabled.

Neither CPU's saved total-cycle count or current PC advances between these
phases. The sprite-root words also remain unchanged. CPU A has sprite IRQ
enabled at 998 boundaries and CPU B at 1,074. All first two phase records are
identical between the two observer runs, including times, PCs, cycle counts,
interrupt fields and root descriptors.

The measured order agrees with the inspected source: `vblank_end` completes
the screen update before incrementing its frame number; the scanline callback
sets the sprite flag and requests the CPU line update; CPU input delivery is
synchronized separately. The observed ordering belongs to the timing variant.
No complete CPU, device or scheduler equivalence with the frozen run is claimed.

## Comparison with the frozen capture

All 1,845 retained reference frame times and six-word sprite-root descriptors
match the diagnostic at the same ordinals. At each of the 13 selected frames,
all **475,168 bytes** across the seven visual RAM regions match exactly, as do
all RGB presentation pixels. This checks explicit frame ordinals and original
times; it does not use a fitted offset or choose frames by appearance.

The first observer used Lua `screen:pixels()` at frame completion. Source
inspection and comparison rejected this as a completed-presentation tap:
`update_quads` has switched `m_curbitmap` to the other buffer, while the completed
output is owned by `m_curtexture`. It disagreed at frames 1209, 1835 and 1843.
The corrected observer uses presentation snapshots; all 13 match. The rejected
tap and its differences are preserved, not silently shifted to another frame.

## Evidence and remaining work

Private reports are `analysis/acceptance-live-owner-join-current.json`,
`analysis/acceptance-sprite-root-cadence-current.json` and
`analysis/acceptance-reference-frame-boundary-current.json`. The last retains
both raw boundary CSVs, observer source/configuration/process records, all
review RAM and presentation snapshots, and the rejected pixel tap losslessly.
All 788 baseline native build-source hashes remained unchanged.

Original executable/source identity, native producer-write provenance,
complete draw/state consumption, common CPU/device handoff, repeated
presentation behavior, audio and full scenario/fixture acceptance remain open.
The measured boundary cannot yet authorize a production render-trigger change.

Automatic approval review rejected the single guarded removal attempt for
`.tmp/acceptance-live-owner-join-20260926`,
`.tmp/acceptance-reference-frame-boundary-20260926` and its `-r2` directory with
"blocked by policy" and no more specific reason. The command did not execute;
temporary RAM, pixels, configuration and intermediate logs remain. Cleanup is
incomplete. No alternate deletion method was attempted, and earlier rejected
deletions were not retried.
