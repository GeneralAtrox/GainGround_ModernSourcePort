#pragma once

#include "unverified_cpu_b_machine.h"

#include <cstdint>

namespace gain_ground::translated::cpu_b_initials_entry_detail {

[[nodiscard]] bool dispatch_initials_region_01(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next);
[[nodiscard]] bool dispatch_initials_region_02(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next);
[[nodiscard]] bool dispatch_initials_region_03(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next);
[[nodiscard]] bool dispatch_initials_region_04(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next);

} // namespace gain_ground::translated::cpu_b_initials_entry_detail
