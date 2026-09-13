#include "cpu_b_record_deadline_detail.h"

namespace gain_ground::translated {
FunctionResult cpu_b_update_record_with_deadline(FunctionContext &context) noexcept
{
    FunctionResult projectile_result;
    if (context.host && context.host->run_projectile_update(context, projectile_result))
        return projectile_result;
    return record_deadline_detail::execute<0x00011fccU, false>(context);
}
} // namespace gain_ground::translated
