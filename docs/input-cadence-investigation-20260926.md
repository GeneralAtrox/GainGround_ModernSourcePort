# Input cadence and initialization timing investigation

A bounded observation from 13.15 to 13.28 seconds isolates a concrete missing
native timing segment during F141 runtime-record initialization. No production
behavior was changed. The added observers preserved the preceding native render,
visual-write, sound-command, PCM and input-read captures byte for byte. The
reference sound, PCM and input captures are also unchanged.

## Observed initialization segment

All 225 observed F141 memory reads and writes in CPU-B work RAM
`$1400–$33ff` match the reference in operation kind, address, masked value, mask,
and order. The native events all occur at **13.215677500 seconds**. Their
reference counterparts span **13.215899757142857136 through
13.216776157142857136 seconds**, an elapsed **876.4 microseconds**.

The first 180 operations initialize 60 record locations. That subset alone
spans 273 microseconds in the reference and zero native device time. The
verification independently checks its address sequence, masks and values.

F141 uses raw host memory helpers without instruction timing. The native
reported context PC remains its entry, `$a65c`; the reference reports original
instruction PCs `$a6be`, `$a6c0`, `$a6dc`, `$a6e4`, `$a6ea` and `$a6ee` for the
225-operation segment. The source and capture establish missing elapsed time
for this observed segment. They do not establish complete instruction
provenance or a complete timing contract for F141.

## Frame-wait boundary

The prior frame-wait release is nearly aligned: the write of `1` to countdown
byte `$0502` occurs at native 13.196168700 seconds and reference
13.196167157142857136 seconds.

Later, the main-loop wait reads `$0502` at native 13.232307100 seconds and
reference 13.252096157142857136 seconds, a difference of about **19.789057 ms**.
Native reads `0xff`; the reference reads `0xfe` after an additional IRQ5
decrement. Both values satisfy the original negative-countdown condition.
This observation does not justify changing the wait condition or inserting an
artificial delay.

The 876.4-microsecond F141 segment does not account for that entire difference.
The remaining initialization work, scheduling and interrupt boundaries require
authoritative timing evidence. The reference still uses the documented modified
loader; these observations are not a complete common-machine-state comparison.

## Implementation gate and acceptance

A fresh project MCP work packet for F141 is valid but reports
`implementation_ready: false`: seven instructions remain unobserved and four
control outcomes are missing. This investigation therefore records the defect
and its evidence without changing the native function. The next integral work
is to complete the affected path/timing contract and account for the intervening
work before implementing a correction.

The retained evidence contains 24,066 native observer events and 15,725
reference memory events, with a verified reference completion footer. Native
function-entry events are a separate event kind and are not compared as original
memory accesses. Native context PCs and original instruction PCs are kept
distinct; frame counters are not used to fit an alignment.

Private evidence is retained in `analysis/acceptance-input-cadence-current.json`
and linked from the canonical acceptance report and current F118/F141 packets.
Full graphics, audio, timing, fixture and scenario acceptance remains open.
