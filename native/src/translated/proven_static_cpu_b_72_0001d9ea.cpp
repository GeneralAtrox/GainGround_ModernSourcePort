#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & 0x00fffffeU,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate(address);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kPrivateRegion, address & 0x00ffffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(
        kPrivateRegion, address & 0x00ffffffU, value, kWordMask);
}

void logic_byte(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void logic_word(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void sub_byte(CpuRegisters &registers, std::uint8_t left,
    std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void add_word(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void bset(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, unsigned bit)
{
    const auto value = read_byte(host, address);
    write_byte(host, address, static_cast<std::uint8_t>(value | (1U << bit)));
    registers.status = static_cast<std::uint16_t>((registers.status & ~0x0004U)
        | (((value & (1U << bit)) == 0U) ? 0x0004U : 0U));
}
} // namespace

FunctionResult proven_static_cpu_b_72_0001d9ea(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];

    write_byte(host, base + 0x54U, 1U);
    logic_byte(registers, 1U);

    const auto subtract_operand = read_byte(host, base + 0x3eU);
    const auto decremented = static_cast<std::uint8_t>(subtract_operand - 1U);
    write_byte(host, base + 0x3eU, decremented);
    sub_byte(registers, subtract_operand, 1U, decremented);

    const auto mask_operand_byte = read_byte(host, base + 0x3eU);
    const auto even = static_cast<std::uint8_t>(mask_operand_byte & 0xfeU);
    write_byte(host, base + 0x3eU, even);
    logic_byte(registers, even);

    bset(host, registers, base + 0x40U, 1U);
    bset(host, registers, base + 0x40U, 3U);

    const auto cursor = read_word(host, base + 0x5cU);
    const auto advanced = static_cast<std::uint16_t>(cursor + 0x0080U);
    write_word(host, base + 0x5cU, advanced);
    add_word(registers, cursor, 0x0080U, advanced);

    const auto mask_operand_word = read_word(host, base + 0x5cU);
    const auto masked = static_cast<std::uint16_t>(mask_operand_word & 0x0780U);
    write_word(host, base + 0x5cU, masked);
    logic_word(registers, masked);

    write_byte(host, base + 0x5eU, 9U);
    logic_byte(registers, 9U);

    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
