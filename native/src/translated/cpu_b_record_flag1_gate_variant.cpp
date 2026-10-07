#include "cpu_b_record_flag1_gate_variant_detail.h"

namespace gain_ground::translated {
using namespace cpu_b_record_flag1_gate_variant_detail;

FunctionResult cpu_b_record_flag1_gate_variant(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const std::uint32_t base = registers.address[5];
    bool extend = (registers.status & kExtendBit) != 0U;

    const auto advance_state_six = [&]() -> FunctionResult {
        write_byte(host, base + 0x5eU, 6U);
        push_return(host, registers, 0x0001e016U);
        registers.program_counter = 0x0001da54U;
        const auto child = host.call_function(346U, 1U, 0x72U, 2U,
            0x0001e012U, 0x0001da54U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        std::uint16_t cursor = read_word(host, base + 0x5cU);
        add_word(registers, 0x0400U, cursor, extend);
        and_word(registers, 0x07ffU, cursor);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | cursor;
        write_word(host, base + 0x5cU, cursor);
        bset_memory(host, registers, base + 0x40U, 0U);
        bset_memory(host, registers, base + 0x40U, 1U);
        bset_memory(host, registers, base + 0x40U, 5U);
        return FunctionResult::complete(1U, pop_return(host, registers));
    };
    const auto run_df8c = [&]() -> FunctionResult {
        const auto state = read_byte(host, base + 0x5eU);
        ConditionCodes codes;
        codes.carry = false; codes.overflow = false;
        codes.zero = state == 1U; codes.negative = (state & 0x80U) != 0U;
        commit_cc(registers, (registers.status & kExtendBit) != 0U, codes);
        if (state == 1U) return advance_state_six();
        write_byte(host, base + 0x5eU, 2U);
        push_return(host, registers, 0x0001df9eU);
        registers.program_counter = 0x0001da54U;
        const auto child = host.call_function(346U, 1U, 0x72U, 2U,
            0x0001df9aU, 0x0001da54U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        bset_memory(host, registers, base + 0x40U, 0U);
        bset_memory(host, registers, base + 0x40U, 1U);
        bset_memory(host, registers, base + 0x40U, 5U);
        return FunctionResult::complete(1U, pop_return(host, registers));
    };
    const auto run_df64 = [&]() -> FunctionResult {
        const auto state = read_byte(host, base + 0x5eU);
        ConditionCodes codes;
        codes.carry = false; codes.overflow = false;
        codes.zero = state == 2U; codes.negative = (state & 0x80U) != 0U;
        commit_cc(registers, (registers.status & kExtendBit) != 0U, codes);
        if (state == 2U) return advance_state_six();
        write_byte(host, base + 0x5eU, 1U);
        push_return(host, registers, 0x0001df78U);
        registers.program_counter = 0x0001da54U;
        const auto child = host.call_function(346U, 1U, 0x72U, 2U,
            0x0001df74U, 0x0001da54U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        bset_memory(host, registers, base + 0x40U, 0U);
        bset_memory(host, registers, base + 0x40U, 1U);
        bset_memory(host, registers, base + 0x40U, 5U);
        return FunctionResult::complete(1U, pop_return(host, registers));
    };

    // 0x1de30 btst.b #1,(0x40,a5); beq.s continues when clear, otherwise rts.
    {
        const auto flags = read_byte(host, base + 0x40U);
        const bool zero = test_bit_clear(flags, 1U);
        ConditionCodes codes;
        codes.carry = false;
        codes.overflow = false;
        codes.zero = zero;
        codes.negative = false;
        extend = (registers.status & kExtendBit) != 0U;
        commit_cc(registers, extend, codes);
        if (!zero) {
            return FunctionResult::complete(1U, pop_return(host, registers));
        }
    }

    // 0x1de3a tst.b (0x3e,a5) — both branch outcomes join at 0x1de42.
    {
        const auto phase = read_byte(host, base + 0x3eU);
        ConditionCodes codes;
        codes.carry = false;
        codes.overflow = false;
        codes.zero = phase == 0U;
        codes.negative = (phase & 0x80U) != 0U;
        const bool extend = (registers.status & kExtendBit) != 0U;
        commit_cc(registers, extend, codes);
        if (phase != 0U)
            return FunctionResult::complete(1U, pop_return(host, registers));
    }

    // 0x1de42..0x1de55 descriptor table walk.
    registers.address[0] = read_long(host, base + 0x6eU);
    std::uint16_t d0 = read_word(host, registers.address[0] + 8U);
    {
        extend = (registers.status & kExtendBit) != 0U;
        ConditionCodes codes;
        codes.carry = false;
        codes.overflow = false;
        codes.zero = d0 == 0U;
        codes.negative = (d0 & 0x8000U) != 0U;
        commit_cc(registers, extend, codes);
        lsl_word(registers, 2U, d0, extend);
    }
    std::uint32_t table = kTableBase + d0;
    registers.address[0] = table;

    // 0x1de56..0x1ea9 x-axis bounds against table bytes 0/1.
    std::uint32_t wide_x = read_long(host, base + 0x1eU) + read_long(host, base + 0x12U);
    {
        const auto swapped = (wide_x >> 16U) | (wide_x << 16U);
        wide_x = swapped;
        registers.data[0] = swapped;
        set_swap_flags(registers, swapped);
    }
    std::uint16_t x = static_cast<std::uint16_t>(wide_x & 0xffffU);
    sub_word_immediate(registers, 0x000aU, x, extend);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | x;

    {
        const auto bound = static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(static_cast<std::int16_t>(
                static_cast<std::int8_t>(read_byte(host, registers.address[0])))) << 3U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | bound;
        registers.address[0]++;
        ConditionCodes codes;
        const bool taken = [] (std::uint16_t v, std::uint16_t b, ConditionCodes &cc) {
            const auto diff = static_cast<std::uint16_t>(v - b);
            cc.carry = v < b;
            cc.overflow = ((v ^ b) & (v ^ diff) & 0x8000U) != 0U;
            cc.zero = diff == 0U;
            cc.negative = (diff & 0x8000U) != 0U;
            const std::int16_t a = static_cast<std::int16_t>(v);
            const std::int16_t bq = static_cast<std::int16_t>(b);
            return a < bq;
        }(x, bound, codes);
            const bool extend_now = (registers.status & kExtendBit) != 0U;
            commit_cc(registers, extend_now, codes);
            if (taken) return run_df8c();
    }
    {
        add_word(registers, 0x0014U, x, extend);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | x;
        const auto bound = static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(static_cast<std::int16_t>(
                static_cast<std::int8_t>(read_byte(host, registers.address[0])))) << 3U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | bound;
        registers.address[0]++;
        ConditionCodes codes;
        (void)compare_signed_gt(x, bound, codes);
        const bool exceeded = static_cast<std::int16_t>(x)
            >= static_cast<std::int16_t>(bound);
        const bool extend_now = (registers.status & kExtendBit) != 0U;
        commit_cc(registers, extend_now, codes);
        if (exceeded) return run_df8c();
    }

    // 0x1de80..0x1dea9 y-axis bounds against table bytes 2/3.
    std::uint32_t wide_y = read_long(host, base + 0x26U) + read_long(host, base + 0x1aU);
    {
        const auto swapped = (wide_y >> 16U) | (wide_y << 16U);
        wide_y = swapped;
        registers.data[1] = swapped;
        set_swap_flags(registers, swapped);
    }
    std::uint16_t y = static_cast<std::uint16_t>(wide_y & 0xffffU);
    sub_word_immediate(registers, 0x0009U, y, extend);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | y;

    {
        const auto bound = static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(static_cast<std::int16_t>(
                static_cast<std::int8_t>(read_byte(host, registers.address[0])))) << 3U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | bound;
        registers.address[0]++;
        ConditionCodes codes;
        const bool taken = [] (std::uint16_t v, std::uint16_t b, ConditionCodes &cc) {
            const auto diff = static_cast<std::uint16_t>(v - b);
            cc.carry = v < b;
            cc.overflow = ((v ^ b) & (v ^ diff) & 0x8000U) != 0U;
            cc.zero = diff == 0U;
            cc.negative = (diff & 0x8000U) != 0U;
            const std::int16_t a = static_cast<std::int16_t>(v);
            const std::int16_t bq = static_cast<std::int16_t>(b);
            return a < bq;
        }(y, bound, codes);
        const bool extend_now = (registers.status & kExtendBit) != 0U;
        commit_cc(registers, extend_now, codes);
        if (taken) return run_df64();
    }
    {
        add_word(registers, 0x0012U, y, extend);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | y;
        const auto bound = static_cast<std::uint16_t>(
            static_cast<std::uint16_t>(static_cast<std::int16_t>(
                static_cast<std::int8_t>(read_byte(host, registers.address[0])))) << 3U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | bound;
        registers.address[0]++;
        ConditionCodes codes;
        const bool exceeded = compare_signed_gt(y, bound, codes);
        const bool extend_now = (registers.status & kExtendBit) != 0U;
        commit_cc(registers, extend_now, codes);
        if (exceeded) return run_df64();
    }

    std::uint16_t d3 = 0U;
    if (const auto result = scan_record_corners(context, base, x, y, extend, d3))
        return *result;

    // 0x1df00 andi/lsl/jmp dispatch.
    and_word(registers, 0x000fU, d3);
    registers.data[3] = (registers.data[3] & 0xffff0000U) | d3;
    lsl_word(registers, 2U, d3, extend);
    registers.data[3] = (registers.data[3] & 0xffff0000U) | d3;

    switch (d3 >> 2) {
        case 0:  // 0x1df0a -> 0x1df4e
        {
            const auto value = read_byte(host, base + 0x5fU);
            registers.data[0] = (registers.data[0] & 0xffffff00U) | value;
            write_byte(host, base + 0x5eU, value);
            ConditionCodes codes;
            codes.carry = false;
            codes.overflow = false;
            codes.zero = value == 0U;
            codes.negative = (value & 0x80U) != 0U;
            const bool extend_now = (registers.status & kExtendBit) != 0U;
            commit_cc(registers, extend_now, codes);
            bclr_memory(host, registers, base + 0x40U, 1U);
            bclr_memory(host, registers, base + 0x40U, 5U);
            return FunctionResult::complete(1U, pop_return(host, registers));
        }
        case 1: case 2: case 4: case 8:  // -> 0x1e086
        {
            const auto selector = read_word(host, base + 0x54U);
            ConditionCodes codes;
            codes.carry = false;
            codes.overflow = false;
            codes.zero = (selector & 1U) == 0U;
            codes.negative = false;
            const bool extend_now = (registers.status & kExtendBit) != 0U;
            commit_cc(registers, extend_now, codes);
            if ((selector & 1U) != 0U) {
                write_byte(host, base + 0x5eU, 1U);
                push_return(host, registers, 0x0001e09aU);
                registers.program_counter = 0x0001da58U;
                const auto child = host.call_function(347U, 1U, 0x72U, 2U,
                    0x0001e096U, 0x0001da58U, context);
                if (child.status != TranslationStatus::complete || child.control != 1U)
                    return child;
            } else {
                write_byte(host, base + 0x5eU, 2U);
                push_return(host, registers, 0x0001e0b8U);
                registers.program_counter = 0x0001da54U;
                const auto child = host.call_function(346U, 1U, 0x72U, 2U,
                    0x0001e0b4U, 0x0001da54U, context);
                if (child.status != TranslationStatus::complete || child.control != 1U)
                    return child;
            }
            bset_memory(host, registers, base + 0x40U, 0U);
            bset_memory(host, registers, base + 0x40U, 1U);
            bset_memory(host, registers, base + 0x40U, 5U);
            return FunctionResult::complete(1U, pop_return(host, registers));
        }
        case 3: case 12:  // -> 0x1df8c
            return run_df8c();
        case 6: case 9:  // -> 0x1df64
            return run_df64();
        case 5:  // 0x1df1e -> 0x1dfd0
        {
            write_byte(host, base + 0x5eU, 4U);
            push_return(host, registers, 0x0001dfdaU);
            registers.program_counter = 0x0001da54U;
            const auto child = host.call_function(346U, 1U, 0x72U, 2U,
                0x0001dfd6U, 0x0001da54U, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
            bset_memory(host, registers, base + 0x40U, 0U);
            bset_memory(host, registers, base + 0x40U, 1U);
            bset_memory(host, registers, base + 0x40U, 5U);
            return FunctionResult::complete(1U, pop_return(host, registers));
        }
        case 10:  // 0x1df32 -> 0x1dfb2
        {
            write_byte(host, base + 0x5eU, 3U);
            push_return(host, registers, 0x0001dfbcU);
            registers.program_counter = 0x0001da54U;
            const auto child = host.call_function(346U, 1U, 0x72U, 2U,
                0x0001dfb8U, 0x0001da54U, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
            bset_memory(host, registers, base + 0x40U, 0U);
            bset_memory(host, registers, base + 0x40U, 1U);
            bset_memory(host, registers, base + 0x40U, 5U);
            return FunctionResult::complete(1U, pop_return(host, registers));
        }
        case 11: case 14:  // -> 0x1e03a
        {
            write_byte(host, base + 0x5eU, 3U);
            push_return(host, registers, 0x0001e044U);
            registers.program_counter = 0x0001ec00U;
            const auto child = host.call_function(364U, 1U, 0x72U, 2U,
                0x0001e040U, 0x0001ec00U, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
            {
                std::uint16_t cursor = static_cast<std::uint16_t>(registers.data[1]);
                add_word(registers, 0x0400U, cursor, extend);
                registers.data[1] = (registers.data[1] & 0xffff0000U) | cursor;
                write_word(host, base + 0x5cU, cursor);
                set_logic_word(registers, cursor);
            }
            bset_memory(host, registers, base + 0x40U, 0U);
            bset_memory(host, registers, base + 0x40U, 1U);
            bset_memory(host, registers, base + 0x40U, 5U);
            return FunctionResult::complete(1U, pop_return(host, registers));
        }
        case 7: case 13:  // -> 0x1e060
        {
            write_byte(host, base + 0x5eU, 4U);
            push_return(host, registers, 0x0001e06aU);
            registers.program_counter = 0x0001ec00U;
            const auto child = host.call_function(364U, 1U, 0x72U, 2U,
                0x0001e066U, 0x0001ec00U, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
            {
                std::uint16_t cursor = static_cast<std::uint16_t>(registers.data[1]);
                add_word(registers, 0x0400U, cursor, extend);
                registers.data[1] = (registers.data[1] & 0xffff0000U) | cursor;
                write_word(host, base + 0x5cU, cursor);
                set_logic_word(registers, cursor);
            }
            bset_memory(host, registers, base + 0x40U, 0U);
            bset_memory(host, registers, base + 0x40U, 1U);
            bset_memory(host, registers, base + 0x40U, 5U);
            return FunctionResult::complete(1U, pop_return(host, registers));
        }
        default:  // slot 15 -> 0x1df46
        {
            bset_memory(host, registers, base + 0x40U, 1U);
            return FunctionResult::complete(1U, pop_return(host, registers));
        }
    }

}

} // namespace gain_ground::translated
