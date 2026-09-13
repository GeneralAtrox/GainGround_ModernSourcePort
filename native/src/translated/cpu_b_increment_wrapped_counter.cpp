#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if (value == 0U) flags |= 0x0004U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    const std::uint32_t wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags = 0U;
    if (wide > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_compare_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source, std::uint16_t result)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if (destination < source) flags |= 0x0001U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_increment_wrapped_counter(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto counter_address = registers.address[5] + 0x20U;
    const auto counter = host.read_memory_word(kRegion, counter_address, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
    set_logic_word_flags(registers, counter);

    auto result = static_cast<std::uint16_t>(counter + 0x0101U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    set_add_word_flags(registers, counter, 0x0101U, result);
    const auto comparison = static_cast<std::uint16_t>(result - 0x0404U);
    set_compare_word_flags(registers, result, 0x0404U, comparison);
    const bool less_than = ((registers.status & 0x0008U) != 0U)
        != ((registers.status & 0x0002U) != 0U);
    if (!less_than) {
        result = 0x0101U;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
        set_logic_word_flags(registers, result);
    }

    host.write_memory_word(kRegion, counter_address, result, kWordMask);
    set_logic_word_flags(registers, result);
    host.write_memory_word(kRegion, registers.address[6] + 0x60U, result, kWordMask);
    set_logic_word_flags(registers, result);

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
