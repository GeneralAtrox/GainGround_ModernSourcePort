# Fixture census and enclosing-loop investigation — 26 September 2026

Subsequent [comparator repair](fixture-loop-comparator-repair-20260926.md)
integrates the temporary correction and reproduces all 31,971 affected results.
The diagnostic inventory below retains its original comparator identities and
scope; it is not relabelled as a passing production suite.

Every one of the existing **95,169 replay-eligible records was attempted**.
There are 95,168 returned comparisons and one unresolved reset-record timeout.
This is a diagnostic inventory, **not a passing production suite**: the
31,971 F115/F116 records use an explicitly identified temporary fixture-host
correction. Production game code, the production comparator, corpus admission
and compatibility rules remain unchanged.

## Retained results

| Comparator | Returned comparisons | Passed | Diverged |
| --- | ---: | ---: | ---: |
| Unchanged production comparator | 63,197 | 24,556 | 38,641 |
| Temporary enclosing F118 loop correction, F115/F116 only | 31,971 | 10,831 | 21,140 |
| Combined diagnostic inventory | 95,168 | 35,387 | 59,781 |

Record 2, root F1, source index 344 / source fixture ordinal 59, exceeded the
90-second execution limit with the unchanged comparator. A timeout does not
establish an infinite loop or a native-game defect. It remains unresolved.

The function-level split remains 489 functions with all selected comparisons
passing, 32 with divergences and 120 without eligible records. The broader scan
now retains failures beyond each function's first one. These 59,781 failed
comparisons are not 59,781 independent bugs.

| First-divergence subsystem | Records |
| --- | ---: |
| Calls and continuation markers | 58,958 |
| Compatibility provenance | 730 |
| Translation contract | 76 |
| Memory | 13 |
| Hardware | 3 |
| Control result | 1 |

Each returned result retains its original record index, source index, source
fixture ordinal, function, first divergence and reported compatibility counts.
The inventory verifies these identities against the project MCP bundle index,
rejects duplicate records, and accounts for the one unresolved record. The
comparison API does not report every compatibility hit on non-complete
translation returns; no exact compatibility-histogram pass is claimed.

## Why some records previously could not finish

The default comparator executable reserves 2 MiB of host stack. A debugger
backtrace for original record 35113 shows repeated calls through
`ComparingHost::call_function`, `consume_self_continuation_boundary` and
`cpu_b_wait_for_irq5_frame` until stack overflow. The independent broader scan
also observes the same process-failure class in F116 records.

F118's authoritative instruction bytes are:

| Address | Bytes | Instruction |
| --- | --- | --- |
| `85B0` | `4A38 0502` | `TST.B $502.w` |
| `85B4` | `6AFA` | `BPL.B $85B0` |
| `85B6` | `11FC 0001 0502` | `MOVE.B #1,$502.w` |
| `85BC` | `4E75` | `RTS` |

Original F115 record 35113 contains 2,116 call markers; F116 record 35287
contains 5,475. Both include repeated kind-1 markers for the branch at `85B4`
to `85B0`, then an interrupt boundary. Those branch iterations do not create
new original subroutine calls. The current fixture host recursively invokes
F118 when these markers occur under F115/F116.

A diagnostic executable with a 64 MiB stack has a byte-identical `.text`
section to the default-stack diagnostic driver. It lets both representative
records return, but each reports control 4 where the original captured control
is 5. This exposes a second consequence: the recursive marker path converts
the eventual interrupt boundary into a loop-continuation result. Increasing
stack capacity alone does not repair that behavior. This diagnostic returned
504 comparisons before another record, 35606, exceeded its 90-second limit.

## Scoped comparator experiment

The temporary comparator extends its existing F628 enclosing-F118 rule to
roots F115 and F116. It consumes the next exact branch marker and continues
the current native loop. The existing sequence, owner, CPU, state, kind,
callsite, target and pending-hardware checks remain in force. It adds no
compatibility rule, removes no recorded call, and changes no native function.

Both representative records then pass every comparator check at the default
2 MiB stack size. All 31 standalone F118 fixtures still pass; the pre-existing
F628 failure is unchanged. Across the 504 results from the larger-stack
experiment, 330 divergences become passes, eight passes remain passes, and
166 divergences remain divergences.

The expanded diagnostic completes all 31,971 F115/F116 records without a
timeout or process failure, including record 35606. F115 has 171 passes and
13 divergences; F116 has 10,660 passes and 21,127 divergences. Remaining
failures include call counts/sequences, memory and translation contracts;
the experiment does not classify all of their causes.

The live F118 work packet is implementation-ready. This investigation still
makes no production change. The complete renderer contracts and the other
T/G/A/F/H acceptance requirements remain binding.

## Admission gaps and provenance

The bundle contains 95,680 records: 95,169 eligible for replay and 511 retained
only as path evidence. Of the 120 functions without an eligible record, 24
have no aggregate record and 96 have path-only records. Their admission flags
were inventoried, not relaxed.

The earlier default-stack campaign was explicitly stopped after the recursive
fixture-host problem was established. Its in-memory detailed results were not
published or used in these totals. The unchanged-comparator ranges were run
again to retain their complete results, while the corrected-comparator range
is identified separately. There is no claim that the production comparator
completed the whole corpus.

Private canonical evidence:

- `analysis/acceptance-full-fixtures-current.json`
- `analysis/acceptance-fixture-loop-current.json`
- `analysis/acceptance-fixture-loop-census-current.json`
- `analysis/acceptance-fixture-stack-current.json`
- `analysis/acceptance-fixture-admission-current.json`
- `analysis/function-work/function118.json`

All 788 baseline build-source hashes remained unchanged during these runs.
Temporary source variants, commands, binary identities, comparison results
and cleanup outcomes are retained in the evidence. Full title/attract/return,
coin/start, live graphics, audio, timing and review-build acceptance remain open.
