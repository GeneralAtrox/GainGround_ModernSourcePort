#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint32_t kStateAddress = 0x0000082eU;
constexpr std::uint32_t kZeroSeed = 0x2a6d365aU;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kCpuBMainMemoryRegion, address, kFullWordMask);
    const auto low = host.read_memory_word(kCpuBMainMemoryRegion, address + 2U, kFullWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(
        kCpuBMainMemoryRegion, address, static_cast<std::uint16_t>(value >> 16U), kFullWordMask);
    host.write_memory_word(
        kCpuBMainMemoryRegion, address + 2U, static_cast<std::uint16_t>(value), kFullWordMask);
}

} // namespace

FunctionResult cpu_b_update_lcg_state(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    std::uint32_t seed = read_long(host, kStateAddress);
    if (seed == 0U)
        seed = kZeroSeed;

    const std::uint32_t product = seed * 41U;
    const auto product_low = static_cast<std::uint16_t>(product);
    const auto product_high = static_cast<std::uint16_t>(product >> 16U);
    const std::uint32_t folded_wide = static_cast<std::uint32_t>(product_low) + product_high;
    const auto folded = static_cast<std::uint16_t>(folded_wide);
    const std::uint32_t next_state = (static_cast<std::uint32_t>(folded) << 16U) | product_low;

    context.registers.data[0] = (seed & 0xffff0000U) | folded;
    context.registers.data[1] = next_state;
    write_long(host, kStateAddress, next_state);

    constexpr std::uint16_t kConditionCodeMask = 0x001fU;
    constexpr std::uint16_t kExtend = 0x0010U;
    constexpr std::uint16_t kNegative = 0x0008U;
    constexpr std::uint16_t kZero = 0x0004U;
    std::uint16_t condition_codes{};
    if (folded_wide > 0xffffU)
        condition_codes |= kExtend;
    if ((next_state & 0x80000000U) != 0U)
        condition_codes |= kNegative;
    if (next_state == 0U)
        condition_codes |= kZero;
    context.registers.status = static_cast<std::uint16_t>(
        (context.registers.status & ~kConditionCodeMask) | condition_codes);

    const std::uint32_t stack_pointer = context.registers.address[7];
    const std::uint32_t return_address = read_long(host, stack_pointer);
    context.registers.address[7] = stack_pointer + 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
