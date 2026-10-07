#pragma once

#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated::cpu_b_dispatch_state_step_detail {

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address);
[[nodiscard]] bool bit_test(CpuRegisters &registers, std::uint8_t value, unsigned bit);
void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value);
[[nodiscard]] FunctionResult finish(FunctionContext &context);
[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation);
[[nodiscard]] bool child_complete(const FunctionResult &result);
void run_embedded_helper(FunctionContext &context);
[[nodiscard]] FunctionResult run_tail(FunctionContext &context,
    bool clear_accumulator_first);

} // namespace gain_ground::translated::cpu_b_dispatch_state_step_detail
