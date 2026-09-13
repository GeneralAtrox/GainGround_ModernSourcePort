#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kCpuBMainMemoryRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint16_t kExtend = 0x0010U;
constexpr std::uint16_t kNegative = 0x0008U;
constexpr std::uint16_t kZero = 0x0004U;
constexpr std::uint16_t kOverflow = 0x0002U;
constexpr std::uint16_t kCarry = 0x0001U;
constexpr std::uint16_t kConditionCodeMask = 0x001fU;

struct ByteLocation {
    std::uint32_t word_offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation byte_location(std::uint32_t address) noexcept
{
    return {address & ~1U, static_cast<std::uint16_t>((address & 1U) == 0U ? 0xff00U : 0x00ffU),
        (address & 1U) == 0U ? 8U : 0U};
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & kExtend;
    if ((value & 0x80U) != 0U)
        flags |= kNegative;
    if (value == 0U)
        flags |= kZero;
    registers.status = static_cast<std::uint16_t>((registers.status & ~kConditionCodeMask) | flags);
}

void set_add_byte_flags(
    CpuRegisters &registers, std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if ((result & 0x80U) != 0U)
        flags |= kNegative;
    if (result == 0U)
        flags |= kZero;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U)
        flags |= kOverflow;
    if (static_cast<unsigned>(left) + right > 0xffU)
        flags |= kExtend | kCarry;
    registers.status = static_cast<std::uint16_t>((registers.status & ~kConditionCodeMask) | flags);
}

void set_sub_word_flags(
    CpuRegisters &registers, std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U)
        flags |= kNegative;
    if (result == 0U)
        flags |= kZero;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U)
        flags |= kOverflow;
    if (left < right)
        flags |= kExtend | kCarry;
    registers.status = static_cast<std::uint16_t>((registers.status & ~kConditionCodeMask) | flags);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kCpuBMainMemoryRegion, address, kFullWordMask);
    const auto low = host.read_memory_word(kCpuBMainMemoryRegion, address + 2U, kFullWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

} // namespace

FunctionResult cpu_b_advance_byte_phase_counter(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    const std::uint32_t counter_address = context.registers.address[5] + 0x48U;
    const auto counter = host.read_memory_word(
        kCpuBMainMemoryRegion, counter_address, kFullWordMask);
    set_logic_byte_flags(context.registers, static_cast<std::uint8_t>(counter));
    if ((counter & 0x8000U) != 0U)
        context.registers.status |= kNegative;
    else
        context.registers.status &= static_cast<std::uint16_t>(~kNegative);
    if (counter == 0U)
        context.registers.status |= kZero;
    else
        context.registers.status &= static_cast<std::uint16_t>(~kZero);

    const auto phase = byte_location(context.registers.address[5] + 0x5aU);
    if (static_cast<std::int16_t>(counter) <= 0) {
        (void)host.read_memory_word(kCpuBMainMemoryRegion, phase.word_offset, phase.mask);
        host.write_memory_word(kCpuBMainMemoryRegion, phase.word_offset, 0U, phase.mask);
        set_logic_byte_flags(context.registers, 0U);
    } else {
        const auto phase_word = host.read_memory_word(
            kCpuBMainMemoryRegion, phase.word_offset, phase.mask);
        const auto old_phase = static_cast<std::uint8_t>(phase_word >> phase.shift);
        const auto new_phase = static_cast<std::uint8_t>(old_phase + 0x20U);
        host.write_memory_word(
            kCpuBMainMemoryRegion, phase.word_offset,
            static_cast<std::uint16_t>(new_phase) << phase.shift, phase.mask);
        set_add_byte_flags(context.registers, old_phase, 0x20U, new_phase);

        const auto old_counter = host.read_memory_word(
            kCpuBMainMemoryRegion, counter_address, kFullWordMask);
        const auto new_counter = static_cast<std::uint16_t>(old_counter - 1U);
        host.write_memory_word(
            kCpuBMainMemoryRegion, counter_address, new_counter, kFullWordMask);
        set_sub_word_flags(context.registers, old_counter, 1U, new_counter);
    }

    const std::uint32_t return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
