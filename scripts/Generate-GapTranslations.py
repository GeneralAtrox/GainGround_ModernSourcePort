#!/usr/bin/env python3
"""Translate untranslated CPU-B (state 72) routines into the runtime's PC-switch style.

Reads the retained state-72 opcode bank (a private capture, never committed),
follows each requested entry's control flow (branches, jump tables of BRA.W
entries, DBcc loops), pulls in any BSR/JSR/JMP target that no registered
function owns as a further function, and emits:

  * a translated .cpp using unverified::Machine, one function per entry, in the
    same shape as the generated stage callbacks;
  * a registry header with source-owned FunctionContract entries for them.

Registered functions are read from the generated inventory and the pending
entries in native_function_registry.h, so callees resolve to their ids.
This is implementation-first translation, not fixture or parity evidence.

Example:
  python scripts/Generate-GapTranslations.py --entries 0x20046 0x200e0
"""
import argparse
import re
import sys
from pathlib import Path

try:
    from capstone import Cs, CS_ARCH_M68K, CS_MODE_M68K_000
except ImportError:  # pragma: no cover
    sys.exit("capstone is required: python -m pip install capstone")

ROOT = Path(__file__).resolve().parents[1]
BRANCHES = {
    'bra': 'true', 'bhi': '(r.status & 5U) == 0U', 'bls': '(r.status & 5U) != 0U',
    'bcc': '(r.status & 1U) == 0U', 'bhs': '(r.status & 1U) == 0U',
    'bcs': '(r.status & 1U) != 0U', 'blo': '(r.status & 1U) != 0U',
    'bne': '(r.status & 4U) == 0U', 'beq': '(r.status & 4U) != 0U',
    'bvc': '(r.status & 2U) == 0U', 'bvs': '(r.status & 2U) != 0U',
    'bpl': '(r.status & 8U) == 0U', 'bmi': '(r.status & 8U) != 0U',
    'bge': '(((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U',
    'blt': '(((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U',
    'bgt': '((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U)',
    'ble': '((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)',
}
SIZE_BITS = {1: 8, 2: 16, 4: 32}
MASK = {1: '0xffU', 2: '0xffffU', 4: '0xffffffffU'}


def load_registry(functions_header: Path, registry_header: Path, extra_headers):
    registered = {}
    text = functions_header.read_text(encoding='utf-8', errors='replace')
    for m in re.finditer(r'FunctionContract\{ (\d+)U, 1U, 0x72U, true, true, 0x0*([0-9a-f]+)U', text):
        registered[int(m.group(2), 16)] = int(m.group(1))
    for header in [registry_header, *extra_headers]:
        if not header.exists():
            continue
        text = header.read_text(encoding='utf-8', errors='replace')
        for m in re.finditer(r'\{(\d+)U, 0x0*([0-9a-f]+)U, 0x0*[0-9a-f]+U, "', text):
            registered[int(m.group(2), 16)] = int(m.group(1))
        for m in re.finditer(r'FunctionContract\{(\d+)U, 1U, 0x72U, false, true, 0x0*([0-9a-f]+)U', text):
            registered[int(m.group(2), 16)] = int(m.group(1))
    return registered


