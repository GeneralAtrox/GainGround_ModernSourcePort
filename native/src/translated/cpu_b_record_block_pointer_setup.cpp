#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    const std::uint32_t wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags = 0U;
    if (wide > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    const auto extend = static_cast<std::uint16_t>(registers.status & 0x0010U);
    std::uint16_t flags = extend;
    if (value == 0U) flags |= 0x0004U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_record_block_pointer_setup(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[4] = 0x00003400U;

    auto value = static_cast<std::uint16_t>(registers.data[0]);
    auto result = static_cast<std::uint16_t>(value + value);
    set_add_word_flags(registers, value, value, result);
    value = result;
    result = static_cast<std::uint16_t>(value + value);
    set_add_word_flags(registers, value, value, result);
    value = result;
    result = static_cast<std::uint16_t>(value + 0x000cU);
    set_add_word_flags(registers, value, 0x000cU, result);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;

    registers.address[4] = static_cast<std::uint32_t>(registers.address[4]
        + static_cast<std::int32_t>(static_cast<std::int16_t>(result)));
    const auto first = host.read_memory_word(kRegion, registers.address[4], kWordMask);
    registers.address[4] += 2U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | first;
    set_move_word_flags(registers, first);
    const auto second = host.read_memory_word(kRegion, registers.address[4], kWordMask);
    registers.address[4] += 2U;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | second;
    set_move_word_flags(registers, second);

    const auto stack_pointer = registers.address[7];
    const auto target = read_long(host, stack_pointer);
    registers.address[7] = stack_pointer + 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
