#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionMask = 0x001fU;

void set_logic_word(CpuRegisters &registers, std::uint16_t value,
    std::uint16_t carry = 0U)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    flags |= carry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    const bool carry = static_cast<std::uint32_t>(left) + right > 0xffffU;
    const bool overflow = ((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U;
    std::uint16_t flags = carry ? 0x0011U : 0U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto offset = registers.address[7] & 0x0003ffffU;
    const auto high = host.read_memory_word(kShared, offset, kWordMask);
    const auto low = host.read_memory_word(kShared, offset + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgram, address, kWordMask);
}
}

FunctionResult proven_static_cpu_a_plain_00001a04(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x1a04U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | 0x0300U;
    set_logic_word(registers, 0x0300U);
    prefetch(host, 0x00001a08U);

    for (;;) {
        const auto before_rotate = static_cast<std::uint16_t>(registers.data[0]);
        const auto rotated = static_cast<std::uint16_t>(
            (before_rotate << 8U) | (before_rotate >> 8U));
        registers.data[0] = (registers.data[0] & 0xffff0000U) | rotated;
        set_logic_word(registers, rotated,
            (before_rotate & 0x0100U) != 0U ? 0x0001U : 0U);
        prefetch(host, 0x00001a0aU);

        const auto byte = static_cast<std::uint8_t>(registers.data[0]);
        prefetch(host, 0x00001a0cU);
        prefetch(host, 0x00001a0eU);
        prefetch(host, 0x00001a10U);
        host.write_hardware(2U, 0U, 0xffU, 0x00001a0aU,
            0x0080000eU, static_cast<std::uint16_t>(byte) * 0x0101U, 0x00ffU);
        host.write_hardware(4U, 0U, 0xffU, 0x00001a0aU,
            0x0080000eU, byte, 0x00ffU);
        std::uint16_t byte_flags = registers.status & 0x0010U;
        if ((byte & 0x80U) != 0U) byte_flags |= 0x0008U;
        if (byte == 0U) byte_flags |= 0x0004U;
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~kConditionMask) | byte_flags);

        const auto second_before = static_cast<std::uint16_t>(registers.data[0]);
        const auto second_rotated = static_cast<std::uint16_t>(
            (second_before << 8U) | (second_before >> 8U));
        registers.data[0] = (registers.data[0] & 0xffff0000U) | second_rotated;
        set_logic_word(registers, second_rotated,
            (second_before & 0x0100U) != 0U ? 0x0001U : 0U);
        prefetch(host, 0x00001a12U);

        const auto result = static_cast<std::uint16_t>(
            second_rotated + static_cast<std::uint16_t>(registers.data[2]));
        registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
        set_add_word(registers, second_rotated,
            static_cast<std::uint16_t>(registers.data[2]), result);
        prefetch(host, 0x00001a14U);
        prefetch(host, 0x00001a16U);

        const auto counter = static_cast<std::uint16_t>(registers.data[1] - 1U);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | counter;
        prefetch(host, 0x00001a08U);
        if (counter == 0xffffU) break;
    }

    prefetch(host, 0x00001a18U);
    prefetch(host, 0x00001a1aU);
    const auto target = pop_return(host, registers);
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
}
