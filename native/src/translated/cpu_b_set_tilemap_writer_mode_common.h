#pragma once

#include "gain_ground/contract_types.h"

namespace gain_ground::translated {

FunctionResult cpu_b_set_tilemap_writer_mode_common(
    FunctionContext &context, bool inline_tail = false) noexcept;

} // namespace gain_ground::translated
