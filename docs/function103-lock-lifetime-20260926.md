# F103 lock lifetime — 2026-09-26

The original boot now has a bounded dual-CPU observation spanning the loading
gate's release. It executes F103 302 times; every invocation reads a set gate
and a clear lock. The missing busy branch remains unobserved.

## Original execution and observer checks

The initial 15-second diagnostic-loader run recorded 13,743 events and preserved
all five baseline outputs byte-for-byte. It produced no CPU-B IRQ3 or F103
invocation. Its immediate asset copies and startup bypass alter the original
loading intervals, so this run cannot answer the busy-branch question.

An observer-only original boot recorded 5,498 events in 20 seconds. CPU-A still
held the loading gate at the end. A 35-second run included the gate release and
recorded 19,639 events, preserving the shorter run's entire event prefix.
Neither original run adds RAM/register writes, loader substitutions or IRQ
stimuli. The retained source and executable identities are bound in the reports.

All 302 observed F103 paths are `8042 -> 8046 -> 8048 -> 804C -> 804E -> 806C`.
Their TAS reads see zero, their writes set bit 7, and their clears release it.
All have three original RTE stack reads. The observer did not capture the next
instruction at every restored PC, so these logs are not complete fixture bundles
and have not been admitted as such.

The gate set at CPU-A `0x803C6` lasts 5.25564 seconds, until `0x803F6` clears it
at emulated CPU-A time 20.9428075 seconds. CPU-A then acknowledges a pending IRQ3
3.5 microseconds later and reads the lock at `0x80F9E` 10.6 microseconds after
the clear. Its 810 observed IRQ3 acquisitions all follow a zero gate in callback
order. This is one observed latency, not a universal minimum.

CPU-B's gate-read-to-TAS-read gap is 2 microseconds in 264 invocations and
2.85714285714 microseconds in 38, consistent with the original temporary clock
scaling. No busy outcome or intervening delay appears in this window.

The capture has 1,616 backwards timestamp steps between CPU callbacks. Preserve
the original callback ordinal when reconstructing memory effects: sorting by
the CPUs' local timestamps would invent a different event order. Snapshots of
the other CPU do not establish simultaneous hardware state.

## Revised reachability question

All three identified normal CPU-A gate intervals raise IPL to 7 before setting
the gate and clear the gate before lowering IPL. Normal trap returns preserve
that interrupt mask; exceptional FDC/error control flow needs separate proof.
The earlier proposal to acquire the CPU-A IRQ3 lock while the gate remains set
does not follow from those normal paths and is superseded.

F103 samples the gate and lock at different times. A remaining hypothesis is a
set gate sampled by CPU-B, followed by gate release and CPU-A lock acquisition
before CPU-B's TAS. The measured intervals do not produce that overlap here.
Source-proven CPU-B delay/preemption and timer phases must be investigated
before another controlled attempt. The branch is not declared unreachable,
and no RAM-injected witness is accepted.

All 14 retained sources listed by F103's current research packet were also
inspected through the bounded project MCP interface: 101 complete records,
zero busy-branch witnesses. Many records overlap; this is not a count of
independent scenarios or newly admitted fixtures.

## Durable evidence

### Interrupt-window follow-up

The reference CPU microcode gives a 20-clock normal interval between the gate
read and lock read. Relative to TST entry, the gate read starts at clock 4,
TST samples interrupts at clock 8 and finishes at clock 12, the not-taken BEQ
samples at clocks 14 and 16 and finishes at clock 20, and TAS reads the lock at
clock 24. BEQ's second sample replaces its first. An accepted higher interrupt
can therefore dispatch before BEQ or before TAS. TAS itself samples interrupts
only after its lock write. These phases assume no retry, fault or preemption;
the retained 302 read intervals match this normal path.

There is no CPU-B IRQ4/5 acknowledgement between either read in those 302
invocations. The nearest preceding acknowledgement is 233.3 microseconds before
a gate read; the nearest following acknowledgement is 7.468142857135389
milliseconds after a TAS read. These are same-CPU measurements of acknowledgement
events, not interrupt assertion times or a universal exclusion proof.

The preliminary source-review claim that ordinary IRQ4/5 paths always preserve
their return PC was too broad. IRQ4 saves 15 registers (60 bytes), then calls
`0x824C` through `0x8090` when `$0820` bit 7 is clear. If `$081E` bit 2 is set,
`0x8254` writes `0x85A6` to `(0x42,SP)`. At this call depth, the four-byte JSR
return plus the register save place that write at offset 2 in the exception
frame: the saved PC. The original bytes at `0x85A6` lower SR to `$2000` and
enter a branch loop at `0x85AA`. This path abandons the interrupted continuation;
it is not evidence of a delay followed by F103's lock read. Its predicates have
not been established during a candidate nested F103 interrupt. Any such capture
must include the actual resumed instruction and the return-frame provenance.

IRQ5's successful lock-acquisition path also narrows the producer question.
It calls `0x1707E`, clears its acquired lock at `0x8130`, then writes the timer
reload, re-enables IRQ3 at `0x813C` and returns at `0x8148`. If that child returns
normally, IRQ5 cannot leave its own acquired lock set for a resumed F103. This
does not prove every computed child target or abnormal exit, nor exclude a lock
held by another owner when IRQ5's TAS takes its busy branch.

The board source gives a 41-microsecond scanline, a 17.384-millisecond frame,
and a 7.995-millisecond timer period **after a mode-1 reload of `$F3D`**. Writing
`$F3D` updates the reload value but restarts using the synchronized current
counter; it does not reset timer phase. The retained observer did not record
timer mode, reload/counter transitions or IRQ assertion events. Constants and
acknowledgement timestamps alone cannot close the timing domain.

`analysis/validate-function103-preemption.py` checks the captured opcode bytes,
source microphase structure, raw event identity, all 302 read windows and nearest
acknowledgements, the exception-frame offset, and unchanged native/build inputs.
Its passed report is `analysis/acceptance-function103-preemption-current.json`.
This is source and observation evidence, not matched native timing validation.
The next unresolved boundary is the original timer/mask/assertion-to-acceptance
sequence, with IRQ4's conditional return diversion accounted for. No repeated
neutral boot or forced interrupt is justified by the negative window search.

`analysis/validate-function103-lock-lifetime.py` validates the raw event hashes,
driver/source identities, preserved prefix, each observed F103 path, TAS and
clear values, stack-read addresses, gate ordering and bounded source search.
The current report is
`analysis/acceptance-function103-lock-lifetime-contract-current.json`.
No native code, audit gate or review package was changed. Graphics, audio,
timing and comprehensive scenario acceptance remain open.

### Original timer producers and actual returns

A further 35-second neutral boot adds the previously missing timer-register,
board-state and CPU interrupt-state observations. It preserves all 19,639
previous lock-observer events byte-for-byte and the original WAV hash. Its
28,960 additional records include 3,346 timer-register writes and 5,671 reads.
The observer performs no guest writes, forced IRQs or loader substitutions.

All 302 F103 invocations now have an observed instruction at the PC obtained
from their original RTE stack reads, with the expected restored SR and SP.
No return is unmatched. This closes the missing continuation observation for
these invocations; it does not turn the limited logs into complete function
fixtures or cover the still-unobserved busy path.

The single observed timer-mode write selects mode 1 at CPU-A `0x800E0`.
At every F103 gate read, the stored timer counter and reload are `$F3D`, CPU-A's
allow mask is `$1C`, CPU-B's is `$18`, and CPU-B's pending timer flag and current
interrupt level are zero. These are snapshots: `m_irq_tval` is the last
synchronized counter, not a continuously updated value. Write taps run before
the mapped handler, while read taps run after it. A mode-write snapshot of zero
therefore does not mean the requested mode-1 write failed.

