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

void set_logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_subq_byte(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t result)
{
    std::uint16_t flags = 0U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((destination ^ 1U) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (destination < 1U) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_addq_byte(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t result)
{
    std::uint16_t flags = 0U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(destination ^ 1U)) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (destination == 0xffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001396a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    auto byte = read_byte(host, base + 0x3cU);
    const auto decremented = static_cast<std::uint8_t>(byte - 1U);
    write_byte(host, base + 0x3cU, decremented);
    set_subq_byte(registers, byte, decremented);

    const bool zero = (registers.status & 0x0004U) != 0U;
    const bool negative = (registers.status & 0x0008U) != 0U;
    const bool overflow = (registers.status & 0x0002U) != 0U;
    if (!zero && negative == overflow) {
        registers.program_counter = 0x00013980U;
        return host.call_function(584U, 1U, 0x72U, 1U,
            0x0001396eU, 0x00013980U, context);
    }

    write_byte(host, base + 0x3cU, 2U);
    set_logic_byte(registers, 2U);

    byte = read_byte(host, base + 0x3dU);
    const auto incremented = static_cast<std::uint8_t>(byte + 1U);
    write_byte(host, base + 0x3dU, incremented);
    set_addq_byte(registers, byte, incremented);

    byte = read_byte(host, base + 0x3dU);
    const auto masked = static_cast<std::uint8_t>(byte & 3U);
    write_byte(host, base + 0x3dU, masked);
    set_logic_byte(registers, masked);

    registers.program_counter = 0x00013980U;
    return host.call_function(584U, 1U, 0x72U, 0U,
        0x0001397aU, 0x00013980U, context);
}

} // namespace gain_ground::translated
