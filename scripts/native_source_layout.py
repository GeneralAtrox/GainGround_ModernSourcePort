"""Bound native source files while retaining their expanded bytes and C++ scope.

Only marked includes are expanded. Preprocessor directives stay in the original
file, so conditional groups never cross include-file boundaries. Fragments stay
beside their original file, preserving relative include lookup.
"""
from pathlib import Path
import re

LIMIT = 400
CHUNK = 340
MARKER = b" // gground-source-split"
INCLUDE = re.compile(rb'^#include "([^"/\\\r\n]+)" // gground-source-split\r?\n$')
ARRAY_DECL = re.compile(
    rb'(?P<prefix>(?:(?:inline|static|extern)\s+)*(?:constexpr|const)\s+)'
    rb'std::array<(?P<type>[^,>]+),\s*(?P<count>[^>]+)>\s+'
    rb'(?P<name>[A-Za-z_]\w*)\s*\{\{')
ARRAY_HELPER_INCLUDE = b'#include "gain_ground/source_layout_detail.h"\n'


def _units(data):
    """Yield complete lexical lines; never split comments, literals or macros."""
    state, quote, raw_end = "code", "", ""
    pending = []
    for line in data.splitlines(keepends=True):
        pending.append(line)
        text = line.decode("latin1")
        index = 0
        while index < len(text):
            char = text[index]
            if state == "line":
                break
            if state == "block":
                end = text.find("*/", index)
                if end < 0:
                    break
                index, state = end + 2, "code"
            elif state == "raw":
                end = text.find(raw_end, index)
                if end < 0:
                    break
                index, state = end + len(raw_end), "code"
            elif state == "quote":
                if char == "\\":
                    index += 2
                elif char == quote:
                    index, state = index + 1, "code"
                else:
                    index += 1
            elif text.startswith("//", index):
                state = "line"
            elif text.startswith("/*", index):
                index, state = index + 2, "block"
            elif text.startswith('R"', index):
                match = re.match(r'R"([^ ()\\\t\r\n]*)\(', text[index:])
                if not match:
                    raise ValueError("Malformed C++ raw string")
                raw_end = ")" + match[1] + '"'
                index, state = index + len(match[0]), "raw"
            elif char in {'"', "'"}:
                # Apostrophes between numeric-token characters are separators.
                separator = (char == "'" and index > 0 and index + 1 < len(text)
                             and text[index - 1].isalnum() and text[index + 1].isalnum())
                if not separator:
                    quote, state = char, "quote"
                index += 1
            else:
                index += 1
        continued = line.rstrip(b"\r\n").endswith(b"\\")
        if state == "line" and not continued:
            state = "code"
        if state == "code" and not continued:
            yield b"".join(pending)
            pending.clear()
    if pending:
        if state not in {"code", "line"}:
            raise ValueError("Unterminated source comment or literal")
        yield b"".join(pending)


