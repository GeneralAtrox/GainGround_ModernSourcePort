#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

bool test_high_byte_bit(FunctionContext &context, std::uint32_t address, std::uint8_t bit)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = context.host->read_memory_word(kPrivate, address & ~1U, mask);
    const auto byte = static_cast<std::uint8_t>(odd ? word : word >> 8U);
    const bool set = (byte & (1U << bit)) != 0U;
    if (set)
        context.registers.status &= static_cast<std::uint16_t>(~0x0004U);
    else
        context.registers.status |= 0x0004U;
    return set;
}

} // namespace

FunctionResult cpu_b_sound_entry_gate_bit3(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    if (test_high_byte_bit(context, 0x00000820U, 3U)) {
        context.registers.program_counter = 0x0001700cU;
        return context.host->call_function(309U, 1U, 0x72U, 1U,
            0x00017000U, 0x0001700cU, context);
    }

    if (test_high_byte_bit(context, 0x00000403U, 1U)) {
        context.registers.program_counter = 0x0001700cU;
        return context.host->call_function(309U, 1U, 0x72U, 1U,
            0x0001700aU, 0x0001700cU, context);
    }

    context.registers.program_counter = 0x00017024U;
    return context.host->call_function(543U, 1U, 0x72U, 1U,
        0x0001700aU, 0x00017024U, context);
}

} // namespace gain_ground::translated
