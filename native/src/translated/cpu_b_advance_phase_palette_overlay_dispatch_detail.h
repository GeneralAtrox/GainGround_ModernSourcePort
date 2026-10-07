#pragma once

#include "cpu_b_scene_timing.h"

#include <cstdint>

namespace gain_ground::translated::cpu_b_advance_phase_palette_overlay_dispatch_detail {

[[nodiscard]] bool dispatch_lookup_setup(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, CpuBIrqTiming &timing,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_overlay_updates(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, CpuBIrqTiming &timing,
    std::uint32_t pc, std::uint32_t &next, std::uint8_t &transfer_kind);

} // namespace gain_ground::translated::cpu_b_advance_phase_palette_overlay_dispatch_detail
