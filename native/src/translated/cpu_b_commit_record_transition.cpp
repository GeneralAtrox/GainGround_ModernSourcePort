#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kPrivateMask = 0x0003ffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(
    std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_private_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address & kPrivateMask);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

[[nodiscard]] std::uint8_t read_lookup_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const bool shared_alias = (address & 0xffff0000U) == 0xffff0000U;
    const auto region = static_cast<std::uint16_t>(
        shared_alias ? kSharedRegion : kPrivateRegion);
    const auto offset = shared_alias
        ? (0x00030000U | (address & 0x0000ffffU))
        : (address & kPrivateMask);
    const auto location = locate_byte(offset);
    return static_cast<std::uint8_t>(host.read_memory_word(
        region, location.offset, location.mask) >> location.shift);
}

void write_private_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const auto location = locate_byte(address & kPrivateMask);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    const auto offset = address & kPrivateMask;
    const auto high = host.read_memory_word(
        kPrivateRegion, offset, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, (offset + 2U) & kPrivateMask, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(ExecutionHost &host, std::uint32_t address,
    std::uint32_t value)
{
    const auto offset = address & kPrivateMask;
    host.write_memory_word(kPrivateRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, (offset + 2U) & kPrivateMask,
        static_cast<std::uint16_t>(value), kWordMask);
}

void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_byte_flags(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source)
{
    const auto result = static_cast<std::uint8_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if (destination < source) flags |= 0x0001U;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_byte_flags(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint16_t>(left) + right > 0xffU)
        flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint8_t abcd(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source)
{
    const auto extend = static_cast<std::uint16_t>(
        (registers.status & 0x0010U) != 0U);
    const auto binary = static_cast<std::uint16_t>(
        destination + source + extend);
    auto adjusted = binary;
    if (static_cast<std::uint16_t>((destination & 0x0fU)
        + (source & 0x0fU) + extend) > 9U)
        adjusted = static_cast<std::uint16_t>(adjusted + 6U);
    const bool carry = adjusted > 0x99U;
    if (carry) adjusted = static_cast<std::uint16_t>(adjusted + 0x60U);
    const auto result = static_cast<std::uint8_t>(adjusted);

    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if ((registers.status & 0x0004U) != 0U && result == 0U)
        flags |= 0x0004U;
    if (((~binary) & adjusted & 0x80U) != 0U) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return result;
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t target)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], target);
}

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    const auto target = pop_return(*context.host, context.registers);
    context.registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_commit_record_transition(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    if (registers.program_counter != 0x00010374U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};
    FunctionResult character_result;
    if (host.run_character_exit(context, character_result)) return character_result;

    const auto x = host.read_memory_word(kPrivateRegion,
        (registers.address[5] + 0x12U) & kPrivateMask, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | x;
    set_logic_flags(registers, x, 0x8000U);
    const auto y = host.read_memory_word(kPrivateRegion,
        (registers.address[5] + 0x1aU) & kPrivateMask, kWordMask);
    registers.data[1] = (registers.data[1] & 0xffff0000U) | y;
    set_logic_flags(registers, y, 0x8000U);

    push_return(host, registers, 0x00010382U);
    registers.program_counter = 0x00015edeU;
    auto child = host.call_function(287U, 1U, 0x72U, 2U,
        0x0001037cU, 0x00015edeU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto marker = read_lookup_byte(host, registers.address[0]);
    const bool bit_three = (marker & 0x08U) != 0U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x0004U) | (bit_three ? 0U : 0x0004U));
    if (!bit_three) return finish(context);

    host.write_memory_word(kPrivateRegion,
        (registers.address[5] + 0x44U) & kPrivateMask, 7U, kWordMask);
    set_logic_flags(registers, 7U, 0x8000U);
    push_return(host, registers, 0x00010392U);
    registers.program_counter = 0x0000fd10U;
    child = host.call_function(197U, 1U, 0x72U, 2U,
        0x0001038eU, 0x0000fd10U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    host.write_memory_word(kPrivateRegion,
        (registers.address[5] + 0x52U) & kPrivateMask,
        0xffffU, kWordMask);
    set_logic_flags(registers, 0xffffU, 0x8000U);
    const auto record_type = read_private_byte(
        host, registers.address[5] + 0x4bU);
    registers.data[2] = (registers.data[2] & 0xffffff00U) | record_type;
    set_logic_flags(registers, record_type, 0x80U);

    const auto linked = host.read_memory_word(kPrivateRegion,
        (registers.address[5] + 0x5cU) & kPrivateMask, kWordMask);
    set_logic_flags(registers, linked, 0x8000U);
    if (linked == 0U) return finish(context);
    const auto linked_for_address = host.read_memory_word(kPrivateRegion,
        (registers.address[5] + 0x5cU) & kPrivateMask, kWordMask);
    registers.address[6] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(
            static_cast<std::int16_t>(linked_for_address)));

    host.write_memory_word(kPrivateRegion,
        (registers.address[6] + 0x44U) & kPrivateMask, 3U, kWordMask);
    set_logic_flags(registers, 3U, 0x8000U);
    auto value = read_long(host, registers.address[5] + 0x12U);
    write_long(host, registers.address[6] + 0x62U, value);
    set_logic_flags(registers, value, 0x80000000U);
    value = read_long(host, registers.address[5] + 0x1aU);
    write_long(host, registers.address[6] + 0x66U, value);
    set_logic_flags(registers, value, 0x80000000U);

    const auto count = read_private_byte(host, registers.address[4] + 0x40U);
    set_compare_byte_flags(registers, count, 0x3eU);
    if (count >= 0x3eU) return finish(context);

    const auto accepted = host.read_memory_word(
        kPrivateRegion, 0x00000c12U, kWordMask);
    const auto accepted_next = static_cast<std::uint16_t>(accepted + 1U);
    host.write_memory_word(kPrivateRegion, 0x00000c12U,
        accepted_next, kWordMask);
    set_add_word_flags(registers, accepted, 1U, accepted_next);

    const auto count_before_increment = read_private_byte(
        host, registers.address[4] + 0x40U);
    const auto count_next = static_cast<std::uint8_t>(
        count_before_increment + 1U);
    write_private_byte(host, registers.address[4] + 0x40U, count_next);
    set_add_byte_flags(registers, count_before_increment, 1U, count_next);

    auto bcd_value = read_private_byte(host, registers.address[4] + 0x41U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | bcd_value;
    set_logic_flags(registers, bcd_value, 0x80U);
    registers.data[1] = 1U;
    set_logic_flags(registers, 1U, 0x80000000U);
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x001fU);
    bcd_value = abcd(registers, bcd_value, 1U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | bcd_value;
    write_private_byte(host, registers.address[4] + 0x41U, bcd_value);
    set_logic_flags(registers, bcd_value, 0x80U);

    const auto current_count = read_private_byte(
        host, registers.address[4] + 0x40U);
    const auto d1_before = static_cast<std::uint8_t>(registers.data[1]);
    const auto index = static_cast<std::uint8_t>(d1_before + current_count);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | index;
    set_add_byte_flags(registers, d1_before, current_count, index);
    const auto linked_type = read_private_byte(
        host, registers.address[6] + 0x4bU);
    const auto destination = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(registers.address[4] + 0x40U)
        + static_cast<std::int16_t>(static_cast<std::uint16_t>(
            registers.data[1])));
    write_private_byte(host, destination, linked_type);
    set_logic_flags(registers, linked_type, 0x80U);

    return finish(context);
}

} // namespace gain_ground::translated
