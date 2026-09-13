#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kConditionCodeMask = 0x001fU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool high = (address & 1U) == 0U;
    const auto word = host.read_memory_word(
        kRegion, address & ~1U, high ? 0xff00U : 0x00ffU);
    return static_cast<std::uint8_t>(high ? word >> 8U : word);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const bool high = (address & 1U) == 0U;
    host.write_memory_word(kRegion, address & ~1U,
        high ? static_cast<std::uint16_t>(value) << 8U : value,
        high ? 0xff00U : 0x00ffU);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    host.write_memory_word(kRegion, address,
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, address + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x80000000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void abcd_predecrement(ExecutionHost &host, CpuRegisters &registers)
{
    --registers.address[0];
    const auto source = read_byte(host, registers.address[0]);
    --registers.address[1];
    const auto destination = read_byte(host, registers.address[1]);
    const auto extend = (registers.status & kExtend) != 0U ? 1U : 0U;
    const auto low_sum = static_cast<std::uint8_t>(
        (destination & 0x0fU) + (source & 0x0fU) + extend);
    const auto binary = static_cast<std::uint16_t>(destination + source + extend);
    auto adjusted = static_cast<std::uint16_t>(
        binary + (low_sum > 9U ? 6U : 0U));
    if (adjusted > 0x009fU)
        adjusted = static_cast<std::uint16_t>(adjusted + 0x0060U);

    std::uint16_t flags{};
    if ((adjusted & 0x00ffU) == 0U
        && (registers.status & kZero) != 0U)
        flags |= kZero;
    if ((adjusted & 0x0080U) != 0U) flags |= kNegative;
    if ((adjusted & 0x0300U) != 0U) flags |= kExtend | kCarry;
    if ((adjusted & 0x0080U) != 0U && (binary & 0x0080U) == 0U)
        flags |= kOverflow;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
    write_byte(host, registers.address[1], static_cast<std::uint8_t>(adjusted));
}
} // namespace

FunctionResult proven_static_cpu_b_72_00015e7e(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    registers.address[0] = 0x00000826U;
    write_long(host, registers.address[0], registers.data[0]);
    registers.address[0] += 4U;
    set_move_long_flags(registers, registers.data[0]);

    registers.address[1] = registers.address[0];
    write_long(host, registers.address[1], registers.data[1]);
    registers.address[1] += 4U;
    set_move_long_flags(registers, registers.data[1]);

    registers.status = static_cast<std::uint16_t>(
        registers.status & ~kConditionCodeMask);
    for (unsigned index = 0; index != 4U; ++index)
        abcd_predecrement(host, registers);

    if ((registers.status & kCarry) != 0U) {
        write_long(host, registers.address[1], 0x99999999U);
        set_move_long_flags(registers, 0x99999999U);
    }

    registers.data[0] = read_long(host, registers.address[1]);
    set_move_long_flags(registers, registers.data[0]);
    const auto return_address = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
