"""Check source-package boundaries and physical line counts; not legal clearance."""
from collections import Counter
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[1]
EXACT = {
    ".gitignore", ".gitattributes", "README.md", "NOTICE.md", "launch.bat",
    "native/README.md", "native/CMakeLists.txt", "docs/source-release.md",
    "scripts/Audit-SourcePackage.py", "scripts/Build-Source.ps1", "scripts/Test-RomImport.py",
    "docs/acceptance-baseline-20260926.md",
    "docs/audio-sample-contract-20260926.md",
    "docs/descriptor-timing-investigation-20260926.md",
    "docs/fixture-census-investigation-20260926.md",
    "docs/fixture-input-boundary-investigation-20260926.md",
    "docs/fixture-loop-comparator-repair-20260926.md",
    "docs/function103-lock-lifetime-20260926.md",
    "docs/function141-path-evidence-20260926.md",
    "docs/function141-timing-evidence-20260926.md",
    "docs/function324-interrupt-clock-20260926.md",
    "docs/function324-interrupt-entry-20260926.md",
    "docs/function324-interrupt-sampling-20260926.md",
    "docs/function324-register-phases-20260926.md",
    "docs/graphics-inventory-investigation-20260926.md",
    "docs/input-cadence-investigation-20260926.md",
    "docs/input-repair-live-validation-20260926.md",
    "docs/input-sampling-repair-20260926.md",
    "docs/irq3-contract-blockers-20260926.md",
    "docs/live-handoff-investigation-20260926.md",
    "docs/live-owner-boundary-investigation-20260926.md",
    "docs/live-render-clock-investigation-20260926.md",
    "docs/native-visual-writer-investigation-20260926.md",
    "docs/presentation-contract-audit-20260926.md",
    "docs/render-state-contract-audit-20260926.md",
    "scripts/Generate-GapTranslations.py",
    "scripts/Resolve-Samples.py",
    "scripts/Test-StageSelect.ps1",
    "scripts/native_source_layout.py", "scripts/test_native_source_layout.py",
    "native/CheckSourceLineLimit.cmake",
    "docs/source-line-split-20260927.md", "docs/source-line-split-20260927.json",
    "docs/native-game-loop-20261007.md", "docs/native-game-loop-20261007.json",
}
PREFIXES = ("native/include/", "native/src/", "native/generated/", "native/tests/", "native/third_party/")
EXTENSIONS = {".cpp", ".h", ".hpp", ".c", ".inc", ".ipp", ".cmake", ".txt"}
EXCLUDED = {"native/generated/gground_game_definitions.cpp"}


def permitted(name):
    return name not in EXCLUDED and not name.startswith(
        ("native/generated/gground_game_definitions.cpp.part",
         "native/generated/gground_game_definitions.cpp.module",
         "native/generated/gground_game_definitions.module")) and (name in EXACT or
        (name.startswith(PREFIXES) and Path(name).suffix in EXTENSIONS))


def main():
    names = subprocess.check_output(["git", "ls-files", "-z"], cwd=ROOT).decode().split("\0")
    names = [name for name in names if name]
    errors = []
    over = []
    native_count = 0
    total_bytes = 0
    for name in names:
        if not permitted(name):
            errors.append("Outside source allowlist: " + name)
            continue
        data = (ROOT / name).read_bytes()
        total_bytes += len(data)
        if b"\0" in data:
            errors.append("Binary content in source path: " + name)
        if name.startswith("native/") and Path(name).suffix in {".cpp", ".h", ".hpp", ".c", ".inc", ".ipp"}:
            native_count += 1
            lines = len(data.splitlines())
            if lines > 400:
                over.append({"path": name, "lines": lines})
    notice = ROOT / "native/third_party/SYSTEM24-NOTICE.txt"
    if not notice.exists() or "Redistribution and use" not in notice.read_text():
        errors.append("Missing System 24 redistribution notice")
    for path in (ROOT / "native/third_party/ymfm").glob("*.*"):
        if path.suffix in {".h", ".cpp"} and "Copyright" not in path.read_text():
            errors.append("Missing ymfm attribution: " + path.name)
    report = {"files": len(names), "source_bytes": total_bytes,
              "top_level": dict(sorted(Counter(n.split('/')[0] for n in names).items())),
              "native_code_files": native_count, "native_over_400": len(over),
              "largest_files": sorted(over, key=lambda row: -row["lines"]),
              "errors": errors,
              "scope": "Path/content packaging audit only. Translated code remains subject to provenance review."}
    print(json.dumps(report, indent=2))
    return int(bool(errors))


if __name__ == "__main__":
    raise SystemExit(main())
