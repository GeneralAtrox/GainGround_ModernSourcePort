#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {

FunctionResult cpu_b_record_block_scan_variant_common(FunctionContext &context,
    bool clear_selector_on_exhaustion, std::uint32_t child_callsite,
    std::uint32_t child_continuation) noexcept;

FunctionResult cpu_b_record_block_scan_variant(FunctionContext &context) noexcept
{
    return cpu_b_record_block_scan_variant_common(
        context, false, 0x0001d85eU, 0x0001d862U);
}

} // namespace gain_ground::translated
