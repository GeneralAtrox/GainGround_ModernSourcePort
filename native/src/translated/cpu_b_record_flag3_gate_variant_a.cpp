#include "cpu_b_record_flag3_gate_variants.h"

namespace gain_ground::translated {
FunctionResult cpu_b_record_flag3_gate_variant_a(FunctionContext &context) noexcept
{
    return record_flag3_detail::execute<0x00020b1aU, 0x22U, 332U, false>(context);
}
} // namespace gain_ground::translated
