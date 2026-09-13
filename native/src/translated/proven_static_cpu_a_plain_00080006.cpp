#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {

FunctionResult proven_static_cpu_a_plain_00080006(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    constexpr std::uint16_t kProgramRegion = 1U;
    constexpr std::uint16_t kSharedRegion = 3U;
    constexpr std::uint16_t kWordMask = 0xffffU;
    constexpr std::uint32_t kTarget = 0x00000400U;

    (void)host.read_memory_word(kSharedRegion, 0x0000000aU, kWordMask);
    (void)host.read_memory_word(kProgramRegion, kTarget, kWordMask);
    (void)host.read_memory_word(kProgramRegion, kTarget + 2U, kWordMask);
    context.registers.program_counter = kTarget;
    (void)host.call_function(
        494U, 0U, 0xffU, 1U, 0x00080006U, kTarget, context);
    return FunctionResult::complete(3U, kTarget);
}

} // namespace gain_ground::translated
