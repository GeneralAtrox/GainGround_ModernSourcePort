#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kPrivateRegion, address & 0x0003ffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address,
                std::uint16_t value)
{
    host.write_memory_word(
        kPrivateRegion, address & 0x0003ffffU, value, kWordMask);
}

void write_long(ExecutionHost &host, std::uint32_t address,
                std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void set_logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_word(CpuRegisters &registers, std::uint16_t left,
                  std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (right > left) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
                 std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_long(host, registers.address[7], value);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto result = (static_cast<std::uint32_t>(
        read_word(host, registers.address[7])) << 16U)
        | read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return result;
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000a95a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x00001600U;
    const auto source_count = read_word(host, 0x00024836U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | source_count;
    set_logic_word(registers, source_count);

    auto counter = static_cast<std::uint16_t>(source_count - 5U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
    set_sub_word(registers, source_count, 5U, counter);
    for (;;) {
        (void)read_word(host, registers.address[0]);
        write_word(host, registers.address[0], 0U);
        set_logic_word(registers, 0U);
        registers.address[0] += 0x80U;
        counter = static_cast<std::uint16_t>(counter - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    registers.data[0] = (registers.data[0] & 0xffff0000U) | 0x005aU;
    set_logic_word(registers, 0x005aU);
    push_return(host, registers, 0x0000a97aU);
    registers.program_counter = 0x00016ff8U;
    const auto child = host.call_function(
        308U, 1U, 0x72U, 2U, 0x0000a974U, 0x00016ff8U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
