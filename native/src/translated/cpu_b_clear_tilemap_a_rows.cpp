#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kCpuBRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kCpuBRegion, address, kWordMask);
    const auto low = host.read_memory_word(kCpuBRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_move_long_zero_flags(CpuRegisters &registers)
{
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x000fU) | 0x0004U);
}
} // namespace

FunctionResult cpu_b_clear_tilemap_a_rows(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    std::uint32_t source = context.registers.address[1];
    const auto source_offset = host.read_memory_word(kCpuBRegion, source, kWordMask);
    source += 2U;
    const auto initial_count = host.read_memory_word(kCpuBRegion, source, kWordMask);
    source += 2U;
    const auto record_offset = host.read_memory_word(
        kCpuBRegion, context.registers.address[5] + 0x7aU, kWordMask);
    std::uint32_t destination = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(0x00200000U) +
        static_cast<std::int16_t>(source_offset) +
        static_cast<std::int16_t>(record_offset));

    std::uint16_t counter = initial_count;
    do {
        const auto offset = destination - 0x00200000U;
        host.write_memory_word(kTileRegion, offset, 0U, kWordMask);
        host.write_memory_word(kTileRegion, offset + 2U, 0U, kWordMask);
        destination += 0x80U;
        set_move_long_zero_flags(context.registers);
        counter = static_cast<std::uint16_t>(counter - 1U);
    } while (counter != 0xffffU);

    context.registers.data[0] = 0U;
    context.registers.data[2] =
        (context.registers.data[2] & 0xffff0000U) | counter;
    context.registers.address[0] = destination;
    context.registers.address[1] = source;
    const auto return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}
} // namespace gain_ground::translated
