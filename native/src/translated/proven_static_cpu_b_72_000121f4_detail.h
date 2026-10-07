#pragma once

#include "unverified_cpu_b_machine.h"

#include <cstdint>

namespace gain_ground::translated::proven_static_cpu_b_72_000121f4_detail {

[[nodiscard]] bool dispatch_descriptor_setup(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_record_fields(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind);
[[nodiscard]] bool dispatch_tail_fields(FunctionContext &context,
    CpuRegisters &registers, unverified::Machine &machine, std::uint32_t pc,
    std::uint32_t &next, std::uint8_t &transfer_kind);

} // namespace gain_ground::translated::proven_static_cpu_b_72_000121f4_detail
