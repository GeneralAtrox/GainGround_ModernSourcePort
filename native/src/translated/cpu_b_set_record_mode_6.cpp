#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_b_set_record_mode_6(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    context.host->write_memory_word(2U, context.registers.address[6] + 0x58U, 0x0006U, 0xffffU);
    context.registers.status = static_cast<std::uint16_t>(context.registers.status & ~0x000fU);
    context.registers.program_counter = 0x00011300U;
    (void)context.host->call_function(241U, 1U, 0x72U, 0U,
        0x000112faU, 0x00011300U, context);
    return FunctionResult::complete(3U, 0x00011300U);
}
} // namespace gain_ground::translated
