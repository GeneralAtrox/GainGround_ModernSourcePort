#include "gain_ground/contract_types.h"
#include <cstdint>
namespace gain_ground::translated {
FunctionResult cpu_a_trap8_fdc_drive_trampoline(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    (void)host.read_memory_word(3U, 0x8eU, 0xffffU);
    (void)host.read_memory_word(1U, 0x21ccU, 0xffffU);
    (void)host.read_memory_word(1U, 0x21ceU, 0xffffU);
    context.registers.program_counter = 0x21ccU;
    return host.call_function(34U, 0U, 0xffU, 1U, 0x8008aU, 0x21ccU, context);
}
}
