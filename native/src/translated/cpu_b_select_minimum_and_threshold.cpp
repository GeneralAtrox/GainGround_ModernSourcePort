#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;

void set_logic_flags(CpuRegisters &registers, std::uint16_t value,
                     std::uint16_t sign_mask)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & sign_mask) != 0U) flags |= kNegative;
    if (value == 0U) flags |= kZero;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_compare_word_flags(CpuRegisters &registers,
                            std::uint16_t destination,
                            std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = registers.status & kExtend;
    if ((result & 0x8000U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if (destination < source) flags |= kCarry;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
                        std::uint16_t destination,
                        std::uint16_t source,
                        std::uint16_t result)
{
    const bool carry = static_cast<std::uint32_t>(destination) + source > 0xffffU;
    const bool overflow =
        ((~(destination ^ source) & (destination ^ result)) & 0x8000U) != 0U;
    std::uint16_t flags{};
    if (carry) flags |= static_cast<std::uint16_t>(kExtend | kCarry);
    if ((result & 0x8000U) != 0U) flags |= kNegative;
    if (result == 0U) flags |= kZero;
    if (overflow) flags |= kOverflow;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &registers)
{
    const auto high = host.read_memory_word(kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_select_minimum_and_threshold(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    for (std::uint32_t offset = 0x7aU; offset <= 0x7eU; offset += 2U) {
        host.write_memory_word(kRegion, registers.address[5] + offset,
            0x7fffU, kWordMask);
        set_logic_flags(registers, 0x7fffU, 0x8000U);
    }

    registers.data[0] &= 0xffff0000U;
    set_logic_flags(registers, 0U, 0x8000U);
    const auto mask_word = host.read_memory_word(kRegion, 0x00000820U, 0xff00U);
    const auto active_mask = static_cast<std::uint8_t>(mask_word >> 8U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | active_mask;
    set_logic_flags(registers, active_mask, 0x0080U);
    registers.data[2] = (registers.data[2] & 0xffff0000U) | 0x007aU;
    set_logic_flags(registers, 0x007aU, 0x8000U);
    registers.data[3] = 2U;
    set_logic_flags(registers, 2U, 0x8000U);
    registers.address[0] = 0x00007b6eU;

    for (unsigned slot = 0; slot != 3U; ++slot) {
        const auto bit = static_cast<unsigned>(registers.data[0] & 31U);
        const bool enabled = ((registers.data[1] >> bit) & 1U) != 0U;
        registers.status = static_cast<std::uint16_t>(
            enabled ? (registers.status & ~kZero) : (registers.status | kZero));
        if (enabled) {
            const auto value = host.read_memory_word(
                kRegion, registers.address[0], kWordMask);
            const auto destination = registers.address[5]
                + static_cast<std::int16_t>(registers.data[2]);
            host.write_memory_word(kRegion, destination, value, kWordMask);
            set_logic_flags(registers, value, 0x8000U);
        }

        const auto d0_before = static_cast<std::uint16_t>(registers.data[0]);
        const auto d0_after = static_cast<std::uint16_t>(d0_before + 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | d0_after;
        set_add_word_flags(registers, d0_before, 1U, d0_after);
        const auto d2_before = static_cast<std::uint16_t>(registers.data[2]);
        const auto d2_after = static_cast<std::uint16_t>(d2_before + 2U);
        registers.data[2] = (registers.data[2] & 0xffff0000U) | d2_after;
        set_add_word_flags(registers, d2_before, 2U, d2_after);
        registers.address[0] += 4U;
        registers.data[3] = (registers.data[3] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[3] - 1U);
    }

    auto minimum = host.read_memory_word(
        kRegion, registers.address[5] + 0x7aU, kWordMask);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | minimum;
    set_logic_flags(registers, minimum, 0x8000U);
    auto candidate = host.read_memory_word(
        kRegion, registers.address[5] + 0x7cU, kWordMask);
    set_compare_word_flags(registers, minimum, candidate);
    if (static_cast<std::int16_t>(minimum) > static_cast<std::int16_t>(candidate)) {
        minimum = host.read_memory_word(
            kRegion, registers.address[5] + 0x7cU, kWordMask);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | minimum;
        set_logic_flags(registers, minimum, 0x8000U);
    }
    candidate = host.read_memory_word(
        kRegion, registers.address[5] + 0x7eU, kWordMask);
    set_compare_word_flags(registers, minimum, candidate);
    if (static_cast<std::int16_t>(minimum) > static_cast<std::int16_t>(candidate)) {
        minimum = host.read_memory_word(
            kRegion, registers.address[5] + 0x7eU, kWordMask);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | minimum;
        set_logic_flags(registers, minimum, 0x8000U);
    }

    registers.address[0] = 0x000247f0U;
    for (;;) {
        const auto threshold = host.read_memory_word(
            kRegion, registers.address[0], kWordMask);
        registers.address[0] += 2U;
        set_compare_word_flags(registers, minimum, threshold);
        if (static_cast<std::int16_t>(minimum) <= static_cast<std::int16_t>(threshold))
            break;
        registers.address[0] += 2U;
    }

    const auto result = host.read_memory_word(
        kRegion, registers.address[0], kWordMask);
    registers.address[0] += 2U;
    registers.data[1] = (registers.data[1] & 0xffff0000U) | result;
    set_logic_flags(registers, result, 0x8000U);
    set_compare_word_flags(registers, minimum, 0x7fffU);
    if (minimum == 0x7fffU) {
        registers.data[1] = (registers.data[1] & 0xffff0000U) | 4U;
        set_logic_flags(registers, 4U, 0x8000U);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
