#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(
    std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & 0x00fffffeU,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, address, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_zero_flag(CpuRegisters &registers, bool bit_was_set)
{
    if (bit_was_set)
        registers.status = static_cast<std::uint16_t>(
            registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(
            registers.status | 0x0004U);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001f096(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    registers.address[0] = read_long(host, base + 0x66U);

    registers.data[0] = 0U;
    set_logic_word_flags(registers, 0U);

    const auto selected = read_byte(host, registers.address[0] + 0x13U);
    registers.data[0] = selected;
    set_logic_byte_flags(registers, selected);

    host.write_memory_word(kPrivateRegion,
        base + 0x74U, static_cast<std::uint16_t>(registers.data[0]), kWordMask);
    set_logic_word_flags(registers,
        static_cast<std::uint16_t>(registers.data[0]));

    const auto flag_address = base + 0x41U;
    auto flags = read_byte(host, flag_address);
    const bool bit1_was_set = (flags & 0x02U) != 0U;
    flags = static_cast<std::uint8_t>(flags | 0x02U);
    write_byte(host, flag_address, flags);
    set_bit_zero_flag(registers, bit1_was_set);

    flags = read_byte(host, flag_address);
    const bool bit0_was_set = (flags & 0x01U) != 0U;
    flags = static_cast<std::uint8_t>(flags | 0x01U);
    write_byte(host, flag_address, flags);
    set_bit_zero_flag(registers, bit0_was_set);

    flags = read_byte(host, flag_address);
    const bool bit2_was_set = (flags & 0x04U) != 0U;
    flags = static_cast<std::uint8_t>(flags & ~0x04U);
    write_byte(host, flag_address, flags);
    set_bit_zero_flag(registers, bit2_was_set);

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
