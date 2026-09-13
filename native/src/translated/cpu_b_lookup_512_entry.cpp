#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    constexpr std::uint16_t kConditionCodeMask = 0x000fU;
    constexpr std::uint16_t kNegative = 0x0008U;
    constexpr std::uint16_t kZero = 0x0004U;
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U)
        flags |= kNegative;
    if (value == 0U)
        flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void set_logic_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    constexpr std::uint16_t kConditionCodeMask = 0x000fU;
    constexpr std::uint16_t kNegative = 0x0008U;
    constexpr std::uint16_t kZero = 0x0004U;
    std::uint16_t flags{};
    if ((value & 0x80000000U) != 0U)
        flags |= kNegative;
    if (value == 0U)
        flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void set_add_word_flags(
    CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    constexpr std::uint16_t kConditionCodeMask = 0x001fU;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    if ((((~(left ^ right)) & (left ^ result)) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kCpuBMainMemoryRegion, address, kFullWordMask);
    const auto low = host.read_memory_word(kCpuBMainMemoryRegion, address + 2U, kFullWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_lookup_512_entry(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    const std::uint32_t index = context.registers.data[1] & 0x000001ffU;
    context.registers.data[1] = index;
    set_logic_long_flags(context.registers, index);

    auto word = static_cast<std::uint16_t>(context.registers.data[1]);
    auto doubled = static_cast<std::uint16_t>(word + word);
    context.registers.data[1] = (context.registers.data[1] & 0xffff0000U) | doubled;
    set_add_word_flags(context.registers, word, word, doubled);

    word = doubled;
    doubled = static_cast<std::uint16_t>(word + word);
    context.registers.data[1] = (context.registers.data[1] & 0xffff0000U) | doubled;
    set_add_word_flags(context.registers, word, word, doubled);
    context.registers.address[0] += context.registers.data[1];

    const auto first = host.read_memory_word(
        kCpuBMainMemoryRegion, context.registers.address[0], kFullWordMask);
    context.registers.address[0] += 2U;
    context.registers.data[1] = first;
    set_move_word_flags(context.registers, first);

    const auto second = host.read_memory_word(
        kCpuBMainMemoryRegion, context.registers.address[0], kFullWordMask);
    context.registers.address[0] += 2U;
    context.registers.data[0] = (context.registers.data[0] & 0xffff0000U) | second;
    set_move_word_flags(context.registers, second);

    const std::uint32_t return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