def _scan_cpp_units(units):
    """Return safe declaration boundaries and namespace delimiter units."""
    stack, safe, namespace_delimiters = [], set(), set()
    condition_depth = 0
    state, quote, raw_end = "code", "", ""
    statement = []

    for unit_index, unit in enumerate(units):
        stripped = unit.lstrip()
        if stripped.startswith(b"#"):
            directive = stripped.split(None, 1)[0].decode("ascii", "ignore")
            if directive in {"#if", "#ifdef", "#ifndef"}:
                condition_depth += 1
            elif directive == "#endif":
                condition_depth = max(0, condition_depth - 1)
            if not stack and condition_depth == 0:
                safe.add(unit_index + 1)
            continue

        delimiter_seen = False
        text = unit.decode("latin1")
        index = 0
        while index < len(text):
            char = text[index]
            if state == "line":
                if char == "\n":
                    state = "code"
                index += 1
            elif state == "block":
                end = text.find("*/", index)
                if end < 0:
                    index = len(text)
                else:
                    index, state = end + 2, "code"
            elif state == "raw":
                end = text.find(raw_end, index)
                if end < 0:
                    index = len(text)
                else:
                    index, state = end + len(raw_end), "code"
            elif state == "quote":
                if char == "\\":
                    index += 2
                elif char == quote:
                    index, state = index + 1, "code"
                else:
                    index += 1
            elif text.startswith("//", index):
                state, index = "line", index + 2
            elif text.startswith("/*", index):
                state, index = "block", index + 2
            elif text.startswith('R"', index):
                match = re.match(r'R"([^ ()\\\t\r\n]*)\(', text[index:])
                if not match:
                    raise ValueError("Malformed C++ raw string")
                raw_end = ")" + match[1] + '"'
                state, index = "raw", index + len(match[0])
            elif char in {'"', "'"}:
                separator = (char == "'" and index > 0 and index + 1 < len(text)
                             and text[index - 1].isalnum() and text[index + 1].isalnum())
                if separator:
                    statement.append(char)
                    index += 1
                else:
                    quote, state, index = char, "quote", index + 1
            elif char == "{":
                prefix = "".join(statement).strip()
                is_namespace = bool(re.fullmatch(
                    r"(?:inline\s+)?namespace(?:\s+[A-Za-z_]\w*(?:::\w*)*)?", prefix))
                stack.append(is_namespace)
                statement.clear()
                if is_namespace:
                    namespace_delimiters.add(unit_index)
                index += 1
            elif char == "}":
                if stack:
                    is_namespace = stack.pop()
                    if is_namespace:
                        namespace_delimiters.add(unit_index)
                statement.clear()
                delimiter_seen = True
                index += 1
            elif char == ";":
                statement.clear()
                delimiter_seen = True
                index += 1
            else:
                statement.append(char)
                index += 1

        if delimiter_seen and not any(not is_namespace for is_namespace in stack) and condition_depth == 0:
            safe.add(unit_index + 1)

    return safe, namespace_delimiters


def _module_labels(body):
    """Give a source fragment a stable label based on its complete declarations."""
    text = body.decode("latin1")
    candidates = re.findall(
        r"\b(?:FunctionResult|FunctionContext|FunctionContract|CheckpointKindContract|"
        r"CaptureCheckpointEventContract|FunctionFixtureCatalogEntry|FixtureMarkerCompatibilityRange|"
        r"EnemyDefinition|LevelDefinition|bool|void|std::span<[^\n>]+>|inline\s+bool)\s+"
        r"([A-Za-z_]\w*)\s*(?:\(|\{)", text)
    if not candidates:
        candidates = re.findall(r"\b(?:constexpr|const|inline)\s+[^;\n]*?\b([A-Za-z_]\w*)\s*\{", text)
    if not candidates:
        candidates = re.findall(r"\b([A-Za-z_]\w*)\s*\{\s*$", text, re.M)
    if not candidates:
        return "declarations"
    first, last = candidates[0], candidates[-1]
    label = first if first == last else f"{first}_through_{last}"
    return re.sub(r"[^A-Za-z0-9_]+", "_", label)[:96]


def _split_array_elements(data, start):
    """Return complete initializer elements and the byte after the array semicolon."""
    depth, state, quote, raw_end = 2, "code", "", b""
    index, item_start, entries = start, start, []
    while index < len(data):
        char = chr(data[index])
        if state == "line":
            if char == "\n":
                state = "code"
            index += 1
        elif state == "block":
            if data[index:index + 2] == b"*/":
                state, index = "code", index + 2
            else:
                index += 1
        elif state == "raw":
            if data.startswith(raw_end, index):
                state, index = "code", index + len(raw_end)
            else:
                index += 1
        elif state == "quote":
            if char == "\\":
                index += 2
            elif char == quote:
                state, index = "code", index + 1
            else:
                index += 1
        elif data[index:index + 2] == b"//":
            state, index = "line", index + 2
        elif data[index:index + 2] == b"/*":
            state, index = "block", index + 2
        elif data[index:index + 2] == b'R"':
            match = re.match(rb'R"([^ ()\\\t\r\n]*)\(', data[index:])
            if not match:
                raise ValueError("Malformed raw string in generated array")
            raw_end = b")" + match[1] + b'"'
            state, index = "raw", index + len(match[0])
        elif char in {'"', "'"}:
            separator = (char == "'" and index > 0 and index + 1 < len(data)
                         and chr(data[index - 1]).isalnum() and chr(data[index + 1]).isalnum())
            if separator:
                index += 1
            else:
                quote, state, index = char, "quote", index + 1
        elif char == "{":
            depth += 1
            index += 1
        elif char == "}":
            depth -= 1
            if depth == 1:
                final_entry = data[item_start:index]
                if final_entry.strip():
                    trivia = re.fullmatch(
                        rb"(?:\s|//[^\r\n]*(?:\r?\n|$)|/\*.*?\*/)*", final_entry, re.S)
                    if trivia and entries:
                        entries[-1] += final_entry
                    else:
                        entries.append(final_entry + b",")
                item_start = index + 1
            if depth == 0:
                if data[index + 1:index + 2] != b";":
                    raise ValueError("Generated std::array does not end with `}};`")
                return entries, index + 2
            index += 1
        elif char == "," and depth == 2:
            entries.append(data[item_start:index + 1])
            item_start = index + 1
            index += 1
        else:
            index += 1
    raise ValueError("Unterminated generated std::array")


