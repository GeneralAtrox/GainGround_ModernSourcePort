# Gain Ground Workspace Agent Rules

These instructions apply to every agent working on **Gain Ground** (Sega System 24 reverse engineering and modern native porting).

Recreate the original’s observable behaviour and visual presentation so they are indistinguishable in equivalent play situations. Byte-for-byte implementation is unnecessary. Nuance is required functionality, not optional polish.

For active native-function work, follow `docs/native-function-workflow.md`.
Use one canonical current packet per selected function; retain historical evidence
without treating it as live status.

---

## 1. Safety & Test-Build Cleanup

- **Mandatory:** Every agent that creates, copies, or configures a test build or temporary capture run must remove that build and all associated binaries, object files, compiler caches, and intermediate logs before ending its task.
- Treat test builds under `build/`, `tmp/`, `.tmp/`, and `research-runtime-*` as disposable. Clean them up immediately upon test completion.
- Durable conclusions must be recorded in tracked manifests, schemas, tests, or markdown documentation before cleanup.

---

## 2. FastMCP Tooling Surface (`MCP/server.py`)

New agents should utilize the project-scoped MCP server (`gainground-dev`) instead of writing ad-hoc shell scripts:

### Available Tools

1. **`gground_fixture_inspect`**:
   - Inspects `GGFTST1` binary fixture bundles (`.ggfz`) to extract function names, fixture counts, and bundle offsets.
   ```json
   {
     "name": "gground_fixture_inspect",
     "arguments": {"function_id": "sub_4200", "limit": 25}
   }
   ```

2. **`gground_source_fixture_inspect`**:
   - Streams one validated `GGFIX1` source capture without loading it into memory or
     rebuilding the aggregate corpus. Results are restricted to one function, capped at
     100 records, and may filter memory prestates by byte offset.
   ```json
   {
     "name": "gground_source_fixture_inspect",
     "arguments": {
       "file_path": "capture/corpus-fixtures/corpus-v1-early-start-window-init-r1/functions.ggfx",
       "function_id": 133,
       "limit": 10,
       "memory_offset": 2102
     }
   }
   ```

3. **`gground_asset_inspect`**:
   - Inspects extracted System 24 graphics, tilemaps, sprite sheets, and palette tables from `analysis/assets/`.
   ```json
   {
     "name": "gground_asset_inspect",
     "arguments": {"asset_kind": ".bin"}
   }
   ```

3. **`gground_system24_memory_inspect`**:
   - Decodes Sega System 24 hardware memory address spaces:
     - `0x000000`–`0x03FFFF`: Main 68000 Work RAM (256 KB)
     - `0x040000`–`0x07FFFF`: Tilemap Layer A Video RAM
     - `0x080000`–`0x0BFFFF`: Tilemap Layer B Video RAM
     - `0x0C0000`–`0x0DFFFF`: Sprite Engine Object RAM
     - `0x0E0000`–`0x0FFFFF`: Color Palette DAC RAM (8,192 colors)
     - `0x800000`–`0x9FFFFF`: Dual-68000 Inter-CPU Shared Communication Window
   ```json
   {
     "name": "gground_system24_memory_inspect",
     "arguments": {"address": 327680, "size_bytes": 64}
   }
   ```

4. **`gground_checkpoint_inspect`**:
   - Inspects `.ggcap` binary checkpoint packages and event headers.
   ```json
   {
     "name": "gground_checkpoint_inspect",
     "arguments": {"file_path": "repro/checkpoint-01.ggcap"}
   }
   ```

5. **`gground_protocol_inspect`**:
   - Validates `GGRPRT1` dual-68000 inter-CPU communication protocol captures.
   ```json
   {
     "name": "gground_protocol_inspect",
     "arguments": {"file_path": "capture/protocol-run-01.ggrprt"}
   }
   ```

6. **`gground_function_coverage_audit`**:
   - Tracks decompiled C++ functions vs 106 fallback stubs in `native/generated/`.
   ```json
   {
     "name": "gground_function_coverage_audit",
     "arguments": {}
   }
   ```

7. **`gground_function_dependency_audit`**:
   - Reports fixture-observed direct call ownership and unresolved native child implementations.

8. **`gground_function_path_coverage_audit`**:
   - Conservatively compares authoritative static instructions/control flow with captured local edges and calls.
   - Unobserved instructions, missing conditional outcomes, unresolved targets, and uncatalogued external targets block contract completeness.
   - Use `include_data_ranges: false` when only the translation gate is needed.

9. **`gground_function_implementation_readiness_audit`**:
   - Audits the full transitive fixture-observed call closure before translating a selected function.
   - Requires a complete path contract for the selected root and every reachable child, plus native ownership for every non-self dependency. An unimplemented mutually recursive SCC is allowed only when the audit explicitly returns it as an atomic translation unit with direct fixtures and complete contracts for every member and native ownership for every dependency outside the unit; all members must then be implemented and catalogued together.
   ```json
   {
     "name": "gground_function_implementation_readiness_audit",
     "arguments": {"function_id": 302}
   }
   ```

10. **`gground_function_work_packet`**:
   - Builds one live packet capped at 256 KiB for native work: exact classified code
     units, convenience disassembly, blockers, dependencies, live implementation
     ownership, and up to three path-distinct fixture references.
   - Does not embed full fixtures, history/provenance reports, or complete record inventories.
   - A valid packet is not translation authority. Require
     `translation_gate.implementation_ready: true` before editing native behavior.
   ```json
   {
     "name": "gground_function_work_packet",
     "arguments": {"function_id": 302, "fixture_limit": 3}
   }
   ```

11. **`gground_fixture_record_slice_inspect`**:
   - Reads one bounded section/page of a representative fixture. Prefer it to the
     complete-record tool for native work; list pages are capped at 100 items.
   ```json
   {
     "name": "gground_fixture_record_slice_inspect",
     "arguments": {
       "record_index": 213,
       "section": "memory-prestates",
       "offset": 0,
       "limit": 25
     }
   }
   ```

7. **`gground_fixture_record_inspect`**:
   - Decodes one complete `.ggfz` record, including entry/exit registers, control-flow edges,
     memory prestates/deltas, ordered child calls, and hardware effects.
   - Its response is unbounded; use it only when a complete record is explicitly required.
   ```json
   {
     "name": "gground_fixture_record_inspect",
     "arguments": {"record_index": 29814}
   }
   ```

8. **`gground_function_dependency_audit`**:
   - Audits fixture-observed child calls for one function or every implemented function and
     reports dependencies that still resolve to fallback stubs.
   ```json
   {
     "name": "gground_function_dependency_audit",
     "arguments": {"function_id": 142}
   }
   ```

---

## 3. Running Tests & Builds

- **MCP Tests**:
  ```powershell
  $env:PYTHONPATH="C:\Projects\GainGround\MCP;C:\Projects\GainGround"
  python -m pytest MCP/tests
  ```
- **Native C++ Contract & Fixture Tests**:
  ```powershell
  & "scripts/Test-GainGroundNative.ps1"
  ```
