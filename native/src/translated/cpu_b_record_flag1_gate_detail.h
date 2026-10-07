#pragma once

#include "gain_ground/contract_types.h"

#include <cstddef>
#include <cstdint>

namespace gain_ground::translated::cpu_b_record_flag1_gate_detail {

inline constexpr std::uint16_t kRegion = 2U;
inline constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address);
void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value);
[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address);
void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value);
void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign_bit, std::uint32_t width_mask);
void set_zero_only(CpuRegisters &registers, bool zero);
void set_add_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result);
void set_sub_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result, bool preserve_x = false);
void set_sub_byte_flags(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result);
void set_asl_word_flags(CpuRegisters &registers, std::uint16_t input,
    unsigned count, std::uint16_t result);
void set_data_word(CpuRegisters &registers, std::size_t index, std::uint16_t value);
void bit_clear(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, std::uint8_t bit);
void bit_set(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, std::uint8_t bit);
void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value);
[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers);
[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation);
[[nodiscard]] bool signed_less(const CpuRegisters &registers);
[[nodiscard]] bool signed_less_equal(const CpuRegisters &registers);
[[nodiscard]] FunctionResult return_from_function(ExecutionHost &host,
    CpuRegisters &registers);

} // namespace gain_ground::translated::cpu_b_record_flag1_gate_detail
