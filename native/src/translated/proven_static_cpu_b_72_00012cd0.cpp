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
} // namespace

FunctionResult proven_static_cpu_b_72_00012cd0(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    auto byte = read_byte(host, base + 1U);
    registers.status = static_cast<std::uint16_t>(
        (byte & 0x04U) != 0U
            ? (registers.status & ~0x0004U)
            : (registers.status | 0x0004U));
    write_byte(host, base + 1U, static_cast<std::uint8_t>(byte | 0x04U));

    write_word(host, base + 2U, 0x0001U);
    write_word(host, base + 4U, 0x2ceaU);
    set_logic(registers, 0x00012ceaU, 0x80000000U, 0xffffffffU);

    byte = read_byte(host, base + 0x3fU);
    set_logic(registers, byte, 0x80U, 0xffU);
    write_byte(host, base + 0x3fU, static_cast<std::uint8_t>(byte | 0x80U));

    write_word(host, base + 0x10U, 0x2f2fU);
    set_logic(registers, 0x2f2fU, 0x8000U, 0xffffU);

    registers.program_counter = 0x00012cf6U;
    return host.call_function(537U, 1U, 0x72U, 1U,
        0x00012ce8U, 0x00012cf6U, context);
}

} // namespace gain_ground::translated
