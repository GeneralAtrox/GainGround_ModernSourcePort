#include "gain_ground/contract_types.h"

#include <cstdint>
#include <initializer_list>
#include <utility>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address & 0x00ffffffU, value, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(
        kRegion, address & 0x00fffffeU, mask);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    host.write_memory_word(kRegion, address & 0x00fffffeU,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U), mask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long_move(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void write_long_rmw(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
}

void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_long_flags(CpuRegisters &registers,
    std::uint32_t left, std::uint32_t right, std::uint32_t result)
{
    std::uint16_t flags{};
    if (result < left) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_long_flags(CpuRegisters &registers,
    std::uint32_t left, std::uint32_t right, std::uint32_t result)
{
    std::uint16_t flags{};
    if (left < right) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (left < right) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_data_word(CpuRegisters &registers, std::size_t index, std::uint16_t value)
{
    registers.data[index] = (registers.data[index] & 0xffff0000U) | value;
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7], static_cast<std::uint16_t>(value >> 16U));
    write_word(host, registers.address[7] + 2U, static_cast<std::uint16_t>(value));
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    auto &registers = context.registers;
    auto &host = *context.host;
    push_return(host, registers, continuation);
    registers.program_counter = target;
    return host.call_function(
        function_id, 1U, 0x72U, 2U, callsite, target, context);
}

[[nodiscard]] FunctionResult finish(ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_advance_record_position_with_offset(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];
    if (registers.program_counter != 0x0001fea2U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};
    const auto reject = [&]() -> FunctionResult {
        const auto child = call_child(context, 389U,
            0x0001ff0eU, 0x0002089cU, 0x0001ff12U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        return finish(host, registers);
    };

    const auto busy = read_byte(host, base + 0x3fU);
    set_logic_flags(registers, busy, 0x80U, 0xffU);
    if (busy != 0U) return reject();

    {
        const auto child = call_child(context, 387U,
            0x0001fea8U, 0x00020724U, 0x0001feacU);
        // F387's original 20744 ADDQ #4,SP / BCLR / RTS discards our
        // BSR frame and returns to our caller. This invocation is complete;
        // do not propagate the consumed nonlocal-return marker or pop again.
        if (child.status == TranslationStatus::complete && child.control == 8U)
            return FunctionResult::complete(1U, child.exit_program_counter);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        if ((registers.status & 0x0001U) != 0U) return reject();
    }
    {
        const auto child = call_child(context, 385U,
            0x0001feaeU, 0x0002060cU, 0x0001feb2U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        if ((registers.status & 0x0001U) != 0U) return reject();
    }

    for (const auto pair : {std::pair{0x1eU, 0x12U}, std::pair{0x26U, 0x1aU}}) {
        const auto delta = read_long(host, base + pair.first);
        registers.data[0] = delta;
        set_logic_flags(registers, delta, 0x80000000U, 0xffffffffU);
        const auto position = read_long(host, base + pair.second);
        const auto result = position + delta;
        write_long_rmw(host, base + pair.second, result);
        set_add_long_flags(registers, position, delta, result);
    }

    {
        const auto value = read_long(host, base + 0x22U);
        registers.data[0] = value;
        set_logic_flags(registers, value, 0x80000000U, 0xffffffffU);
        const auto offset = read_long(host, base + 0x50U);
        const auto result = value - offset;
        registers.data[0] = result;
        set_sub_long_flags(registers, value, offset, result);
        write_long_move(host, base + 0x22U, result);
        set_logic_flags(registers, result, 0x80000000U, 0xffffffffU);
        const auto position = read_long(host, base + 0x16U);
        const auto accumulated = position + result;
        write_long_rmw(host, base + 0x16U, accumulated);
        set_add_long_flags(registers, position, result, accumulated);
    }

    auto d0 = read_word(host, base + 0x16U);
    set_data_word(registers, 0U, d0);
    set_logic_flags(registers, d0, 0x8000U, 0xffffU);
    {
        const auto result = static_cast<std::uint16_t>(d0 - 0x0010U);
        set_sub_word_flags(registers, d0, 0x0010U, result);
        d0 = result;
        set_data_word(registers, 0U, d0);
    }
    set_data_word(registers, 1U, d0);
    set_logic_flags(registers, d0, 0x8000U, 0xffffU);
    const auto masked = static_cast<std::uint16_t>(d0 & 0xffe0U);
    set_data_word(registers, 1U, masked);
    set_logic_flags(registers, masked, 0x8000U, 0xffffU);
    const auto shift_right = [&] {
        const bool carry = (d0 & 1U) != 0U;
        d0 = static_cast<std::uint16_t>(
            static_cast<std::int16_t>(d0) >> 1U);
        set_data_word(registers, 0U, d0);
        std::uint16_t flags = carry ? 0x0011U : 0U;
        if ((d0 & 0x8000U) != 0U) flags |= 0x0008U;
        if (d0 == 0U) flags |= 0x0004U;
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~0x001fU) | flags);
    };
    if (masked == 0U) shift_right();
    shift_right();
    {
        const auto result = static_cast<std::uint16_t>(d0 + 0x003fU);
        set_add_word_flags(registers, d0, 0x003fU, result);
        d0 = result;
        set_data_word(registers, 0U, d0);
    }
    const auto timer = static_cast<std::uint8_t>(d0);
    write_byte(host, base + 0x10U, timer);
    set_logic_flags(registers, timer, 0x80U, 0xffU);
    write_byte(host, base + 0x11U, timer);
    set_logic_flags(registers, timer, 0x80U, 0xffU);

    {
        const auto value = read_byte(host, base + 1U);
        write_byte(host, base + 1U, static_cast<std::uint8_t>(value | 0x04U));
        if ((value & 0x04U) == 0U)
            registers.status |= 0x0004U;
        else
            registers.status &= static_cast<std::uint16_t>(~0x0004U);
    }
    {
        const auto child = call_child(context, 280U,
            0x0001fefaU, 0x00015d24U, 0x0001ff00U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
    {
        const auto child = call_child(context, 388U,
            0x0001ff00U, 0x000207c2U, 0x0001ff06U);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
    {
        const auto child = call_child(context, 282U,
            0x0001ff06U, 0x00015df2U, 0x0001ff0cU);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
    return finish(host, registers);
}

} // namespace gain_ground::translated
