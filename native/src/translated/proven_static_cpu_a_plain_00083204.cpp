#include "gain_ground/contract_types.h"

namespace gain_ground::translated {

FunctionResult proven_static_cpu_a_plain_00083204(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    (void)host.read_memory_word(3U, 0x00004090U, 0xffffU);
    (void)host.read_memory_word(3U, 0x00004092U, 0xffffU);
    context.registers.program_counter = 0x00084090U;
    return host.call_function(467U, 0U, 0xffU, 1U,
        0x00083204U, 0x00084090U, context);
}

} // namespace gain_ground::translated