The same run's protocol stream contains 43,754 IRQ events. All 4,976 protocol
acknowledgements join to the Lua CPU-space acknowledgements by CPU and ordered
IRQ level. The protocol hook marks vector-lookup start; the CPU-space read occurs
later after the reference VPA wait. Their timestamps must not be equated.
Protocol callback ordinals and Lua observer ordinals remain separate;
the join does not invent an arbitrary ordering between the two streams.

The protocol header also exposes an identity defect: the executable has 425
compiled function entries, while the supplied current function metadata has
641. The extractor validates stream geometry, complete compression/footer,
counts and IRQ line-mask continuity, but cannot claim complete protocol semantic
validation. Function-owner fields are not used as authority, and this source is
not admitted to the fixture corpus. A subsequent admissible protocol capture
must identify the executable's actual compiled table correctly.

The durable capture is `analysis/acceptance-function103-producers-current.json`.
`analysis/validate-function103-producers.py` passes its bounded checks and writes
`analysis/acceptance-function103-producer-contract-current.json`. All 792 checked
native/build inputs remain unchanged. No acceptance gate was promoted.

### Timer deadline arithmetic and an immediate-expiry exception

The retained events now support a bounded deadline check for 1,112 reload-data
writes and all 1,769 recorded timer assertions. Each write is associated with
its following enable write and the uniquely matching protocol IRQ-clear event.
The source schedules the next expiry from the write time plus
`(4096 - synchronized counter) * 41 microseconds`; subsequent mode-1 reloads
use 7.995 milliseconds. Writing even an unchanged reload value reschedules the
timer from the write's clock phase. It does not reset the counter to that value.

An initial assumption that the following enable snapshot always preserves the
counter synchronized by the data write failed once. At CPU-B `0x8134`, time
16.655695999997999998 seconds, the pre-write stored counter is 4092. The last
sprite boundary is 16.655512 seconds; the source's floor calculation adds four
ticks, reaching 4096. The timer restarts with zero delay and asserts CPU-A IRQ3
at the write timestamp, 2.6 microseconds before its previously scheduled expiry.
Its callback reloads 3901 before CPU-B's enable write two microseconds later.
That later snapshot therefore witnesses the callback's reload, not the counter
just synchronized by the data write.

Accounting for this source-backed exception explains all 1,769 recorded
assertions with zero residual and no fitted time offset. The 302 CPU-B timer
assertions occur once per observed frame, at scanlines 5 through 201. These are
properties of this capture, not a universal exclusion of later IRQ4/5 preemption.

`analysis/validate-function103-timer-deadlines.py` passes and records the measured
counter witnesses, source arithmetic, falsified assumption and each comparison
in `analysis/acceptance-function103-timer-deadlines-current.json`. The comparison
does not independently reconstruct every counter update or individually capture
masked timer callbacks. It does not establish native timing parity, close the
protocol identity mismatch, admit new fixtures or prove the busy branch
unreachable. No native change follows from this result; the existing device
source already handles a synchronized counter reaching 4096.

Automatic approval review rejected removal of the verified temporary root
`.tmp/acceptance-function103-producers-20260926` as "blocked by policy".
The completed run's data and source snapshots are retained in the report;
the temporary directory remains and its deletion will not be retried.

### Timer acceptance and sound-handler masking

The retained producer capture contains 302 complete CPU-B timer interrupt
assertion, vector-lookup and clear cycles. Same-CPU ordered acknowledgement
joins associate each cycle with its CPU-space bus acknowledgement and F103 gate
read. The recorded assertion-to-vector-lookup interval is 1.4 to 3.572
microseconds; assertion-to-gate-read is 9.4 to 15.572 microseconds. The next
higher interrupt assertion is at least 7.4667 milliseconds after that gate
read. These are observed ranges in this boot, not worst-case bounds or matched
native timings. Vector lookup and the later bus acknowledgement remain distinct.

All 1,112 observed IRQ5 sound-handler gate checks have CPU-B allow mask `$18`,
timer pending flag 1 and IPL5. A pending flag does not imply an asserted timer
interrupt: the original callback sets the pending flag even when the allow bit
is off. The 810 zero-gate checks perform no observed lock or timer writes before
the next CPU-B interrupt acknowledgement. The 302 successful gate-high paths
acquire and release the lock, then write the reload and `$1C` enable mask.
Every sampled operation before the enable handler still has allow mask `$18`.
Writing that mask clears both timer pending and the timer line; it does not
deliver an accumulated timer interrupt. These snapshots do not exclude an
unobserved child write between samples.

For the successful paths, sound interrupt bus acknowledgement to lock clear
takes 179.7 to 261.286 microseconds; lock ownership lasts 131.6 to 188
microseconds. The endpoint is the observed lock clear, **not** the sound
handler's RTE. Thus the observed sound duration alone does not explain a long
pending IRQ3 acceptance delay: the timer is disabled at the sampled sound
operations and re-enabled after lock release. A different input or lifecycle
sequence still needs its own evidence.

The live F315 packet now covers a fixture-observed transitive closure of 25
functions. Its own 27-instruction path contract is complete, but children F477,
F497 and F498 retain respectively 17, 9 and 82 missing control outcomes. F498
also retains two unresolved control targets. This blocks a complete sound-path
duration or abnormal-return proof. Static inspection additionally identifies
the startup IPL0 write at state58 `0x84D0`; ordinary observed F103 entries run
at IPL3. IRQ4's conditional saved-PC rewrite and the sound TRAP5 path remain
separate unresolved conditions.

`analysis/validate-function103-interrupt-latency.py` passes its bounded checks
and records the per-invocation witnesses in
`analysis/acceptance-function103-interrupt-latency-current.json`. An independent
read-only review found no integral correction. All 792 bound native/build
inputs remain unchanged. No new run, native edit, fixture admission or acceptance
gate promotion follows from this result. The next source question is the
producer and return domain of the incomplete sound children, including their
stream loops and computed dispatches, rather than another unchanged boot.

### F498 dispatch domain and ordinary TRAP return

The original state72 sequence at `0x174BA` is `ANDI.W #31,D0`, two
`ADD.W D0,D0` instructions, then the indexed jump at `0x174C2`. For every
incoming word it produces exactly the offsets 0, 4, ..., 124. All 32 resulting
four-byte slots at `0x174C6` are original `BRA.W` instructions already owned by
F498; they lead to 24 distinct branch destinations. The new hash-bound primary
selector proof is consumed by the computed-control contract. No producer-domain
restriction or newly invented instruction is needed for this arithmetic result.

The audit now resolves that primary target set, while requiring evidence for
31 still-unobserved slots. F498's unresolved computed sites decrease from two
to one; its explicitly missing control outcomes increase from 82 to 113. This
is a more complete statement of the required coverage, not newly passing
behavior. F498, F315 and the broader timing gate remain blocked. No fixture or
native implementation changed.

The secondary jump at `0x17644` masks a stream byte to four bits and similarly
produces 16 arithmetic targets at `0x17648 + 4*selector`. Only the first 12 are
branch slots. The final four enter overlapping instruction bytes:

| Selector | Target | Source behavior with ordinary accesses |
| --- | --- | --- |
| 12 | `0x17678` | Clears the word at `A6+$32`, then branches to `0x176E2`. |
| 13 | `0x1767C` | Executes `ORI.B #0,$62(A2,D0.W)`, then continues at `0x17682`. The derived D0.W is 52, so the access is at A2+150. It still reads/writes memory and changes flags. |
| 14 | `0x17680` | Executes `ORI.W #$026E,-(A2)`, then reaches opcode `$07FF` at `0x17684`. |
| 15 | `0x17684` | Enters opcode `$07FF` directly. |

