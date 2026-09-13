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

void clear_byte(ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    (void)read_byte(host, address);
    write_byte(host, address, 0U);
    set_logic_flags(registers, 0U, 0x80U, 0xffU);
}

void clear_word(ExecutionHost &host, CpuRegisters &registers, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address, kWordMask);
    host.write_memory_word(kRegion, address, 0U, kWordMask);
    set_logic_flags(registers, 0U, 0x8000U, 0xffffU);
}
} // namespace

FunctionResult cpu_b_initialize_callback_record_slot(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto destination = registers.address[6];

    registers.address[0] = read_long(host, registers.address[5] + 0x66U);
    const auto source = registers.address[0];

    const auto callback = read_long(host, source + 0x04U);
    write_long(host, destination + 0x02U, callback);
    set_logic_flags(registers, callback, 0x80000000U, 0xffffffffU);

    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x0c00U;
    set_logic_flags(registers, 0x0c00U, 0x8000U, 0xffffU);
    const auto left = static_cast<std::uint8_t>(registers.data[0]);
    const auto addend = read_byte(host, source + 0x08U);
    const auto sum = static_cast<std::uint8_t>(left + addend);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | sum;
    set_add_byte_flags(registers, left, addend, sum);

    const auto d0 = static_cast<std::uint16_t>(registers.data[0]);
    host.write_memory_word(kRegion, destination + 0x08U, d0, kWordMask);
    set_logic_flags(registers, d0, 0x8000U, 0xffffU);
    host.write_memory_word(kRegion, destination + 0x5aU, d0, kWordMask);
    set_logic_flags(registers, d0, 0x8000U, 0xffffU);

    auto byte = read_byte(host, source + 0x0aU);
    write_byte(host, destination + 0x3cU, byte);
    set_logic_flags(registers, byte, 0x80U, 0xffU);
    byte = read_byte(host, source + 0x0bU);
    write_byte(host, destination + 0x0bU, byte);
    set_logic_flags(registers, byte, 0x80U, 0xffU);

    const auto payload = read_long(host, source + 0x0cU);
    write_long(host, destination + 0x48U, payload);
    set_logic_flags(registers, payload, 0x80000000U, 0xffffffffU);

    byte = read_byte(host, source + 0x09U);
    write_byte(host, destination + 0x36U, byte);
    set_logic_flags(registers, byte, 0x80U, 0xffU);

    clear_byte(host, registers, destination + 0x3fU);
    clear_word(host, registers, destination + 0x46U);
    clear_word(host, registers, destination + 0x56U);

    const auto return_address = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
