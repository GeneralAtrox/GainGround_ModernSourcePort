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

struct ByteLocation {
    std::uint32_t word_offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    return {address & ~1U,
        static_cast<std::uint16_t>((address & 1U) == 0U ? 0xff00U : 0x00ffU),
        (address & 1U) == 0U ? 8U : 0U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(
        host.read_memory_word(kRegion, location.word_offset, location.mask) >> location.shift);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address, kWordMask);
    const auto low = host.read_memory_word(kRegion, address + 2U, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x8000U) != 0U)
        flags |= kNegative;
    if (value == 0U)
        flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
}

[[nodiscard]] std::uint16_t add_word(
    CpuRegisters &registers, std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left + right);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= kNegative;
    if (result == 0U)
        flags |= kZero;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= kExtend | kCarry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
    return result;
}

[[nodiscard]] std::uint16_t subtract_word(
    CpuRegisters &registers, std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= kNegative;
    if (result == 0U)
        flags |= kZero;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if (left < right)
        flags |= kExtend | kCarry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kConditionCodeMask) | flags);
    return result;
}

void write_result(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kWordMask);
    set_move_word_flags(registers, value);
}

[[nodiscard]] std::int16_t next_extent(
    ExecutionHost &host, CpuRegisters &registers, std::uint32_t &table,
    std::uint32_t preserved_d1_high)
{
    const auto raw = read_byte(host, table++);
    const auto extent = static_cast<std::int16_t>(static_cast<std::int8_t>(raw));
    registers.data[1] = preserved_d1_high | static_cast<std::uint16_t>(extent);
    return extent;
}

} // namespace

FunctionResult cpu_b_compute_descriptor_bounds(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    const std::uint32_t base = context.registers.address[5];
    const std::uint32_t d0_high = context.registers.data[0] & 0xffff0000U;
    const std::uint32_t d1_high = context.registers.data[1] & 0xffff0000U;
    const auto descriptor = host.read_memory_word(kRegion, base + 0x06U, kWordMask);
    auto index = static_cast<std::uint16_t>(descriptor - 0x00b2U);
    index = static_cast<std::uint16_t>(index + index);
    const auto tripled = static_cast<std::uint16_t>(index + index + index);
    context.registers.data[0] = d0_high | tripled;
    context.registers.data[1] = d1_high | index;
    std::uint32_t table = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(0x0003ab24U) + static_cast<std::int16_t>(tripled));
    context.registers.address[0] = table;

    std::uint16_t coordinate = host.read_memory_word(kRegion, base + 0x12U, kWordMask);
    context.registers.data[0] = d0_high | coordinate;
    const auto attributes = read_byte(host, base + 0x01U);
    if ((attributes & 0x02U) == 0U) {
        auto extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        auto result = subtract_word(context.registers, coordinate, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x2aU, result);
        extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        result = add_word(context.registers, result, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x2cU, result);
    } else {
        auto extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        auto result = add_word(context.registers, coordinate, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x2cU, result);
        extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        result = subtract_word(context.registers, result, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x2aU, result);
    }

    coordinate = host.read_memory_word(kRegion, base + 0x16U, kWordMask);
    context.registers.data[0] = d0_high | coordinate;
    const auto vertical_attributes = read_byte(host, base + 0x01U);
    if ((vertical_attributes & 0x01U) == 0U) {
        auto extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        auto result = subtract_word(context.registers, coordinate, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x2eU, result);
        extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        result = add_word(context.registers, result, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x30U, result);

        coordinate = host.read_memory_word(kRegion, base + 0x1aU, kWordMask);
        context.registers.data[0] = d0_high | coordinate;
        extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        result = subtract_word(context.registers, coordinate, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x32U, result);
        extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        result = add_word(context.registers, result, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x34U, result);
    } else {
        auto extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        auto result = add_word(context.registers, coordinate, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x30U, result);
        extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        result = subtract_word(context.registers, result, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x2eU, result);

        coordinate = host.read_memory_word(kRegion, base + 0x1aU, kWordMask);
        context.registers.data[0] = d0_high | coordinate;
        extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        result = add_word(context.registers, coordinate, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x34U, result);
        extent = static_cast<std::uint16_t>(next_extent(host, context.registers, table, d1_high));
        result = subtract_word(context.registers, result, extent);
        context.registers.data[0] = d0_high | result;
        write_result(host, context.registers, base + 0x32U, result);
    }

    context.registers.address[0] = table;
    const std::uint32_t return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
