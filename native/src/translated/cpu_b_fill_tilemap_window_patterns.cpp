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

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kPrivateRegion, address & kPrivateMask,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, (address + 2U) & kPrivateMask,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}
} // namespace

FunctionResult cpu_b_fill_tilemap_window_patterns(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[0] = 0x00202000U;
    const auto row_offset = read_word(host, registers.address[5] + 0x7aU);
    registers.address[0] = static_cast<std::uint32_t>(registers.address[0]
        + static_cast<std::int16_t>(row_offset));
    registers.address[1] = 0x00010532U;

    registers.data[0] = 8U;
    for (;;) {
        const auto source = read_word(host, registers.address[1]);
        registers.address[1] += 2U;
        registers.data[1] = (registers.data[1] & 0xffff0000U) | source;
        set_logic_word(registers, source);
        const auto attribute = read_word(host, registers.address[5] + 0x64U);
        const auto value = static_cast<std::uint16_t>(source | attribute);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | value;
        set_logic_word(registers, value);
        host.write_memory_word(kTilemapARegion,
            registers.address[0] - kTilemapABase, value, kWordMask);
        set_logic_word(registers, value);
        registers.address[0] += 2U;
        const auto count = static_cast<std::uint16_t>(registers.data[0]);
        registers.data[0] = (registers.data[0] & 0xffff0000U)
            | static_cast<std::uint16_t>(count - 1U);
        if (count == 0U) break;
    }

    registers.address[0] = 0x00202080U;
    const auto second_offset = read_word(host, registers.address[5] + 0x7aU);
    registers.address[0] = static_cast<std::uint32_t>(registers.address[0]
        + static_cast<std::int16_t>(second_offset));
    registers.data[2] = (registers.data[2] & 0xffff0000U) | 6U;
    for (;;) {
        registers.data[0] = 7U;
        const auto source = read_word(host, registers.address[1]);
        registers.address[1] += 2U;
        registers.data[1] = (registers.data[1] & 0xffff0000U) | source;
        set_logic_word(registers, source);
        const auto attribute = read_word(host, registers.address[5] + 0x64U);
        const auto value = static_cast<std::uint16_t>(source | attribute);
        registers.data[1] = (registers.data[1] & 0xffff0000U) | value;
        set_logic_word(registers, value);
        for (;;) {
            host.write_memory_word(kTilemapARegion,
                registers.address[0] - kTilemapABase, value, kWordMask);
            set_logic_word(registers, value);
            registers.address[0] += 0x10U;
            const auto inner = static_cast<std::uint16_t>(registers.data[0]);
            registers.data[0] = (registers.data[0] & 0xffff0000U)
                | static_cast<std::uint16_t>(inner - 1U);
            if (inner == 0U) break;
        }
        const auto outer = static_cast<std::uint16_t>(registers.data[2]);
        registers.data[2] = (registers.data[2] & 0xffff0000U)
            | static_cast<std::uint16_t>(outer - 1U);
        if (outer == 0U) break;
    }

    registers.address[1] = 0x00010552U;
    push_return(host, registers, 0x0000fbeaU);
    registers.program_counter = 0x00016016U;
    const auto child = host.call_function(
        293U, 1U, 0x72U, 2U, 0x0000fbe4U, 0x00016016U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
