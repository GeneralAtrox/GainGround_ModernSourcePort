#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

void write_d0(ExecutionHost &host, CpuRegisters &registers, bool postincrement)
{
    const auto value = static_cast<std::uint16_t>(registers.data[0]);
    host.write_memory_word(kTileRegion, registers.address[0] - 0x00200000U, value, kWordMask);
    if (postincrement) registers.address[0] += 2U;
    set_move_word_flags(registers, value);
}

void increment_d0_word(CpuRegisters &registers)
{
    const auto left = static_cast<std::uint16_t>(registers.data[0]);
    const auto result = static_cast<std::uint16_t>(left + 1U);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (left == 0x7fffU) flags |= 0x0002U;
    if (left == 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto high = host.read_memory_word(kPrivateRegion, stack, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, stack + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_write_two_tilemap_a_pairs(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    write_d0(host, registers, true);
    increment_d0_word(registers);
    write_d0(host, registers, false);
    registers.address[0] += 0x7eU;
    increment_d0_word(registers);
    write_d0(host, registers, true);
    increment_d0_word(registers);
    write_d0(host, registers, false);

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
