// Stage object callbacks 13EF2..14015 (CPU B, state 72). Stage slot lists
// install these as the record callback (+2) of timed spawners and animated
// scenery; no captured run reached them (first seen loading Round 1 Stage 6
// through the Stage menu). Translated from the retained state-72 opcode bank:
//
//   13EF2 spawner A: tst.w $1e(a5); bne 13F12; tst.w $46(a5); bmi 13F66;
//         move.b #4,$b(a5); move.w #1,$1e(a5); move.w #1,$9e(a5); bra 13F66
//   13F12 subq.w #1,$22(a5); bgt 13F66; move.w #$1e,$22(a5); bclr #0,(a5);
//         addq.w #1,$6(a5); addq.w #1,$1e(a5); lea 15C8C(pc),a0;
//         move.w $1e(a5),d0; subq.w #2,d0; bne 13F58;
//         move.w (a0)+,$e(a5); move.w (a0)+,$2a(a5); move.w (a0)+,$2c(a5);
//         move.w #$3a,d0; tst.w $c00.w; beq 13F50; move.w #$3c,d0;
//   13F50 jsr 1700C; bra 13F66
//   13F58 move.w $6(a0),$2c(a5); move.l #13F66,$2(a5)
//   13F66 jsr 15DF2; rts
//   13F6E spawner B: tst.w $1e(a5); beq 13FB8; subq.w #1,$22(a5); bgt 13FB8;
//         move.w #$1e,$22(a5); bclr #0,(a5); addq.w #1,$6(a5); addq.w #1,$1e(a5);
//         lea 15C94(pc),a0; move.w $1e(a5),d0; subq.w #2,d0; bne 13FA6;
//         move.w (a0)+,$e(a5); move.w (a0)+,$2a(a5); move.w (a0)+,$2c(a5); bra 13FB8
//   13FA6 tas.b $d0d.w; move.w $6(a0),$2a(a5); move.l #13FB8,$2(a5)
//   13FB8 jsr 15DF2; rts
//   13FC0 cycle 4: bchg #0,$22(a5); bne 13FE0; addq.w #2,$20(a5);
//         andi.w #6,$20(a5); move.w $20(a5),d0; lea 15C9C(pc),a0;
//         move.w (a0,d0.w),$8(a5)
//   13FE0 jsr 15DF2; rts
//   13FE8 cycle 7: bchg #0,$22(a5); bne 1400E; addq.w #2,$20(a5);
//         cmpi.w #$e,$20(a5); bmi 14000; clr.w $20(a5)
//   14000 move.w $20(a5),d0; lea 15CA4(pc),a0; move.w (a0,d0.w),$8(a5)
//   1400E jsr 15DF2; rts
//
// 15DF2 is F282 (sorted position entry) and 1700C is F309 (sound entry).
#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U, kMask = 0xffffU;
constexpr std::uint32_t kSortedEntry = 0x00015df2U, kSoundEntry = 0x0001700cU;
constexpr std::uint32_t kSortedEntryFunction = 282U, kSoundEntryFunction = 309U;

