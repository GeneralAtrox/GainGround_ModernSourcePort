#pragma once

#include "cpu_b_scene_timing.h"

#include <cstdint>

namespace gain_ground::translated::cpu_b_build_sprite_lists_detail {

[[nodiscard]] bool dispatch_pc_range_01(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_pc_range_02(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_pc_range_03(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_pc_range_04(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_pc_range_05(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_pc_range_06(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    CpuBIrqTiming &timing, std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);

} // namespace gain_ground::translated::cpu_b_build_sprite_lists_detail
