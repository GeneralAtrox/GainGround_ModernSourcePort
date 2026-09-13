"""Exercise the importer using a user-supplied set; no game data is bundled.

Usage: python scripts/Test-RomImport.py <gain_ground_rom_import.exe> <ROM folder>
The executable's runtime DLLs must be beside it or on PATH.
"""
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import zipfile


def main():
    executable, source = (Path(arg).resolve() for arg in sys.argv[1:])
    members = ("epr-12187.ic2", "epr-12186.ic1", "ds3-5000-03d-rev-a.img")
    results = []
    with tempfile.TemporaryDirectory(prefix="gain-ground-import-test-") as temporary:
        root = Path(temporary)

        def run(name, selected, cache, expected):
            process = subprocess.run([str(executable), str(selected), str(cache)],
                                     capture_output=True, text=True, check=False)
            result = {"case": name, "exit": process.returncode,
                      "expected": expected, "output": (process.stdout + process.stderr).strip()}
            results.append(result)
            if process.returncode != expected:
                raise AssertionError(result)

        run("extracted folder", source, root / "folder", 0)
        run("selected member", source / members[0], root / "member", 0)
        run("cache reuse", source, root / "folder", 0)
        for compression, name in ((zipfile.ZIP_STORED, "stored"), (zipfile.ZIP_DEFLATED, "deflated")):
            archive = root / (name + ".zip")
            with zipfile.ZipFile(archive, "w", compression=compression) as output:
                for member in members:
                    output.write(source / member, "gground/" + member)
            run(name + " ZIP", archive, root / name, 0)
        for kind in ("missing", "wrong-hash", "duplicate"):
            archive = root / (kind + ".zip")
            with zipfile.ZipFile(archive, "w", compression=zipfile.ZIP_DEFLATED) as output:
                for member in members:
                    if kind == "missing" and member == members[0]:
                        continue
                    data = (source / member).read_bytes()
                    if kind == "wrong-hash" and member == members[0]:
                        data = bytes([data[0] ^ 1]) + data[1:]
                    output.writestr(member, data)
                    if kind == "duplicate" and member == members[0]:
                        output.writestr("duplicate/" + member, data)
            run(kind, archive, root / "rejected", 1)
        damaged = bytearray((root / "stored.zip").read_bytes())
        # A stored member starts after the first local header and filename.
        start = 30 + int.from_bytes(damaged[26:28], "little") + int.from_bytes(damaged[28:30], "little")
        damaged[start] ^= 1
        (root / "bad-crc.zip").write_bytes(damaged)
        run("CRC failure", root / "bad-crc.zip", root / "rejected", 1)
        (root / "truncated.zip").write_bytes(damaged[:50])
        run("truncated ZIP", root / "truncated.zip", root / "rejected", 1)
        bios = root / "folder/cpu_a_program.bin"
        data = bios.read_bytes()
        bios.write_bytes(bytes([data[0] ^ 1]) + data[1:])
        run("corrupt cache is not reused or overwritten", source, root / "folder", 1)
        assert not (root / "rejected").exists()
        assert not list(root.glob("*.import-*"))
    print(json.dumps(results, indent=2))


if __name__ == "__main__":
    main()