The pinned CPU decoder has no legal match for `$07FF`, so that word enters
the illegal-instruction path if fetched in the retained state72 opcode view.
This does not prove either selector occurs naturally or establish the actual
exception/return sequence. The secondary computed contract remains unresolved;
neither the memory effects nor the exceptional targets may be treated as unused
without evidence.

For the separate `TRAP #5` at `0x177CC`, the retained data snapshots have vector
`$94 = $400`, and the state72 opcode at `$400` is `RTE`. The original CPU source
preserves IPL through software TRAP; the ordinary RTE restores the saved SR and
continuation `0x177CE`. When entered from IRQ5 at IPL5, this path therefore does
not make IRQ3 acceptable. IRQ5's earlier special compare exits FD1094 IRQ mode;
the software trap performs no interrupt acknowledgement and does not select
state04. Live vector identity, intact frames, actual continuation and exceptional
accesses still need dynamic evidence before claiming the complete trap path.

The admission result is
`analysis/acceptance-function498-primary-domain-current.json`; the repeatable
source check is `analysis/validate-function498-sound-contract.py`, with passed
results in `analysis/acceptance-function498-sound-contract-current.json`.
Independent read-only review confirmed the primary domain and secondary opcode
classification. Current packets 1, 99, 103, 315, 324 and 498 were refreshed;
the retained interrupt-latency check passed again against the updated sound
audit. All 792 native/build inputs remain unchanged. No build or capture was
created. Graphics, audio, complete timing and comprehensive scenario acceptance
remain open.



Automatic approval review rejected the verified-path cleanup command as
"blocked by policy". The three temporary directories remain under `.tmp/`:
`acceptance-function103-lock-lifetime-20260926`,
`acceptance-function103-original-lock-lifetime-20260926`, and
`acceptance-function103-original-gate-release-20260926`. Their durable event
data, script snapshots and identities are retained in the analysis reports.

### Whole-bank secondary-selector frontier

An exhaustive byte-pair scan of the original 32,768-byte sound load finds 134
adjacent `E4`/operand pairs. None selects secondary value 12, 14 or 15. The sole
pair selecting 13 is at offsets `$00A9/$00AA`: `E4 1D`. It crosses the runtime
directory's `$56` entry (`$1BE4` at `$00A8`) and `$58` entry (`$1D16` at `$00AA`).
The final bank byte is `$FF`, so there is no final-byte `E4` edge omitted by the
pair scan.

The source proves why adjacency matters on the ordinary parser path: each of
F477's four command callsites reads `(A4)+`, requires a byte at least `$E0`,
then calls F498. Only `$E4` selects its primary slot 4. That slot goes to
`0x1763A`, which reads the next `(A4)+` byte without an ordinary intervening A4
change. Thus selectors 12/14/15 are excluded **conditional on** both reads
using this unchanged bank and no intervening state change. Selector 13 would
require the directory-overlap location to be fetched as a command. This is a
byte-content result, not a global reachability proof.

The existing expanded stream graph has current direct input hashes, 177 roots
and 26,796 abstract states; it never visits `$00A9` as a command. Its explicit
limits remain: malformed initialization definitions, inactive descriptors and
queue/bank aliases are not closed. The older selected-root report has a stale
path-reachability input hash and is retained as historical evidence. Neither
report is promoted into an exhaustive runtime-domain proof.

The pointer and storage conditions are concrete. IRQ3/5 set A5 to `$FB0000`;
F477 reconstructs A4 from the relative long at `A3+4`. F545 derives that long
from the original descriptor, but its destination selector mask permits all
32 indices. In the retained table, entries 24–31 do not map to the 18 tick
channels. Moreover, `$FB0000` belongs to writable shared RAM. Descriptor bounds,
pointer lifetime and CPU/alias writes must therefore be established before the
conditional exclusion can clear a path gate. One existing F498 fixture is
consistent with an ordinary `$F1` command at bank offset `$0AE8`; it does not
cover secondary dispatch or prove the bank's lifetime.

`analysis/validate-function498-bank-frontier.py` passes and records all 134
byte pairs, exact caller/constructor bytes, graph limits and the bounded
fixture observation in `analysis/acceptance-function498-bank-frontier-current.json`.
Independent review found no integral correction. No fixture or native gate was
promoted.

### Reference executable's compiled protocol-table identity

The retained executable's PE/COFF symbols identify the actual
`gground_protocol_capture::FUNCTIONS` and `FUNCTION_RANGES` arrays. Their named
locations and auxiliary section lengths contain exactly 425 forty-byte function
definitions and 536 eight-byte ranges. Every scalar field, all 425 resolved label
pointers and every range match the retained generated includes byte-for-byte.
The executable and include hashes also match the original producer-capture
report. This establishes the compiled **table** identity; it does not establish
equivalence of all executable code to the present source.

The extracted table is
`analysis/reference-compiled-function-table-current.json`. The existing protocol
function-index validator accepts its IDs and nonoverlapping ranges, and all
range endpoints resolve to their recorded owners. Of the shared IDs in today's
641-entry catalog, 319 labels and 29 actual range sets differ from the compiled
table. There are also 216 current IDs absent from this executable, including
F498. Using the first 425 current entries would therefore still be incorrect.

`analysis/validate-reference-compiled-table.py` passes and writes
`analysis/acceptance-reference-compiled-table-current.json`; independent review
confirmed the scope. The old capture header still names the supplied 641-entry
metadata and is not rewritten or retroactively admitted. A subsequent protocol
capture needs matching contracts and executable-bound metadata plus complete
semantic validation. A function-fixture campaign for absent owners requires an
appropriate capture build. No new build or capture was created for this identity
check. Full graphics, audio, timing and scenario acceptance remain open.

### Corpus fixture executable and metadata drift

The separate retained `mame-fixtures/gground_corpus_fixture_research.exe`
does contain F498. Its executable hash, byte count, fourteen recorded research
sources and current contract/table hashes match the September 10 build manifest.
Named PE/COFF symbols establish all 641 function definitions and label pointers,
748 body ranges, 37 proven body extensions and four terminal outcomes. All match
the generated source byte-for-byte. This proves the compiled arrays, not complete
source-to-machine-code equivalence. CPU-B/state-72 `$400` has no fixture owner,
including in the extension array; a TRAP path still lacks that child identity.

The current capture preflight nevertheless fails. Running the generator in memory
leaves both C++ includes unchanged, but changes the computed-control source hash
inside the contract and four table fields: computed-control hash, projection hash,
site count and derived contract hash. Removing only the newly proved F498 `$174C2`
domain reproduces the prior projection hash exactly (40 sites versus 41). This is
outside the existing generator-hash-only compatibility rule, whose historical
proof also names 436 functions. A new capture therefore requires coordinated
generated metadata and supported build provenance; matching array bytes alone
cannot authorize rewriting the old manifest or old capture headers.

`analysis/validate-corpus-capture-identity.py` passes and records these exact
differences in `analysis/acceptance-corpus-capture-identity-current.json`.
Independent review found no integral correction. Canonical metadata, historical
build identity and executable were preserved; no new build or capture was made.

### Retained controlled F498 candidates

Before creating another capture, the bounded source reader inspected the four
F498 records already present in `remaining-evidence-f153-transitions-r1`.
Primary selectors are 17, 21, 17 and 13. Thus selectors 13 and 21 offer two
additional target candidates relative to the admitted primary path inventory;
none observes secondary `$17644` dispatch or TRAP `$177CC`.

