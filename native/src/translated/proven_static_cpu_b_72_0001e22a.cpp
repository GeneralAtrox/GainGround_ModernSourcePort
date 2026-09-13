#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t return_address)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kCpuBMainMemoryRegion, registers.address[7],
        static_cast<std::uint16_t>(return_address >> 16U), kFullWordMask);
    host.write_memory_word(kCpuBMainMemoryRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(return_address), kFullWordMask);
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

FunctionResult proven_static_cpu_b_72_0001e22a(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    push_return(host, registers, 0x0001e230U);
    registers.program_counter = 0x00015d24U;
    auto child = host.call_function(280U, 1U, 0x72U, 2U,
        0x0001e22aU, 0x00015d24U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    push_return(host, registers, 0x0001e236U);
    registers.program_counter = 0x00015df2U;
    child = host.call_function(282U, 1U, 0x72U, 2U,
        0x0001e230U, 0x00015df2U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
