#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kTilemapARegion = 5U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint32_t kTilemapABase = 0x00200000U;
constexpr std::uint32_t kMainMemoryMask = 0x0003ffffU;

[[nodiscard]] std::int32_t signed_word(std::uint16_t value)
{
    return static_cast<std::int16_t>(value);
}

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    constexpr std::uint16_t kConditionCodeMaskWithoutExtend = 0x000fU;
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U)
        flags |= 0x0008U;
    if (value == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMaskWithoutExtend) | flags);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7] & kMainMemoryMask;
    const auto high = host.read_memory_word(
        kCpuBMainMemoryRegion, stack, kFullWordMask);
    const auto low = host.read_memory_word(
        kCpuBMainMemoryRegion, (stack + 2U) & kMainMemoryMask, kFullWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_write_tilemap_a_rows_with_bias(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[0] = kTilemapABase;

    const auto first_bias = host.read_memory_word(
        kCpuBMainMemoryRegion, registers.address[1] & kMainMemoryMask, kFullWordMask);
    registers.address[1] += 2U;
    registers.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(registers.address[0]) + signed_word(first_bias));

    const auto initial_count = host.read_memory_word(
        kCpuBMainMemoryRegion, registers.address[1] & kMainMemoryMask, kFullWordMask);
    registers.address[1] += 2U;
    registers.data[2] = (registers.data[2] & 0xffff0000U) | initial_count;
    set_logic_word_flags(registers, initial_count);

    const auto bias = host.read_memory_word(
        kCpuBMainMemoryRegion, (registers.address[5] + 0x62U) & kMainMemoryMask,
        kFullWordMask);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | bias;
    set_logic_word_flags(registers, bias);

    const auto second_bias = host.read_memory_word(
        kCpuBMainMemoryRegion, (registers.address[5] + 0x7aU) & kMainMemoryMask,
        kFullWordMask);
    registers.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(registers.address[0]) + signed_word(second_bias));

    auto counter = initial_count;
    do {
        const auto source = host.read_memory_word(
            kCpuBMainMemoryRegion, registers.address[1] & kMainMemoryMask,
            kFullWordMask);
        registers.address[1] += 2U;
        registers.data[0] = (registers.data[0] & 0xffff0000U) | source;
        set_logic_word_flags(registers, source);

        const auto value = static_cast<std::uint16_t>(source | bias);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
        set_logic_word_flags(registers, value);
        host.write_memory_word(
            kTilemapARegion, registers.address[0] - kTilemapABase,
            value, kFullWordMask);
        set_logic_word_flags(registers, value);
        registers.address[0] += 0x80U;

        counter = static_cast<std::uint16_t>(counter - 1U);
    } while (counter != 0xffffU);

    registers.data[2] = (registers.data[2] & 0xffff0000U) | counter;
    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