class Operand:
    def __init__(self, text):
        self.text = text.strip()
        t = self.text
        self.kind = None
        m = re.fullmatch(r'#(-?)\$?([0-9a-f]+)', t)
        if m:
            v = int(m.group(2), 16 if '$' in t else 10)
            self.kind, self.value = 'IMM', (-v if m.group(1) else v)
            return
        m = re.fullmatch(r'(d[0-7])', t)
        if m:
            self.kind, self.reg = 'D', int(t[1]); return
        m = re.fullmatch(r'(a[0-7]|sp)', t)
        if m:
            self.kind, self.reg = 'A', (7 if t == 'sp' else int(t[1])); return
        m = re.fullmatch(r'\((a[0-7]|sp)\)\+', t)
        if m:
            self.kind, self.reg = 'POST', (7 if m.group(1) == 'sp' else int(m.group(1)[1])); return
        m = re.fullmatch(r'-\((a[0-7]|sp)\)', t)
        if m:
            self.kind, self.reg = 'PRE', (7 if m.group(1) == 'sp' else int(m.group(1)[1])); return
        m = re.fullmatch(r'\((a[0-7]|sp)\)', t)
        if m:
            self.kind, self.reg, self.disp = 'DISP', (7 if m.group(1) == 'sp' else int(m.group(1)[1])), 0; return
        m = re.fullmatch(r'(-?)\$([0-9a-f]+)\((a[0-7]|sp)\)', t)
        if m:
            self.kind = 'DISP'; self.reg = 7 if m.group(3) == 'sp' else int(m.group(3)[1])
            self.disp = -int(m.group(2), 16) if m.group(1) else int(m.group(2), 16); return
        m = re.fullmatch(r'(?:(-?)\$([0-9a-f]+))?\((a[0-7]|sp|pc), ([da][0-7])\.([wl])\)', t)
        if m:
            disp = (-int(m.group(2), 16) if m.group(1) else int(m.group(2), 16)) if m.group(2) else 0
            base = m.group(3)
            self.kind = 'PCIDX' if base == 'pc' else 'IDX'
            self.reg = None if base == 'pc' else (7 if base == 'sp' else int(base[1]))
            self.disp = disp  # capstone prints PC-relative bases already resolved
            self.index_kind = m.group(4)[0]; self.index_reg = int(m.group(4)[1]); self.index_long = m.group(5) == 'l'
            return
        m = re.fullmatch(r'\$([0-9a-f]+)\(pc\)', t)
        if m:
            self.kind, self.value = 'ABS', int(m.group(1), 16); return
        m = re.fullmatch(r'\$([0-9a-f]+)\.w', t)
        if m:
            v = int(m.group(1), 16)
            self.kind, self.value = 'ABS', (v | 0xffff0000) if 0x8000 <= v <= 0xffff else v; return
        m = re.fullmatch(r'\$([0-9a-f]+)\.l', t)
        if m:
            self.kind, self.value = 'ABS', int(m.group(1), 16); return
        m = re.fullmatch(r'\$([0-9a-f]+)', t)
        if m:
            self.kind, self.value = 'ABS', int(m.group(1), 16); return
        raise ValueError(f'unsupported operand {t!r}')


def split_operands(op_str):
    parts, depth, cur = [], 0, ''
    for ch in op_str:
        if ch == '(':
            depth += 1
        elif ch == ')':
            depth -= 1
        if ch == ',' and depth == 0:
            parts.append(cur); cur = ''
        else:
            cur += ch
    if cur.strip():
        parts.append(cur)
    return [Operand(p) for p in parts]


