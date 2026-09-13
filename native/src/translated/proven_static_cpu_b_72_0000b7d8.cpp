#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {

FunctionResult proven_static_cpu_b_72_0000b7d8(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &registers = context.registers;
    if (registers.program_counter != 0x0000b7d8U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    registers.address[1] = 0x0000c5c0U;
    registers.program_counter = 0x0000891eU;
    return context.host->call_function(
        129U, 1U, 0x72U, 1U, 0x0000b7dcU, 0x0000891eU, context);
}

} // namespace gain_ground::translated
