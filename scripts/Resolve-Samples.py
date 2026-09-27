#!/usr/bin/env python3
"""Rank functions from a HostSampler histogram (GAIN_GROUND_SAMPLE output).

Reads `nm -n --demangle` of the sampled executable and its preferred image
base (`objdump -p`), maps each sampled offset to the enclosing symbol, and
prints the functions that account for the most samples.

Example:
  python scripts/Resolve-Samples.py build-prof/gain_ground_runtime.exe build-prof/samples.txt
"""
import bisect
import re
import subprocess
import sys


def symbols(exe):
    base_text = subprocess.run(['objdump', '-p', exe], capture_output=True, text=True, check=True).stdout
    base = int(re.search(r'ImageBase\s+([0-9a-f]+)', base_text).group(1), 16)
    table = []
    for line in subprocess.run(['nm', '-n', '--demangle', exe], capture_output=True, text=True, check=True).stdout.splitlines():
        parts = line.split(' ', 2)
        if len(parts) == 3 and parts[0] and parts[1] in 'tTwW':
            table.append((int(parts[0], 16) - base, parts[2]))
    table.sort()
    return [a for a, _ in table], [n for _, n in table]


def main():
    exe, histogram = sys.argv[1], sys.argv[2]
    limit = int(sys.argv[3]) if len(sys.argv) > 3 else 30
    addresses, names = symbols(exe)
    totals, total, outside = {}, 0, 0
    for line in open(histogram, encoding='utf-8'):
        if line.startswith('#'):
            print(line.strip())
            continue
        offset, count = line.split()
        offset, count = int(offset, 16), int(count)
        total += count
        if offset == 0xffffffff:
            outside += count
            continue
        i = bisect.bisect_right(addresses, offset) - 1
        name = names[i] if i >= 0 else '?'
        totals[name] = totals.get(name, 0) + count
    print(f'{total} samples, {outside} outside the executable ({100.0 * outside / max(total, 1):.1f}%)')
    for name, count in sorted(totals.items(), key=lambda kv: -kv[1])[:limit]:
        short = name if len(name) <= 110 else name[:107] + '...'
        print(f'{100.0 * count / max(total, 1):6.2f}%  {count:7d}  {short}')


if __name__ == '__main__':
    main()
