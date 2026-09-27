# Live validation after the input sampling repair

The repaired native build completed the same neutral 14-second run without a
fault. All 792 build-source hashes were revalidated. This investigation changed
no production code and did not promote a review build.

## Graphics and audio preservation

The fresh capture is byte-identical to the preceding native capture for render
records, visual writes, sound commands, PCM audio, and terminal status. The
validation decoded all 805 render records and compared every header, video RAM
byte, and pixel byte against the losslessly retained records. It independently
reconstructed all seven video regions at every render from 363,929 visual events,
checking 5,635 region hashes.

This establishes preservation of the observed native output under the input
repair. It does not establish equality to the arcade game. Previously recorded
graphics ownership, presentation, and audio differences remain open.

## Actual input reads and timing

A read-only native observer logged the values returned to executing code. A
read-only MAME observer logged actual bus reads, including the instruction PC
(`CURPC`), separately from the advanced execution PC (`PC`). The MAME observer
used the retained modified-loader diagnostic and the same neutral input replay;
it is not a normal-boot or complete common-machine-state proof. Its sound log,
WAV, and input-port log are byte-identical to the preceding reference run.

The successful reference observer recorded 4,909 reads and a completion footer.
The first observer attempt used an unavailable Lua finalization API and was
rejected; its evidence is retained separately and is not admitted as complete.

For the fixed inclusive interval from zero to 14 seconds:

| Check | Result |
| --- | --- |
| Native input reads | 4,565 |
| Reference input reads | 4,561 |
| Consecutive reads matching CPU, instruction PC, address, mask, and value from capture start | 4,297 |
| Exactly equal timestamps within that prefix | 5 |
| Nonzero upper-byte values | Zero in both captures |
| Complete read order and timing parity | Failed |

For the matching prefix, native-minus-reference timing ranges from approximately
-1.657143 to +1.742857 microseconds. The private report retains integer
attosecond differences without rounding. No time shift or event resynchronization
was applied.

At zero-based read ordinal 4,297, native code reads address `0x800000` from
instruction `0x85d0` at 13.232317700 seconds. The reference's next read comes from
instruction `0x81c8` at 13.246637357142857136 seconds. The complete fixed interval
contains four additional native reads from `0x85d0`, one at each of four input
addresses. This is an observed ordering/cadence difference, not a proven cause.
The capture frame counters represent different boundaries and are not treated
as an authoritative frame join.

## Remaining acceptance limits

The input upper-lane correction is exercised by the live run, and the native
graphics/audio output is preserved. Exact input timing and order remain unequal.
The next timing investigation must establish the execution and machine-state
cause of the first ordering divergence before changing production behavior.
Active input scenarios, complete scene coverage, full fixture validation, live
graphics provenance, matched audio samples, and presentation acceptance remain
open. Function coverage and this neutral run do not close those requirements.

Private evidence: `analysis/acceptance-input-repair-live-current.json` and
`analysis/acceptance-reference-input-reads-current.json`. They retain the raw
input traces, observer sources, process results, binary/source identities,
completion footer, exact timing differences, and links to the verified lossless
visual payloads. Private capture and ROM material is excluded from this source
repository.
