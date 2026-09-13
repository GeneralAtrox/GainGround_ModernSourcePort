#include "cpu_b_record_flag3_gate_variants.h"

#include <array>

namespace gain_ground::translated {
FunctionResult cpu_b_record_position_gate_tail(FunctionContext &context) noexcept
{
    constexpr std::uint32_t entry = 0x00020c9aU;
    if (context.host == nullptr || context.registers.program_counter != entry)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto record = registers.address[5];
    const auto reload = record_flag3_detail::read_byte(host, record + 0x3fU);
    record_flag3_detail::set_test_flags(registers, reload);
    if (reload != 0U) {
        host.write_memory_word(record_flag3_detail::kRegion,
            (record + 0x25eU) & 0x00ffffffU, 0x0808U,
            record_flag3_detail::kWordMask);
        registers.status = static_cast<std::uint16_t>(
            registers.status & ~0x000fU);
    }

    constexpr std::array<std::uint32_t, 7> ids{
        326U, 352U, 353U, 355U, 356U, 357U, 359U};
    constexpr std::array<std::uint32_t, 7> targets{
        0x0001bd0cU, 0x0001dbc4U, 0x0001dbf0U, 0x0001de30U,
        0x0001e0eeU, 0x0001e124U, 0x0001e180U};
    for (std::size_t index = 0; index < ids.size(); ++index) {
        const auto result = record_flag3_detail::call_child(context,
            ids[index], 0x00020ca6U + static_cast<std::uint32_t>(index) * 4U,
            targets[index]);
        if (result.status != TranslationStatus::complete || result.control != 1U)
            return result;
    }
    return record_flag3_detail::return_from(context);
}
} // namespace gain_ground::translated
