#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kTileMemoryRegion = 5U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint32_t kTileMemoryBase = 0x00200000U;

[[nodiscard]] std::uint32_t read_return_address(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kCpuBMainMemoryRegion, address, kFullWordMask);
    const auto low = host.read_memory_word(kCpuBMainMemoryRegion, address + 2U, kFullWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

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

} // namespace

FunctionResult cpu_b_copy_vertical_words_stride_0x80(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    std::uint32_t source = context.registers.address[1];
    std::uint32_t destination = context.registers.address[0];
    const std::uint16_t initial_count = host.read_memory_word(
        kCpuBMainMemoryRegion, source, kFullWordMask);
    source += 2U;
    std::uint16_t counter = initial_count;

    do {
        const std::uint16_t value = host.read_memory_word(
            kCpuBMainMemoryRegion, source, kFullWordMask);
        source += 2U;
        host.write_memory_word(
            kTileMemoryRegion, destination - kTileMemoryBase, value, kFullWordMask);
        destination += 0x80U;
        set_move_word_flags(context.registers, value);
        counter = static_cast<std::uint16_t>(counter - 1U);
    } while (counter != 0xffffU);

    context.registers.data[1] = (context.registers.data[1] & 0xffff0000U) | counter;
    context.registers.address[0] = destination;
    context.registers.address[1] = source;

    const std::uint32_t return_address = read_return_address(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