The four complete flags must not be described as four observed outer returns.
Records 5130, 5142 and 5160 end in tail transfers. Record 5161 executes seven
instructions and returns at `$177AA` to `$17348`; its captured stack words and
four-byte SP increment match that return. Every owned opcode edge matches the
state-72 opcode image. An initial validator assertion used incorrect immediate
bytes for the final bit-set instruction; inspection corrected it to the actual
`08EB 0004 0001`, followed by `4E75`, before the check passed.

The retained source hash and validation identity match, but the capture explicitly
declares `nativeReplayEligible:false`, and F498 is absent from its route-only
admission owners. These are controlled research candidates, not newly admitted
fixtures or natural reachability evidence. Intervention boundaries, historical
identity and tail continuations require validation before admission.
`analysis/validate-function498-retained-candidates.py` records this distinction in
`analysis/acceptance-function498-retained-candidates-current.json`. No native
behavior or acceptance gate changed. Independent review found no integral
correction to the controlled-path, tail-transfer or RTS classifications.

### Controlled F498 paths admitted to the path audit

The subsequent admission review distinguishes **path evidence** from **native
replay eligibility**. MCP's supplemental path reader accepts validated controlled
execution records; the native replay flags and route-only owner list govern the
separate native admission. The previous candidate review correctly withheld native
admission but did not yet establish this narrower permissible path use.

The complete retained stream passed the current `FixtureValidator` structural and
semantic checks: 5,206 fixture records, comprising 5,173 complete and 33 incomplete
records, twelve debugger-action records and one valid footer. The header retained
its historical identity. Current function descriptors were validated and matched
through the existing hash-bound `next15-contract-registration-proof.json`;
this check did not claim that the old executable equals today's executable or
that the stale computed-domain metadata now passes generator preflight.

`analysis/validate-function498-controlled-paths.py` independently binds the source,
descriptor compatibility, script and manifest, then decodes the twelve recorded
actions from the stream. Six actions precede every selected F498 invocation, and
six follow its enclosing caller's return. None occurs within any selected F477
caller interval. All four F498 records have a unique enclosing completed F477
invocation with matching A3/stack nesting and a verified RTS stack return.

Two tail transfers additionally join a retained F544 or F320 child by exact
instruction boundary and the complete register image, with captured return to
`$17348`. Record 5160 has only the enclosing completed caller and tail-return edge;
there is no separate retained matching child record, and no such record is
invented. Record 5161 has its own captured RTS. All owner-local opcode edges match
the state-72 original image. Independent review found no integral correction.

F498 was added to the existing **controlled-path-only** supplemental source entry.
The live MCP packet now contains one aggregate and four supplemental records.
Observed instructions increased from 14 to 27 of 230; missing control outcomes
decreased from 113 to 105. Primary dispatch observes selectors 13, 17 and 21,
leaving 29 of 32 primary targets unobserved. The secondary dispatch remains
unresolved. These are primary selectors; this does not establish secondary
selector 13's reachability.

No native fixtures were admitted, no natural reachability was asserted, and no
native function or timing behavior changed. The affected sound, bank and latency
validators passed against refreshed packets; all 792 recorded native/build source
identities remain unchanged. The sound closure still has incomplete contracts in
F477, F497 and F498. Full graphics, audio, timing and scenario acceptance remain
open.

### Stage-music F498 path evidence

The retained `next15-stage-music-r1` source passed the current full stream
validator: 6,824 fixture records (6,789 complete and 35 incomplete), four recorded
debugger actions and a valid footer. Its ten complete F498 records share the
existing hash-bound descriptor compatibility used above. All four route mutations
occurred at CPU-B/state-72 `$D740`, instruction 25,935,458, before the selected
sound invocations. None occurs inside their enclosing execution intervals.

The first attempted caller-return check disproved the assumption that every F477
caller here ends in RTS. Seven do; three instead tail-transfer to F544. The
validator now records those three enclosing F315 RTS invocations separately,
including stack-return evidence and F544's `$17324 -> $17106` return edge. It does
not reclassify a tail transfer as an RTS. Five F498 records themselves end in RTS;
five end in tail transfers. Three of the latter have separately retained child
records matching the exact instruction boundary and full register state. The
other two retain the enclosing-return evidence limitation.

The alternate F477 callsite `$173CE` in record 5471 returns to `$173D2`, rather
than the earlier `$17344 -> $17348` route. Original BSR bytes and displacement,
captured child state and the F544 return establish that distinction. The updated
validator checks each observed callsite instead of assuming one continuation.

Secondary dispatch `$17644 -> $17648` appears twice, in records 5183 and 5471.
An initial review incorrectly reported missing operand prestates; direct
region-relative, aligned-word inspection disproved that assertion. At A4
`$FB049C`, region-3 word offset `$3049C` has initial value `$0000` and read mask
`$FF00`. At A4 `$FB1F61`, offset `$31F60` has initial value `$E400` and read mask
`$00FF`. Both therefore capture an operand byte of `$00`, with one read, no write
and first memory sequence zero. The original bank also has `E4 00` at the matching
command/operand offsets. These are two captured selector-zero invocations, not
proof of globally immutable shared RAM or a complete secondary input domain.

`analysis/validate-function498-stage-music-paths.py` passes and records all joins,
prestates and limits in `analysis/acceptance-function498-stage-music-paths-current.json`.
F498 was added to that source's existing controlled-path supplemental entry.
The current audit now has one aggregate and fourteen supplemental records,
66 observed instructions out of 230, and 87 missing control outcomes. Primary
selectors 3, 4, 7, 10, 13, 17, 20 and 21 are observed; 24 primary targets remain
unobserved. Secondary dispatch remains unresolved despite its observed zero case.

The other two retained sources identified by the summary census,
`next15-transition-branches-r2` and `function498-command14-first-decode-r1`, add no
new owner-local instruction PCs or control outcomes relative to this current
packet. They were not added merely to increase the evidence count.

The affected sound-contract, bank, controlled-path, retained-candidate and latency
checks passed after refreshing the seven selected packets. The observed sound
call closure expands from 25 to 27 functions; F477, F497 and F498 remain incomplete.
No native replay admission, native behavior change, new build or new capture
occurred. Full graphics, audio, timing and scenario acceptance remain open.

## F498 secondary candidate inventory

The current 230-unit F498 packet does not enumerate every locally mask-permitted
secondary entry. A traversal of the pinned CPU-B state72 image, starting at
`$174BA`, enumerates all 32 primary and 16 secondary arithmetic targets and both
conditional outcomes. Call and TRAP continuations are explicitly conditional on
return; external branches, return instructions and the illegal opcode terminate
local traversal. This is a conservative source-image inventory, not proof of
runtime reachability, exception return or complete function ownership.

It contains all 230 current entries and these 15 additional starts:

| Category | Original instruction starts |
| --- | --- |
| Five secondary BRA.W slots | `$1765C`, `$17660`, `$1766C`, `$17670`, `$17674` |
| Seven handler instructions | `$176AA`, `$176B0`, `$176B4`, `$176BA`, `$176D2`, `$176D8`, `$176DC` |
| Two overlapping ordinary entries | `$1767C` ORI.B and `$17680` ORI.W |
| Overlapping illegal entry | `$17684`, original opcode `$07FF` |

The 245 candidate starts cover exactly 866 unique bytes, `$174BA..$1781B`;
their instruction lengths sum to 878 because the alternative entries overlap.
The old body partition contains 810 unique bytes, omitting 56 bytes occupied by
the five branch slots and seven handler instructions. A normal linear decode of
the whole range yields 242 instructions; it misses the three alternative starts.
No candidate byte collides with another owner in the current state72 catalogue.
The ten external branch sites target `$17324` or `$18010`, both catalogued;
ordinary calls are separate from this branch inventory.

