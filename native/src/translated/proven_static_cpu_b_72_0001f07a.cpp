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

void write_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    host.write_memory_word(kPrivateRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
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

void change_bit(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, unsigned bit, bool set)
{
    const auto before = read_byte(host, address);
    const auto mask = static_cast<std::uint8_t>(1U << bit);
    const auto after = static_cast<std::uint8_t>(set
        ? (before | mask)
        : (before & static_cast<std::uint8_t>(~mask)));
    write_byte(host, address, after);
    set_bit_zero_flag(registers, (before & mask) != 0U);
}

[[nodiscard]] FunctionResult call_child(
    FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], 0x0001f082U);
    registers.program_counter = 0x000204c0U;
    return host.call_function(384U, 1U, 0x72U, 2U,
        0x0001f07eU, 0x000204c0U, context);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001f07a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    registers.address[0] = read_long(host, base + 0x66U);

    const auto child = call_child(context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto flag_address = base + 0x41U;
    change_bit(host, registers, flag_address, 1U, false);
    change_bit(host, registers, flag_address, 0U, true);
    change_bit(host, registers, flag_address, 2U, true);

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
