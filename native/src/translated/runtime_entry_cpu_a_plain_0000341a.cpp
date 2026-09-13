#include "gain_ground/contract_types.h"

namespace gain_ground::translated {
FunctionResult runtime_entry_cpu_a_plain_0000341a(FunctionContext &context) noexcept
{
    if (context.host == nullptr) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    (void)host.read_memory_word(1U, 0x0000343aU, 0xffffU);
    (void)host.read_memory_word(1U, 0x0000343cU, 0xffffU);
    context.registers.program_counter = 0x0000343aU;
    return host.call_function(509U, 0U, 0xffU, 1U, 0x0000341aU, 0x0000343aU, context);
}
} // namespace gain_ground::translated