def _batch_long_arrays(data):
    """Compose oversized generated arrays from named, ordered constexpr batches."""
    changed, offset, replacements = False, 0, []
    while match := ARRAY_DECL.search(data, offset):
        entries, end = _split_array_elements(data, match.end())
        if len(data[match.start():end].splitlines()) > LIMIT:
            batches, batch, batch_lines = [], [], 0
            for entry in entries:
                entry_lines = len(entry.splitlines())
                if entry_lines > LIMIT:
                    raise ValueError(f"One {match['name'].decode()} element exceeds {LIMIT} lines")
                if batch and batch_lines + entry_lines > 300:
                    batches.append(batch)
                    batch, batch_lines = [], 0
                batch.append(entry)
                batch_lines += entry_lines
            if batch:
                batches.append(batch)
            if not batches:
                raise ValueError(f"Cannot batch empty array {match['name'].decode()}")
            prefix, element_type, name = match['prefix'], match['type'], match['name']
            rendered, batch_names = [], []
            for batch_index, batch_entries in enumerate(batches):
                batch_name = name + f"_batch_{batch_index:03d}".encode()
                batch_names.append(batch_name)
                rendered.extend((
                    prefix + b"std::array<" + element_type + b", " + str(len(batch_entries)).encode()
                    + b"> " + batch_name + b"{{\n",
                    b"".join(batch_entries),
                    b"}};\n",
                ))
            args = b",\n    ".join(batch_names)
            rendered.append(prefix + b"auto " + name + b" = "
                            b"::gain_ground::source_layout_detail::concatenate_arrays(\n    "
                            + args + b");\n")
            replacements.append((match.start(), end, b"".join(rendered)))
            changed = True
        offset = end
    for start, end, replacement in reversed(replacements):
        data = data[:start] + replacement + data[end:]
    if changed and ARRAY_HELPER_INCLUDE not in data:
        pragma = re.search(rb"^#pragma\s+once\s*\r?\n", data, re.M)
        offset = pragma.end() if pragma else 0
        prefix, suffix = data[:offset], data[offset:]
        separator = b"\n" if offset and not prefix.endswith(b"\n\n") else b""
        data = prefix + separator + ARRAY_HELPER_INCLUDE + suffix
    return data


