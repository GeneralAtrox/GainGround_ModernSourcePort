#pragma once

#include "cpu_b_scene_timing.h"

#include <cstdint>

namespace gain_ground::translated::cpu_b_mode_dispatch_d368_detail {

[[nodiscard]] bool dispatch_alternate_paths(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);

} // namespace gain_ground::translated::cpu_b_mode_dispatch_d368_detail
