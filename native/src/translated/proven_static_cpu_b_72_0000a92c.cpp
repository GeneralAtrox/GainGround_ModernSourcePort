#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void write_tile_word(ExecutionHost &host, std::uint32_t address,
                     std::uint16_t value)
{
    host.write_memory_word(kTileRegion, address - 0x00200000U,
        value, kWordMask);
}

void write_tile_long(ExecutionHost &host, std::uint32_t address,
                     std::uint32_t value)
{
    write_tile_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_tile_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto target = (static_cast<std::uint32_t>(
        host.read_memory_word(kPrivateRegion, stack, kWordMask)) << 16U)
        | host.read_memory_word(kPrivateRegion, stack + 2U, kWordMask);
    registers.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000a92c(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x0020000cU;
    registers.data[2] = 0x0000002fU;
    set_move_word_flags(registers, 0x002fU);
    registers.data[1] = 0U;
    set_move_word_flags(registers, 0U);

    for (;;) {
        registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0039U;
        set_move_word_flags(registers, 0x0039U);
        for (;;) {
            write_tile_word(host, registers.address[0],
                static_cast<std::uint16_t>(registers.data[1]));
            registers.address[0] += 2U;
            set_move_word_flags(registers,
                static_cast<std::uint16_t>(registers.data[1]));
            const auto inner = static_cast<std::uint16_t>(
                registers.data[0] - 1U);
            registers.data[0] = (registers.data[0] & 0xffff0000U) | inner;
            if (inner == 0xffffU) break;
        }
        registers.address[0] += 0x0cU;
        const auto outer = static_cast<std::uint16_t>(
            registers.data[2] - 1U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | outer;
        if (outer == 0xffffU) break;
    }

    registers.address[0] = 0x00204000U;
    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x07ffU;
    set_move_word_flags(registers, 0x07ffU);
    for (;;) {
        write_tile_long(host, registers.address[0], registers.data[1]);
        registers.address[0] += 4U;
        set_move_long_flags(registers, registers.data[1]);
        const auto counter = static_cast<std::uint16_t>(
            registers.data[0] - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