def factor_large_pc_membership_switches(source):
    """Move oversized generated next-PC case lists into named lookup helpers."""
    lines = source.splitlines()
    function_starts = [index for index, line in enumerate(lines)
                       if re.fullmatch(r"FunctionResult [A-Za-z_]\w*\(FunctionContext &c\) noexcept \{", line)]
    for position in reversed(range(len(function_starts))):
        start = function_starts[position]
        end = function_starts[position + 1] if position + 1 < len(function_starts) else len(lines)
        if end - start <= LIMIT:
            continue
        switch_start = next((index for index in range(start, end)
                             if lines[index].strip() == "switch (next) {"), None)
        if switch_start is None:
            continue
        default = next((index for index in range(switch_start + 1, end)
                        if "default: return m.dispatch(c, pc, next, transfer_kind, m.state);" in lines[index]), None)
        if default is None or default + 1 >= end or lines[default + 1].strip() != "}":
            raise ValueError(f"Cannot factor generated next-PC switch in {lines[start]}")
        cases = [line.strip() for line in lines[switch_start + 1:default]
                 if re.fullmatch(r"case\s+[^:]+:", line.strip())]
        if not cases:
            continue
        name = re.match(r"FunctionResult ([A-Za-z_]\w*)", lines[start])[1]
        batch_names, helper = [], []
        for batch_index in range(0, len(cases), 250):
            batch_name = f"{name}_contains_next_{batch_index // 250:03d}"
            batch_names.append(batch_name)
            helper.extend((f"static bool {batch_name}(std::uint32_t next) noexcept {{",
                           "    switch (next) {"))
            helper.extend(f"    {case}" for case in cases[batch_index:batch_index + 250])
            helper.extend(("        return true;", "    default: return false;", "    }", "}", ""))
        helper_name = f"{name}_contains_next"
        if len(batch_names) == 1:
            call = f"if (!{batch_names[0]}(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);"
        else:
            helper.extend((f"static bool {helper_name}(std::uint32_t next) noexcept {{",
                           "    return " + " || ".join(f"{batch_name}(next)" for batch_name in batch_names) + ";",
                           "}", ""))
            call = f"if (!{helper_name}(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);"
        lines[start:start] = helper
        switch_start += len(helper)
        default += len(helper)
        lines[switch_start:default + 2] = [f"        {call}"]
    for position in reversed(range(len([i for i, line in enumerate(lines)
                                        if re.fullmatch(r"FunctionResult [A-Za-z_]\w*\(FunctionContext &c\) noexcept \{", line)]))):
        starts = [i for i, line in enumerate(lines)
                  if re.fullmatch(r"FunctionResult [A-Za-z_]\w*\(FunctionContext &c\) noexcept \{", line)]
        start = starts[position]
        end = starts[position + 1] if position + 1 < len(starts) else len(lines)
        if end - start <= LIMIT:
            continue
        switch_start = next((index for index in range(start, end)
                             if lines[index].strip() == "switch (pc) {"), None)
        if switch_start is None:
            continue
        default = next((index for index in range(switch_start + 1, end)
                        if "default: return {TranslationStatus::contract_violation, 0U, pc};" in lines[index]), None)
        if default is None or default + 1 >= end or lines[default + 1].strip() != "}":
            raise ValueError(f"Cannot factor generated PC switch in {lines[start]}")
        case_starts = [index for index in range(switch_start + 1, default)
                       if re.match(r"\s*case\s+.*:\s*\{", lines[index])]
        blocks = []
        for block_index, block_start in enumerate(case_starts):
            block_end = case_starts[block_index + 1] if block_index + 1 < len(case_starts) else default
            blocks.append(lines[block_start:block_end])
        retained = [block for block in blocks if any(re.search(r"\breturn\b", line) for line in block)]
        moved = [block for block in blocks if block not in retained]
        helper_groups, group, group_lines = [], [], 0
        for block in moved:
            size = len(block)
            if group and group_lines + size > 250:
                helper_groups.append(group)
                group, group_lines = [], 0
            group.append(block)
            group_lines += size
        if group:
            helper_groups.append(group)
        if not helper_groups:
            continue
        name = re.match(r"FunctionResult ([A-Za-z_]\w*)", lines[start])[1]
        helpers = []
        helper_names = []
        for group_index, case_group in enumerate(helper_groups):
            helper_name = f"{name}_dispatch_cases_{group_index:03d}"
            helper_names.append(helper_name)
            helpers.extend((
                f"static bool {helper_name}([[maybe_unused]] unverified::Machine &m,",
                "                               [[maybe_unused]] FunctionContext &c,",
                "                               [[maybe_unused]] std::uint32_t pc,",
                "                               [[maybe_unused]] std::uint32_t &next,",
                "                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {",
                "    [[maybe_unused]] auto &r = c.registers;",
                "    switch (pc) {",
            ))
            helpers.extend(line for block in case_group for line in block)
            helpers.extend(("    default: return false;", "    }", "    return true;", "}", ""))
        replacement = ["        switch (pc) {"]
        replacement.extend(line for block in retained for line in block)
        replacement.append("        default:")
        condition = " || ".join(
            f"{helper_name}(m, c, pc, next, transfer_kind)" for helper_name in helper_names)
        replacement.extend((f"            if (!({condition})) return {{TranslationStatus::contract_violation, 0U, pc}};",
                            "            break;", "        }"))
        lines[start:start] = helpers
        switch_start += len(helpers)
        default += len(helpers)
        lines[switch_start:default + 2] = replacement
    result = "\n".join(lines)
    return result + ("\n" if source.endswith("\n") else "")


