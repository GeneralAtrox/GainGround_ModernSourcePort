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

std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto high = (address & 1U) == 0U;
    const auto word = host.read_memory_word(
        kRegion, address & ~1U, high ? 0xff00U : 0x00ffU);
    return static_cast<std::uint8_t>(high ? word >> 8U : word);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto high = (address & 1U) == 0U;
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

void set_move_long_flags(CpuRegisters &registers, std::uint32_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x80000000U) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

void add_bcd_byte(CpuRegisters &registers, ExecutionHost &host,
    std::uint32_t source_address, std::uint32_t destination_address)
{
    const auto source = read_byte(host, source_address);
    const auto destination = read_byte(host, destination_address);
    const auto extend = (registers.status & kExtend) != 0U ? 1U : 0U;
    const auto half = static_cast<std::uint8_t>(
        (destination & 0x0fU) + (source & 0x0fU) + extend);
    const auto binary = static_cast<std::uint16_t>(destination + source + extend);
    auto result = static_cast<std::uint16_t>(
        binary + (half > 9U ? 6U : 0U));
    if (result > 0x009fU)
        result = static_cast<std::uint16_t>(result + 0x0060U);

    std::uint16_t flags{};
    if ((result & 0x00ffU) == 0U
        && (registers.status & kZero) != 0U)
        flags |= kZero;
    if ((result & 0x0080U) != 0U) flags |= kNegative;
    if ((result & 0x0300U) != 0U) flags |= kExtend | kCarry;
    if ((result & 0x0080U) != 0U && (binary & 0x0080U) == 0U)
        flags |= kOverflow;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
    write_byte(host, destination_address, static_cast<std::uint8_t>(result));
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(
        kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(
        kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_bcd_scale_value(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[0] = registers.address[4] + 0x84U;
    registers.address[1] = 0x00000826U;
    write_long(host, registers.address[1], registers.data[0]);
    registers.address[1] += 4U;
    registers.status = static_cast<std::uint16_t>(
        registers.status & ~kConditionCodeMask);

    for (unsigned index = 0; index != 4U; ++index) {
        --registers.address[1];
        --registers.address[0];
        add_bcd_byte(registers, host,
            registers.address[1], registers.address[0]);
    }

    if ((registers.status & kCarry) != 0U) {
        write_long(host, registers.address[0], 0x99999999U);
        set_move_long_flags(registers, 0x99999999U);
    }

    const auto return_address = pop_return(host, registers);
    registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
