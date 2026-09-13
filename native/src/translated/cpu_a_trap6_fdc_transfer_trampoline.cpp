#include "gain_ground/contract_types.h"
#include <cstdint>
namespace gain_ground::translated {
FunctionResult cpu_a_trap6_fdc_transfer_trampoline(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    (void)host.read_memory_word(3U, 0x82U, 0xffffU);
    (void)host.read_memory_word(1U, 0x220cU, 0xffffU);
    (void)host.read_memory_word(1U, 0x220eU, 0xffffU);
    context.registers.program_counter = 0x220cU;
    return host.call_function(35U, 0U, 0xffU, 1U, 0x8007eU, 0x220cU, context);
}
}
