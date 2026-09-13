#include "gain_ground/contract_types.h"
#include <cstdint>
namespace gain_ground::translated {
FunctionResult cpu_a_trap3_fdc_read_trampoline(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    (void)host.read_memory_word(3U, 0x70U, 0xffffU);
    (void)host.read_memory_word(1U, 0x21c0U, 0xffffU);
    (void)host.read_memory_word(1U, 0x21c2U, 0xffffU);
    context.registers.program_counter = 0x21c0U;
    (void)host.call_function(32U, 0U, 0xffU, 1U, 0x8006cU, 0x21c0U, context);
    return FunctionResult::complete(3U, 0x21c0U);
}
}
