#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_a_irq3_vector_trampoline(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    SoundCallerTiming host(context);
    if (context.host->resumes_interrupts_inline()) host.begin(0x00080042U);
    constexpr std::uint16_t kRegion = 3U;
    constexpr std::uint16_t kMask = 0xffffU;
    constexpr std::uint32_t kTarget = 0x00080f96U;
    (void)host.read_memory_word(kRegion, 0x00000046U, kMask);
    (void)host.read_memory_word(kRegion, 0x00000f96U, kMask);
    (void)host.read_memory_word(kRegion, 0x00000f98U, kMask);
    context.registers.program_counter = kTarget;
    const auto child = host.call_function(72U, 0U, 0xffU, 1U,
        0x00080042U, kTarget, context);
    if (child.status != TranslationStatus::complete)
        return child;
    return FunctionResult::complete(3U, kTarget);
}
} // namespace gain_ground::translated
