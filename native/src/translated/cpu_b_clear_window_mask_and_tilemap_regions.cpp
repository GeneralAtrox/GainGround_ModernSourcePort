#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWindowMaskRegion = 7U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_clear_window_mask_and_tilemap_regions(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto record_word = host.read_memory_word(
        kPrivateRegion, registers.address[4] + 0x84U, kWordMask);
    (void)record_word;
    host.write_memory_word(
        kPrivateRegion, registers.address[4] + 0x84U, 0U, kWordMask);
    set_logic_word(registers, 0U);

    registers.address[0] = 0x0020c000U;
    const auto window_offset = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x78U, kWordMask);
    registers.address[0] = static_cast<std::uint32_t>(
        registers.address[0] + static_cast<std::int16_t>(window_offset));
    registers.data[0] = 0x0000007fU;
    registers.data[1] = 0U;
    set_logic_word(registers, 0U);
    for (;;) {
        host.write_memory_word(kWindowMaskRegion,
            registers.address[0] - 0x0020c000U,
            static_cast<std::uint16_t>(registers.data[1]), kWordMask);
        set_logic_word(registers, static_cast<std::uint16_t>(registers.data[1]));
        registers.address[0] += 8U;
        const auto count = static_cast<std::uint16_t>(registers.data[0]);
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(count - 1U);
        if (count == 0U) break;
    }

    registers.address[0] = 0x00200090U;
    const auto tile_offset = host.read_memory_word(
        kPrivateRegion, registers.address[5] + 0x7aU, kWordMask);
    registers.address[0] = static_cast<std::uint32_t>(
        registers.address[0] + static_cast<std::int16_t>(tile_offset));
    registers.data[0] = 6U;
    do {
        host.write_memory_word(kTileRegion,
            registers.address[0] - 0x00200000U,
            static_cast<std::uint16_t>(registers.data[1]), kWordMask);
        set_logic_word(registers, static_cast<std::uint16_t>(registers.data[1]));
        registers.address[0] += 0x80U;
        const auto count = static_cast<std::uint16_t>(registers.data[0]);
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(count - 1U);
        if (count == 0U) break;
    } while (true);

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
