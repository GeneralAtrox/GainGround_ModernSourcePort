#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kWorkRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kWorkOffsetMask = 0x0003ffffU;

} // namespace

FunctionResult cpu_b_delay_10000(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x2710U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);

    for (std::uint32_t iteration = 0; iteration != 10001U; ++iteration) {
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[0] - 1U);
    }

    const auto stack_offset = registers.address[7] & kWorkOffsetMask;
    const auto return_address = (static_cast<std::uint32_t>(
        host.read_memory_word(kWorkRegion, stack_offset, kWordMask)) << 16U)
        | host.read_memory_word(kWorkRegion, stack_offset + 2U, kWordMask);
    registers.address[7] += 4U;
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
