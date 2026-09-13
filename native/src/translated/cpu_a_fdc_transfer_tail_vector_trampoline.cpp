#include "gain_ground/contract_types.h"
#include <cstdint>
namespace gain_ground::translated {
FunctionResult cpu_a_fdc_transfer_tail_vector_trampoline(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    (void)host.read_memory_word(3U, 0x88U, 0xffffU);
    (void)host.read_memory_word(1U, 0x2220U, 0xffffU);
    (void)host.read_memory_word(1U, 0x2222U, 0xffffU);
    context.registers.program_counter = 0x2220U;
    (void)host.call_function(36U, 0U, 0xffU, 1U, 0x80084U, 0x2220U, context);
    return FunctionResult::complete(3U, 0x2220U);
}
}
