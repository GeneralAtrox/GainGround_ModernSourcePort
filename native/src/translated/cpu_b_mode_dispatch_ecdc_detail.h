#pragma once

#include "cpu_b_scene_timing.h"

#include <cstdint>
#include <optional>

namespace gain_ground::translated::cpu_b_mode_dispatch_ecdc_detail {

[[nodiscard]] bool dispatch_record_setup(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind, bool &returned_from_child,
    std::optional<FunctionResult> &outcome);

} // namespace gain_ground::translated::cpu_b_mode_dispatch_ecdc_detail
