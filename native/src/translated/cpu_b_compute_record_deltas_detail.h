#pragma once

#include "gain_ground/contract_types.h"

#include <cstdint>
#include <optional>

namespace gain_ground::translated::cpu_b_compute_record_deltas_detail {

inline constexpr std::uint16_t kNegativeBit = 0x0008U;

[[nodiscard]] bool get_x(CpuRegisters &registers);
void set_x(CpuRegisters &registers, bool value);
[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address);
void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t data);
[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address);
void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value);
[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers);
void commit_nzvc(CpuRegisters &registers, bool n, bool z, bool v, bool c);
void commit_full(CpuRegisters &registers, bool x, bool n, bool z, bool v, bool c);
[[nodiscard]] std::optional<FunctionResult> prepare_phase(
    FunctionContext &context, std::uint32_t a5);

} // namespace gain_ground::translated::cpu_b_compute_record_deltas_detail
