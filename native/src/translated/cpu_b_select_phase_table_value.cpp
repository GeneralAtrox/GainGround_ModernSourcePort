#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint16_t read_word(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & 0x00ffffffU, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(kRegion, address & 0x00ffffffU, value, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto mask = static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U);
    const auto word = host.read_memory_word(
        kRegion, (address & ~1U) & 0x00ffffffU, mask);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const bool odd = (address & 1U) != 0U;
    host.write_memory_word(kRegion, (address & ~1U) & 0x00ffffffU,
        static_cast<std::uint16_t>(odd ? value : value << 8U),
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U));
}

void logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t mask, std::uint32_t sign) noexcept
{
    value &= mask;
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void add_flags(CpuRegisters &registers, std::uint32_t destination,
    std::uint32_t source, std::uint32_t result,
    std::uint32_t mask, std::uint32_t sign) noexcept
{
    destination &= mask;
    source &= mask;
    result &= mask;
    std::uint16_t flags{};
    if ((result & sign) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(destination ^ source)) & (destination ^ result) & sign) != 0U)
        flags |= 0x0002U;
    if (destination + source > mask) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void asl_word(CpuRegisters &registers, unsigned count) noexcept
{
    auto value = static_cast<std::uint16_t>(registers.data[0]);
    bool overflow{};
    bool carry{};
    for (unsigned index = 0; index != count; ++index) {
        const bool old_sign = (value & 0x8000U) != 0U;
        carry = old_sign;
        value = static_cast<std::uint16_t>(value << 1U);
        overflow = overflow || (old_sign != ((value & 0x8000U) != 0U));
    }
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    std::uint16_t flags{};
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (overflow) flags |= 0x0002U;
    if (carry) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] FunctionResult finish(
    ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = read_word(host, registers.address[7]);
    const auto low = read_word(host, registers.address[7] + 2U);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_select_phase_table_value(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;
    if (registers.program_counter != 0x000103e4U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    const auto record = registers.address[5];
    const auto current = read_word(host, record + 0x52U);
    logic_flags(registers, current, 0xffffU, 0x8000U);
    if ((current & 0x8000U) != 0U)
        return finish(host, registers);

    registers.data[0] = 0U;
    logic_flags(registers, 0U, 0xffffffffU, 0x80000000U);
    const auto selector = read_byte(host, record + 0x3eU);
    registers.data[0] = selector;
    logic_flags(registers, selector, 0xffU, 0x80U);
    asl_word(registers, 2U);

    if (selector == 0U) {
        const auto phase = read_word(host, record + 0x58U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | phase;
        logic_flags(registers, phase, 0xffffU, 0x8000U);
        asl_word(registers, 2U);
        const auto bias = read_byte(host, record + 0x3dU);
        const auto old_low = static_cast<std::uint8_t>(registers.data[0]);
        const auto result = static_cast<std::uint8_t>(old_low + bias);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | result;
        add_flags(registers, old_low, bias, result, 0xffU, 0x80U);
        write_word(host, record + 0x52U,
            static_cast<std::uint16_t>(registers.data[0]));
        logic_flags(registers, static_cast<std::uint16_t>(registers.data[0]),
            0xffffU, 0x8000U);
        return finish(host, registers);
    }

    if (selector != 1U && selector != 2U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    registers.address[0] = selector == 1U ? 0x000108e0U : 0x000108e8U;
    registers.data[0] = 0U;
    logic_flags(registers, 0U, 0xffffffffU, 0x80000000U);
    const auto table_row = read_byte(host, record + 0x4bU);
    registers.data[0] = table_row;
    logic_flags(registers, table_row, 0xffU, 0x80U);
    asl_word(registers, 4U);
    const auto phase = read_word(host, record + 0x58U);
    const auto old_index = static_cast<std::uint16_t>(registers.data[0]);
    const auto index = static_cast<std::uint16_t>(old_index + phase);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | index;
    add_flags(registers, old_index, phase, index, 0xffffU, 0x8000U);
    const auto table_address = static_cast<std::uint32_t>(
        registers.address[0] + static_cast<std::int16_t>(index));
    const auto table_value = read_byte(host, table_address);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | table_value;
    logic_flags(registers, table_value, 0xffU, 0x80U);
    const auto bias = read_byte(host, record + 0x3dU);
    const auto result = static_cast<std::uint8_t>(table_value + bias);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | result;
    add_flags(registers, table_value, bias, result, 0xffU, 0x80U);
    write_byte(host, record + 0x53U, result);
    logic_flags(registers, result, 0xffU, 0x80U);
    return finish(host, registers);
}

} // namespace gain_ground::translated
