#include "cpu_b_record_deadline_detail.h"

namespace gain_ground::translated {
FunctionResult cpu_b_update_record_with_deadline_alt(FunctionContext &context) noexcept
{
    return record_deadline_detail::execute<0x00012008U, true>(context);
}
} // namespace gain_ground::translated
