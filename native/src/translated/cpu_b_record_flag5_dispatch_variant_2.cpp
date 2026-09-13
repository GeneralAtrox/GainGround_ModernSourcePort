#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {

FunctionResult cpu_b_update_wrapped_offset_and_ratio_common(
    FunctionContext &context,
    std::uint32_t entry_pc,
    std::uint32_t initial_child_id,
    std::uint32_t initial_callsite,
    std::uint32_t initial_target,
    std::uint32_t initial_continuation,
    std::uint32_t zero_callsite,
    std::uint32_t zero_continuation,
    std::uint32_t gate_callsite,
    std::uint32_t gate_continuation) noexcept;

FunctionResult cpu_b_record_flag5_dispatch_variant_2(FunctionContext &context) noexcept
{
    return cpu_b_update_wrapped_offset_and_ratio_common(context,
        0x0001d47cU,
        342U, 0x0001d486U, 0x0001d7d8U, 0x0001d48aU,
        0x0001d4c0U, 0x0001d4c4U,
        0x0001d4eaU, 0x0001d4eeU);
}

} // namespace gain_ground::translated
