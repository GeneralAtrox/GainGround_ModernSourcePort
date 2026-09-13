#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (left < right)
        flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void arithmetic_shift_left_two(CpuRegisters &registers)
{
    const auto original = static_cast<std::uint16_t>(registers.data[7]);
    const auto result = static_cast<std::uint16_t>(original << 2U);
    const bool carry = (original & 0x4000U) != 0U;
    const bool overflow = ((original ^ static_cast<std::uint16_t>(original << 1U))
            | (static_cast<std::uint16_t>(original << 1U) ^ result))
        & 0x8000U;
    std::uint16_t flags{};
    if (carry)
        flags |= 0x0011U;
    if (overflow)
        flags |= 0x0002U;
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    registers.data[7] = (registers.data[7] & 0xffff0000U) | result;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void logical_shift_right_eight(CpuRegisters &registers)
{
    const auto original = static_cast<std::uint16_t>(registers.data[0]);
    const auto result = static_cast<std::uint16_t>(original >> 8U);
    std::uint16_t flags{};
    if ((original & 0x0080U) != 0U)
        flags |= 0x0011U;
    if (result == 0U)
        flags |= 0x0004U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001e1d8(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    arithmetic_shift_left_two(registers);

    auto value = host.read_memory_word(
        kRegion, registers.address[5] + 0x5cU, kWordMask);
    value = host.enemy_walking_heading(registers.address[5], value);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    set_logic_word(registers, value);

    auto result = static_cast<std::uint16_t>(value + 0x0080U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    set_add_word_flags(registers, value, 0x0080U, result);
    logical_shift_right_eight(registers);

    value = static_cast<std::uint16_t>(registers.data[0]);
    result = static_cast<std::uint16_t>(value & 0x0007U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    set_logic_word(registers, result);

    value = result;
    result = static_cast<std::uint16_t>(value - 1U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | result;
    set_sub_word_flags(registers, value, 1U, result);

    const bool negative = (registers.status & 0x0008U) != 0U;
    const auto child_id = negative ? 554U : 597U;
    const auto target = negative ? 0x0001e1f2U : 0x0001e1ecU;
    registers.program_counter = target;
    const auto child = host.call_function(child_id, 1U, 0x72U, 1U,
        0x0001e1eaU, target, context);
    if (child.status != TranslationStatus::complete)
        return child;
    if (child.control == 3U
        && child.exit_program_counter == 0x0001e22aU)
        return proven_static_cpu_b_72_0001e22a(context);
    return child;
}

} // namespace gain_ground::translated