def layout(path, data):
    """Split only after complete C++ declarations; indivisible long items fail."""
    path = Path(path)
    if isinstance(data, str):
        data = data.encode("utf-8")
    if len(data.splitlines()) <= LIMIT:
        return {path: data}
    data = _batch_long_arrays(data)

    units = list(_units(data))
    wrapper_preamble = []
    pragma_index = next((index for index, unit in enumerate(units)
                         if re.fullmatch(rb"\s*#pragma\s+once\s*\r?\n?", unit)), None)
    if pragma_index is not None:
        prelude = units[:pragma_index]
        if all(not unit.strip() or unit.lstrip().startswith((b"//", b"/*")) for unit in prelude):
            wrapper_preamble = units[:pragma_index + 1]
            units = units[pragma_index + 1:]
    safe, namespace_delimiters = _scan_cpp_units(units)
    outputs, wrapper, pending = {}, list(wrapper_preamble), []
    pending_lines = 0
    declaration = []
    declaration_lines = 0
    ordinal = 0

    def emit():
        nonlocal ordinal, pending_lines
        if not pending:
            return
        body = b"".join(pending)
        lines = len(body.splitlines())
        if lines > LIMIT:
            raise ValueError(
                f"Indivisible C++ declaration exceeds {LIMIT} lines: {path} ({lines}, {_module_labels(body)})")
        ordinal += 1
        label = _module_labels(body)
        fragment = path.with_name(f"{path.stem}.module{ordinal:03d}_{label}.inc")
        outputs[fragment] = body
        wrapper.append(b'#include "' + fragment.name.encode() + b'"' + MARKER + b"\n")
        pending.clear()
        pending_lines = 0

    def finish_declaration():
        nonlocal declaration_lines, pending_lines
        if not declaration:
            return
        if declaration_lines > LIMIT:
            body = b"".join(declaration)
            raise ValueError(
                f"Indivisible C++ declaration exceeds {LIMIT} lines: "
                f"{path} ({declaration_lines}, {_module_labels(body)})")
        if pending and pending_lines + declaration_lines > CHUNK:
            emit()
        pending.extend(declaration)
        pending_lines += declaration_lines
        declaration.clear()
        declaration_lines = 0

    for unit_index, unit in enumerate(units):
        if unit_index in namespace_delimiters:
            finish_declaration()
            emit()
            wrapper.append(unit)
            continue
        declaration.append(unit)
        declaration_lines += len(unit.splitlines())
        if unit_index + 1 in safe:
            finish_declaration()
    finish_declaration()
    emit()
    outputs[path] = b"".join(wrapper)
    if len(outputs[path].splitlines()) > LIMIT:
        raise ValueError(f"Source include list exceeds {LIMIT} lines: {path}")
    rebuilt = b"".join(outputs[path.parent / match[1].decode()] if (match := INCLUDE.match(line)) else line
                       for line in outputs[path].splitlines(keepends=True))
    if rebuilt != data:
        raise ValueError(f"Split does not reconstruct its input: {path}")
    return outputs


def expanded_bytes(path, stack=()):
    path = Path(path)
    if path in stack:
        raise ValueError(f"Cyclic source fragment: {path}")
    result = []
    for line in path.read_bytes().splitlines(keepends=True):
        match = INCLUDE.match(line)
        result.append(expanded_bytes(path.parent / match[1].decode(), (*stack, path))
                      if match else line)
    return b"".join(result)


def read_source(path):
    """Text reader for existing generator/registry consumers (universal newlines)."""
    return expanded_bytes(path).decode("utf-8-sig").replace("\r\n", "\n")


def source_paths(path):
    """List physical dependencies for source-hash manifests and audits."""
    path = Path(path)
    result, pending = set(), [path]
    while pending:
        current = pending.pop()
        if current in result:
            continue
        result.add(current)
        for line in current.read_bytes().splitlines(keepends=True):
            if match := INCLUDE.match(line):
                pending.append(current.parent / match[1].decode())
    return sorted(result)


def write_source(path, text):
    path = Path(path)
    previous = set(source_paths(path)) if path.exists() else set()
    outputs = layout(path, text)
    for output, data in outputs.items():
        output.parent.mkdir(parents=True, exist_ok=True)
        if not output.exists() or output.read_bytes() != data:
            output.write_bytes(data)
    for obsolete in previous - outputs.keys():
        obsolete.unlink()
    return outputs
