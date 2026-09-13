#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {

FunctionResult proven_static_cpu_b_72_0001f104(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
    constexpr std::uint16_t kFullWordMask = 0xffffU;
    const std::uint32_t stack_pointer = context.registers.address[7];
    const auto high = context.host->read_memory_word(
        kCpuBMainMemoryRegion, stack_pointer, kFullWordMask);
    const auto low = context.host->read_memory_word(
        kCpuBMainMemoryRegion, stack_pointer + 2U, kFullWordMask);
    const std::uint32_t return_address =
        (static_cast<std::uint32_t>(high) << 16U) | low;

    context.registers.address[7] = stack_pointer + 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
