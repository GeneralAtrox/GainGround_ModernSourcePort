# F141 path evidence recovered from a retained capture

F141 now has observations for all **73 original instructions**. Its missing
conditional outcomes decreased from four to **one** after admitting three
complete invocations from the retained `final9-terrain-stage2-r1` capture.
No production code changed and no new emulator run or test build was created.

The full retained capture passed current structural validation against its
recorded identity and matching function, fixture and ROM contracts: 7,960
complete and 64 incomplete records. The three selected F141 records are all
complete; the incomplete records are not F141 evidence. Their local instruction
opcodes match the current authoritative original-code inventory.

The newly useful invocation enters F141 with stage word `$0c02 = 1` and flag
byte `$0820` bit 3 set. It executes the previously missing nonzero-stage
division path, the nonzero-remainder branch and the odd-stage output path.
All seven previously unobserved instructions and three missing conditional
outcomes are now represented in captured original execution.

## Controlled-input limits

This is controlled original-ROM execution, not a natural gameplay replay pass.
The historical debugger script changes stage-transition state before the
selected initialization and later changes actor state for a terrain experiment.
Its exact script, ten captured debugger actions and intervention manifest are
retained. None of those recorded CPU-B actions occurs inside any of the three
selected F141 instruction intervals; their PCs are also absent from the
captured F141 execution closures.

The source is admitted only for F141 path evidence. It is not added to the
native replay corpus and does not establish natural joint reachability, live
graphics, audio or timing parity. The capture executable's recorded hash
differs from the currently installed research executable; one generated
research-source include also differs. The current executable is not presented
as the historical capture binary. The retained capture, recorded identity and
matching contracts were revalidated without claiming the run was reproduced.

## Remaining requirement

The unobserved edge is `$a73a → $a73e`: F141 must see a nonzero stage word
divisible by ten after the first `$0820` bit-3 test succeeds. Stage 10 is a
candidate input, but existing evidence does not prove this combination occurs
in a natural run. Original producer instructions establish stage increments
and flag updates; those facts alone do not prove the joint state at this branch.

The current project audit still returns `implementation_ready: false`.
Completing that path evidence and the instruction/bus/interrupt timing contract
is required before correcting the measured F141 timing omission. The previously
observed 876.4-microsecond missing timing segment and approximately 19.789-ms
main-loop arrival difference remain unresolved.

Private evidence is retained in
`analysis/acceptance-function141-paths-current.json`, linked from the canonical
acceptance report and the current F141 packet. The earlier cadence investigation
records the coverage state before this admission. No broad acceptance gate is
closed by this update.