class Emitter:
    def __init__(self):
        self.lines = []
        self.temp = 0

    def line(self, s):
        self.lines.append('            ' + s)

    def tmp(self, prefix='v'):
        self.temp += 1
        return f'{prefix}{self.temp}'

    # Returns a C++ expression for the effective address, emitting any
    # post-increment / pre-decrement side effects.
    def ea(self, op, size):
        step = size if not (op.kind in ('POST', 'PRE') and op.reg == 7 and size == 1) else 2
        if op.kind == 'DISP':
            return f'r.address[{op.reg}]' if op.disp == 0 else f'(r.address[{op.reg}] + {op.disp & 0xffffffff:#x}U)'
        if op.kind == 'POST':
            t = self.tmp('ea'); self.line(f'const auto {t} = r.address[{op.reg}]; r.address[{op.reg}] += {step}U;'); return t
        if op.kind == 'PRE':
            t = self.tmp('ea'); self.line(f'r.address[{op.reg}] -= {step}U; const auto {t} = r.address[{op.reg}];'); return t
        if op.kind in ('IDX', 'PCIDX'):
            idx = f'r.{"data" if op.index_kind == "d" else "address"}[{op.index_reg}]'
            idx = idx if op.index_long else f'static_cast<std::uint32_t>(static_cast<std::int16_t>({idx}))'
            base = f'r.address[{op.reg}]' if op.kind == 'IDX' else f'{op.disp:#x}U'
            disp = f' + {op.disp & 0xffffffff:#x}U' if op.kind == 'IDX' and op.disp else ''
            return f'({base}{disp} + {idx})'
        if op.kind == 'ABS':
            return f'{op.value & 0xffffffff:#x}U'
        raise ValueError(f'no effective address for {op.text}')

    def read(self, op, size):
        bits = SIZE_BITS[size]
        if op.kind == 'IMM':
            return f'{op.value & ((1 << bits) - 1):#x}U'
        if op.kind == 'D':
            return f'r.data[{op.reg}]' if size == 4 else f'(r.data[{op.reg}] & {MASK[size]})'
        if op.kind == 'A':
            return f'r.address[{op.reg}]' if size == 4 else f'(r.address[{op.reg}] & {MASK[size]})'
        fn = {1: 'byte', 2: 'word', 4: 'lng'}[size]
        return f'm.{fn}({self.ea(op, size)})'

    def write(self, op, size, value, ea=None):
        if op.kind == 'D':
            if size == 4:
                self.line(f'r.data[{op.reg}] = {value};')
            else:
                self.line(f'm.{"db" if size == 1 else "dw"}({op.reg}, {value});')
            return
        if op.kind == 'A':
            self.line(f'r.address[{op.reg}] = {value};' if size == 4 else
                      f'r.address[{op.reg}] = static_cast<std::uint32_t>(static_cast<std::int16_t>({value}));')
            return
        fn = {1: 'byte', 2: 'word', 4: 'lng'}[size]
        self.line(f'm.{fn}({ea or self.ea(op, size)}, {value});')


def size_of(ins):
    m = re.search(r'\.([bwl])$', ins.mnemonic)
    if m:
        return {'b': 1, 'w': 2, 'l': 4}[m.group(1)]
    if ins.mnemonic in ('moveq', 'lea', 'jmp', 'jsr', 'rts', 'nop', 'swap', 'bra', 'bsr'):
        return 4 if ins.mnemonic in ('moveq', 'lea', 'swap') else 2
    return ins.op_size.size or 2


