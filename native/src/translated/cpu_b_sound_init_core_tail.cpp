#include "gain_ground/contract_types.h"

namespace gain_ground::translated {

FunctionResult cpu_b_sound_init_core_tail(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    context.registers.program_counter = 0x00017e2eU;
    return context.host->call_function(317U, 1U, 0x72U, 1U,
        0x00017072U, 0x00017e2eU, context);
}

} // namespace gain_ground::translated
