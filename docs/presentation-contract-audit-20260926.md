# Selected presentation contract audit — 26 September 2026

Every packed 32-bit pixel in the 13 frozen review presentations is reproduced by
looking up each captured indexed pixel in its own captured adjusted palette,
then mapping source `(x, y)` to destination `(y, 495 - x)`. This verifies the
recorded composition at those exact frames without mixing source frames,
fitting a time offset or replacing captured pixels.

The native renderer's stored words remain different: the reference's top byte
is `FF`, while native's production encoding makes it `00`. RGB equality must not
be reported as raw 32-bit buffer equality.

## Frame ownership, layout and lookup

For every selected frame the audit checks frame-begin/end ownership, indexed
CRC32, frame-pixel linkage, presentation ordinal and completed-frame linkage,
screen frame number, capture/snapshot time, visible rectangle, row layout,
orientation, pixel format and byte counts. The indexed surface is 496 by 384;
the presentation is 384 by 496, with orientation 6, native snapshot view,
bilinear filtering disabled and no repeated frame at these selected records.

Across **2,476,032 pixels**, the captured indexed/palette/rotation composition
has zero differing packed words against the captured presentations. The RGB
hashes also match the earlier actual native renderer probes supplied with each
frame's authoritative visual RAM. This remains a captured-input result, not
live CPU/frame correspondence.

The three blank frames use special palette index 16384 for every visible pixel,
whose captured value is `FF000000`. The original submits no draws. Native's
blank branch submits no draws and directly emits RGB black, without an indexed
surface. The visible outputs agree; the internal indexed states are not equal.
Special white entry 16385 is never used in any selected frame.

For nonblank frames, all final indices are below 16384. The previously measured
6,720 differing adjusted-palette entries per nonblank frame are absent from
the final lookup set. The indexed draw stages operate on indices and priority,
not these final adjusted RGB values. This explains their irrelevance to the
selected output pixels; it does not establish equality of the full palette or
excuse the separate eight visible startup palette differences in the live run.

## Stored top byte versus displayed RGB

The reference's captured words retain their top byte. MAME's software snapshot
renderer has a direct-format conversion path that returns the source word
unchanged. Native's color function assembles only red, green and blue bits.
Consequently, all **2,476,032 stored words differ in their top byte**, even where
their RGB channels match under authoritative input.

The inspected Win32 path uses a top-down, 32-bit `BI_RGB` DIB with `SRCCOPY`.
Microsoft specifies that this format does not use the top byte for display.
See [BITMAPINFOHEADER, 32-bit BI_RGB](https://learn.microsoft.com/en-us/previous-versions/dd183376(v=vs.85)).
This is a scoped display-ABI argument, not permission to discard that byte from
the retained reference or claim byte parity.

An in-memory GDI check submits each captured image twice, with only the top byte
changed between `FF` and `00`, into a 24-bit DIB at 1:1 dimensions. All **26
StretchDIBits calls** return the expected 496 scanlines and reproduce exactly
the captured RGB. Bitmap/DC handles are released after each case; no window,
temporary executable or capture directory is created. The test does not measure
window scaling, paint cadence, compositor output or repeated presentations.

## Coverage and identity limits

The audit adds 30 presentation dispositions and retains a cumulative 635-row
inventory. Together with the earlier source audit, 74 paths have a selected
source-stage or presentation disposition. Earlier native-value observations
remain recorded separately. None is promoted to full live consumption.

Five generic MAME support files were also checked against the capture's base
Git commit. Their contents agree after CRLF normalization; their raw file hashes
differ from the Git blobs. Both identities and this distinction are retained.
Those files were not individually listed in the original 15-file manifest,
so this check does not expand the claim about original executable provenance.

The overall coverage check, contract completeness and full parity remain false.
Raw buffer identity, full palette state, live frame/producer correspondence,
presentation timing, audio and full scenario/fixture acceptance remain open.
All 788 baseline native build-source hashes remain unchanged.

Reproduction: `analysis/audit-presentation-contract.py`. Detailed per-frame
checks, GDI results, source/ABI identities and the cumulative coverage inventory
are in `analysis/acceptance-presentation-current.json`. No production code changed.
