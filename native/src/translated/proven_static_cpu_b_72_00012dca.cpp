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

FunctionResult proven_static_cpu_b_72_00012dca(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.data[0] = 0U;
    set_logic(registers, 0U, 0x80000000U, 0xffffffffU);

    const auto selector = read_byte(host, registers.address[5] + 0x3dU);
    registers.data[0] = selector;
    set_logic(registers, selector, 0x80U, 0xffU);

    registers.address[0] = 0x0001360aU +
        static_cast<std::uint16_t>(registers.data[0]);

    const auto word = read_word(host, registers.address[0]);
    registers.address[0] += 2U;
    write_word(host, registers.address[5] + 6U, word);
    set_logic(registers, word, 0x8000U, 0xffffU);

    auto byte = read_byte(host, registers.address[0]);
    registers.address[0] += 1U;
    write_byte(host, registers.address[5] + 1U, byte);
    set_logic(registers, byte, 0x80U, 0xffU);

    byte = read_byte(host, registers.address[0]);
    registers.address[0] += 1U;
    write_byte(host, registers.address[5] + 9U, byte);
    set_logic(registers, byte, 0x80U, 0xffU);

    registers.program_counter = 0x00012de2U;
    return host.call_function(583U, 1U, 0x72U, 0U,
        0x00012ddeU, 0x00012de2U, context);
}

} // namespace gain_ground::translated
