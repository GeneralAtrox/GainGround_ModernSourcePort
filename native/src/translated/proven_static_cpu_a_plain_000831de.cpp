#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kMask = 0x0003ffffU;

void prefetch(ExecutionHost &host, std::uint32_t address)
{
    (void)host.read_memory_word(kRegion, address & kMask, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(kRegion, address & kMask, value, kWordMask);
}

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kMask, kWordMask);
}

void set_move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_address(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value));
    write_word(host, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U));
    set_move_long_flags(registers, value);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U));
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value));
}

std::uint32_t pop_address(ExecutionHost &host, CpuRegisters &registers)
{
    const auto low = read_word(host, registers.address[7] + 2U);
    const auto high = read_word(host, registers.address[7]);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_a_plain_000831de(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    prefetch(host, 0x000831e2U);
    push_address(host, registers, registers.address[5]);
    prefetch(host, 0x000831e4U);
    push_address(host, registers, registers.address[6]);
    prefetch(host, 0x000831e6U);
    prefetch(host, 0x000831e8U);
    prefetch(host, 0x000831eaU);
    prefetch(host, 0x000831ecU);
    registers.address[6] = 0xffffc000U;
    prefetch(host, 0x000831eeU);
    prefetch(host, 0x000831f0U);
    registers.address[5] = 0x00fb0000U;

    push_return(host, registers, 0x000831f2U);
    prefetch(host, 0x00083204U);
    prefetch(host, 0x00083206U);
    registers.program_counter = 0x00083204U;
    const auto child = host.call_function(466U, 0U, 0xffU, 2U,
        0x000831eeU, 0x00083204U, context);
    if (child.status != TranslationStatus::complete)
        return child;

    registers.address[6] = pop_address(host, registers);
    prefetch(host, 0x000831f6U);
    registers.address[5] = pop_address(host, registers);
    prefetch(host, 0x000831f8U);
    const auto target = pop_return(host, registers);
    prefetch(host, target);
    prefetch(host, target + 2U);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
