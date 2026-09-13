#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host,
    std::uint32_t address, std::uint16_t mask = kWordMask)
{
    return host.read_memory_word(kPrivateRegion,
        address & kAddressMask, mask);
}

void write_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value, std::uint16_t mask = kWordMask)
{
    host.write_memory_word(kPrivateRegion,
        address & kAddressMask, value, mask);
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto value = read_word(host, address & ~1U, mask);
    return static_cast<std::uint8_t>(value >> (odd ? 0U : 8U));
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    write_word(host, address & ~1U,
        static_cast<std::uint16_t>(value) << (odd ? 0U : 8U), mask);
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

[[nodiscard]] std::uint32_t pop_return(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult proven_static_cpu_b_72_00012cf6(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto old_value = read_byte(host, registers.address[5]);
    registers.status = static_cast<std::uint16_t>(
        (old_value & 1U) != 0U
            ? (registers.status & ~0x0004U)
            : (registers.status | 0x0004U));
    write_byte(host, registers.address[5],
        static_cast<std::uint8_t>(old_value & ~1U));

    push_return(host, registers, 0x00012d00U);
    registers.program_counter = 0x00015d24U;
    auto child = host.call_function(280U, 1U, 0x72U, 2U,
        0x00012cfaU, 0x00015d24U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    push_return(host, registers, 0x00012d06U);
    registers.program_counter = 0x00015df2U;
    child = host.call_function(282U, 1U, 0x72U, 2U,
        0x00012d00U, 0x00015df2U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
