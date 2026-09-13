#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_move_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void move_word(CpuRegisters &registers, std::size_t index,
    std::uint16_t value)
{
    registers.data[index] = (registers.data[index] & 0xffff0000U) | value;
    set_move_word_flags(registers, value);
}

void add_word(CpuRegisters &registers, std::size_t destination_index,
    std::uint16_t source)
{
    const auto destination = static_cast<std::uint16_t>(
        registers.data[destination_index]);
    const auto wide_result = static_cast<std::uint32_t>(destination) + source;
    const auto result = static_cast<std::uint16_t>(wide_result);
    registers.data[destination_index] =
        (registers.data[destination_index] & 0xffff0000U) | result;

    std::uint16_t flags = 0U;
    if (wide_result > 0xffffU) flags |= 0x0011U;
    if (((~(destination ^ source)) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & 0x0010U;
    if (destination < source) flags |= 0x0001U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host,
    CpuRegisters &registers)
{
    const auto stack = registers.address[7];
    const auto high = host.read_memory_word(kRegion, stack, kWordMask);
    const auto low = host.read_memory_word(kRegion, stack + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_test_record_overlap_y(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto bounds = registers.address[5];
    const auto record = registers.address[6];

    const auto separated = [&](std::uint32_t coordinate_offset,
                               std::int16_t lower_delta,
                               std::int16_t upper_delta,
                               std::uint32_t lower_bound_offset,
                               std::uint32_t upper_bound_offset) {
        move_word(registers, 1U, static_cast<std::uint16_t>(lower_delta));
        move_word(registers, 2U, static_cast<std::uint16_t>(upper_delta));
        const auto coordinate = host.read_memory_word(
            kRegion, record + coordinate_offset, kWordMask);
        move_word(registers, 0U, coordinate);
        add_word(registers, 1U, coordinate);
        add_word(registers, 2U, coordinate);

        const auto upper_bound = host.read_memory_word(
            kRegion, bounds + upper_bound_offset, kWordMask);
        const auto lower_value = static_cast<std::uint16_t>(registers.data[1]);
        set_compare_word_flags(registers, lower_value, upper_bound);
        if (static_cast<std::int16_t>(lower_value)
                > static_cast<std::int16_t>(upper_bound))
            return true;

        const auto lower_bound = host.read_memory_word(
            kRegion, bounds + lower_bound_offset, kWordMask);
        const auto upper_value = static_cast<std::uint16_t>(registers.data[2]);
        set_compare_word_flags(registers, upper_value, lower_bound);
        return static_cast<std::int16_t>(upper_value)
            < static_cast<std::int16_t>(lower_bound);
    };

    const bool is_separated =
        separated(0x12U, -10, 10, 0x2aU, 0x2cU)
        || separated(0x16U, -8, 12, 0x2eU, 0x30U)
        || separated(0x1aU, -9, 9, 0x32U, 0x34U);

    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | (is_separated ? 0U : 1U));
    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
