#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kWordMask);
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = host.read_memory_word(kRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7], static_cast<std::uint16_t>(value >> 16U));
    write_word(host, registers.address[7] + 2U, static_cast<std::uint16_t>(value));
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_zero(CpuRegisters &registers, bool clear)
{
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | (clear ? 0x0004U : 0U));
}

void set_word_flags(CpuRegisters &registers, std::uint16_t value,
    bool carry = false, bool overflow = false)
{
    std::uint16_t flags = carry ? 0x0011U : 0U;
    if (overflow) flags |= 0x0002U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if (left < right) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

FunctionResult call_child(FunctionContext &context, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t return_address, std::uint32_t id)
{
    auto &host = *context.host;
    push_return(host, context.registers, return_address);
    context.registers.program_counter = target;
    return host.call_function(id, 1U, 0x72U, 2U, callsite, target, context);
}

FunctionResult transfer_to_load_tail(FunctionContext &context,
    std::uint32_t callsite)
{
    context.registers.program_counter = 0x0001e1f2U;
    const auto tail = context.host->call_function(554U, 1U, 0x72U, 1U,
        callsite, 0x0001e1f2U, context);
    if (tail.status != TranslationStatus::complete)
        return tail;
    if (tail.control == 3U
        && tail.exit_program_counter != 0x0001e1f2U)
        return proven_static_cpu_b_72_0001e22a(context);
    return tail;
}
} // namespace

FunctionResult cpu_b_record_flag1_gate_alt(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    auto flags = read_byte(host, base + 0x41U);
    set_bit_zero(registers, (flags & 0x02U) == 0U);
    if ((flags & 0x02U) == 0U) {
        registers.program_counter = 0x0001e18aU;
        return host.call_function(553U, 1U, 0x72U, 1U,
            0x0001e442U, 0x0001e18aU, context);
    }

    auto child = call_child(context, 0x0001e446U, 0x0001ebf0U,
        0x0001e44aU, 363U);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    flags = read_byte(host, base + 0x41U);
    set_bit_zero(registers, (flags & 0x04U) == 0U);
    registers.data[1] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U, 0xffffffffU);

    const auto d7 = static_cast<std::uint16_t>(registers.data[7]);
    auto d1 = static_cast<std::uint16_t>(0U - d7);
    registers.data[1] = d1;
    set_word_flags(registers, d1, d7 != 0U, d7 == 0x8000U);

    const auto doubled = static_cast<std::uint32_t>(d1) + d1;
    const auto output = static_cast<std::uint16_t>(doubled);
    registers.data[1] = output;
    set_word_flags(registers, output, doubled > 0xffffU,
        ((~(d1 ^ d1)) & (d1 ^ output) & 0x8000U) != 0U);

    if ((flags & 0x04U) == 0U)
        return transfer_to_load_tail(context, 0x0001e458U);

    const auto state = read_word(host, base + 0x74U);
    const auto difference = static_cast<std::uint16_t>(state - 3U);
    set_compare_word_flags(registers, state, 3U, difference);
    if (static_cast<std::int16_t>(state) > 3)
        return transfer_to_load_tail(context, 0x0001e468U);

    d1 = static_cast<std::uint16_t>(output + d7);
    registers.data[1] = d1;
    set_word_flags(registers, d1,
        static_cast<std::uint32_t>(output) + d7 > 0xffffU,
        ((~(output ^ d7)) & (output ^ d1) & 0x8000U) != 0U);
    return transfer_to_load_tail(context, 0x0001e46eU);
}

} // namespace gain_ground::translated
