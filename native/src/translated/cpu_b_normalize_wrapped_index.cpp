#include "gain_ground/contract_types.h"
#include "cpu_b_normalize_wrapped_index_common.h"

#include <cstdint>
#include <utility>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

void set_compare_word_flags(
    CpuRegisters &registers, std::uint16_t destination, std::uint16_t source)
{
    constexpr std::uint16_t kConditionCodeMaskWithoutExtend = 0x000fU;
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    if ((((destination ^ source) & (destination ^ result)) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (destination < source)
        flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMaskWithoutExtend) | flags);
}

void set_subtract_word_flags(
    CpuRegisters &registers, std::uint16_t destination,
    std::uint16_t source, std::uint16_t result)
{
    constexpr std::uint16_t kConditionCodeMask = 0x001fU;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    if ((((destination ^ source) & (destination ^ result)) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (destination < source)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void set_negate_word_flags(
    CpuRegisters &registers, std::uint16_t operand, std::uint16_t result)
{
    constexpr std::uint16_t kConditionCodeMask = 0x001fU;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    if (operand == 0x8000U)
        flags |= 0x0002U;
    if (operand != 0U)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t result)
{
    constexpr std::uint16_t kConditionCodeMaskWithoutExtend = 0x000fU;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMaskWithoutExtend) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    registers.address[7] -= 4U;
    const auto stack = registers.address[7] & kAddressMask;
    host.write_memory_word(
        kCpuBMainMemoryRegion, stack,
        static_cast<std::uint16_t>(address >> 16U), kFullWordMask);
    host.write_memory_word(
        kCpuBMainMemoryRegion, (stack + 2U) & kAddressMask,
        static_cast<std::uint16_t>(address), kFullWordMask);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7] & kAddressMask;
    const auto high = host.read_memory_word(
        kCpuBMainMemoryRegion, stack, kFullWordMask);
    const auto low = host.read_memory_word(
        kCpuBMainMemoryRegion, (stack + 2U) & kAddressMask, kFullWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_normalize_wrapped_index_common(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto initial = static_cast<std::uint16_t>(registers.data[1]);
    set_compare_word_flags(registers, initial, 0x0101U);

    if ((registers.status & 0x0001U) != 0U) {
        registers.program_counter = 0x00016308U;
        return host.call_function(
            303U, 1U, 0x72U, 1U, 0x000162f6U, 0x00016308U, context);
    }

    const auto subtracted = static_cast<std::uint16_t>(initial - 0x0200U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | subtracted;
    set_subtract_word_flags(registers, initial, 0x0200U, subtracted);

    const auto negated = static_cast<std::uint16_t>(0U - subtracted);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | negated;
    set_negate_word_flags(registers, subtracted, negated);

    const auto masked = static_cast<std::uint16_t>(negated & 0x00ffU);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | masked;
    set_logic_word_flags(registers, masked);

    push_return(host, registers, 0x00016304U);
    registers.program_counter = 0x00016308U;
    const auto child = host.call_function(
        303U, 1U, 0x72U, 2U, 0x00016302U, 0x00016308U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    std::swap(registers.data[0], registers.data[1]);
    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

FunctionResult cpu_b_normalize_wrapped_index(FunctionContext &context) noexcept
{
    return cpu_b_normalize_wrapped_index_common(context);
}

} // namespace gain_ground::translated
