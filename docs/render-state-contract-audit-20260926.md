# Selected render-state contract audit — 26 September 2026

All 15 source files declared by the frozen capture identity are available with
their exact recorded SHA-256 hashes. Three files that differ in the timing
reference tree have matching copies under `mame-research`: `m68000.cpp`,
`m68000.h` and `segas24.cpp`. The remaining 12 resolve under
`mame-research-c2d-v3`. No source files were replaced or combined into a build.

This recovers the declared source evidence, not the original executable or its
complete transitive build inputs. The original executable remains absent at its
recorded path. The three timing-variant diffs are retained in the audit report;
they add timing-observer facilities. Their inspection does not establish binary
identity, full build equivalence or a new authoritative runtime capture.

## Selected draw-stage coverage

The audit checks the original source, current native source and all explicit
state values in 120 tile and 469 sprite submissions across the 13 frozen review
frames. All selected tile submissions use the indexed rectangle path, window
mode zero and no rowscroll. Dispositions apply to this selected draw stage, not
to every game scenario, producer state or final presentation operation.

| New source disposition | Ledger paths |
| --- | ---: |
| Generic draw-state fields proven unused in this stage | 20 |
| Mixer registers unused by frame composition | 3 |
| Fixed palette, sampler and occlusion modes mapped to native behavior | 6 |
| Mixer register fields with consumed and ignored bits identified | 13 |
| Explicit-unused declaration masks validated | 2 |
| Total | 44 |

The ten generic state fields are depth test/write, alpha test, face culling,
general alpha blending, render-target copy/mask, EFB copy, XFB copy and
TEV/shader stage. Each is captured as zero for both draw types. The complete
indexed draw paths use color and priority bitmap operations, without the named
generic facilities. This source evidence supplements the capture declarations;
zero-valued fields alone are not the proof.

This does **not** discard active transparency, shadowing, masks or priority:

- Tiles use packed-nibble glyph samples, palette groups, category/transparent-pen
  rejection, the opaque base pass, window RAM masks and priority-bit updates.
- Sprites use integer zoom accumulators, an indirect palette, clipping and
  priority-mask tests. Indirect color zero skips; color one sets the alternate
  palette bit; other colors write an index. Accepted nontransparent pixels
  update priority to 0xff.
- Mixer registers 0–11 contribute their low three priority bits, and register
  13 contributes its blanking bit. The remaining bits and registers 12, 14 and
  15 do not enter either inspected frame-composition function.

The **complete sprite raster function** matches mechanically between the
hash-bound original source and current native source after precisely removing
the read-only capture block and its reverse-ordinal bookkeeping, changing
`sprite_ram.get()` to `sprite_ram.data()`, and ignoring comments/whitespace.
This is stronger than comparing a selected pixel result, but does not prove
that live native execution supplies the correct sprite RAM at the right time.

Tile mode mappings are source-reviewed and bound to the existing authoritative-RAM
comparison; the whole tile renderer is not claimed textually identical. The
adjusted palette array, generic tilemap state outside the selected path, producer
clocks and presentation remain separate obligations. In particular, the eight
visible startup palette differences from the writer investigation remain open.

## Fail-closed coverage result

The report preserves one row for each of the **635** original ledger paths.
It adds source dispositions to 44 rows and leaves 591 without a new disposition
from this audit. The earlier 54 native-value comparisons and eight resource
input bindings remain recorded separately at their original, limited scope.
No evidence is silently upgraded to live native consumption.

The overall coverage check returns **false**, as do contract completeness and
full parity. All T/G/A/F/H gates remain open. These selected-stage results do
not authorize production changes before the complete affected-frame contract
and native-function readiness requirements are satisfied.

`analysis/audit-render-state-contract.py` reproduces the source identity checks,
complete sprite-body comparison, captured-state checks and 635-row coverage
inventory. The detailed report is
`analysis/acceptance-render-state-current.json`, including source function
fragments, hashes, line locations and the exact timing-variant diffs. All 788
baseline native source hashes remain unchanged. No temporary build or capture
run was created in this pass.
