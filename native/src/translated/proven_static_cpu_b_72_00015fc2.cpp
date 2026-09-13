#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word(CpuRegisters &registers, std::uint16_t value) noexcept
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
                  std::uint16_t right, std::uint16_t result) noexcept
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (wide > 0xffffU)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers) noexcept
{
    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7] & 0x0003ffffU, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, (registers.address[7] + 2U) & 0x0003ffffU, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_b_72_00015fc2(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto source = host.read_memory_word(
        kPrivateRegion, registers.address[1] & 0x0003ffffU, kWordMask);
    registers.address[1] += 2U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | source;
    set_logic_word(registers, source);

    const auto first_tile_offset =
        (registers.address[0] - 0x00200000U) & 0x0003ffffU;
    host.write_memory_word(kTileRegion, first_tile_offset, source, kWordMask);
    registers.address[0] += 2U;
    set_logic_word(registers, source);

    const auto incremented = static_cast<std::uint16_t>(source + 1U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | incremented;
    set_add_word(registers, source, 1U, incremented);

    host.write_memory_word(
        kTileRegion,
        (registers.address[0] - 0x00200000U) & 0x0003ffffU,
        incremented,
        kWordMask);
    set_logic_word(registers, incremented);
    registers.address[0] += 0x7eU;

    const auto counter = static_cast<std::uint16_t>(registers.data[2]);
    const auto next_counter = static_cast<std::uint16_t>(counter - 1U);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | next_counter;
    if (next_counter != 0xffffU) {
        registers.program_counter = 0x00015fc2U;
        const auto loop = host.call_function(
            474U, 1U, 0x72U, 1U, 0x00015fceU, 0x00015fc2U, context);
        if (loop.status == TranslationStatus::complete && loop.control == 3U)
            return FunctionResult::complete(4U, 0x00015fc2U);
        return loop;
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
