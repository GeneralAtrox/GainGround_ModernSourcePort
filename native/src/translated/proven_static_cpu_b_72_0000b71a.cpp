#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
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

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kPrivateRegion, address, kWordMask);
}

void write_word(
    ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kPrivateRegion, address, value, kWordMask);
}

std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_long(
    ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void move_word_postincrement(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t destination)
{
    const auto value = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    write_word(host, destination, value);
    set_move_word_flags(registers, value);
}

void move_long_postincrement(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t destination)
{
    const auto value = read_long(host, registers.address[0]);
    registers.address[0] += 4U;
    write_long(host, destination, value);
    set_move_long_flags(registers, value);
}

void move_word_no_increment(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t destination)
{
    const auto value = read_word(host, registers.address[0]);
    write_word(host, destination, value);
    set_move_word_flags(registers, value);
}

void clear_word(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t destination)
{
    (void)read_word(host, destination);
    write_word(host, destination, 0U);
    set_move_word_flags(registers, 0U);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000b71a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    if (registers.program_counter != 0x0000b71aU)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    const auto destination = registers.address[6];
    move_word_postincrement(host, registers, destination);
    move_long_postincrement(host, registers, destination + 0x02U);
    move_word_postincrement(host, registers, destination + 0x06U);
    move_word_postincrement(host, registers, destination + 0x08U);
    move_word_no_increment(host, registers, destination + 0x0cU);
    move_word_postincrement(host, registers, destination + 0x12U);
    clear_word(host, registers, destination + 0x14U);
    move_word_no_increment(host, registers, destination + 0x0eU);
    move_word_postincrement(host, registers, destination + 0x16U);
    clear_word(host, registers, destination + 0x18U);
    move_word_postincrement(host, registers, destination + 0x1aU);
    move_long_postincrement(host, registers, destination + 0x1eU);
    move_long_postincrement(host, registers, destination + 0x22U);
    move_word_no_increment(host, registers, destination + 0x26U);
    move_word_postincrement(host, registers, destination + 0x28U);
    clear_word(host, registers, destination + 0x2aU);
    move_word_postincrement(host, registers, destination + 0x2cU);
    move_long_postincrement(host, registers, destination + 0x2eU);

    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
