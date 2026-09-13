#include "gain_ground/contract_types.h"

namespace gain_ground::translated {

FunctionResult cpu_b_apply_mixer_priorities(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    context.registers.address[1] = 0x0000abd8U;
    context.registers.program_counter = 0x0000891eU;
    (void)context.host->call_function(129U, 1U, 0x72U, 1U,
        0x0000a7ceU, 0x0000891eU, context);
    return FunctionResult::complete(3U, 0x0000891eU);
}

} // namespace gain_ground::translated
