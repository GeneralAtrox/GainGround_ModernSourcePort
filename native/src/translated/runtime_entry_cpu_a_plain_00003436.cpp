#include "gain_ground/contract_types.h"

namespace gain_ground::translated {
FunctionResult runtime_entry_cpu_a_plain_00003436(FunctionContext &context) noexcept
{
    if (context.host == nullptr) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    (void)host.read_memory_word(1U, 0x0000345eU, 0xffffU);
    (void)host.read_memory_word(1U, 0x00003460U, 0xffffU);
    context.registers.program_counter = 0x0000345eU;
    return host.call_function(512U, 0U, 0xffU, 1U, 0x00003436U, 0x0000345eU, context);
}
} // namespace gain_ground::translated
