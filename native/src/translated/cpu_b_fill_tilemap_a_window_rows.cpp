#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTilemapARegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kPrivateMask = 0x0003ffffU;
constexpr std::uint32_t kTilemapABase = 0x00200000U;

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint16_t read_word(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kPrivateRegion, address & kPrivateMask, kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_fill_tilemap_a_window_rows(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[0] = 0x00202000U;

    const auto initial_offset = read_word(host, registers.address[1]);
    registers.address[1] += 2U;
    registers.address[0] = static_cast<std::uint32_t>(registers.address[0]
        + static_cast<std::int16_t>(initial_offset));

    auto outer_count = read_word(host, registers.address[1]);
    registers.address[1] += 2U;
    registers.data[3] = (registers.data[3] & 0xffff0000U) | outer_count;
    set_logic_word(registers, outer_count);

    const auto inner_initial = read_word(host, registers.address[1]);
    registers.address[1] += 2U;
    registers.data[4] = (registers.data[4] & 0xffff0000U) | inner_initial;
    set_logic_word(registers, inner_initial);

    const auto attribute = read_word(host, registers.address[5] + 0x64U);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | attribute;
    set_logic_word(registers, attribute);

    const auto row_offset = read_word(host, registers.address[5] + 0x7aU);
    registers.address[0] = static_cast<std::uint32_t>(registers.address[0]
        + static_cast<std::int16_t>(row_offset));
    for (;;) {
        registers.address[2] = registers.address[0];

        auto inner_count = static_cast<std::uint16_t>(registers.data[4]);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | inner_count;
        set_logic_word(registers, inner_count);
        for (;;) {
            const auto source = read_word(host, registers.address[1]);
            registers.address[1] += 2U;
            registers.data[0] = (registers.data[0] & 0xffff0000U) | source;
            set_logic_word(registers, source);
            const auto value = static_cast<std::uint16_t>(source | attribute);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
            set_logic_word(registers, value);
            host.write_memory_word(kTilemapARegion,
                registers.address[2] - kTilemapABase, value, kWordMask);
            set_logic_word(registers, value);
            registers.address[2] += 0x80U;

            const auto before = inner_count;
            inner_count = static_cast<std::uint16_t>(inner_count - 1U);
            registers.data[2] = (registers.data[2] & 0xffff0000U) | inner_count;
            if (before == 0U) break;
        }

        registers.address[0] -= 2U;
        const auto before = outer_count;
        outer_count = static_cast<std::uint16_t>(outer_count - 1U);
        registers.data[3] = (registers.data[3] & 0xffff0000U) | outer_count;
        if (before == 0U) break;
    }

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
