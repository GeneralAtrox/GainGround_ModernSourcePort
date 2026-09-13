#pragma once

#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {

// Executes an authoritative cross-owner continuation inside Function 10
// without emitting a synthetic 68000 call event. The caller must have already
// prefetched entry_pc, exactly as the transferring branch did on hardware.
FunctionResult cpu_a_fdc_read_span_continuation(
    FunctionContext &context, std::uint32_t entry_pc) noexcept;

} // namespace gain_ground::translated
