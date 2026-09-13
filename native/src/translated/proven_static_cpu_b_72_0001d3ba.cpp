#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint16_t kZero = 0x0004U;

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = host.read_memory_word(
        kCpuBMainMemoryRegion, address & ~1U, mask);
    return static_cast<std::uint8_t>(odd ? value : value >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    host.write_memory_word(kCpuBMainMemoryRegion, address & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U), mask);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto stack_pointer = registers.address[7];
    const auto high = host.read_memory_word(
        kCpuBMainMemoryRegion, stack_pointer, kFullWordMask);
    const auto low = host.read_memory_word(
        kCpuBMainMemoryRegion, stack_pointer + 2U, kFullWordMask);
    registers.address[7] = stack_pointer + 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001d3ba(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto address = registers.address[5] + 0x41U;
    const auto value = read_byte(host, address);
    write_byte(host, address, static_cast<std::uint8_t>(value & ~0x20U));
    registers.status = static_cast<std::uint16_t>((registers.status & ~kZero)
        | ((value & 0x20U) == 0U ? kZero : 0U));

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
