#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0001611e(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &r = context.registers;
    const auto address = r.address[5] + 0x7aU;
    const auto displacement = static_cast<std::int16_t>(
        context.host->read_memory_word(2U, address, 0xffffU));
    r.address[0] += static_cast<std::int32_t>(displacement);
    r.program_counter = 0x00016122U;
    return context.host->call_function(585U, 1U, 0x72U, 0U,
        0x0001611eU, 0x00016122U, context);
}
} // namespace gain_ground::translated
