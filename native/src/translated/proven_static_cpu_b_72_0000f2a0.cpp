#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionMask = 0x000fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kCarry = 0x0001U;

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(
        kPrivateRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value) noexcept
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x80U) != 0U)
        flags |= kNegative;
    if (value == 0U)
        flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~(kExtend | kConditionMask)) | flags);
}

void set_logic_long_flags(CpuRegisters &registers,
                          std::uint32_t value) noexcept
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x80000000U) != 0U)
        flags |= kNegative;
    if (value == 0U)
        flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~(kExtend | kConditionMask)) | flags);
}

void push_return(FunctionContext &context, std::uint32_t address) noexcept
{
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    context.host->write_memory_word(kPrivateRegion, registers.address[7],
        static_cast<std::uint16_t>(address >> 16U), kWordMask);
    context.host->write_memory_word(kPrivateRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(address), kWordMask);
}

std::uint32_t pop_return(FunctionContext &context) noexcept
{
    auto &registers = context.registers;
    const auto stack = registers.address[7];
    const auto high = context.host->read_memory_word(
        kPrivateRegion, stack, kWordMask);
    const auto low = context.host->read_memory_word(
        kPrivateRegion, stack + 2U, kWordMask);
    registers.address[7] = stack + 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000f2a0(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &registers = context.registers;
    auto &host = *context.host;

    registers.data[2] = 0U;
    set_logic_long_flags(registers, registers.data[2]);

    const auto value = read_byte(host, registers.address[4] + 0x9fU);
    registers.data[2] = (registers.data[2] & 0xffffff00U) | value;
    set_logic_byte_flags(registers, value);

    const auto before_rotate = registers.data[2];
    registers.data[2] = (before_rotate >> 8U) | (before_rotate << 24U);
    std::uint16_t rotate_flags = registers.status & kExtend;
    if ((registers.data[2] & 0x80000000U) != 0U)
        rotate_flags |= kNegative;
    if (registers.data[2] == 0U)
        rotate_flags |= kZero;
    if ((before_rotate & 0x00000080U) != 0U)
        rotate_flags |= kCarry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~(kExtend | kConditionMask)) | rotate_flags);

    registers.address[1] = 0x0001066aU;
    push_return(context, 0x0000f2b2U);
    registers.program_counter = 0x0001610eU;
    const auto child = host.call_function(
        296U, 1U, 0x72U, 2U, 0x0000f2acU, 0x0001610eU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto target = pop_return(context);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
