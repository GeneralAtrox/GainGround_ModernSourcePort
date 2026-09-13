#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kStatusMask = 0x001fU;
constexpr std::uint16_t kExtendBit = 0x0010U;
constexpr std::uint16_t kNegativeBit = 0x0008U;
constexpr std::uint16_t kZeroBit = 0x0004U;
constexpr std::uint16_t kOverflowBit = 0x0002U;
constexpr std::uint16_t kCarryBit = 0x0001U;

[[nodiscard]] bool get_extend(CpuRegisters &registers)
{
    return (registers.status & kExtendBit) != 0U;
}

void set_extend(CpuRegisters &registers, bool value)
{
    if (value)
        registers.status |= kExtendBit;
    else
        registers.status &= static_cast<std::uint16_t>(~kExtendBit);
}

void commit_ccr(CpuRegisters &registers, bool extend,
                bool carry, bool overflow, bool zero, bool negative)
{
    std::uint16_t flags = extend ? kExtendBit : 0U;
    if (negative) flags |= kNegativeBit;
    if (zero) flags |= kZeroBit;
    if (overflow) flags |= kOverflowBit;
    if (carry) flags |= kCarryBit;
    registers.status =
        static_cast<std::uint16_t>((registers.status & ~kStatusMask) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kRegion,
        registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion,
        registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_signed_restoring_divide(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;

    // 0x1fc4a: moveq #0,d4
    // Full-register store: d4 = $00000000. Flags: N=0 Z=1 V=0 C=0, X preserved.
    r.data[4] = 0x00000000U;
    {
        const bool ext = get_extend(r);
        commit_ccr(r, ext, false, false, true, false);
    }

    // 0x1fc4c: tst.w d3w
    // Tests low word only. N = bit15(d3w), Z = (d3w == 0), V=0 C=0, X preserved.
    {
        const auto value = static_cast<std::uint16_t>(r.data[3] & 0xffffU);
        const bool ext = get_extend(r);
        commit_ccr(r, ext, false, false, value == 0U, (value & 0x8000U) != 0U);
    }

    // 0x1fc4e: bpl.b 0x1fc56 — taken when N==0 (dividend non-negative).
    // Falls through when N==1 (dividend negative).
    const bool dividend_negative = (r.status & kNegativeBit) != 0U;

    if (dividend_negative) {
        // 0x1fc50: neg.w d3w (one's complement + 1)
        // X=C=borrow (=1 iff operand != 0), Z=(result==0), N=bit15(result), V=(result==$8000).
        {
            auto value = static_cast<std::uint16_t>(r.data[3] & 0xffffU);
            const auto result = static_cast<std::uint16_t>(0U - value);
            const bool borrow = value != 0U;
            const bool overflow = result == 0x8000U;
            r.data[3] = (r.data[3] & 0xffff0000U) | result;
            set_extend(r, borrow);
            commit_ccr(r, borrow, borrow, overflow, result == 0U,
                (result & 0x8000U) != 0U);
        }

        // 0x1fc52: move.w #-1,d4w
        // Upper word preserved; N=1 Z=0 V=0 C=0, X preserved.
        r.data[4] = (r.data[4] & 0xffff0000U) | 0xffffU;
        {
            const bool ext = get_extend(r);
            commit_ccr(r, ext, false, false, false, true);
        }
    }

    // 0x1fc56: moveq #7,d0
    // Full-register store (sign-extended): d0 = $00000007.
    // Flags: N=0 Z=0 V=0 C=0, X preserved.
    r.data[0] = 0x00000007U;
    {
        const bool ext = get_extend(r);
        commit_ccr(r, ext, false, false, false, false);
    }

    // 0x1fc58: moveq #0,d1 — d1 = $00000000; flags as above.
    r.data[1] = 0x00000000U;
    {
        const bool ext = get_extend(r);
        commit_ccr(r, ext, false, false, true, false);
    }

    // 0x1fc5a: moveq #0,d2 — d2 = $00000000; flags as above.
    r.data[2] = 0x00000000U;
    {
        const bool ext = get_extend(r);
        commit_ccr(r, ext, false, false, true, false);
    }

    // Restoring divide loop: 8 iterations via dbf on d0w starting from 7.
    // DBF decrements the full word and branches while != $ffff.
    // From initial 7: passes at d0w = {7→6→br, 6→5→br, ..., 1→0→br, 0→ffff→fall}.
    // Total: exactly 8 iterations of body, then fall-through.
    for (unsigned iteration = 0; iteration < 8U; ++iteration) {
        // 0x1fc5c: lsl.w #1,d3w
        // X = old bit15; result <<= 1; N = new bit15; Z = (result==0); V=C=0.
        {
            auto value = static_cast<std::uint16_t>(r.data[3] & 0xffffU);
            const bool msb = (value & 0x8000U) != 0U;
            value = static_cast<std::uint16_t>(value << 1U);
            r.data[3] = (r.data[3] & 0xffff0000U) | value;
            set_extend(r, msb);
            commit_ccr(r, msb, msb, false, value == 0U,
                (value & 0x8000U) != 0U);
        }

        // 0x1fc5e: roxl.w #1,d1w
        // 17-bit rotate through X: new bit0 = old X, new X = old bit15(d1).
        // N/Z per result; V=C=0 (X holds the carry conceptually).
        {
            auto value = static_cast<std::uint16_t>(r.data[1] & 0xffffU);
            const bool msb = (value & 0x8000U) != 0U;
            const bool xin = get_extend(r);
            value = static_cast<std::uint16_t>((value << 1U) | (xin ? 1U : 0U));
            r.data[1] = (r.data[1] & 0xffff0000U) | value;
            set_extend(r, msb);
            commit_ccr(r, msb, msb, false, value == 0U,
                (value & 0x8000U) != 0U);
        }

        // 0x1fc60: lsl.w #1,d3w (second shift — extracts next dividend bit)
        {
            auto value = static_cast<std::uint16_t>(r.data[3] & 0xffffU);
            const bool msb = (value & 0x8000U) != 0U;
            value = static_cast<std::uint16_t>(value << 1U);
            r.data[3] = (r.data[3] & 0xffff0000U) | value;
            set_extend(r, msb);
            commit_ccr(r, msb, msb, false, value == 0U,
                (value & 0x8000U) != 0U);
        }

        // 0x1fc62: roxl.w #1,d1w
        {
            auto value = static_cast<std::uint16_t>(r.data[1] & 0xffffU);
            const bool msb = (value & 0x8000U) != 0U;
            const bool xin = get_extend(r);
            value = static_cast<std::uint16_t>((value << 1U) | (xin ? 1U : 0U));
            r.data[1] = (r.data[1] & 0xffff0000U) | value;
            set_extend(r, msb);
            commit_ccr(r, msb, msb, false, value == 0U,
                (value & 0x8000U) != 0U);
        }

        // 0x1fc64: ori #0x10,ccr — force X=1 for trial-bit injection into quotient.
        // Sets X=1; other CCR bits unchanged.
        set_extend(r, true);

        // 0x1fc68: roxl.w #1,d2w
        // Inject trial bit (X=1) into quotient LSB.
        {
            auto value = static_cast<std::uint16_t>(r.data[2] & 0xffffU);
            const bool msb = (value & 0x8000U) != 0U;
            value = static_cast<std::uint16_t>((value << 1U) | 1U);
            r.data[2] = (r.data[2] & 0xffff0000U) | value;
            set_extend(r, msb);
            commit_ccr(r, msb, msb, false, value == 0U,
                (value & 0x8000U) != 0U);
        }

        // 0x1fc6a: sub.w d2w,d1w
        // Trial subtract: d1 = d1 - d2.
        // X=C=borrow(d1<d2), V=signed overflow, N/Z per result.
        {
            const auto src = static_cast<std::uint16_t>(r.data[2] & 0xffffU);
            auto dst = static_cast<std::uint16_t>(r.data[1] & 0xffffU);
            const auto result = static_cast<std::uint16_t>(dst - src);
            const bool borrow = dst < src;
            const bool overflow = ((dst ^ src) & (dst ^ result) & 0x8000U) != 0U;
            r.data[1] = (r.data[1] & 0xffff0000U) | result;
            set_extend(r, borrow);
            commit_ccr(r, borrow, borrow, overflow, result == 0U,
                (result & 0x8000U) != 0U);
        }

        // 0x1fc6c: bcc.b 0x1fc74 — branch when C==0 (no borrow → subtract fit).
        if ((r.status & kCarryBit) == 0U) {
            // 0x1fc74: addq.w #1,d2w — commit quotient bit.
            {
                const auto src = 1U;
                auto dst = static_cast<std::uint16_t>(r.data[2] & 0xffffU);
                const auto result = static_cast<std::uint16_t>(dst + src);
                r.data[2] = (r.data[2] & 0xffff0000U) | result;
                const bool carry = result < src;
                commit_ccr(r, carry, carry,
                    ((~(dst ^ src)) & (dst ^ result) & 0x8000U) != 0U,
                    result == 0U, (result & 0x8000U) != 0U);
            }
        } else {
            // 0x1fc6e: add.w d2w,d1w — restore remainder (undo trial subtract).
            {
                const auto src = static_cast<std::uint16_t>(r.data[2] & 0xffffU);
                auto dst = static_cast<std::uint16_t>(r.data[1] & 0xffffU);
                const auto result = static_cast<std::uint16_t>(dst + src);
                r.data[1] = (r.data[1] & 0xffff0000U) | result;
                const bool carry = result < src;
                const bool overflow = ((~(dst ^ src)) & (dst ^ result) & 0x8000U) != 0U;
                set_extend(r, carry);
                commit_ccr(r, carry, carry, overflow, result == 0U,
                    (result & 0x8000U) != 0U);
            }
            // 0x1fc70: subq.w #1,d2w — undo tentative quotient bit.
            {
                auto dst = static_cast<std::uint16_t>(r.data[2] & 0xffffU);
                const auto result = static_cast<std::uint16_t>(dst - 1U);
                r.data[2] = (r.data[2] & 0xffff0000U) | result;
                const bool borrow = dst < 1U;
                commit_ccr(r, borrow, borrow, dst != 0U && result == 0x7fffU,
                    result == 0U, (result & 0x8000U) != 0U);
            }
            // 0x1fc72: bra.b 0x1fc76 — unconditional, joins post-subtract path.
        }
        // 0x1fc76: dbf d0w,0x1fc5c
        // Decrement d0w (word only); branch while != $ffff. No CCR change.
        // After 8 iterations from 7: d0w reaches $ffff → fall through.
        // Final d0 upper word remains $00000000 (from moveq).
        {
            auto counter = static_cast<std::uint16_t>(r.data[0] & 0xffffU);
            counter = static_cast<std::uint16_t>(counter - 1U);
            r.data[0] = (r.data[0] & 0xffff0000U) | counter;
        }
    }
    // 0x1fc7a: lsr.w #1,d2w
    // Logical shift right: X=old bit0, N=0 always, Z=(result==0), V=C=0.
    {
        auto value = static_cast<std::uint16_t>(r.data[2] & 0xffffU);
        const bool lsb = (value & 1U) != 0U;
        value = static_cast<std::uint16_t>(value >> 1U);
        r.data[2] = (r.data[2] & 0xffff0000U) | value;
        set_extend(r, lsb);
        commit_ccr(r, lsb, lsb, false, value == 0U, false);
    }

    // 0x1fc7c: andi #0x0,ccr — clears X/N/Z/V/C to all zero.
    // X is cleared unconditionally.
    r.status = static_cast<std::uint16_t>(r.status & ~kStatusMask);

    // 0x1fc80: tst.w d4w — tests the sign-latch word.
    {
        const auto value = static_cast<std::uint16_t>(r.data[4] & 0xffffU);
        commit_ccr(r, false, false, false, value == 0U,
            (value & 0x8000U) != 0U);
    }

    // 0x1fc82: bpl.b 0x1fc86 — branch when N==0 (positive → done).
    // Falls through when N==1 (negative → negate quotient).
    if ((r.status & kNegativeBit) != 0U) {
        // 0x1fc84: neg.w d2w
        {
            auto value = static_cast<std::uint16_t>(r.data[2] & 0xffffU);
            const auto result = static_cast<std::uint16_t>(0U - value);
            const bool borrow = value != 0U;
            const bool overflow = result == 0x8000U;
            r.data[2] = (r.data[2] & 0xffff0000U) | result;
            set_extend(r, borrow);
            commit_ccr(r, borrow, borrow, overflow, result == 0U,
                (result & 0x8000U) != 0U);
        }
    }

    // 0x1fc86: rts
    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
