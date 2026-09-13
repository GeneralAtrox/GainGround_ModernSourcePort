#include "gain_ground/contract_types.h"
namespace gain_ground::translated {
FunctionResult cpu_b_record_flag0_gate_variant(FunctionContext&) noexcept;
FunctionResult cpu_b_record_transition_helper(FunctionContext&context) noexcept
{
    if(context.registers.program_counter!=0x0001e3a6U)
        return {TranslationStatus::contract_violation,0U,context.registers.program_counter};
    return cpu_b_record_flag0_gate_variant(context);
}
}
