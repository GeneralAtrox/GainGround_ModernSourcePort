#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t return_address)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(return_address >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(return_address), kWordMask);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void rotate_left_word_seven(CpuRegisters &registers)
{
    const auto original = static_cast<std::uint16_t>(registers.data[2]);
    const auto result = static_cast<std::uint16_t>(
        (static_cast<std::uint32_t>(original) << 7U) | (original >> 9U));
    std::uint16_t flags = registers.status & 0x0010U;
    if ((original & 0x0200U) != 0U)
        flags |= 0x0001U;
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | result;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | flags);
}

void subtract_one_word(CpuRegisters &registers)
{
    const auto left = static_cast<std::uint16_t>(registers.data[2]);
    const auto result = static_cast<std::uint16_t>(left - 1U);
    std::uint16_t flags{};
    if (left < 1U)
        flags |= 0x0011U;
    if (((left ^ 1U) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | result;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001e1c6(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    push_return(host, registers, 0x0001e1caU);
    registers.program_counter = 0x0001ebf0U;
    const auto child = host.call_function(363U, 1U, 0x72U, 2U,
        0x0001e1c6U, 0x0001ebf0U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    registers.data[1] = 0U;
    set_logic_long(registers, 0U);
    rotate_left_word_seven(registers);
    subtract_one_word(registers);

    const bool negative = (registers.status & 0x0008U) != 0U;
    const auto child_id = negative ? 596U : 595U;
    const auto target = negative ? 0x0001e1d8U : 0x0001e1d2U;
    registers.program_counter = target;
    return host.call_function(child_id, 1U, 0x72U, 1U,
        0x0001e1d0U, target, context);
}

} // namespace gain_ground::translated
