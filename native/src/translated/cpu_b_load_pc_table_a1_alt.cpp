#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_b_emit_tilemap_a_incrementing_pairs_with_bias_resume_1611e(
    FunctionContext &context) noexcept;

namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto shift = odd ? 0U : 8U;
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, address & ~1U, mask) >> shift);
}

void set_logic_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_long_flags(
    CpuRegisters &registers, std::uint32_t destination,
    std::uint32_t source)
{
    const auto result = destination - source;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ source) & (destination ^ result)
            & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (destination < source) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] bool test_bit_three(
    CpuRegisters &registers, std::uint8_t value)
{
    const bool set = (value & 0x08U) != 0U;
    if (set)
        registers.status = static_cast<std::uint16_t>(
            registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(
            registers.status | 0x0004U);
    return set;
}

void push_return(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t target)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(target >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(target), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult cpu_b_load_pc_table_a1_alt(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    if (registers.program_counter != 0x0000fb44U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    registers.address[1] = 0x00010646U;
    registers.data[2] = read_long(host, registers.address[4] + 0x80U);
    set_logic_long_flags(registers, registers.data[2]);

    const auto limit = read_long(host, 0x00007800U);
    set_compare_long_flags(registers, registers.data[2], limit);
    const bool below_limit = registers.data[2] < limit;
    const bool use_tail = !below_limit
        && test_bit_three(registers, read_byte(host, 0x00000832U));

    if (use_tail) {
        registers.address[0] = 0x00200386U;
        registers.data[1] = 0U;
        set_logic_long_flags(registers, registers.data[1]);
        registers.data[3] = 7U;
        set_logic_long_flags(registers, registers.data[3]);
        registers.program_counter = 0x0001611eU;
        return cpu_b_emit_tilemap_a_incrementing_pairs_with_bias_resume_1611e(
            context);
    }

    push_return(host, registers, 0x0000fb70U);
    registers.program_counter = 0x0001610eU;
    const auto child = host.call_function(296U, 1U, 0x72U, 2U,
        0x0000fb6aU, 0x0001610eU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
