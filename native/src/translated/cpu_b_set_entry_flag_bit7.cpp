#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kHighByteMask = 0xff00U;
constexpr std::uint16_t kZero = 0x0004U;

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_set_entry_flag_bit7(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto flag_address = registers.address[6];
    const auto flag_word = host.read_memory_word(kRegion, flag_address, kHighByteMask);
    const auto original_byte = static_cast<std::uint8_t>(flag_word >> 8U);
    host.write_memory_word(kRegion, flag_address,
        static_cast<std::uint16_t>(original_byte | 0x80U) << 8U, kHighByteMask);
    if ((original_byte & 0x80U) == 0U)
        registers.status = static_cast<std::uint16_t>(registers.status | kZero);
    else
        registers.status = static_cast<std::uint16_t>(registers.status & ~kZero);

    const auto pointer_high = host.read_memory_word(
        kRegion, registers.address[3] + 0x20U, kWordMask);
    const auto pointer_low = host.read_memory_word(
        kRegion, registers.address[3] + 0x22U, kWordMask);
    registers.address[1] = (static_cast<std::uint32_t>(pointer_high) << 16U) | pointer_low;

    push_return(host, registers, 0x0001133eU);
    registers.program_counter = 0x00010c9aU;
    const auto child = host.call_function(223U, 1U, 0x72U, 2U,
        0x0001133aU, 0x00010c9aU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
