#include "gain_ground/contract_types.h"

namespace gain_ground::translated {

FunctionResult cpu_b_probe_record_bounds_common(FunctionContext &context,
    bool secondary) noexcept;

FunctionResult cpu_b_probe_record_bounds_secondary(FunctionContext &context) noexcept
{
    return cpu_b_probe_record_bounds_common(context, true);
}

} // namespace gain_ground::translated