std::uint16_t rw(ExecutionHost &h, std::uint32_t a) { return h.read_memory_word(kPrivate, a & 0xffffffU, kMask); }
void ww(ExecutionHost &h, std::uint32_t a, std::uint16_t v) { h.write_memory_word(kPrivate, a & 0xffffffU, v, kMask); }
std::uint8_t rb(ExecutionHost &h, std::uint32_t a)
{
    const bool odd = (a & 1U) != 0U;
    const auto w = h.read_memory_word(kPrivate, (a & ~1U) & 0xffffffU, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? w : w >> 8U);
}
void wb(ExecutionHost &h, std::uint32_t a, std::uint8_t v)
{
    const bool odd = (a & 1U) != 0U;
    h.write_memory_word(kPrivate, (a & ~1U) & 0xffffffU, static_cast<std::uint16_t>(odd ? v : v << 8U), odd ? 0x00ffU : 0xff00U);
}
void wl(ExecutionHost &h, std::uint32_t a, std::uint32_t v) { ww(h, a, static_cast<std::uint16_t>(v >> 16U)); ww(h, a + 2U, static_cast<std::uint16_t>(v)); }
// MOVE/TST/AND/CLR condition codes: N and Z from the result, V and C clear, X kept.
void nz(CpuRegisters &r, std::uint32_t v, unsigned bits)
{
    const std::uint32_t sign = 1U << (bits - 1U), mask = bits == 32U ? 0xffffffffU : (1U << bits) - 1U;
    std::uint16_t f = r.status & 0x10U;
    if (v & sign) f |= 8U;
    if ((v & mask) == 0U) f |= 4U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | f);
}
// Bit instructions set only Z, from the tested bit before the change.
void zbit(CpuRegisters &r, bool was_clear) { r.status = static_cast<std::uint16_t>((r.status & ~4U) | (was_clear ? 4U : 0U)); }
void add_word(ExecutionHost &h, CpuRegisters &r, std::uint32_t a, std::uint16_t n)
{
    const auto old = rw(h, a);
    const auto v = static_cast<std::uint16_t>(old + n);
    ww(h, a, v);
    const bool carry = static_cast<std::uint32_t>(old) + n > 0xffffU;
    const bool overflow = ((~(old ^ n)) & (old ^ v) & 0x8000U) != 0U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U) | (v == 0U ? 4U : 0U) | (v & 0x8000U ? 8U : 0U));
}
// SUBQ.W #1 with the BGT decision (Z clear and N equals V).
bool decrement_word_gt(ExecutionHost &h, CpuRegisters &r, std::uint32_t a)
{
    const auto old = rw(h, a);
    const auto v = static_cast<std::uint16_t>(old - 1U);
    ww(h, a, v);
    const bool carry = old < 1U;
    const bool overflow = ((old ^ 1U) & (old ^ v) & 0x8000U) != 0U;
    const bool negative = (v & 0x8000U) != 0U, zero = v == 0U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U) | (zero ? 4U : 0U) | (negative ? 8U : 0U));
    return !zero && negative == overflow;
}
FunctionResult call(FunctionContext &c, std::uint32_t id, std::uint32_t site, std::uint32_t target)
{
    auto &h = *c.host; auto &r = c.registers;
    r.address[7] -= 4U;
    wl(h, r.address[7], site + 6U);
    r.program_counter = target;
    return h.call_function(id, 1U, 0x72U, 2U, site, target, c);
}
FunctionResult finish(FunctionContext &c)
{
    auto &h = *c.host; auto &r = c.registers;
    const auto target = (static_cast<std::uint32_t>(rw(h, r.address[7])) << 16U) | rw(h, r.address[7] + 2U);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
// Shared tail: JSR 15DF2 at `site`, then RTS.
FunctionResult sorted_entry_and_return(FunctionContext &c, std::uint32_t site)
{
    const auto child = call(c, kSortedEntryFunction, site, kSortedEntry);
    if (child.status != TranslationStatus::complete || child.control != 1U) return child;
    return finish(c);
}
bool valid(const FunctionContext &c, std::uint32_t entry) { return c.host != nullptr && c.registers.program_counter == entry; }
FunctionResult reject(const FunctionContext &c) { return {TranslationStatus::contract_violation, 0U, c.registers.program_counter}; }

// 13F12..13F65 and 13F74..13FB7 share one shape; `table` and the field the
// later steps update differ, and only spawner B raises $d0d.
FunctionResult spawner_step(FunctionContext &c, std::uint32_t table, std::uint32_t later_field,
                            std::uint32_t tail, bool raise_d0d, bool sound)
{
    auto &h = *c.host; auto &r = c.registers; const auto a5 = r.address[5];
    if (decrement_word_gt(h, r, a5 + 0x22U)) return sorted_entry_and_return(c, tail);
    ww(h, a5 + 0x22U, 0x1eU); nz(r, 0x1eU, 16U);
    const auto flags = rb(h, a5); wb(h, a5, static_cast<std::uint8_t>(flags & ~1U)); zbit(r, (flags & 1U) == 0U);
    add_word(h, r, a5 + 0x6U, 1U);
    add_word(h, r, a5 + 0x1eU, 1U);
    r.address[0] = table;
    auto d0 = static_cast<std::uint16_t>(rw(h, a5 + 0x1eU)); nz(r, d0, 16U);
    const auto step = static_cast<std::uint16_t>(d0 - 2U);
    const bool carry = d0 < 2U, overflow = ((d0 ^ 2U) & (d0 ^ step) & 0x8000U) != 0U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U) | (step == 0U ? 4U : 0U) | (step & 0x8000U ? 8U : 0U));
    r.data[0] = (r.data[0] & 0xffff0000U) | step;
    if (step == 0U) {
        for (const auto field : {0xeU, 0x2aU, 0x2cU}) {
            const auto v = rw(h, r.address[0]); r.address[0] += 2U; ww(h, a5 + field, v); nz(r, v, 16U);
        }
        if (sound) {
            std::uint16_t cue = 0x3aU; nz(r, cue, 16U);
            const auto bank = rw(h, 0xc00U); nz(r, bank, 16U);
            if (bank != 0U) { cue = 0x3cU; nz(r, cue, 16U); }
            r.data[0] = (r.data[0] & 0xffff0000U) | cue;
            const auto child = call(c, kSoundEntryFunction, 0x13f50U, kSoundEntry);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
        }
        return sorted_entry_and_return(c, tail);
    }
    if (raise_d0d) { const auto old = rb(h, 0xd0dU); wb(h, 0xd0dU, static_cast<std::uint8_t>(old | 0x80U)); nz(r, old, 8U); }
    const auto later = rw(h, r.address[0] + 6U); ww(h, a5 + later_field, later); nz(r, later, 16U);
    wl(h, a5 + 2U, tail); nz(r, tail, 32U);
    return sorted_entry_and_return(c, tail);
}
} // namespace

