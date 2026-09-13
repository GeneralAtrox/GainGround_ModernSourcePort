#include "cpu_b_record_flag3_gate_variants.h"

#include <array>

namespace gain_ground::translated {
FunctionResult cpu_b_record_flag3_gate(FunctionContext &context) noexcept
{
    constexpr std::uint32_t entry = 0x00020990U;
    if (context.host == nullptr || context.registers.program_counter != entry)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];
    const auto flags = record_flag3_detail::read_byte(host, record + 0x41U);
    const bool alternate = (flags & 0x08U) != 0U;
    if (alternate)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x04U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x04U);

    const auto gate = record_flag3_detail::read_byte(host, record + 0x3fU);
    record_flag3_detail::set_test_flags(registers, gate);
    if (gate != 0U) {
        registers.address[6] = 0x00003400U;
        record_flag3_detail::increment_byte(host, registers,
            registers.address[6] + 0x22U);
    }

    if (alternate) {
        constexpr std::array<std::uint32_t, 6> ids{
            334U, 352U, 353U, 357U, 360U, 365U};
        constexpr std::array<std::uint32_t, 6> targets{
            0x0001cef6U, 0x0001dbc4U, 0x0001dbf0U,
            0x0001e124U, 0x0001e304U, 0x0001ece4U};
        for (std::size_t index = 0; index < ids.size(); ++index) {
            const auto result = record_flag3_detail::call_child(context,
                ids[index], 0x000209a6U + static_cast<std::uint32_t>(index) * 4U,
                targets[index]);
            if (result.status != TranslationStatus::complete
                || result.control != 1U)
                return result;
        }
        return record_flag3_detail::return_from(context);
    }

    constexpr std::array<std::uint32_t, 8> ids{
        330U, 368U, 352U, 353U, 355U, 356U, 357U, 360U};
    constexpr std::array<std::uint32_t, 8> targets{
        0x0001c1e0U, 0x0001efdcU, 0x0001dbc4U, 0x0001dbf0U,
        0x0001de30U, 0x0001e0eeU, 0x0001e124U, 0x0001e304U};
    for (std::size_t index = 0; index < ids.size(); ++index) {
        const auto result = record_flag3_detail::call_child(context,
            ids[index], 0x000209ceU + static_cast<std::uint32_t>(index) * 4U,
            targets[index]);
        if (result.status != TranslationStatus::complete
            || result.control != 1U)
            return result;
    }
    return record_flag3_detail::return_from(context);
}
} // namespace gain_ground::translated
