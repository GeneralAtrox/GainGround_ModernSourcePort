#include "gain_ground/contract_types.h"
#include "gain_ground/sound_caller_timing.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_a_irq5_vector_trampoline(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    SoundCallerTiming host(context);
    if (context.host->resumes_interrupts_inline()) host.begin(0x0008004eU);
    constexpr std::uint16_t kRegion = 3U;
    constexpr std::uint16_t kMask = 0xffffU;
    constexpr std::uint32_t kTarget = 0x00080fdaU;
    (void)host.read_memory_word(kRegion, 0x00000052U, kMask);
    (void)host.read_memory_word(kRegion, 0x00000fdaU, kMask);
    (void)host.read_memory_word(kRegion, 0x00000fdcU, kMask);
    context.registers.program_counter = kTarget;
    (void)host.call_function(74U, 0U, 0xffU, 1U,
        0x0008004eU, kTarget, context);
    return FunctionResult::complete(3U, kTarget);
}
} // namespace gain_ground::translated
