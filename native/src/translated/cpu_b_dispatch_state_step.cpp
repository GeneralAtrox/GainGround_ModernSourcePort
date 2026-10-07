#include "cpu_b_dispatch_state_step_detail.h"

namespace gain_ground::translated {
using namespace cpu_b_dispatch_state_step_detail;

FunctionResult cpu_b_dispatch_state_step(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto flags_address = registers.address[5] + 0x41U;
    if (!bit_test(registers, read_byte(host, flags_address), 5U)) {
        const auto clear = call_child(context, 344U,
            0x0001d5bcU, 0x0001d902U, 0x0001d5c0U);
        if (!child_complete(clear)) return clear;
        if (!bit_test(registers, read_byte(host, flags_address), 5U))
            return finish(context);
        return run_tail(context, true);
    }

    push_return(host, registers, 0x0001d5eeU);
    registers.program_counter = 0x0001d614U;
    // Original BSR at 1D5EA targets the separately registered owner 549.
    // A successful child returns to 1D5EE; it does not finish this caller.
    const auto helper = host.call_function(549U,
        1U, 0x72U, 2U, 0x0001d5eaU, 0x0001d614U, context);
    if (helper.status == TranslationStatus::unimplemented) {
        run_embedded_helper(context);
    } else if (!child_complete(helper) || registers.program_counter != 0x0001d5eeU) {
        return helper;
    }
    if (!bit_test(registers, read_byte(host, flags_address), 5U))
        return finish(context);
    return run_tail(context, false);
}

} // namespace gain_ground::translated
