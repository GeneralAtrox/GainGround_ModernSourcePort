#include "gain_ground/contract_types.h"
#include <cstdint>
namespace gain_ground::translated {
FunctionResult cpu_a_sound_write_modulation_value(FunctionContext &context) noexcept
{
    if (!context.host) return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &registers = context.registers;
    auto &host = *context.host;
    (void)host.read_memory_word(3U, 0x42b4U, 0xffffU);
    const auto address = static_cast<std::uint32_t>(registers.address[3]
        + static_cast<std::int16_t>(registers.data[3]));
    const auto offset = address & 0x0003ffffU;
    const auto even_offset = offset & ~1U;
    const auto mask = static_cast<std::uint16_t>((offset & 1U) == 0U ? 0xff00U : 0x00ffU);
    const auto shift = (offset & 1U) == 0U ? 8U : 0U;
    const auto source = static_cast<std::uint8_t>(
        host.read_memory_word(3U, even_offset, mask) >> shift);
    const auto right = static_cast<std::uint8_t>(registers.data[2]);
    const auto sum = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(source) + right);
    const auto result = static_cast<std::uint8_t>(sum);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | result;
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(source ^ right)) & (source ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (sum > 0xffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
    (void)host.read_memory_word(3U, 0x42b6U, 0xffffU);
    (void)host.read_memory_word(3U, 0x42b8U, 0xffffU);
    if ((result & 0x80U) != 0U) {
        registers.data[1] = 0x7fU;
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x000fU);
    }
    (void)host.read_memory_word(3U, 0x42baU, 0xffffU);
    (void)host.read_memory_word(3U, 0x42bcU, 0xffffU);
    (void)host.read_memory_word(3U, 0x4126U, 0xffffU);
    (void)host.read_memory_word(3U, 0x4128U, 0xffffU);
    registers.program_counter = 0x84126U;
    (void)host.call_function(88U, 0U, 0xffU, 1U, 0x842baU, 0x84126U, context);
    return FunctionResult::complete(3U, 0x84126U);
}
}
