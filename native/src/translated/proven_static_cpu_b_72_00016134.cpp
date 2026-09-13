#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00016134(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &r = context.registers;
    const bool was_clear = (r.data[3] & 0x80000000U) == 0U;
    r.data[3] |= 0x80000000U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x0004U)
        | (was_clear ? 0x0004U : 0U));
    r.program_counter = 0x00016138U;
    return context.host->call_function(587U, 1U, 0x72U, 0U,
        0x00016134U, 0x00016138U, context);
}
} // namespace gain_ground::translated
