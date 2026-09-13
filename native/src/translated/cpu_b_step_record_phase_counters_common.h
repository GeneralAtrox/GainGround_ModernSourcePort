#pragma once

#include "gain_ground/contract_types.h"

namespace gain_ground::translated {

FunctionResult cpu_b_step_record_phase_counters_common(
    FunctionContext &context, bool start_at_counter) noexcept;

} // namespace gain_ground::translated
