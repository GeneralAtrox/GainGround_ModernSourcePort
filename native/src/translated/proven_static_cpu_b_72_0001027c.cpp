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

void set_logic(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags = 0U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
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

FunctionResult proven_static_cpu_b_72_0001027c(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    write_word(host, registers.address[5] + 0x44U, 6U);
    set_logic(registers, 6U, 0x8000U, 0xffffU);

    const auto byte = read_byte(host, registers.address[5] + 0x6dU);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | byte;
    set_logic(registers, byte, 0x80U, 0xffU);

    const auto bit = static_cast<unsigned>(byte & 7U);
    const auto old_value = read_byte(host, 0x00000c06U);
    const auto bit_mask = static_cast<std::uint8_t>(1U << bit);
    registers.status = static_cast<std::uint16_t>(
        (old_value & bit_mask) != 0U
            ? (registers.status & ~0x0004U)
            : (registers.status | 0x0004U));
    write_byte(host, 0x00000c06U,
        static_cast<std::uint8_t>(old_value | bit_mask));

    const auto counter = read_word(host, 0x00000c10U);
    const auto decremented = static_cast<std::uint16_t>(counter - 1U);
    write_word(host, 0x00000c10U, decremented);
    set_sub_word(registers, counter, 1U, decremented);

    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | 0x0001U);

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