`analysis/validate-function498-secondary-inventory.py` independently checks the
arithmetic, original bytes, complete candidate traversal, current-unit inclusion,
overlaps, catalogue collisions and pinned 68000 decoder's illegal `$07FF` result.
It passes and writes
`analysis/acceptance-function498-secondary-inventory-current.json`. A fresh MCP
packet exactly matches current core hash
`61f21e4943059f43e8c3234219b13ef8df6f21d7de92b52222d3054f1515e1fd`.
The bounded independent review agrees with the 245/866/878 counts and the limits.

The representation gap must be resolved before admitting this as canonical
ownership. F498 takes MCP's proven-static decoder, whose current 230-instruction
count matches the descriptor; its full-domain fallback is therefore inactive.
That decoder consumes invalidated supplemental spans, but does not consume
active `supplementalInstructionSpans`. The generic decoder's span support does
not apply here. The canonical proven-static generator also requires disjoint,
decodable body extensions, which cannot by themselves represent these overlapping
starts and the illegal entry. A corrected inventory needs explicit exceptional
entry classification and must retain historical fixture descriptor provenance.
Resolving target arithmetic alone cannot certify ownership, observed paths or
exception return.

The sound-contract report's obsolete statement that no secondary execution was
captured is corrected: two controlled selector-zero invocations are established
by the stage-music evidence. Other selectors and the full natural producer domain
remain unproved. Both updated validators pass, all 792 native/build input hashes
remain unchanged, and no native gate or replay admission is promoted. No build,
temporary capture or native behavior change was made.

## F498 static inventory admission

The ordinary representation gap is now repaired through the existing body
extension mechanism. The source-bound proof
`repro/function498-secondary-static-body-proof.json` and its validator
`scripts/gain_ground_function498_static_contract.py` retain the historical F498
descriptor (230 instructions, 810 bytes) and prove four disjoint extensions:
`$1765C..$17663`, `$1766C..$17677`, `$176AA..$176BD`, and `$176D2..$176E1`.
These add the twelve ordinary instructions and 56 bytes identified above.
The canonical inventory, partition TSV and MAME extension include were
regenerated. Existing function IDs, base contract and base function table are
unchanged; the extension count rises from 37 to 41.

The three alternative starts are represented separately as potential entries.
MCP consumes all three and renders them as `unresolved-potential-entry`, including
the two that Capstone can decode. Both the per-function path audit and batch
readiness calculation keep these entries blocked even if an isolated instruction
observation were supplied. The original illegal opcode at `$17684` and TRAP at
`$177CC` are now explicit unresolved exception controls. No exception vector
target, exception return, natural producer domain or native parity is inferred.
Secondary `$17644` remains an unresolved computed control.

The independent review identified a missing-field failure case: deleting both
new inventory arrays could silently omit these blockers. F498 now requires
exactly three potential entries and two exception sites, and fails closed if its
inventory is absent or incomplete. A regression verifies this case. Further
checks prove all 245 candidate entries are consumed, a decodable potential entry
is not silently admitted, and an observed potential entry still blocks batch
readiness. Seven focused MCP checks pass, including existing sequence/rendering,
sequential-successor and asynchronous-IRQ cases. The generated inventory's
`--check` passes. The secondary-inventory, controlled-path, stage-music,
sound-contract, bank-frontier, retained-candidate, capture-identity and interrupt
latency validators pass against the refreshed packets.

F498 now has 66 observed entries out of 245 represented entries, with 95 missing
control outcomes and three unresolved control sites. The increase from 87 missing
outcomes reflects eight newly represented branches; it is not eight new gameplay
failures. The three unresolved potential entries are additional explicit blockers.
The 27-function observed sound closure still has incomplete contracts for F477,
F497 and F498. No native translation gate is promoted by this admission.

The retained capture executable still contains 37 extension ranges. Its old
extension include is reconstructed exactly by removing only the four new F498
rows, matching the historical build-manifest hash and all 37 compiled rows.
Thirteen of fourteen current research source files match that build; the new
41-row extension include does not. The capture-identity report records this
difference explicitly and keeps capture preflight blocked. A future capture
needs the updated extension source and corresponding executable provenance;
historical headers and build identity were not rewritten.

All 792 native/build input hashes remain unchanged. No build, new capture,
native replay admission or temporary build artifact was created. The receipt is
`analysis/acceptance-function498-static-admission-current.json`. Live graphics
provenance, matched audio samples, native timing comparisons and comprehensive
scenario/fixture acceptance remain open.

## F498 secondary target arithmetic admission

The original `$1763A..$17647` sequence is `MOVE.B (A4)+,D0`, `ANDI.W #15,D0`,
two `ADD.W D0,D0` operations, then the indexed jump. For ordinary execution
through those instructions, the mask and additions produce exactly the offsets
0, 4, ..., 60; the PC-relative base is `$17648`. All sixteen resulting addresses
are now admitted as the secondary jump's static target set. This establishes
the calculation, not the natural input domain or execution/return of each target.

The proof is
`config/gground-function498-secondary-target-arithmetic-v1.json`. MCP validates
its specific schema, fixed owner/site, original opcode sequence, complete offset
and target lists, current static-inventory identities, and each target's bytes
and classification. It rejects a missing target, a potential entry relabelled
as ordinary code, a claimed exception return, a different mask or stale inventory.
The three alternative entries remain unresolved; illegal `$17684` and TRAP
`$177CC` remain unresolved exception controls. No native readiness gate opens.

The review confirmed the arithmetic and required semantic validation rather
than a file-hash check alone. A later suggestion that deleting `inputContract`
would bypass validation was tested against the actual loader and disproved:
the existing final evidence check raises `Computed-control selector evidence is
missing for (498, 95812)`. No redundant workaround was added for that hypothesis.

Twelve focused MCP checks pass. The generated static inventory's `--check`,
secondary-arithmetic, secondary-inventory, stage-music, controlled-path,
sound-contract, bank-frontier, retained-candidate, capture-identity and interrupt
latency checks pass against the refreshed packets. The arithmetic validator
compares before/after control sites and proves that only `$17644` changed:
all other control sites, fixture counts, observed instruction PCs, alternative
entries and exception blockers remain unchanged. F498 still has 66 observed
entries out of 245, from one aggregate and fourteen controlled path records.

Secondary selector zero remains the only observed secondary target; the other
fifteen are now counted explicitly as missing outcomes. Total missing outcomes
rise from 95 to 110, while unresolved control sites fall from three to the two
exception transfers. These are corrections to coverage accounting, not new
gameplay failures or additional captured fixtures.

The live computed-control registry now contains 42 sites. Historical base
contract/table metadata still pins 40, and the retained capture executable still
contains 37 extension ranges rather than the current 41. The updated capture
identity check proves the unchanged historical tables and keeps preflight
blocked; no old headers, executable or build manifest were rewritten.
The current admission report is
`analysis/acceptance-function498-secondary-arithmetic-current.json`; the prior
static-admission receipt remains historical evidence of that earlier step.
All 792 native/build input hashes remain unchanged. No new build, capture or
native replay admission occurred. Full acceptance remains open.

## Capture metadata migration for the F498 domains

The circular metadata dependency is removed. The secondary arithmetic proof now
binds a canonical projection of the exact F498 extension rows, potential entries,
exception sites, count invariants and static-proof source identities. The full
static inventory still independently verifies every source-document hash. This
allows the canonical table to change and the inventory to rebind that table
without changing the arithmetic proof again. A negative regression verifies
that changes to any of the three F498 entry categories change the projection.

