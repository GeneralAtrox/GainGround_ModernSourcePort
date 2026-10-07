#pragma once

#include "gain_ground/contract_types.h"

#include <cstdint>
#include <optional>

namespace gain_ground::translated::cpu_b_record_flag1_gate_variant_detail {

inline constexpr std::uint32_t kTableBase = 0x00032decU;
inline constexpr std::uint16_t kExtendBit = 0x0010U;

struct ConditionCodes {
    bool carry{};
    bool overflow{};
    bool zero{};
    bool negative{};
};
[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address);
void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t data);
[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address);
void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value);
void bset_memory(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, unsigned bit);
void bclr_memory(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, unsigned bit);
[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address);
void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value);
[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers);
void set_logic_word(CpuRegisters &registers, std::uint16_t value);
void set_logic_long(CpuRegisters &registers, std::uint32_t value);
void set_swap_flags(CpuRegisters &registers, std::uint32_t value);
void commit_cc(CpuRegisters &registers, bool extend, const ConditionCodes &codes);
void add_word(CpuRegisters &registers, std::uint16_t addend,
    std::uint16_t &destination, bool &extend);
void sub_word_immediate(CpuRegisters &registers, std::uint16_t subtrahend,
    std::uint16_t &destination, bool &extend);
void and_word(CpuRegisters &registers, std::uint16_t mask,
    std::uint16_t &destination);
void lsl_word(CpuRegisters &registers, unsigned count,
    std::uint16_t &destination, bool &extend);
[[nodiscard]] bool compare_signed_gt(std::uint16_t left, std::uint16_t right,
    ConditionCodes &codes);
[[nodiscard]] bool test_bit_clear(std::uint8_t value, unsigned bit);
[[nodiscard]] std::optional<FunctionResult> scan_record_corners(
    FunctionContext &context, std::uint32_t base, std::uint16_t &x,
    std::uint16_t &y, bool &extend, std::uint16_t &d3);

} // namespace gain_ground::translated::cpu_b_record_flag1_gate_variant_detail
