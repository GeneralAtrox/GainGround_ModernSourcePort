#include "gain_ground/contract_types.h"

namespace gain_ground::translated {
FunctionResult runtime_entry_cpu_a_plain_0000342e(FunctionContext &context) noexcept
{
    if (context.host == nullptr) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    (void)host.read_memory_word(1U, 0x00003452U, 0xffffU);
    (void)host.read_memory_word(1U, 0x00003454U, 0xffffU);
    context.registers.program_counter = 0x00003452U;
    return host.call_function(511U, 0U, 0xffU, 1U, 0x0000342eU, 0x00003452U, context);
}
} // namespace gain_ground::translated