The canonical contract/table now match generator output for all 42 computed
sites. Every field of all 641 table descriptors and all 641 contract descriptors
compares equal before and after migration. The 748 base ranges and both base
capture includes are unchanged. Only four table metadata fields and the
contract's computed-control source hash change. The static inventory was then
regenerated; both generators' current-output checks pass, proving this metadata
dependency has stabilized.

Exact old contract/table bytes are retained under
`repro/corpus-metadata-before-function498-20260926/`. The migration proof is
`analysis/function-work/function498-corpus-metadata-migration.json`, verified by
`scripts/gain_ground_corpus_metadata_migration.py`. Existing historical proofs
are unchanged. The two selected controlled-path validators explicitly bridge
their historical proof endpoints through this new proof to the current metadata.

All 345 catalogued source identities still match exactly one compatibility
identity, including the legacy identity. Only `final7-audio-expanded-cases-r1`
moves from the previous current identity to an explicit historical compatibility.
Its actual source validator passes with the original F477-only selection and
controlled admission; this adds no native replay admission. Identity
classification of the other 344 sources is not a new full record-validation run.
The aggregate bundle retains its old identity and was not rebuilt or replayed.

Thirteen focused MCP tests pass (70 deselected). The migration, capture-identity,
secondary arithmetic, static entry inventory, controlled paths, stage-music
paths, sound contract, bank frontier, retained candidates and interrupt-latency
validators pass. Seven selected packets were refreshed, and all seven core hashes
are identical to their pre-migration values. Independent read-only review found
no concrete defect in the projection or historical compatibility bridge.

The remaining capture blocker is now the executable/build identity: the retained
executable has 37 extension ranges; current source has 41, and its historical
manifest still pins the old metadata. The build and capture wrappers currently
use fixed executable/manifest paths. A current capture must use a matching build
while preserving those historical artifacts. MAME exposes `SEPARATE_BIN` and
`BUILDDIR`, but their use by these wrappers has not yet been implemented or
validated. No build or new capture was started in this migration.

The current receipt is `analysis/acceptance-corpus-metadata-migration-current.json`.
All 792 native/build input hashes remain unchanged. F498 still has 110 missing
control outcomes, three unresolved potential entries and two unresolved exception
transfers. This removes a tooling prerequisite for new evidence; live graphics,
matched audio, timing and comprehensive scenario/fixture acceptance remain open.


## Current isolated capture build and ordinary secondary experiment

The earlier executable-identity blocker is resolved. The build wrapper now
accepts an isolated build tag and uses MAME SEPARATE_BIN; the capture wrapper
accepts an explicit build manifest and a validation-only preflight. Defaults
retain the historical paths. Invalid tags and the historical manifest with
current metadata are rejected. The isolated current manifest passes all identity
checks and ROM verification.

The first parallel build failed; its serial continuation exposed an empty bimg
archive. Its 26 existing object files were added to that archive in place, and
the wrapper then linked successfully without source or compiler-flag changes.
Automatic approval review rejected deleting the empty archive as "blocked by
policy". No alternate deletion was attempted; the isolated build remains in use
by the bounded capture batch. The verification receipt is
`analysis/acceptance-corpus-isolated-build-current.json` and includes the build
manifest, recovery details and cleanup limitation. The compiled PE tables match
641 descriptors/labels, 748 base ranges, 41 extensions and four terminal outcomes.
The historical executable and manifest hashes remain unchanged.

The selector-1 capture finishes with 26 complete records and no incomplete
records. The discriminating F498 record 29 follows 17644 -> 1764C -> 17682 ->
17688 -> 176E2; parent F477 record 30 and outer F315 record 31 retain the actual
return to 8064. Five debugger writes are recorded: four retained stage setup
writes and one operand-byte write before F498 entry. None occurs during that
F498 invocation. The recorder stores the latter address as shared-region offset
3049C, corresponding to bus address FB049C; a validator initially compared it
against the bus address and was corrected against the recorder source.

`analysis/validate-function498-secondary-capture.py` passes this capture, including
original opcode identity, captured operand prestate, path edges, parent relation,
stack and actual enclosing return. Independent read-only review found no blocker
for F498-only controlled-path admission. There is no separately retained F544
tail-child record, no native replay admission and no natural producer-domain
proof. The same pre-entry method is running for ordinary selectors 2 through 11;
each must independently pass before batch admission. No native behavior changed.


## Ordinary secondary batch admitted

All eleven selector captures completed successfully. Each has 26 complete and
zero incomplete records. Each discriminating F498 invocation contains exactly
one secondary-dispatch edge, the declared operand byte in its entry prestate,
no debugger write during the child, opcode-matched handler edges and the actual
enclosing return to 8064. The stricter single-dispatch assertion was added after
review, and all earlier results were revalidated against that validator.

The batch selects exactly eleven F498 records and excludes 66 repeated F498
records. F477/F315 records remain unselected because the operand was changed
inside those invocations. The admission script restores the exact prior catalog
and packet bytes if its subsequent audit fails. Admission and all seven selected
packet refreshes passed. F498 has one aggregate plus 25 supplemental records,
96 observed instruction entries (previously 66), and 79 missing control outcomes
(previously 110). Secondary targets 0 through 11 are observed; targets 12 through
15 remain missing. The three unresolved potential entries and both unresolved
exception transfers remain blockers. F315's closure reflects the additional
F498 coverage; readiness remains false. F324's previously true gate is unchanged.
All 792 native/build inputs remain unchanged, and no native replay is admitted.

The current batch receipt is
`analysis/acceptance-function498-secondary-batch-current.json`. Earlier
arithmetic-only, controlled-path and stage-music receipts retain their historical
checkpoint meaning; their cumulative-count assertions are not current coverage.
The complete MCP suite is running; failures have been observed and are not being
reported as a pass. Graphics, matched audio, timing and comprehensive scenario
acceptance remain open.

Both capture sessions are terminal and the capture executable is no longer
running. Temporary build cleanup remains blocked by the earlier automatic
approval rejection. No alternative deletion or removal of its parent directory
was attempted. Durable capture and admission evidence is retained separately.


## Full MCP validation exposed older audit drift

The first complete MCP run returned 87 passes and ten failures. Five tests
still expected 639 functions rather than the authoritative 641. Four path tests
had stale closure expectations: F179/F201/F353 retain unresolved producer-domain
contracts despite observing their local paths, while F249's previously missing
outcomes are now covered. A differential check removing exactly the eleven new
source rows gives identical results for these four audits and the missing-body
error. The reconstructed catalog establishes a semantic comparison only; the
original catalog serialization was not recovered and is not asserted identical.

The missing-body audit initially rejected the current generated overlay. Direct
inspection of its runtime sets found only 24 expected ranges against 41 actual
ranges. The first review incorrectly attributed this only to the four new F498
ranges; adding those exposed thirteen older omissions: eleven F178 ranges and
two F421 ranges. The audit now consumes the seven retained proof sources for
those thirteen ranges, compares their complete instruction tuples with the
hash/opcode/owner-verified extension reader, and consumes F498's four static
proof ranges. The final exact owner/range union comparison remains mandatory.
The audit returns 41 verified extensions and no implementation authorization.

Tests now require the authoritative 641-function count and explicitly preserve
unresolved selectors at F179:F40C, F201:FF1A and F353:1DC6A. The formerly failing
missing-body test passes individually, and static-inventory generation checks
all 641 functions and 41 ranges successfully. An intermediate full run, which
had loaded the audit before the thirteen-range correction, returned 96 passes
and that one failure. A final complete suite run against the finished sources
is in progress. No native/gameplay source was changed by these audit repairs.


## Final batch validation

The complete MCP suite against the finished audit/test sources passes **97/97**
in 295.84 seconds. The static-inventory generator check also passes for all 641
functions and 41 extensions. No native behavior changed; the eleven admitted
records remain controlled path evidence and the F498 readiness gate remains
false. The complete acceptance goal remains open.

