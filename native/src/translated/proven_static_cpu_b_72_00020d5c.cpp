#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0001e22a(FunctionContext &) noexcept;
FunctionResult proven_static_cpu_b_72_00020d5c(FunctionContext &context) noexcept
{
    auto &r = context.registers;
    if (context.host == nullptr || r.program_counter != 0x20d5cU)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    auto &host = *context.host;
    const auto call = [&](std::uint32_t id, std::uint32_t site,
                          std::uint32_t target, std::uint32_t next) {
        r.address[7] -= 4U;
        host.write_memory_word(2U, r.address[7], static_cast<std::uint16_t>(next >> 16U), 0xffffU);
        host.write_memory_word(2U, r.address[7] + 2U, static_cast<std::uint16_t>(next), 0xffffU);
        r.program_counter = target;
        return host.call_function(id, 1U, 0x72U, 2U, site, target, context);
    };
    auto result = call(327U, 0x20d5cU, 0x1bdc0U, 0x20d60U);
    if (result.status != TranslationStatus::complete || result.control != 1U) return result;
    result = call(368U, 0x20d60U, 0x1efdcU, 0x20d64U);
    if (result.status != TranslationStatus::complete || result.control != 1U) return result;
    result = call(361U, 0x20d64U, 0x1e3a6U, 0x20d68U);
    // The host records sequential owner boundaries without executing their body.
    // Continue the captured 554 -> 555 fallthrough without submitting it twice.
    if (result.status == TranslationStatus::complete && result.control == 3U
        && r.program_counter == 0x1e22aU)
        result = proven_static_cpu_b_72_0001e22a(context);
    if (result.status != TranslationStatus::complete || result.control != 1U) return result;
    const auto high = host.read_memory_word(2U, r.address[7], 0xffffU);
    const auto low = host.read_memory_word(2U, r.address[7] + 2U, 0xffffU);
    r.address[7] += 4U;
    r.program_counter = (static_cast<std::uint32_t>(high) << 16U) | low;
    return FunctionResult::complete(1U, r.program_counter);
}
} // namespace gain_ground::translated
