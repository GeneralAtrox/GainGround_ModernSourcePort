#include "cpu_b_record_flag3_gate_variants.h"

namespace gain_ground::translated {
FunctionResult cpu_b_record_flag3_gate_variant_c(FunctionContext &context) noexcept
{
    return record_flag3_detail::execute<0x00020bdaU, 0x26U, 331U, false>(context);
}
} // namespace gain_ground::translated
