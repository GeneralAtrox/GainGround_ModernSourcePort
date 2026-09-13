#include "gain_ground/contract_types.h"
#include <cstdint>
namespace gain_ground::translated {
FunctionResult cpu_a_trap10_fdc_read_trampoline(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    (void)host.read_memory_word(3U, 0x9aU, 0xffffU);
    (void)host.read_memory_word(1U, 0x21c6U, 0xffffU);
    (void)host.read_memory_word(1U, 0x21c8U, 0xffffU);
    context.registers.program_counter = 0x21c6U;
    return host.call_function(33U, 0U, 0xffU, 1U, 0x80096U, 0x21c6U, context);
}
}
