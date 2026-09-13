#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgram = 1U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionMask = 0x001fU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kProgram, address, kWordMask);
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void set_cmp_byte(CpuRegisters &registers, std::uint8_t destination,
    std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    const bool carry = source > destination;
    const bool overflow = ((destination ^ source) & (destination ^ result) & 0x80U) != 0U;
    std::uint16_t flags = carry ? 0x0011U : 0U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

void set_add_byte(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result)
{
    const bool carry = static_cast<std::uint16_t>(left) + right > 0xffU;
    const bool overflow = ((~(left ^ right)) & (left ^ result) & 0x80U) != 0U;
    std::uint16_t flags = carry ? 0x0011U : 0U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | flags);
}

std::uint8_t read_fdc(ExecutionHost &host, std::uint32_t pc,
    std::uint32_t address)
{
    return static_cast<std::uint8_t>(host.read_hardware(
        1U, 0U, 0xffU, pc, address & ~1U, 0x00ffU));
}

void write_fdc(ExecutionHost &host, std::uint32_t pc,
    std::uint32_t address, std::uint8_t value)
{
    host.write_hardware(2U, 0U, 0xffU, pc, address & ~1U,
        static_cast<std::uint16_t>(value) * 0x0101U, 0x00ffU);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    const auto offset = registers.address[7] & 0x0003ffffU;
    host.write_memory_word(kShared, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kShared, offset + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto offset = registers.address[7] & 0x0003ffffU;
    const auto high = host.read_memory_word(kShared, offset, kWordMask);
    const auto low = host.read_memory_word(kShared, offset + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00001862(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr || context.registers.program_counter != 0x1862U)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto fdc = registers.address[5];

    registers.data[1] = (registers.data[1] & 0xffff0000U)
        | static_cast<std::uint16_t>(registers.data[7]);
    const auto shifted_source = static_cast<std::uint8_t>(registers.data[1]);
    const auto shifted = static_cast<std::uint8_t>(shifted_source >> 1U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | shifted;
    std::uint16_t shift_flags = (shifted_source & 1U) != 0U ? 0x0011U : 0U;
    if ((shifted & 0x80U) != 0U) shift_flags |= 0x0008U;
    if (shifted == 0U) shift_flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionMask) | shift_flags);

    prefetch(host, 0x1866U);
    prefetch(host, 0x1868U);
    prefetch(host, 0x186aU);
    const auto track = read_fdc(host, 0x1866U, fdc + 2U);
    set_cmp_byte(registers, static_cast<std::uint8_t>(registers.data[1]), track);
    prefetch(host, 0x186cU);
    if ((registers.status & 0x0001U) != 0U) {
        prefetch(host, 0x186eU);
        push_return(host, registers, 0x1870U);
        prefetch(host, 0x17e4U);
        prefetch(host, 0x17e6U);
        registers.program_counter = 0x17e4U;
        const auto child = host.call_function(
            21U, 0U, 0xffU, 2U, 0x186cU, 0x17e4U, context);
        if (child.status != TranslationStatus::complete) return child;
    } else {
        prefetch(host, 0x1870U);
    }

    for (;;) {
        prefetch(host, 0x1872U);
        prefetch(host, 0x1874U);
        prefetch(host, 0x1876U);
        const auto status = read_fdc(host, 0x1870U, fdc);
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~0x0004U) | ((status & 1U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x1878U);
        if ((registers.status & 0x0004U) != 0U) break;
        prefetch(host, 0x1870U);
    }

    prefetch(host, 0x187aU);
    prefetch(host, 0x187cU);
    write_fdc(host, 0x1878U, fdc + 6U,
        static_cast<std::uint8_t>(registers.data[1]));
    set_logic_byte(registers, static_cast<std::uint8_t>(registers.data[1]));
    for (;;) {
        prefetch(host, 0x187eU);
        prefetch(host, 0x1880U);
        prefetch(host, 0x1882U);
        const auto status = read_fdc(host, 0x187cU, fdc);
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~0x0004U) | ((status & 1U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x1884U);
        if ((registers.status & 0x0004U) != 0U) break;
        prefetch(host, 0x187cU);
    }

    prefetch(host, 0x1886U); prefetch(host, 0x1888U); prefetch(host, 0x188aU);
    write_fdc(host, 0x1884U, fdc, 0x10U); set_logic_byte(registers, 0x10U);
    registers.data[1] = (registers.data[1] & 0xffff0000U)
        | static_cast<std::uint16_t>(registers.data[7]);
    prefetch(host, 0x188cU); prefetch(host, 0x188eU); prefetch(host, 0x1890U);
    const auto anded = static_cast<std::uint8_t>(registers.data[1]) & 1U;
    registers.data[1] = (registers.data[1] & 0xffffff00U) | anded;
    set_logic_byte(registers, anded);
    const auto shifted_left = static_cast<std::uint8_t>(anded << 2U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | shifted_left;
    set_logic_byte(registers, shifted_left);
    prefetch(host, 0x1892U); prefetch(host, 0x1894U); prefetch(host, 0x1896U);
    const auto added = static_cast<std::uint8_t>(shifted_left + 10U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | added;
    set_add_byte(registers, shifted_left, 10U, added);
    prefetch(host, 0x1898U); prefetch(host, 0x189aU);
    write_fdc(host, 0x1896U, fdc + 10U, added); set_logic_byte(registers, added);

    for (;;) {
        prefetch(host, 0x189cU); prefetch(host, 0x189eU); prefetch(host, 0x18a0U);
        const auto status = read_fdc(host, 0x189aU, fdc + 8U);
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~0x0004U) | ((status & 2U) == 0U ? 0x0004U : 0U));
        prefetch(host, 0x18a2U);
        if ((registers.status & 0x0004U) == 0U) break;
        prefetch(host, 0x189aU);
    }

    prefetch(host, 0x18a4U);
    const auto target = pop_return(host, registers);
    prefetch(host, target); prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace gain_ground::translated
