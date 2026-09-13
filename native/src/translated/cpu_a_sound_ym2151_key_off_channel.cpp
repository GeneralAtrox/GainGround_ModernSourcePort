#include "gain_ground/contract_types.h"
#include <cstdint>
namespace gain_ground::translated {
FunctionResult cpu_a_sound_ym2151_key_off_channel(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &registers = context.registers;
    auto &host = *context.host;
    registers.data[1] = (registers.data[1] & 0xffffff00U) | (registers.data[7] & 0xffU);
    registers.data[0] = 8U;
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
    (void)host.read_memory_word(3U, 0x4158U, 0xffffU);
    (void)host.read_memory_word(3U, 0x415aU, 0xffffU);
    (void)host.read_memory_word(3U, 0x4126U, 0xffffU);
    (void)host.read_memory_word(3U, 0x4128U, 0xffffU);
    registers.program_counter = 0x84126U;
    (void)host.call_function(88U, 0U, 0xffU, 1U, 0x84158U, 0x84126U, context);
    return FunctionResult::complete(3U, 0x84126U);
}
}
