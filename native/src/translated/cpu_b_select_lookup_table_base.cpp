#include "gain_ground/contract_types.h"

namespace gain_ground::translated {

FunctionResult cpu_b_select_lookup_table_base(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    context.registers.address[0] = 0x0001640aU;
    context.registers.program_counter = 0x00016376U;
    return context.host->call_function(542U, 1U, 0x72U, 1U,
        0x00016370U, 0x00016376U, context);
}

} // namespace gain_ground::translated
