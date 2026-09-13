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

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    std::uint16_t flags = registers.status & 0x0010U;
    if (right > left) flags |= 0x0001U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void shift_left_word_five(CpuRegisters &registers)
{
    const auto original = static_cast<std::uint16_t>(registers.data[1]);
    const auto result = static_cast<std::uint16_t>(original << 5U);
    std::uint16_t flags{};
    if ((original & 0x0800U) != 0U) flags |= 0x0011U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | result;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void rotate_left_word_seven(CpuRegisters &registers)
{
    const auto original = static_cast<std::uint16_t>(registers.data[2]);
    const auto result = static_cast<std::uint16_t>(
        (static_cast<std::uint32_t>(original) << 7U) | (original >> 9U));
    std::uint16_t flags = registers.status & 0x0010U;
    if ((original & 0x0200U) != 0U) flags |= 0x0001U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | result;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001e244(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    set_compare_word(registers, static_cast<std::uint16_t>(registers.data[1]),
        static_cast<std::uint16_t>(registers.data[2]));
    if ((registers.status & 0x0004U) != 0U) {
        registers.program_counter = 0x0001e22aU;
        return host.call_function(555U, 1U, 0x72U, 1U,
            0x0001e246U, 0x0001e22aU, context);
    }

    push_return(host, registers, 0x0001e24cU);
    registers.program_counter = 0x0001ebf0U;
    const auto child = host.call_function(363U, 1U, 0x72U, 2U,
        0x0001e248U, 0x0001ebf0U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    auto value = static_cast<std::uint16_t>(registers.data[7]);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | value;
    set_logic_word(registers, value);
    shift_left_word_five(registers);
    rotate_left_word_seven(registers);

    value = static_cast<std::uint16_t>(registers.data[2] & 1U);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | value;
    set_logic_word(registers, value);
    if (value == 0U) {
        registers.program_counter = 0x0001e25aU;
        return host.call_function(600U, 1U, 0x72U, 1U,
            0x0001e256U, 0x0001e25aU, context);
    }

    const auto left = static_cast<std::uint16_t>(registers.data[1]);
    const auto right = static_cast<std::uint16_t>(registers.data[7]);
    value = static_cast<std::uint16_t>(left + right);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | value;
    set_add_word(registers, left, right, value);
    registers.program_counter = 0x0001e25aU;
    return host.call_function(600U, 1U, 0x72U, 0U,
        0x0001e258U, 0x0001e25aU, context);
}

} // namespace gain_ground::translated
