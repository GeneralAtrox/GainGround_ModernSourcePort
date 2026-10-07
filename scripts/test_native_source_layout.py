"""Regression tests for the physical native-source limit and fragment readers."""
import shutil
import re
import subprocess
import tempfile
import unittest
from pathlib import Path

from native_source_layout import (expanded_bytes, factor_large_pc_membership_switches,
                                  layout, read_source, source_paths, write_source)


class SourceLayoutTests(unittest.TestCase):
    def setUp(self):
        self.scratch = tempfile.TemporaryDirectory(prefix="gground-layout-")
        self.addCleanup(self.scratch.cleanup)
        self.root = Path(self.scratch.name)

    def test_byte_preservation_and_extension_collisions(self):
        for extension in ("cpp", "h"):
            path = self.root / ("same." + extension)
            data = b"// mixed endings\r\n" + b"int value;\n" * 801 + b"// no final newline"
            write_source(path, data)
            self.assertEqual(expanded_bytes(path), data)
            self.assertTrue(all(len(p.read_bytes().splitlines()) <= 400 for p in source_paths(path)))
        self.assertNotEqual(set(source_paths(self.root / "same.cpp")),
                            set(source_paths(self.root / "same.h")))

    def test_regeneration_is_idempotent_and_removes_only_old_fragments(self):
        path = self.root / "generated.cpp"
        text = "int value;\n" * 900
        first = write_source(path, text)
        timestamps = {p: p.stat().st_mtime_ns for p in first}
        self.assertEqual(write_source(path, text), first)
        self.assertEqual(timestamps, {p: p.stat().st_mtime_ns for p in first})
        unrelated = self.root / "unrelated.inc"
        unrelated.write_text("keep")
        write_source(path, "int short_value;\n")
        self.assertEqual(set(self.root.iterdir()), {path, unrelated})

    def test_refuses_indivisible_long_literals(self):
        data = b'auto text = R"tag(\n' + b"payload\n" * 405 + b')tag";\n'
        with self.assertRaisesRegex(ValueError, "Indivisible"):
            layout(self.root / "long.cpp", data)

    def test_refuses_a_function_that_cannot_fit_as_a_complete_module(self):
        data = "int oversized() {\n" + "    value += 1;\n" * 405 + "    return value;\n}\n"
        with self.assertRaisesRegex(ValueError, "Indivisible C\\+\\+ declaration"):
            layout(self.root / "oversized.cpp", data)

    def test_large_arrays_use_ordered_named_batches(self):
        path = self.root / "catalog.h"
        rows = "".join(f"    Entry{{ {index}U }},\n" for index in range(500))
        source = ("#include <array>\nnamespace sample {\nstruct Entry { unsigned value; };\n"
                  f"inline constexpr std::array<Entry, 500> kEntries{{{{\n{rows}}}}};\n"
                  "}\n")
        write_source(path, source)
        expanded = read_source(path)
        self.assertIn("kEntries_batch_000", expanded)
        self.assertIn("kEntries_batch_001", expanded)
        self.assertIn("concatenate_arrays", expanded)
        self.assertIn('#include "gain_ground/source_layout_detail.h"', expanded)
        self.assertNotIn("template <typename T", expanded)
        self.assertEqual(expanded.count("Entry{"), 500)
        self.assertNotIn("\n,}};", expanded)
        positions = [expanded.index(f"Entry{{ {index}U }}") for index in range(500)]
        self.assertEqual(positions, sorted(positions))
        self.assertTrue(all(".module" in fragment.name for fragment in source_paths(path) if fragment != path))

    def test_header_once_directive_remains_on_the_wrapper(self):
        path = self.root / "generated.h"
        source = b"// generated header\n#pragma once\n\n#include <cstddef>\n" + b"int value;\n" * 420
        write_source(path, source)
        wrapper = path.read_bytes()
        self.assertTrue(wrapper.startswith(b"// generated header\n#pragma once\n"))
        self.assertIn(b"#include \"generated.module", wrapper)
        self.assertEqual(expanded_bytes(path), source)
        self.assertFalse(any(b"#pragma once" in fragment.read_bytes()
                             for fragment in source_paths(path) if fragment != path))

    def test_large_generated_dispatch_keeps_complete_pc_case_inventory(self):
        blocks = []
        for index in range(36):
            blocks.extend((f"        case 0x{index:04x}U: {{", "            next = pc + 2U;")
                          + tuple("            next += 0U;" for _ in range(12))
                          + ("            break;", "        }"))
        source = "\n".join([
            "FunctionResult sample(FunctionContext &c) noexcept {",
            "    auto &r = c.registers;",
            "    unverified::Machine m{*c.host, r, 1U, 0x72U};",
            "    for (;;) {",
            "        const auto pc = r.program_counter;",
            "        auto next = pc;",
            "        std::uint8_t transfer_kind = 0U;",
            "        switch (pc) {",
            *blocks,
            "        default: return {TranslationStatus::contract_violation, 0U, pc};",
            "        }",
            "        switch (next) {",
            "        case 0x0000U:",
            "            break;",
            "        default: return m.dispatch(c, pc, next, transfer_kind, m.state);",
            "        }",
            "    }",
            "}",
            "",
        ])
        original_cases = re.findall(r"^\s*case\s+(0x[0-9a-f]+U): \{", source, re.M)
        transformed = factor_large_pc_membership_switches(source)
        transformed_cases = re.findall(r"^\s*case\s+(0x[0-9a-f]+U): \{", transformed, re.M)
        self.assertEqual(sorted(transformed_cases), sorted(original_cases))
        self.assertIn("sample_dispatch_cases_000", transformed)
        self.assertLessEqual(max(len(body.splitlines()) for body in
                                 layout(self.root / "dispatch.cpp", transformed).values()), 400)

    def test_cycles_are_rejected(self):
        path = self.root / "cycle.cpp"
        path.write_bytes(b'#include "cycle.cpp" // gground-source-split\n')
        with self.assertRaisesRegex(ValueError, "Cyclic"):
            expanded_bytes(path)

    def test_preprocessor_and_literals_compile_and_execute(self):
        compiler = shutil.which("g++")
        if not compiler:
            self.skipTest("g++ unavailable")
        path = self.root / "program.cpp"
        # Preprocessor groups and long bodies stay intact; cuts occur between functions.
        data = (b"#include <cstring>\n#define VALUE(x) \\\n ((x) + 1)\n"
                + b"int first() {\nint sum = 0;\n" + b"sum += 1;\n" * 210
                + b"return sum;\n}\nint second() {\nint sum = 0;\n"
                + b"sum += 1;\n" * 210 + b"return sum;\n}\n"
                + b"int main() {\nint sum = first() + second();\n#if defined(__cplusplus)\nsum += 1;\n"
                + b"#else\n#error inactive branch\n#endif\n"
                + b'const char *s = R"tag(first\nsecond)tag";\n'
                + b"/* block\ncomment */\n// continued \\\ncomment\n"
                + b"return sum == 421 && VALUE(9'999) == 10000 "
                  b'&& std::strcmp(s, "first\\nsecond") == 0 ? 0 : 1;\n}\n')
        write_source(path, data)
        self.assertEqual(expanded_bytes(path), data)
        self.assertEqual(read_source(path), data.decode())
        fragments = [p for p in source_paths(path) if p != path]
        self.assertTrue(fragments)
        for fragment in fragments:
            body = fragment.read_bytes()
            self.assertLessEqual(len(body.splitlines()), 400)
            self.assertRegex(fragment.name, r"\.module\d{3}_.*\.inc$")
        bodies = [fragment.read_bytes() for fragment in fragments]
        self.assertTrue(any(b"int first()" in body and b"int second()" not in body for body in bodies))
        self.assertTrue(any(b"int second()" in body and b"int main()" not in body for body in bodies))
        self.assertTrue(any(b"int main()" in body and b"#endif" in body for body in bodies))
        executable = self.root / "check.exe"
        subprocess.run([compiler, "-std=c++20", str(path), "-o", str(executable)],
                       check=True, capture_output=True, text=True)
        subprocess.run([str(executable)], check=True, capture_output=True)


if __name__ == "__main__":
    unittest.main()
