#pragma once

#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {

FunctionResult cpu_b_lookup_flagged_512_entry_common(
    FunctionContext &context, std::uint32_t table_base,
    bool inline_normalize_tail = false) noexcept;

} // namespace gain_ground::translated
