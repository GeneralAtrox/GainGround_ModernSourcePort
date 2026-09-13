#pragma once
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated::scene_timing {
// Pinned sr_nz_u: merge the high word without clearing V/C/X or reasserting Z.
inline void nz_high(CpuRegisters &r, std::uint16_t high) {
    r.status = static_cast<std::uint16_t>((r.status & ~0x0cU) |
        ((high & 0x8000U) ? 8U : 0U) | ((high == 0U && (r.status & 4U)) ? 4U : 0U));
}

inline void lsr_long(CpuBIrqTiming &t, unverified::Machine &m,
                     std::uint32_t pc, unsigned reg, unsigned count) {
    auto &r = m.r;
    auto value = r.data[reg];
    // srrl2 initially exposes the low word's flags.
    m.logic(value, 16U); t.prefetch(pc + 4U);
    for (unsigned n = 0; n < count; ++n) {
        const auto carry = value & 1U;
        value >>= 1U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) |
            (carry ? 0x11U : 0U) | (value == 0U ? 4U : 0U));
        t.clocks(2U);
    }
    r.data[reg] = value; t.clocks(2U);
    nz_high(r, static_cast<std::uint16_t>(value >> 16U)); t.clocks(2U);
}

// mulm3/mulm4 retain their data-dependent two-clock ALU phases. This is the
// unsigned reference shift/add sequence, including its accumulating Z flag.
inline void multiply_unsigned(CpuBIrqTiming &t, unverified::Machine &m,
                              std::uint32_t pc, unsigned reg, std::uint16_t immediate) {
    auto &r = m.r;
    t.prefetch(pc + 4U);
    const auto multiplicand = static_cast<std::uint16_t>(r.data[reg]);
    std::uint16_t high = 0, low = immediate;
    m.logic(0U, 16U); m.dw(reg, 0U); t.prefetch(pc + 6U);
    auto accumulate = [&](unsigned flags) {
        r.status = static_cast<std::uint16_t>((r.status & ~0x0bU & (flags | ~4U)) | (flags & 0x0bU));
    };
    for (unsigned n = 0; n < 16U; ++n) {
        bool carry = false;
        if (low & 1U) {
            const unsigned sum = unsigned(high) + multiplicand;
            const unsigned flags = ((sum & 0xffffU) == 0U ? 4U : 0U) |
                (sum & 0x8000U ? 8U : 0U) | (sum & 0x10000U ? 1U : 0U) |
                (((high & multiplicand & ~sum) | (~high & ~multiplicand & sum)) & 0x8000U ? 2U : 0U);
            high = static_cast<std::uint16_t>(sum);
            carry = (sum & 0x10000U) != 0U;
            accumulate(flags); t.clocks(2U);
        }
        const auto rotated = (std::uint32_t(high) << 15U) | (low >> 1U) | (carry ? 0x80000000U : 0U);
        high = static_cast<std::uint16_t>(rotated >> 16U);
        low = static_cast<std::uint16_t>(rotated);
        accumulate((high == 0U ? 4U : 0U) | (high & 0x8000U ? 8U : 0U)); t.clocks(2U);
    }
    r.data[reg] = (std::uint32_t(high) << 16U) | low;
    accumulate((high == 0U ? 4U : 0U) | (high & 0x8000U ? 8U : 0U)); t.clocks(2U);
}

// Shared ordinary-path adapters for generated scene candidates. Unverified.
// The existing timing scope still reports incomplete opcode/IPL observations.
inline bool condition(std::uint16_t sr, unsigned cc) {
    const bool c = (sr & 1U) != 0, v = (sr & 2U) != 0;
    const bool z = (sr & 4U) != 0, n = (sr & 8U) != 0;
    switch (cc) {
    case 0: return true; case 1: return false;
    case 2: return !c && !z; case 3: return c || z;
    case 4: return !c; case 5: return c;
    case 6: return !z; case 7: return z;
    case 8: return !v; case 9: return v;
    case 10: return !n; case 11: return n;
    case 12: return n == v; case 13: return n != v;
    case 14: return !z && n == v; case 15: return z || n != v;
    }
    return false;
}

// ADDQ/ADDA.W to An commits the low half before prefetch, then the high half.
inline void address_add(CpuBIrqTiming &t, CpuRegisters &r, unsigned reg,
                        std::uint32_t value, std::uint32_t prefetch) {
    const auto result = r.address[reg] + value;
    r.address[reg] = (r.address[reg] & 0xffff0000U) | (result & 0xffffU);
    t.prefetch(prefetch); t.clocks(2U);
    r.address[reg] = result; t.clocks(2U);
}

// Word shifts: sriw/srrw -> srrw3 iterations -> nbcr3. The register is
// committed after the loop; flags are committed at every two-clock ALU step.
inline void shift_word(CpuBIrqTiming &t, unverified::Machine &m,
                       std::uint32_t pc, unsigned reg, unsigned count,
                       bool left, bool arithmetic) {
    auto &r = m.r;
    auto value = static_cast<std::uint16_t>(r.data[reg]);
    count &= 63U;
    m.logic(value, 16U); t.prefetch(pc + 4U);
    for (unsigned i = 0; i < count; ++i) {
        const auto old = value;
        const bool carry = (old & (left ? 0x8000U : 1U)) != 0;
        const auto shifted = static_cast<std::uint16_t>(left ? old << 1U : old >> 1U);
        value = static_cast<std::uint16_t>(shifted |
            (!left && arithmetic ? old & 0x8000U : 0U));
        // Pinned alu_asl retains V between steps. alu_asr computes Z before
        // extending the sign; retain that reference operation order here.
        const bool overflow = left && arithmetic &&
            (((r.status & 2U) != 0U) || ((old ^ value) & 0x8000U));
        r.status = static_cast<std::uint16_t>((r.status & ~0x1fU) |
            (carry ? 0x11U : 0U) | (overflow ? 2U : 0U) |
            (shifted == 0U ? 4U : 0U) | (value & 0x8000U ? 8U : 0U));
        t.clocks(2U);
    }
    m.dw(reg, value); t.clocks(2U);
}

enum class CallForm { bsr, jsr_indirect, jsr_pc, jsr_absolute };
inline FunctionResult call(FunctionContext &c, CpuBIrqTiming &t,
                            unverified::Machine &m, std::uint32_t pc,
                            std::uint32_t target, std::uint32_t next, CallForm form) {
    auto &r = c.registers;
    if (form == CallForm::bsr || form == CallForm::jsr_pc) t.clocks(2U);
    else if (form == CallForm::jsr_absolute) t.prefetch(pc + 4U);
    if (form != CallForm::bsr) t.prefetch(target);
    r.address[7] -= 4U;
    t.word(r.address[7], static_cast<std::uint16_t>(next >> 16U));
    t.word(r.address[7] + 2U, static_cast<std::uint16_t>(next));
    if (form == CallForm::bsr) t.prefetch(target);
    t.prefetch(target + 2U);
    r.program_counter = target; t.stop();
    if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
    if (const auto event = m.interrupt(c, pc, target)) return *event;
    // Resolve the actual calculated target; no guessed dispatch table entry.
    const auto result = m.dispatch(c, pc, target, 2U, c.state);
    if (result.status != TranslationStatus::complete || result.control != 1U) return result;
    if (result.exit_program_counter != r.program_counter || c.state != m.state)
        return {TranslationStatus::contract_violation, result.control, r.program_counter};
    // Nonlocal children own their actual SP and return PC. Do not normalize.
    return result;
}
} // namespace gain_ground::translated::scene_timing
