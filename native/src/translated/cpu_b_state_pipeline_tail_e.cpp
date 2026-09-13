#include "cpu_b_record_flag3_gate_variants.h"

#include <array>

namespace gain_ground::translated {
FunctionResult cpu_b_state_pipeline_tail_e(FunctionContext &context) noexcept
{
    constexpr std::uint32_t entry = 0x00020ce4U;
    if (context.host == nullptr || context.registers.program_counter != entry)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    constexpr std::array<std::uint32_t, 3> ids{327U, 372U, 361U};
    constexpr std::array<std::uint32_t, 3> targets{
        0x0001bdc0U, 0x0001f24aU, 0x0001e3a6U};
    for (std::size_t index = 0; index < ids.size(); ++index) {
        const auto result = record_flag3_detail::call_child(context,
            ids[index], entry + static_cast<std::uint32_t>(index) * 4U,
            targets[index]);
        if (result.status != TranslationStatus::complete || result.control != 1U)
            return result;
    }
    return record_flag3_detail::return_from(context);
}
} // namespace gain_ground::translated
