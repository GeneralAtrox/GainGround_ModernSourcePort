#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
FunctionResult cpu_b_test_entry_flag_8c_bit1_alt3_resume_10b02(
    FunctionContext &context) noexcept;

namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kMask = 0xffffU;

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const unsigned shift = odd ? 0U : 8U;
    return static_cast<std::uint8_t>(
        host.read_memory_word(kRegion, address & ~1U, mask) >> shift);
}

[[nodiscard]] bool test_bit_one(CpuRegisters &registers, std::uint8_t value)
{
    const bool set = (value & 0x02U) != 0U;
    if (set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
    return set;
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto high = host.read_memory_word(kRegion, registers.address[7], kMask);
    const auto low = host.read_memory_word(kRegion, registers.address[7] + 2U, kMask);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_test_entry_flag_8c_bit1_alt2(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &registers = context.registers;
    if (!test_bit_one(registers,
            read_byte(*context.host, registers.address[4] + 0x8cU)))
        return finish(context);

    registers.program_counter = 0x00010b02U;
    return cpu_b_test_entry_flag_8c_bit1_alt3_resume_10b02(context);
}
} // namespace gain_ground::translated