Automatic approval review separately rejected guarded removal of the temporary
`analysis/function-work/function498-secondary-batch-mcp-before.txt` output log as
"blocked by policy". The failed-run conclusions are retained in a structured
manifest and this document. The log remains alongside the already-blocked
isolated build cleanup; neither deletion was retried or bypassed.

## Secondary selectors 12 and 13, 2026-09-27

Two further controlled original paths are admitted for F498 only. Each source
contains 26 complete records and no incomplete records. Only its discriminating
F498 record 29 is selected; six repeated F498 records and the enclosing F477/F315
invocations are excluded from admission. The fifth debugger write injects the
operand before F498 entry, inside its callers. Both selected invocations have
zero debugger writes during F498 and an observed enclosing RTS to `$8064`.

Selector 12 dispatches directly to `$17678`, clears `$32(A6)`, and takes the
ordinary `$1767E -> $176E2` branch. Its first capture timed out without a valid
footer and remains rejected. A diagnostic capture with read-only position
markers completed and passed the source, intervention, opcode and return checks;
the timeout is not evidence about the original branch's behavior.

Selector 13 enters the overlapping bytes `$1767C: 0032 6000 0062`. Original
execution follows the indexed byte OR through `$17682`, `$17688` and `$176E2`.
The captured A2 is `$B5BC`, D0 is `$34`, and the indexed byte address is `$B652`.
The byte is 4 before and after one recorded write; its containing word remains
`$0448`, with write/read masks `$FF00`. The two accesses and the single write
are captured in the fixture, independently corroborated by debugger snapshots.
SR remains `$2300`, as required for byte `4 OR 0`. The console's prefetched PC
is two bytes ahead of the instruction PC; fixture edges establish the actual
instruction addresses.

The live packet now has 97 observed instructions, 77 missing control outcomes,
and 14 of 16 secondary targets observed, using one aggregate and 27 supplemental
records. The three overlapping-entry classifications remain unresolved for
ownership/reachability, and both exception transfers remain unresolved. Neither
source establishes natural producer reachability, native replay eligibility,
timing parity or implementation readiness. All 792 native build input hashes
remain unchanged.

Evidence is bound by `analysis/acceptance-function498-secondary-direct-entry-current.json`
and `analysis/acceptance-function498-secondary-overlap-current.json`. The complete
MCP suite passed 97/97 after selector12 admission (307.64 seconds), then passed
97/97 against the final selector13 catalog (291.90 seconds). The final result
and exact inputs are bound in
`analysis/acceptance-function498-secondary-overlap-verification-current.json`.
All seven selected packet cores and input bindings match current evidence.
These are MCP tooling checks; the native fixture corpus was not rerun because
its bundle and native build inputs are unchanged and neither source is admitted
for native replay. Its outstanding failures remain open. No new build was
created. The previously rejected temporary-build/log cleanup remains blocked;
no alternate deletion was attempted.

## Secondary exception diagnostics, 2026-09-27

Selectors 14 and 15 now have bounded original execution evidence. Both capture
containers pass independent source validation with 23 complete and three
incomplete records. The capture wrapper rejects both as
`rejected-requested-function-evidence`: the selected F498 invocation and its
F477/F315 callers had not returned at the diagnostic stop. Their statuses are
preserved, neither source is added to the supplemental catalog, and the live
admitted coverage remains 14/16 secondary targets and 77 missing outcomes.

For selector 15, the original dispatch reaches `$17684` with opcode `$07FF`.
The live program-space vector at `$10` points to `$400`; that handler's live
opcode is `$4E73` (`RTE`). Its stack contains saved SR `$2300` and saved PC
`$17684`. SP changes from `$7FAC` to `$7FA6`, then returns to `$7FAC` with SR
`$2300` at the second visit to `$17684`. Incomplete record 29 independently
contains state `$72` edges `$17644 -> $17684 -> $400 -> $17684`.

For selector 14, `$17680: 0062 026E` executes before the illegal instruction.
Live A2 decrements from `$B5BC` to `$B5BA`. One guarded program-space read and
one write are observed at `$B5BA`, changing `$1638` to `$167E`, the exact result
of OR with `$026E`. The resulting SR is `$2300`. It then performs the same
observed exception/RTE/re-entry cycle. The watchpoint's `wpsize` value `$10`
means 16 bits, not 16 bytes: `debug_watchpoint::triggered` supplies
`size * unit_size`. The diagnostic validator was corrected to this source-backed
unit interpretation; no native behavior or capture was altered.

These observations establish one cycle for each controlled input, not infinite
looping or natural producer reachability. The recorder classifies `$07FF` as a
generic successor and deliberately strips memory/call effects and clears exit
registers for incomplete records. The exception interpretation therefore binds
the live vector, opcode, saved-stack snapshots, original CPU source and retained
state-qualified edges. Selector 14's memory observations are explicitly debugger
watchpoint evidence, not reconstructed fixture effects.

Diagnostic receipts are
`analysis/acceptance-function498-secondary-exception-current.json` and
`analysis/acceptance-function498-secondary-predecrement-exception-current.json`.
Their validators recheck source integrity without rewriting rejected identities
or relaxing normal fixture admission. The ordinary-return expectation does not
apply to these two controlled inputs. Remaining work includes natural stream
reachability, handler ownership/full exception effects and the other open path
and timing contracts. No native implementation or readiness promotion follows
from this diagnostic result.
# Constructor coordinates and remaining bank writers — 2026-09-27

The retained constructor evidence now has a reproducible validator at
`analysis/validate-function498-constructor-frontier.py` and a receipt at
`analysis/acceptance-function498-constructor-frontier-current.json`.
This is producer-domain investigation, not native implementation or fixture admission.

Original instruction `$17C3A` consumes the count word once; DBRA at `$17C9C`
returns to `$17C3C`. Thus `$17C3E` reads the descriptor-index byte at record+2.
The earlier review's record+4 interpretation and all-zero active-index count
are disproved. The 183 decoded records (177 initially active) contain indices
0 through 15; none contains 24 through 31. This does not make the decoded
records an exhaustive live input domain.

With conditional A6=`$FFFFC000` and the retained program-data table at `$17834`,
index 25 gives A3=`$FF5801`. The original pre-copy BSET at `$17C56` and BCLR at
`$17C62` would address `$FF54E1` and `$FF5581`, which alias sound-bank offsets
`$54E1` and `$5581`. Snapshot bytes `$30` and `$AE` would become `$34` and `$2E`.
These are potential writes derived from original instructions, not observed
writes or proof that index 25 is naturally reachable. All four shared-memory
aliases are accounted for in the address calculation.

The four unparsed initialization definitions cannot be dismissed because their
record counts exceed channel capacity. A conditional linear scan within the
retained bank gives these boundaries (record indices are zero-based):

| Initialization command | Requested records | First odd word destination | First index-25 record |
| --- | ---: | ---: | ---: |
| 18 (`$12`) | 27,431 | 10 | 153 |
| 20 (`$14`) | 36,870 | 45 | 78 |
| 22 (`$16`) | 36,869 | 36 | 69 |
| 24 (`$18`) | 36,870 | 8 | 133 |

Each odd destination occurs at the constructor's MOVE.W at `$17C68`, before
the first index-25 record in that snapshot scan. Pointer preservation, writes
by earlier records, child calls, and exception behavior remain unproved;
the scan is not an execution trace. The next discriminating question is whether
the original initialization queue can deliver these four commands with the
retained directory intact, and what original execution does at the first
unclosed boundary if one is reachable.

