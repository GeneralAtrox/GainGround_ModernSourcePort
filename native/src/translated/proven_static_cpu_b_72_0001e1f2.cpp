#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U)
        flags |= 0x0008U;
    if ((value & mask) == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_byte_flags(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if (static_cast<unsigned>(left) + right > 0xffU)
        flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x80U) != 0U)
        flags |= 0x0008U;
    if (result == 0U)
        flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_zero(CpuRegisters &registers, bool bit_was_set)
{
    if (bit_was_set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t return_address)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], return_address);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001e1f2(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    registers.address[0] = read_long(host, base + 0x48U);
    registers.address[0] = static_cast<std::uint32_t>(registers.address[0]
        + static_cast<std::int16_t>(registers.data[1]));
    write_long(host, base + 0x4cU, registers.address[0]);

    auto word = host.read_memory_word(kRegion, registers.address[0], kWordMask);
    registers.address[0] += 2U;
    host.write_memory_word(kRegion, base + 0x06U, word, kWordMask);
    set_logic_flags(registers, word, 0x8000U, 0xffffU);

    auto byte = read_byte(host, registers.address[0]++);
    write_byte(host, base + 0x10U, byte);
    set_logic_flags(registers, byte, 0x80U, 0xffU);
    byte = read_byte(host, registers.address[0]++);
    write_byte(host, base + 0x11U, byte);
    set_logic_flags(registers, byte, 0x80U, 0xffU);

    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0c00U;
    set_logic_flags(registers, 0x0c00U, 0x8000U, 0xffffU);
    const auto left = static_cast<std::uint8_t>(registers.data[0]);
    const auto right = read_byte(host, registers.address[0]++);
    const auto sum = static_cast<std::uint8_t>(left + right);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | sum;
    set_add_byte_flags(registers, left, right, sum);
    host.write_memory_word(kRegion, base + 0x08U,
        static_cast<std::uint16_t>(registers.data[0]), kWordMask);
    set_logic_flags(registers, static_cast<std::uint16_t>(registers.data[0]),
        0x8000U, 0xffffU);

    byte = read_byte(host, base + 1U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | byte;
    set_logic_flags(registers, byte, 0x80U, 0xffU);
    byte = static_cast<std::uint8_t>(byte & 0xf8U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | byte;
    set_logic_flags(registers, byte, 0x80U, 0xffU);
    byte = static_cast<std::uint8_t>(byte | read_byte(host, registers.address[0]++));
    registers.data[0] = (registers.data[0] & 0xffffff00U) | byte;
    set_logic_flags(registers, byte, 0x80U, 0xffU);
    write_byte(host, base + 1U, byte);
    set_logic_flags(registers, byte, 0x80U, 0xffU);

    push_return(host, registers, 0x0001e224U);
    registers.program_counter = 0x0001e158U;
    auto child = host.call_function(358U, 1U, 0x72U, 2U,
        0x0001e220U, 0x0001e158U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto flag_address = base + 0x40U;
    const auto prior = read_byte(host, flag_address);
    write_byte(host, flag_address, static_cast<std::uint8_t>(prior & ~0x08U));
    set_bit_zero(registers, (prior & 0x08U) != 0U);

    registers.program_counter = 0x0001e22aU;
    return host.call_function(555U, 1U, 0x72U, 0U,
        0x0001e224U, 0x0001e22aU, context);
}

} // namespace gain_ground::translated
