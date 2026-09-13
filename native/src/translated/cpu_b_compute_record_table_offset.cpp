#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint16_t kHighByteMask = 0xff00U;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kCpuBMainMemoryRegion, address, kFullWordMask);
    const auto low = host.read_memory_word(kCpuBMainMemoryRegion, address + 2U, kFullWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_compute_record_table_offset(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    const std::uint32_t byte_address = context.registers.address[5] + 0x38U;
    const auto source = static_cast<std::uint8_t>(
        host.read_memory_word(kCpuBMainMemoryRegion, byte_address, kHighByteMask) >> 8U);
    const auto doubled = static_cast<std::uint16_t>(static_cast<std::uint16_t>(source) * 2U);
    const auto result = static_cast<std::uint16_t>(static_cast<std::uint32_t>(source) * 6U);

    context.registers.data[0] = (context.registers.data[0] & 0xffff0000U) | doubled;
    context.registers.data[7] = result;

    constexpr std::uint16_t kConditionCodeMask = 0x001fU;
    constexpr std::uint16_t kExtend = 0x0010U;
    constexpr std::uint16_t kNegative = 0x0008U;
    constexpr std::uint16_t kZero = 0x0004U;
    constexpr std::uint16_t kOverflow = 0x0002U;
    constexpr std::uint16_t kCarry = 0x0001U;
    const std::uint32_t add_wide = static_cast<std::uint32_t>(source) * 4U + doubled;
    const auto left = static_cast<std::uint16_t>(static_cast<std::uint16_t>(source) * 4U);
    std::uint16_t condition_codes{};
    if (add_wide > 0xffffU)
        condition_codes |= kExtend | kCarry;
    if ((result & 0x8000U) != 0U)
        condition_codes |= kNegative;
    if (result == 0U)
        condition_codes |= kZero;
    if (((~(left ^ doubled)) & (left ^ result) & 0x8000U) != 0U)
        condition_codes |= kOverflow;
    context.registers.status = static_cast<std::uint16_t>(
        (context.registers.status & ~kConditionCodeMask) | condition_codes);

    const std::uint32_t stack_pointer = context.registers.address[7];
    const std::uint32_t return_address = read_long(host, stack_pointer);
    context.registers.address[7] = stack_pointer + 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