class Translator:
    def __init__(self, data, registered, first_id, source_path):
        self.data = data
        self.registered = dict(registered)
        self.next_id = first_id
        self.source_path = source_path
        self.md = Cs(CS_ARCH_M68K, CS_MODE_M68K_000); self.md.detail = True
        self.functions = {}   # entry -> dict(id, name, insns{pc: ins}, body_max)
        self.warnings = []

    def decode(self, pc):
        return next(self.md.disasm(self.data[pc:pc + 12], pc), None)

    def base(self, ins):
        return re.sub(r'\.[bwls]$', '', ins.mnemonic)

    def target_of(self, op_str):
        m = re.fullmatch(r'\$([0-9a-f]+)', op_str.strip())
        return int(m.group(1), 16) if m else None

    def schedule(self, entry):
        if entry in self.registered or entry in self.functions:
            return
        fid = self.next_id; self.next_id += 1
        self.registered[entry] = fid
        self.functions[entry] = {'id': fid, 'name': f'cpu_b_stage_gap_{entry:06x}', 'insns': {}, 'pending': True}

    def follow(self, entry):
        f = self.functions[entry]
        insns = f['insns']
        work = [entry]
        while work:
            pc = work.pop()
            if pc in insns or pc < entry - 0x2000 or pc > entry + 0x4000:
                continue
            ins = self.decode(pc)
            if ins is None:
                self.warnings.append(f'{entry:05x}: undecodable at {pc:05x}')
                continue
            insns[pc] = ins
            base = self.base(ins); nxt = pc + ins.size
            if base in ('rts', 'rte'):
                continue
            if base == 'jmp':
                tgt = self.target_of(ins.op_str)
                if tgt is not None:
                    self.schedule(tgt); continue
                m = re.fullmatch(r'\$([0-9a-f]+)\(pc, d([0-7])\.w\)', ins.op_str.strip())
                if m:
                    table = int(m.group(1), 16)
                    at = table
                    while True:
                        e = self.decode(at)
                        if e is None or self.base(e) != 'bra' or at in insns:
                            break
                        insns[at] = e
                        work.append(self.target_of(e.op_str))
                        at += e.size
                    continue
                self.warnings.append(f'{entry:05x}: indirect jmp at {pc:05x}: {ins.op_str}')
                continue
            if base == 'bra':
                work.append(self.target_of(ins.op_str)); continue
            if base == 'bsr':
                self.schedule(self.target_of(ins.op_str)); work.append(nxt); continue
            if base == 'jsr':
                tgt = self.target_of(ins.op_str)
                if tgt is not None:
                    self.schedule(tgt)
                work.append(nxt); continue
            if base in BRANCHES:
                work.append(self.target_of(ins.op_str)); work.append(nxt); continue
            if base.startswith('db'):
                work.append(self.target_of(ins.op_str.split(',')[-1])); work.append(nxt); continue
            # A record callback installed by `move.l #routine, $2(An)` is code
            # the dispatcher will call later; translate it as its own entry.
            m = re.fullmatch(r'#\$([0-9a-f]+), \$2\(a[0-7]\)', ins.op_str.strip())
            if base == 'move' and ins.mnemonic.endswith('.l') and m:
                routine = int(m.group(1), 16)
                if 0x400 <= routine < len(self.data) and routine % 2 == 0:
                    self.schedule(routine)
            work.append(nxt)
        f['body_max'] = max(pc + ins.size - 1 for pc, ins in insns.items())
        f['pending'] = False

    def run(self, entries):
        for e in entries:
            if e in self.registered and e not in self.functions:
                print(f'note: {e:05x} is already registered as function {self.registered[e]}', file=sys.stderr)
                continue
            self.schedule(e)
        while True:
            pending = [e for e, f in self.functions.items() if f['pending']]
            if not pending:
                break
            for e in pending:
                self.follow(e)

    # ---- emission -------------------------------------------------------
    def emit_instruction(self, pc, ins, em, fn):
        base = self.base(ins); size = size_of(ins); bits = SIZE_BITS.get(size, 16)
        ops = split_operands(ins.op_str) if ins.op_str else []
        nxt = pc + ins.size
        em.line(f'next = {nxt:#x}U;')
        L = em.line
        if base in ('move', 'movea'):
            src, dst = ops
            v = em.tmp(); L(f'const auto {v} = {em.read(src, size)};')
            if dst.kind == 'A':
                em.write(dst, size, v)
            else:
                em.write(dst, size, v); L(f'm.logic({v}, {bits}U);')
        elif base == 'moveq':
            src, dst = ops
            L(f'r.data[{dst.reg}] = {src.value & 0xffffffff:#x}U; m.logic(r.data[{dst.reg}], 32U);')
        elif base == 'clr':
            (dst,) = ops
            if dst.kind in ('D', 'A'):
                em.write(dst, size, '0U')
            else:
                ea = em.ea(dst, size); L(f'(void)m.{ {1: "byte", 2: "word", 4: "lng"}[size] }({ea});'); em.write(dst, size, '0U', ea)
            L(f'm.logic(0U, {bits}U);')
        elif base == 'tst':
            (src,) = ops
            L(f'm.logic({em.read(src, size)}, {bits}U);')
        elif base == 'lea':
            src, dst = ops
            L(f'r.address[{dst.reg}] = {em.ea(src, 4)};')
        elif base in ('cmp', 'cmpi', 'cmpa'):
            src, dst = ops
            if base == 'cmpa' or dst.kind == 'A':
                s = em.read(src, size)
                s = s if size == 4 else f'static_cast<std::uint32_t>(static_cast<std::int16_t>({s}))'
                L(f'(void)m.sub(r.address[{dst.reg}], {s}, 32U, true);')
            else:
                L(f'(void)m.sub({em.read(dst, size)}, {em.read(src, size)}, {bits}U, true);')
        elif base in ('add', 'addi', 'addq', 'adda', 'sub', 'subi', 'subq', 'suba'):
            src, dst = ops
            add = base.startswith('add')
            if dst.kind == 'A':
                s = em.read(src, size)
                s = s if size == 4 or src.kind == 'IMM' else f'static_cast<std::uint32_t>(static_cast<std::int16_t>({s}))'
                L(f'r.address[{dst.reg}] {"+" if add else "-"}= {s};')
            else:
                ea = em.ea(dst, size) if dst.kind not in ('D', 'A') else None
                d = em.read(dst, size) if ea is None else f'm.{ {1: "byte", 2: "word", 4: "lng"}[size] }({ea})'
                v = em.tmp(); L(f'const auto {v} = m.{"add" if add else "sub"}({d}, {em.read(src, size)}, {bits}U);')
                em.write(dst, size, v, ea)
        elif base in ('and', 'andi', 'or', 'ori', 'eor', 'eori'):
            src, dst = ops
            opc = {'and': '&', 'or': '|', 'eor': '^'}[base.rstrip('i')]
            ea = em.ea(dst, size) if dst.kind not in ('D', 'A') else None
            d = em.read(dst, size) if ea is None else f'm.{ {1: "byte", 2: "word", 4: "lng"}[size] }({ea})'
            v = em.tmp(); L(f'const auto {v} = ({d} {opc} {em.read(src, size)}) & {MASK[size]};')
            em.write(dst, size, v, ea); L(f'm.logic({v}, {bits}U);')
        elif base in ('not', 'neg'):
            (dst,) = ops
            ea = em.ea(dst, size) if dst.kind not in ('D', 'A') else None
            d = em.read(dst, size) if ea is None else f'm.{ {1: "byte", 2: "word", 4: "lng"}[size] }({ea})'
            v = em.tmp()
            if base == 'not':
                L(f'const auto {v} = (~{d}) & {MASK[size]};'); em.write(dst, size, v, ea); L(f'm.logic({v}, {bits}U);')
            else:
                L(f'const auto {v} = m.sub(0U, {d}, {bits}U);'); em.write(dst, size, v, ea)
        elif base == 'ext':
            (dst,) = ops
            if size == 2:
                L(f'm.dw({dst.reg}, static_cast<std::uint32_t>(static_cast<std::int8_t>(r.data[{dst.reg}])) & 0xffffU); m.logic(r.data[{dst.reg}], 16U);')
            else:
                L(f'r.data[{dst.reg}] = static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[{dst.reg}])); m.logic(r.data[{dst.reg}], 32U);')
        elif base == 'swap':
            (dst,) = ops
            L(f'r.data[{dst.reg}] = (r.data[{dst.reg}] << 16U) | (r.data[{dst.reg}] >> 16U); m.logic(r.data[{dst.reg}], 32U);')
        elif base in ('asl', 'asr', 'lsl', 'lsr'):
            src, dst = ops
            if dst.kind != 'D' or size != 2 or src.kind != 'IMM':
                self.warnings.append(f'{pc:05x}: unsupported shift form {ins.mnemonic} {ins.op_str}')
                L('return {TranslationStatus::contract_violation, 0U, pc};')
            elif base == 'asl':
                L(f'm.asl_word({dst.reg}, {src.value}U);')
            else:
                L(f'm.shift_word({dst.reg}, {src.value}U, {"true" if base == "lsl" else "false"}, {"true" if base == "asr" else "false"});')
        elif base in ('rol', 'ror'):
            src, dst = ops
            if dst.kind != 'D' or size != 2 or src.kind != 'IMM':
                self.warnings.append(f'{pc:05x}: unsupported rotate form {ins.mnemonic} {ins.op_str}')
                L('return {TranslationStatus::contract_violation, 0U, pc};')
            elif base == 'ror':
                L(f'm.rotate_right({dst.reg}, {src.value}U, 16U);')
            else:
                # ROL.W: C is the last bit rotated out, which lands in bit 0; V clear.
                v = em.tmp(); c = src.value % 16
                L(f'const auto {v} = ((r.data[{dst.reg}] << {c}U) | ((r.data[{dst.reg}] & 0xffffU) >> {16 - c}U)) & 0xffffU;')
                L(f'm.dw({dst.reg}, {v}); m.logic({v}, 16U); r.status = static_cast<std::uint16_t>((r.status & ~1U) | ({v} & 1U));')
        elif base in ('btst', 'bset', 'bclr', 'bchg'):
            src, dst = ops
            if dst.kind == 'D':
                bit = f'{src.value}U' if src.kind == 'IMM' else f'(r.data[{src.reg}] & 31U)'
                v = em.tmp(); L(f'const auto {v} = r.data[{dst.reg}];')
                width = 32
            else:
                bit = f'{src.value & 7}U' if src.kind == 'IMM' else f'(r.data[{src.reg}] & 7U)'
                ea = em.ea(dst, 1); v = em.tmp(); L(f'const auto {v} = m.byte({ea});')
                width = 8
            L(f'r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((({v} >> {bit}) & 1U) ? 0U : 4U));')
            if base != 'btst':
                mod = {'bset': f'{v} | (1U << {bit})', 'bclr': f'{v} & ~(1U << {bit})', 'bchg': f'{v} ^ (1U << {bit})'}[base]
                if width == 32:
                    L(f'r.data[{dst.reg}] = {mod};')
                else:
                    L(f'm.byte({ea}, ({mod}) & 0xffU);')
        elif base == 'tas':
            (dst,) = ops
            ea = em.ea(dst, 1); v = em.tmp(); L(f'const auto {v} = m.byte({ea}); m.logic({v}, 8U); m.byte({ea}, {v} | 0x80U);')
        elif base in ('mulu', 'muls'):
            src, dst = ops
            s = em.read(src, 2)
            if base == 'mulu':
                L(f'r.data[{dst.reg}] = (r.data[{dst.reg}] & 0xffffU) * {s}; m.logic(r.data[{dst.reg}], 32U);')
            else:
                L(f'r.data[{dst.reg}] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[{dst.reg}])) * static_cast<std::int32_t>(static_cast<std::int16_t>({s}))); m.logic(r.data[{dst.reg}], 32U);')
        elif base in ('divu', 'divs'):
            src, dst = ops
            L(f'm.divide({dst.reg}, {em.read(src, 2)}, {"true" if base == "divs" else "false"});')
        elif base == 'nop':
            pass
        elif base == 'bsr':
            tgt = self.target_of(ins.op_str)
            L(f'const auto result = m.call(c, {self.registered[tgt]}U, {pc:#x}U, {tgt:#x}U, {nxt:#x}U);')
            # A callee may return to our own caller (control 8, both frames
            # popped, PC already at that return address): finish as an RTS.
            L('if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);')
            L('if (result.status != TranslationStatus::complete || result.control != 1U) return result;')
            L('next = r.program_counter;')
        elif base == 'jsr':
            tgt = self.target_of(ins.op_str)
            if tgt is not None:
                L(f'const auto result = m.call(c, {self.registered[tgt]}U, {pc:#x}U, {tgt:#x}U, {nxt:#x}U);')
            else:
                L(f'const auto result = m.indirect_call(c, {pc:#x}U, {em.ea(ops[0], 4)}, {nxt:#x}U);')
            # A callee may return to our own caller (control 8, both frames
            # popped, PC already at that return address): finish as an RTS.
            L('if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);')
            L('if (result.status != TranslationStatus::complete || result.control != 1U) return result;')
            L('next = r.program_counter;')
        elif base == 'jmp':
            tgt = self.target_of(ins.op_str)
            if tgt is not None:
                L(f'next = {tgt:#x}U; transfer_kind = 1U;')
            else:
                m = re.fullmatch(r'\$([0-9a-f]+)\(pc, d([0-7])\.w\)', ins.op_str.strip())
                L(f'next = {int(m.group(1), 16):#x}U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[{m.group(2)}])); transfer_kind = 1U;')
        elif base == 'rts':
            L('const auto result = m.ret();')
            L(f'if (auto event = m.interrupt(c, {pc:#x}U, r.program_counter)) return *event;')
            L('return result;')
        elif base in BRANCHES:
            tgt = self.target_of(ins.op_str)
            if base == 'bra':
                L(f'next = {tgt:#x}U; transfer_kind = 1U;')
            else:
                L(f'if ({BRANCHES[base]}) {{ next = {tgt:#x}U; transfer_kind = 1U; }}')
        elif base in ('dbf', 'dbra'):
            reg, tgt = ops[0], self.target_of(ins.op_str.split(',')[-1])
            v = em.tmp(); L(f'const auto {v} = static_cast<std::uint16_t>(r.data[{reg.reg}] - 1U); m.dw({reg.reg}, {v});')
            L(f'if ({v} != 0xffffU) {{ next = {tgt:#x}U; transfer_kind = 1U; }}')
        else:
            self.warnings.append(f'{pc:05x}: unsupported instruction {ins.mnemonic} {ins.op_str}')
            L('return {TranslationStatus::contract_violation, 0U, pc};')

    def emit_function(self, entry):
        f = self.functions[entry]
        out = []
        out.append(f'FunctionResult {f["name"]}(FunctionContext &c) noexcept {{')
        out.append('    auto &r = c.registers;')
        out.append('    if (!c.host || c.cpu != 1U || c.state != 0x72U)')
        out.append('        return {TranslationStatus::contract_violation, 0U, r.program_counter};')
        out.append('    unverified::Machine m{*c.host, r, 1U, 0x72U};')
        out.append('    for (;;) {')
        out.append('        const auto pc = r.program_counter;')
        out.append('        auto next = pc;')
        out.append('        std::uint8_t transfer_kind = 0U;')
        out.append('        switch (pc) {')
        for pc in sorted(f['insns']):
            ins = f['insns'][pc]
            em = Emitter()
            self.emit_instruction(pc, ins, em, f)
            out.append(f'        case {pc:#x}U: {{ // {ins.bytes.hex()} {ins.mnemonic} {ins.op_str}')
            out.extend(em.lines)
            out.append('            break;')
            out.append('        }')
        out.append('        default: return {TranslationStatus::contract_violation, 0U, pc};')
        out.append('        }')
        out.append('        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};')
        out.append('        r.program_counter = next;')
        out.append('        if (auto event = m.interrupt(c, pc, next)) return *event;')
        out.append('        switch (next) {')
        for pc in sorted(f['insns']):
            out.append(f'        case {pc:#x}U:')
        out.append('            break;')
        out.append('        default: return m.dispatch(c, pc, next, transfer_kind, m.state);')
        out.append('        }')
        out.append('    }')
        out.append('}')
        return '\n'.join(out)

    def emit_cpp(self):
        parts = ['// Generated by scripts/Generate-GapTranslations.py from the retained CPU-B',
                 '// state-72 opcode bank. Implementation-first translation of routines no',
                 '// captured run reached; not fixture or parity evidence. Do not edit by hand.',
                 '#include "gain_ground/stage_gap_registry.h"',
                 '#include "unverified_cpu_b_machine.h"',
                 '',
                 'namespace gain_ground::translated {',
                 '']
        for entry in sorted(self.functions):
            parts.append(self.emit_function(entry)); parts.append('')
        parts.append('} // namespace gain_ground::translated')
        return '\n'.join(parts) + '\n'

    def emit_header(self):
        lines = ['#pragma once',
                 '// Generated by scripts/Generate-GapTranslations.py. Source-owned registration',
                 '// of translated routines no captured run reached. Do not edit by hand.',
                 '#include "gain_ground/contract_types.h"',
                 '#include <array>',
                 '',
                 'namespace gain_ground::translated {']
        for entry in sorted(self.functions):
            lines.append(f'FunctionResult {self.functions[entry]["name"]}(FunctionContext &) noexcept;')
        lines += ['}', '', 'namespace gain_ground::stage_gap_registry {',
                  f'inline constexpr std::array<FunctionContract, {len(self.functions)}> kEntries{{{{']
        for entry in sorted(self.functions):
            f = self.functions[entry]
            lines.append(f'    FunctionContract{{{f["id"]}U, 1U, 0x72U, false, true, {entry:#x}U, {entry:#x}U, {f["body_max"]:#x}U, '
                         f'{f["body_max"] + 1 - entry}U, 0U, 0U, "cpu-b", "72", "unverified", "retained-opcode-source", '
                         f'"implementation-first", "implemented-but-unverified", "{f["name"]}", "{self.source_path}", '
                         f'&translated::{f["name"]}}},')
        lines += ['}};', '', 'inline const FunctionContract *find(std::uint8_t cpu, std::uint8_t state, std::uint32_t address) noexcept {',
                  '    for (const auto &entry : kEntries)',
                  '        if (entry.cpu == cpu && entry.state == state && entry.address == address) return &entry;',
                  '    return nullptr;', '}', '} // namespace gain_ground::stage_gap_registry', '']
        return '\n'.join(lines)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--opcodes', default=str(ROOT / 'capture' / 'cpu_b_opcodes.bin'))
    ap.add_argument('--functions', default=str(ROOT / 'native' / 'generated' / 'gground_functions.h'))
    ap.add_argument('--registry', default=str(ROOT / 'native' / 'include' / 'gain_ground' / 'native_function_registry.h'))
    ap.add_argument('--out-cpp', default=str(ROOT / 'native' / 'src' / 'translated' / 'cpu_b_stage_gap_callbacks.cpp'))
    ap.add_argument('--out-header', default=str(ROOT / 'native' / 'include' / 'gain_ground' / 'stage_gap_registry.h'))
    ap.add_argument('--first-id', type=int, default=649)
    ap.add_argument('--entries', nargs='+', required=True, help='entry addresses, hex')
    args = ap.parse_args()
    data = Path(args.opcodes).read_bytes()
    registered = load_registry(Path(args.functions), Path(args.registry), [])
    tr = Translator(data, registered, args.first_id, 'native/src/translated/cpu_b_stage_gap_callbacks.cpp')
    tr.run([int(e, 16) for e in args.entries])
    Path(args.out_cpp).write_text(tr.emit_cpp(), encoding='utf-8', newline='\n')
    Path(args.out_header).write_text(tr.emit_header(), encoding='utf-8', newline='\n')
    for entry in sorted(tr.functions):
        f = tr.functions[entry]
        print(f'{entry:05x} -> id {f["id"]} {f["name"]}: {len(f["insns"])} instructions, body to {f["body_max"]:05x}')
    for w in tr.warnings:
        print('WARNING', w)
    return 1 if tr.warnings else 0


if __name__ == '__main__':
    sys.exit(main())
