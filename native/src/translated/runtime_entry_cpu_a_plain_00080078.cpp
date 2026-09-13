#include "gain_ground/contract_types.h"

namespace gain_ground::translated {
FunctionResult runtime_entry_cpu_a_plain_00080078(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    (void)host.read_memory_word(3U, 0x0000007cU, 0xffffU);
    (void)host.read_memory_word(1U, 0x00002200U, 0xffffU);
    (void)host.read_memory_word(1U, 0x00002202U, 0xffffU);
    context.registers.program_counter = 0x00002200U;
    return host.call_function(499U, 0U, 0xffU, 1U,
        0x00080078U, 0x00002200U, context);
}
} // namespace gain_ground::translated
