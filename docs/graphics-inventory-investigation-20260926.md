# Graphics inventory investigation — 26 September 2026

The current native rasterizer reproduces the complete draw inventory at all
13 selected original review frames **when supplied their authoritative RAM**.
This establishes a bounded renderer observation, not live native CPU, producer
clock, complete render-state consumption, or full graphics acceptance. No
production behavior changed and no review build was promoted.

## Owner, ordering and raster evidence

A temporary observer was inserted into copies of the current production tile
and sprite routines. It observes their actual dispatch and sprite-loop values;
expected owners and draw parameters never enter the executable. It rejects
rowscroll, special window modes and wrapped tile rectangles because those
native calls cannot be mapped directly to one original primitive by this
observer. None of the selected frames required those paths.

The complete capture was streamed, validating 1,581,881 video-memory writes
against their captured old value, write mask and new value. The seven visual
regions were reconstructed independently. Every selected frame and draw's
resource-version ordinal matched the latest reconstructed producer event. No
video-memory write occurred inside an authoritative frame's draw interval.

All 13 selected frames were compared: 1208, 1209, 1230, 1260, 1320, 1350,
1800, 1833, 1834, 1835, 1842, 1843 and 1844. Results:

- All **589 draws** match: 120 tile submissions and 469 sprite submissions.
  The three blank frames have zero draws on both sides. There are zero missing
  or extra submissions within this captured-RAM comparison.
- All **15,565 field comparisons** match, including array-valued descriptor,
  clipping and sprite-priority fields. Global submission order, tile dispatch
  order, sprite owners, reverse list order, geometry, scroll, clip owners,
  pattern offsets, indirect palette offsets and priority values are checked.
  Offscreen sprite owners remain in the inventory.
- Every observed frame's RGB bytes equal those from a separately linked,
  unmodified production renderer and the original presentation capture.
- Every nonblank frame's complete indexed framebuffer equals the original,
  before palette lookup and presentation rotation.
- All 788 native build-source hashes remain equal to the acceptance baseline.

## Palette discrepancy and its cause

Full palette comparison disproves the assumption that converting current
palette RAM reproduces every original adjusted-palette entry. Each of the ten
nonblank review frames has **6,720 different entries out of 16,384** despite
exact indexed and presented pixels.

The captured write history has touched 4,352 of the 8,192 palette RAM words.
All 8,704 corresponding normal/alternate entries match the reference. The
remaining 7,680 entries have never received a captured palette write. Every
one matches the initial rainbow colors set by the pinned source's
`device_palette_interface::allocate_palette` in `src/emu/dipalette.cpp`.
Of those, 6,720 differ from native's RAM-derived conversion; the other 960
are black on both sides. Writes that leave RAM unchanged are included by the
capture producer, so they were not dropped from this write-history check.

None of the differing entries is used in any selected frame's final pixel
lookup. That is bounded evidence about these frames, not permission to ignore
palette initialization in other scenarios. Native's blank branch directly
emits black RGB; it has no indexed surface or generated palette to compare.
The original's two additional black/white palette entries are outside native's
16,384-entry conversion and remain outside this full-array comparison.

The discrepancy is retained as state evidence. No rainbow initialization,
palette substitution, or other native behavior change was implemented.

## Coverage and remaining gate

The private report has one coverage row for each of the original package's
635 ledger entries. It identifies 54 field paths compared to native values
under authoritative RAM and eight resource-version paths bound to probe inputs.
The remaining 573 paths are explicitly unverified by this observer; many
concern other capture domains. None is promoted to a proof of live native
consumption or universal irrelevance.

Live CPU memory correspondence, common starting state, producer cadence,
repeated presentations, complete explicit render-state consumption and the
full scenario inventory remain open. The frozen capture ledger is unchanged.
T/G/A/F/H remain open; this investigation does not authorize production changes
under the exact-parity implementation gate.

Reproduction: `analysis/acceptance-graphics-inventory.py`. Detailed results,
actual owner inventories, resource hashes, executable identities, observer
sources and coverage rows are retained in
`analysis/acceptance-graphics-current.json`, linked from the current acceptance
report. The initial observer attempt rejected snapshot base addresses because
it incorrectly expected zero; the reader was corrected to validate each
region's declared bus base. That rejected attempt is not acceptance evidence.

## Cleanup

The comparison process terminated successfully. Automatic approval review
rejected the guarded removal of `.tmp/acceptance-graphics-20260926` with
"blocked by policy" and no more specific reason. The command did not execute;
the probe executables, generated observer copies, last RAM/RGB outputs and
intermediate logs remain. Cleanup is incomplete. No alternate deletion was
attempted, and none of the earlier rejected deletions was retried.
