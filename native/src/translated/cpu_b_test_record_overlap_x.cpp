#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

[[nodiscard]] std::uint16_t load_d0_word(ExecutionHost &host,
    CpuRegisters &registers, std::uint32_t address)
{
    const auto value = host.read_memory_word(kRegion, address, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;

    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return value;
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

FunctionResult cpu_b_test_record_overlap_x(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    const auto left = registers.address[5];
    const auto right = registers.address[6];

    const auto fails_lower_bound = [&](std::uint32_t left_offset,
                                       std::uint32_t right_offset) {
        const auto value = load_d0_word(host, registers, left + left_offset);
        const auto boundary = host.read_memory_word(
            kRegion, right + right_offset, kWordMask);
        set_compare_word_flags(registers, value, boundary);
        return static_cast<std::int16_t>(value)
            < static_cast<std::int16_t>(boundary);
    };
    const auto fails_upper_bound = [&](std::uint32_t left_offset,
                                       std::uint32_t right_offset) {
        const auto value = load_d0_word(host, registers, left + left_offset);
        const auto boundary = host.read_memory_word(
            kRegion, right + right_offset, kWordMask);
        set_compare_word_flags(registers, value, boundary);
        return static_cast<std::int16_t>(value)
            > static_cast<std::int16_t>(boundary);
    };

    const bool separated =
        fails_lower_bound(0x2cU, 0x2aU)
        || fails_upper_bound(0x2aU, 0x2cU)
        || fails_lower_bound(0x30U, 0x2eU)
        || fails_upper_bound(0x2eU, 0x30U)
        || fails_lower_bound(0x34U, 0x32U)
        || fails_upper_bound(0x32U, 0x34U);

    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | (separated ? 0U : 1U));
    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
