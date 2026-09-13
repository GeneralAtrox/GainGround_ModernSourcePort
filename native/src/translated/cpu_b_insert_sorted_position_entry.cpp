#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint32_t kPrimaryTable = 0x00006c00U;
constexpr std::uint32_t kAlternateTable = 0x00007002U;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
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

FunctionResult cpu_b_insert_sorted_position_entry(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    std::uint32_t table = kPrimaryTable;
    const auto selector = host.read_memory_word(
        kCpuBMainMemoryRegion, table, kFullWordMask);
    table += 2U;
    if (static_cast<std::int16_t>(selector) < 0)
        table = kAlternateTable;

    std::uint16_t position = host.read_memory_word(
        kCpuBMainMemoryRegion, context.registers.address[5] + 0x1aU, kFullWordMask);
    context.registers.data[0] =
        (context.registers.data[0] & 0xffff0000U) | position;

    if (static_cast<std::int16_t>(position) >= 0) {
        if (static_cast<std::int16_t>(position) >= 0x01ef) {
            position = 0x01efU;
        }
        position = static_cast<std::uint16_t>(position + position);
        // ADD.W D0,D0 updates X. Every nonnegative value is clamped to
        // 0x01ef first, so this path cannot carry and therefore clears X.
        context.registers.status = static_cast<std::uint16_t>(
            context.registers.status & ~0x0010U);
        context.registers.data[0] =
            (context.registers.data[0] & 0xffff0000U) | position;
        table = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(table) + static_cast<std::int16_t>(position));
    }

    std::uint16_t entry{};
    do {
        entry = host.read_memory_word(kCpuBMainMemoryRegion, table, kFullWordMask);
        table += 2U;
    } while (entry != 0U);

    table -= 2U;
    const auto record = static_cast<std::uint16_t>(context.registers.address[5]);
    host.write_memory_word(kCpuBMainMemoryRegion, table, record, kFullWordMask);
    set_move_word_flags(context.registers, record);
    context.registers.address[0] = table;

    const std::uint32_t return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
