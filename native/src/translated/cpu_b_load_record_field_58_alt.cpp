#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionCodeMask = 0x001fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kZero = 0x0004U;
} // namespace

FunctionResult cpu_b_load_record_field_58_alt(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    const auto source = host.read_memory_word(
        kRegion, context.registers.address[5] + 0x58U, kWordMask);
    const auto shifted = static_cast<std::uint16_t>(source << 8U);
    context.registers.data[0] =
        (context.registers.data[0] & 0xffff0000U) | shifted;

    host.write_memory_word(
        kRegion, context.registers.address[6] + 0x58U, shifted, kWordMask);

    const auto clear_address = context.registers.address[6] + 0x46U;
    (void)host.read_memory_word(kRegion, clear_address, kWordMask);
    host.write_memory_word(kRegion, clear_address, 0U, kWordMask);

    const auto extend = (source & 0x0100U) != 0U ? kExtend : 0U;
    context.registers.status = static_cast<std::uint16_t>(
        (context.registers.status & ~kConditionCodeMask) | extend | kZero);
    context.registers.program_counter = 0x0001131cU;
    (void)host.call_function(243U, 1U, 0x72U, 0U,
        0x00011318U, 0x0001131cU, context);
    return FunctionResult::complete(3U, 0x0001131cU);
}

} // namespace gain_ground::translated
