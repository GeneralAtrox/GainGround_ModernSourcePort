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

[[nodiscard]] ByteLocation locate_byte(std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
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

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void push_long(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_long(ExecutionHost &host, CpuRegisters &registers)
{
    const auto value = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return value;
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_byte_flags(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source, std::uint8_t result)
{
    const auto wide = static_cast<std::uint16_t>(
        static_cast<std::uint16_t>(destination) + source);
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(destination ^ source)) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (wide > 0xffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source, std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(destination) + source;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(destination ^ source)) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void change_bit(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, bool set)
{
    const auto old = read_byte(host, address);
    if ((old & 0x80U) == 0U)
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    write_byte(host, address,
        static_cast<std::uint8_t>(set ? old | 0x80U : old & 0x7fU));
}

void sbcd(CpuRegisters &registers, std::uint8_t &destination, std::uint8_t source)
{
    const auto extend = static_cast<std::uint16_t>(
        (registers.status & 0x0010U) != 0U);
    const auto binary = static_cast<std::uint16_t>(destination) - source - extend;
    auto adjusted = binary;
    if ((destination & 0x0fU) < static_cast<std::uint16_t>((source & 0x0fU) + extend))
        adjusted = static_cast<std::uint16_t>(adjusted - 6U);
    const bool borrow = static_cast<std::uint16_t>(source) + extend > destination;
    if (borrow)
        adjusted = static_cast<std::uint16_t>(adjusted - 0x60U);
    const auto result = static_cast<std::uint8_t>(adjusted);

    std::uint16_t flags{};
    if (borrow) flags |= 0x0011U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if ((registers.status & 0x0004U) != 0U && result == 0U) flags |= 0x0004U;
    if ((binary & ~adjusted & 0x80U) != 0U) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    destination = result;
}

void addx_word(CpuRegisters &registers, std::uint16_t &destination, std::uint16_t source)
{
    const auto extend = static_cast<std::uint32_t>(
        (registers.status & 0x0010U) != 0U);
    const auto wide = static_cast<std::uint32_t>(destination) + source + extend;
    const auto result = static_cast<std::uint16_t>(wide);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if ((registers.status & 0x0004U) != 0U && result == 0U) flags |= 0x0004U;
    if (((~(destination ^ source)) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    destination = result;
}
} // namespace

FunctionResult cpu_b_decode_record_stream_entry(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[0] = read_long(host, registers.address[5] + 0x50U);
    const auto selector = host.read_memory_word(
        kRegion, registers.address[5] + 0x54U, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | selector;
    set_logic_flags(registers, selector, 0x8000U, 0xffffU);

    for (;;) {
        const auto entry = read_byte(host, registers.address[0]++);
        const auto d0 = static_cast<std::uint8_t>(registers.data[0]);
        if (static_cast<std::int8_t>(d0) < static_cast<std::int8_t>(entry))
            break;
        registers.address[0] += 5U;
    }

    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0c00U;
    set_logic_flags(registers, 0x0c00U, 0x8000U, 0xffffU);
    const auto left = static_cast<std::uint8_t>(registers.data[0]);
    const auto addend = read_byte(host, registers.address[0]++);
    const auto sum = static_cast<std::uint8_t>(left + addend);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | sum;
    set_add_byte_flags(registers, left, addend, sum);

    host.write_memory_word(kRegion, registers.address[5] + 0x08U,
        static_cast<std::uint16_t>(registers.data[0]), kWordMask);
    set_logic_flags(registers, static_cast<std::uint16_t>(registers.data[0]),
        0x8000U, 0xffffU);
    registers.address[0] += 2U;
    const auto command = read_byte(host, registers.address[0]++);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | command;
    set_logic_flags(registers, command, 0x80U, 0xffU);

    if (command != 0U) {
        change_bit(host, registers, registers.address[5], true);
        const auto mode = read_byte(host, registers.address[0]++);
        write_byte(host, registers.address[5] + 1U, mode);
        set_logic_flags(registers, mode, 0x80U, 0xffU);

        const auto old_index = host.read_memory_word(
            kRegion, registers.address[5] + 0x54U, kWordMask);
        const auto new_index = static_cast<std::uint16_t>(old_index + 1U);
        host.write_memory_word(kRegion, registers.address[5] + 0x54U,
            new_index, kWordMask);
        set_add_word_flags(registers, old_index, 1U, new_index);

        push_long(host, registers, 0x0001edceU);
        registers.program_counter = 0x00015df2U;
        const auto child = host.call_function(282U, 1U, 0x72U, 2U,
            0x0001edc8U, 0x00015df2U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    } else {
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x001fU);
        auto d0 = read_byte(host, 0x00000c14U);
        auto d1 = read_byte(host, 0x00000c15U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | d0;
        set_logic_flags(registers, d0, 0x80U, 0xffU);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | d1;
        set_logic_flags(registers, d1, 0x80U, 0xffU);
        registers.data[3] = 1U;
        set_logic_flags(registers, 1U, 0x80000000U, 0xffffffffU);
        registers.data[2] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U, 0xffffffffU);

        sbcd(registers, d1, 1U);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | d1;
        auto d2 = static_cast<std::uint16_t>(registers.data[2]);
        addx_word(registers, d2, d2);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | d2;
        d0 = static_cast<std::uint8_t>(registers.data[0]);
        sbcd(registers, d0, static_cast<std::uint8_t>(d2));
        registers.data[0] = (registers.data[0] & 0xffffff00U) | d0;
        write_byte(host, 0x00000c14U, d0);
        set_logic_flags(registers, d0, 0x80U, 0xffU);
        write_byte(host, 0x00000c15U, d1);
        set_logic_flags(registers, d1, 0x80U, 0xffU);
        change_bit(host, registers, registers.address[5], false);
    }

    const auto return_address = pop_long(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
