#include "cpu_b_record_flag3_gate_variants.h"

namespace gain_ground::translated {
FunctionResult cpu_b_record_flag3_gate_variant_d(FunctionContext &context) noexcept
{
    return record_flag3_detail::execute<0x00020c3aU, 0x28U, 331U, true>(context);
}
} // namespace gain_ground::translated
