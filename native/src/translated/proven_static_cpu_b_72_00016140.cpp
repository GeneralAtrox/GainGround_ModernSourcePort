#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00016140(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &r = context.registers;
    r.data[0] = 0U;
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x001fU) | (r.status & 0x0010U) | 0x0004U);
    r.program_counter = 0x00016142U;
    return context.host->call_function(589U, 1U, 0x72U, 0U,
        0x00016140U, 0x00016142U, context);
}
} // namespace gain_ground::translated