FunctionResult cpu_b_stage_object_spawner_a(FunctionContext &c) noexcept
{
    if (!valid(c, 0x13ef2U)) return reject(c);
    auto &h = *c.host; auto &r = c.registers; const auto a5 = r.address[5];
    const auto armed = rw(h, a5 + 0x1eU); nz(r, armed, 16U);
    if (armed != 0U) return spawner_step(c, 0x15c8cU, 0x2cU, 0x13f66U, false, true);
    const auto bounds = rw(h, a5 + 0x46U); nz(r, bounds, 16U);
    if (bounds & 0x8000U) return sorted_entry_and_return(c, 0x13f66U);
    wb(h, a5 + 0xbU, 4U); nz(r, 4U, 8U);
    ww(h, a5 + 0x1eU, 1U); nz(r, 1U, 16U);
    ww(h, a5 + 0x9eU, 1U); nz(r, 1U, 16U);
    return sorted_entry_and_return(c, 0x13f66U);
}

FunctionResult cpu_b_stage_object_spawner_a_tail(FunctionContext &c) noexcept
{
    if (!valid(c, 0x13f66U)) return reject(c);
    return sorted_entry_and_return(c, 0x13f66U);
}

FunctionResult cpu_b_stage_object_spawner_b(FunctionContext &c) noexcept
{
    if (!valid(c, 0x13f6eU)) return reject(c);
    auto &h = *c.host; auto &r = c.registers; const auto a5 = r.address[5];
    const auto armed = rw(h, a5 + 0x1eU); nz(r, armed, 16U);
    if (armed == 0U) return sorted_entry_and_return(c, 0x13fb8U);
    return spawner_step(c, 0x15c94U, 0x2aU, 0x13fb8U, true, false);
}

FunctionResult cpu_b_stage_object_spawner_b_tail(FunctionContext &c) noexcept
{
    if (!valid(c, 0x13fb8U)) return reject(c);
    return sorted_entry_and_return(c, 0x13fb8U);
}

FunctionResult cpu_b_stage_object_cycle_four_frames(FunctionContext &c) noexcept
{
    if (!valid(c, 0x13fc0U)) return reject(c);
    auto &h = *c.host; auto &r = c.registers; const auto a5 = r.address[5];
    const auto toggle = rb(h, a5 + 0x22U); wb(h, a5 + 0x22U, static_cast<std::uint8_t>(toggle ^ 1U)); zbit(r, (toggle & 1U) == 0U);
    if (toggle & 1U) return sorted_entry_and_return(c, 0x13fe0U);
    add_word(h, r, a5 + 0x20U, 2U);
    const auto phase = static_cast<std::uint16_t>(rw(h, a5 + 0x20U) & 6U); ww(h, a5 + 0x20U, phase); nz(r, phase, 16U);
    r.data[0] = (r.data[0] & 0xffff0000U) | phase;
    r.address[0] = 0x15c9cU;
    const auto frame = rw(h, r.address[0] + phase); ww(h, a5 + 0x8U, frame); nz(r, frame, 16U);
    return sorted_entry_and_return(c, 0x13fe0U);
}

FunctionResult cpu_b_stage_object_cycle_seven_frames(FunctionContext &c) noexcept
{
    if (!valid(c, 0x13fe8U)) return reject(c);
    auto &h = *c.host; auto &r = c.registers; const auto a5 = r.address[5];
    const auto toggle = rb(h, a5 + 0x22U); wb(h, a5 + 0x22U, static_cast<std::uint8_t>(toggle ^ 1U)); zbit(r, (toggle & 1U) == 0U);
    if (toggle & 1U) return sorted_entry_and_return(c, 0x1400eU);
    add_word(h, r, a5 + 0x20U, 2U);
    auto phase = rw(h, a5 + 0x20U);
    if (static_cast<std::int16_t>(phase) - 0xe >= 0) { phase = 0U; ww(h, a5 + 0x20U, 0U); }
    nz(r, phase, 16U);
    r.data[0] = (r.data[0] & 0xffff0000U) | phase;
    r.address[0] = 0x15ca4U;
    const auto frame = rw(h, r.address[0] + phase); ww(h, a5 + 0x8U, frame); nz(r, frame, 16U);
    return sorted_entry_and_return(c, 0x1400eU);
}

} // namespace gain_ground::translated
