#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(kPrivateRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : (word >> 8U));
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_test_entry_flag_8c_bit2(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto flags = read_byte(host, registers.address[4] + 0x8cU);
    if ((flags & 0x04U) == 0U)
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);

    if ((flags & 0x04U) != 0U) {
        push_return(host, registers, 0x00010d76U);
        registers.program_counter = 0x00010e48U;
        const auto child = host.call_function(229U, 1U, 0x72U, 2U,
            0x00010d72U, 0x00010e48U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