The existing initialization-producer report was also recomputed without
changing its historical identity. Its complete 65,536-word input enumeration
still matches: C00 values 0 through 4 yield commands 6, 8, 10, 12 or rejection.
That conditional reduction is already established and need not be repeated.
The remaining question is C00's live domain at `$A744`, particularly the late
`$9BEE` initialization entry, negative values allowed by the repeat BMI test,
and register-derived aliases. The report does not establish that global
lifecycle; its cold-start/callsite premises remain separately qualified.

The current CPU-A inventory was independently regenerated in memory and matches
all semantic fields of the retained inventory: 3,010 instructions and 425
explicit stores. Supporting memory-domain/runtime reports have stale nested
bindings to the function table, contract, or MCP server. The retained projection
of 200 runtime stores with one bank-overlapping candidate at `$83E06` therefore
remains historical; inventory equality alone does not revalidate its domains.
The candidate's local bytes independently match CPU-B `$17CA2`'s guarded writer,
whose original directory guard is nonzero. This conditional early return is not
a global bank-immutability proof.

The validator passes and confirms all 792 native build-input hashes unchanged.
F498's canonical core and false implementation-readiness gate are preserved;
there are zero new fixture admissions. No new build, capture, or native test
run was created for this investigation.
# C00 lifetime and F470 loop envelope — 2026-09-27

`analysis/validate-function498-c00-lifetime-frontier.py` passes and records the
current evidence in `analysis/acceptance-function498-c00-lifetime-frontier-current.json`.
All 23 currently classified F470 instructions match the original opcode image.
The check enumerates all 256 possible copy-count bytes and all 256 possible
post-copy sort-count bytes; it assumes no restricted life-count domain.

At `$FDB0`, the descending copy includes offset zero. Its destination offsets
are 0 through the entry byte at A4+$40 plus one, inclusive. The older partial
proof's minimum offset of one is incorrect. The subsequent sort count is read
after this copy, so it is admitted independently over the full byte domain.

Symbolic original-byte tracking through every one of the 256 copy counts also
disproves a premise of the old zero-count counterexample: entry field0 does
not simply survive until `$FDC0`. For entry field+$40 values 0..62, `$FDB0`
overwrites field0 with that field+$40 value. Therefore a zero sort count in
this range requires entry field+$40 to be zero, regardless of the old field0.
For entry values 63..255, descending overlap instead propagates an original
byte at offset $80, $C0, $100, or $140 into field0. Those source identities
are recorded for every input. The natural field+$40 domain and its timing
relative to the caller remain unproved; the older entry-field0-only lifecycle
argument must not be reused as evidence of a zero sort count.

At `$FDCC`, every outer sort pass resets A0 to A4+2. A count of one skips the
sort. Counts 2 through 255 have maximum conditional swap offset count+1,
at most A4+$100. A zero count underflows the word counters: its first inner
pass has 65,535 comparisons and a maximum conditional write at A4+$10001.
This is an address calculation assuming the original instructions keep executing,
not an observed execution of that many comparisons.

For conditional A4 bases `$E00`, `$1000`, and `$1200`, these explicit local
write envelopes exclude C00 and its CPU-B RAM aliases. However, the zero-count
envelope includes F470's own `$FDA0..$FDE5` instructions in writable low RAM.
Actual stores depend on byte comparisons and prior execution. Consequently,
the local envelope cannot establish stable executable memory or exclude later
C00 writes by altered execution. The retained A4-domain proof also has a stale
direct binding to the FD1094 inventory report; its transitive lifecycle is not
revalidated here. No global alias count or readiness gate is reduced.

The late initialization route has no local C00 range guard: `$D462` compares
task field A5+$10 with `$10B`, and `$D46A` jumps to `$9BC4` when that signed
threshold is exceeded. `$9BEE` calls `$A618`, which calls `$A65C` at `$A638`.
The sound producer at `$A744` is gated by bit 3 of byte `$820`, but that bit
test supplies no C00 bound. Intervening child calls and global lifetimes remain
separate proof obligations.

The repeat route uses `CMPI.W #4,C00` followed by **BMI**, not BLT. Its exact
unsigned input intervals are 0..3 and `$8004..$FFFF`; the overflow cases
`$8000..$8003` do not pass. Earlier prose describing this as signed C00<4 is
therefore too broad. All 128 malformed-command preimages from the retained
producer enumeration were rechecked; 64 pass the actual BMI predicate. None
is established as naturally reachable by this calculation.

The remaining discriminating obligation is the original ordering between the
F470 caller's state-10 predicate, field+$40 and the resulting zero count, and C16 transitions,
together with current C00/A4 alias provenance. Another forced sound-selector
capture would not resolve this lifetime question. All 792 native build-input
hashes remain unchanged; this investigation created no build or capture and
admitted no fixture. Full acceptance remains open.
# Ordinary field-$40 producer and state-10 interval — 2026-09-27

The corrected count provenance now has a current-code producer census and
conditional route proof in `analysis/acceptance-function498-field40-producers-current.json`.
`analysis/validate-function498-field40-producers.py` passes against all 477
state-72 owners and 10,738 unique currently classified instructions. It records
18 explicit field-$40 references and six fixed A4-displacement writers that
overlap that byte: `$EF16`, `$FB02`, `$FD20`, `$FD78`, `$FDBA`, and `$103C4`.
This syntactic inventory does not exclude arbitrary register-derived aliases,
unclassified entries, interrupts, or other CPUs.

Following the ordinary creation path disproves another premise of the old
simple zero-count scenario. `$EECC` calls `$FA92`; all three early-return
branches return with carry clear. Its carry-set return at `$FABE` follows
the `$FAAC` call to `$FAC4`, which writes `$0303` at A4+0. `$EED0` requires
carry set to continue, and C16 values 1 or 2 then select `$EF10`. At `$EF16`,
the newly initialized first byte is copied to A4+$40, establishing a value
of **3** under the original entry/return, register, and memory-stability
conditions. Clearing field0 at `$EF22` does not clear that copied byte.
The source cannot be treated as arbitrary zero on this ordinary route.

The guarded increments at `$FD20` and `$103C4` admit input bytes 0..61 and
produce 1..62; all other byte values skip the increment. Their subsequent
indexed writes target offsets $42..$7F, so they cannot clear field $40.
The additional current writer `$FD78` belongs to F639. Its only currently
classified direct caller is `$F472`, whose `$F442` unsigned >=9 return gate
excludes the same task while its state word at A5+$44 remains 10. This protects
the interval before the first `$F434` call, not later execution after `$F430`
has cleared the task state. F470's own `$FDBA` clear likewise happens after
entry and cannot explain zero at that first entry.

The two ordinary pre-dispatch calls from `$F3AC` were also checked through
their complete direct drawing helpers: `$FA26`, `$FB44`, `$15FE6`, and `$1610E`.
They preserve A4/A5. Conservative signed-word offsets and full word loop
counts place every explicit store above CPU-B low-RAM aliases and below
the high aliases beginning at `$F00000`. They therefore do not directly
overwrite the copied byte or a low-RAM task record. Implicit stack writes,
device effects, interrupts, altered code, and other entries remain separately
qualified; this is not a whole-machine preservation proof.

With byte 3 preserved until F470, the corrected copy yields sort count 3:
three comparisons, maximum swap offset A4+4, and no executable-RAM overlap.
The unresolved case must now identify an actual intervening writer or a
different entry, rather than relying on entry field0 being zero. In particular,
foreign initialization at `$FB02`, task/record aliasing, and replacement or
re-entry ordering remain open. Current A4/task provenance must be established
before promoting the conditional result to global C00 or sound-bank closure.

All 792 native build-input hashes remain unchanged. No build, capture, native
behavior change, fixture admission, or readiness promotion was made.
