#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kPrivateRegion, address, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kPrivateRegion, address, value, kWordMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = host.read_memory_word(kPrivateRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(value >> (odd ? 0U : 8U));
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto data = static_cast<std::uint16_t>(value) << (odd ? 0U : 8U);
    host.write_memory_word(kPrivateRegion, address & ~1U, data, mask);
}

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_long(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void clear_byte(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address)
{
    (void)read_byte(host, address);
    write_byte(host, address, 0U);
    set_logic_byte(registers, 0U);
}

void clear_word(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address)
{
    (void)read_word(host, address);
    write_word(host, address, 0U);
    set_logic_word(registers, 0U);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    return target;
}
} // namespace

FunctionResult cpu_b_load_entry_pointer_10_alt5(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = read_long(host, registers.address[3] + 0x10U);
    auto value_long = read_long(host, registers.address[0]);
    registers.address[0] += 4U;
    write_long(host, registers.address[6] + 0x02U, value_long);
    set_logic_long(registers, value_long);

    const auto base = read_word(host, registers.address[5] + 0x66U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | base;
    set_logic_word(registers, base);
    const auto adjustment = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    const auto sum = static_cast<std::uint16_t>(base + adjustment);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | sum;
    set_add_word(registers, base, adjustment, sum);
    write_word(host, registers.address[6] + 0x08U, sum);
    set_logic_word(registers, sum);

    const auto field_3c = read_byte(host, registers.address[0]++);
    write_byte(host, registers.address[6] + 0x3cU, field_3c);
    set_logic_byte(registers, field_3c);
    const auto field_0b = read_byte(host, registers.address[0]++);
    write_byte(host, registers.address[6] + 0x0bU, field_0b);
    set_logic_byte(registers, field_0b);

    value_long = read_long(host, registers.address[0]);
    registers.address[0] += 4U;
    write_long(host, registers.address[6] + 0x22U, value_long);
    set_logic_long(registers, value_long);
    const auto field_3a = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    write_word(host, registers.address[6] + 0x3aU, field_3a);
    set_logic_word(registers, field_3a);

    const auto a5_word = static_cast<std::uint16_t>(registers.address[5]);
    write_word(host, registers.address[6] + 0x36U, a5_word);
    set_logic_word(registers, a5_word);
    clear_byte(host, registers, registers.address[6] + 0x3fU);
    clear_word(host, registers, registers.address[6] + 0x42U);
    clear_byte(host, registers, registers.address[6] + 0x3dU);

    const auto field_06 = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    write_word(host, registers.address[6] + 0x06U, field_06);
    set_logic_word(registers, field_06);
    const auto field_00 = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    write_word(host, registers.address[6], field_00);
    set_logic_word(registers, field_00);

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
