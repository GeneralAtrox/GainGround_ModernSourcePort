#pragma once

#include "unverified_cpu_b_machine.h"

#include <cstdint>

namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail {

[[nodiscard]] bool dispatch_table_region_01(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_table_region_02(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_table_region_03(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_table_region_04(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_table_region_05(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine,
    std::uint32_t pc, std::uint32_t &next,
    std::uint8_t &transfer_kind);

} // namespace gain_ground::translated::cpu_b_callback_state_table_dispatch_detail
