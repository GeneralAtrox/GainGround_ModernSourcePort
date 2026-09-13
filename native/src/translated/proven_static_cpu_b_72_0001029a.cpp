#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_byte_flags(CpuRegisters &registers, std::uint8_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x80U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_logic_word_flags(CpuRegisters &registers, std::uint16_t value)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_zero(CpuRegisters &registers, bool bit_was_set)
{
    if (bit_was_set)
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x0004U);
    else
        registers.status = static_cast<std::uint16_t>(registers.status | 0x0004U);
}

void set_subtract_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags{};
    if (destination < source) flags |= 0x0011U;
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

FunctionResult proven_static_cpu_b_72_0001029a(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    FunctionResult character_result;
    if (host.run_character_damage(context, character_result)) return character_result;
    const auto a5 = registers.address[5];
    const auto counter = host.read_memory_word(kRegion, a5 + 0x48U, kWordMask);
    set_logic_word_flags(registers, counter);

    if (static_cast<std::int16_t>(counter) > 0) {
        const auto address = registers.address[6] + 0x3fU;
        const auto original = read_byte(host, address);
        write_byte(host, address, static_cast<std::uint8_t>(original | 0x80U));
        set_logic_byte_flags(registers, original);
        registers.status = static_cast<std::uint16_t>(registers.status & ~0x001fU);
    } else {
        host.write_memory_word(kRegion, a5 + 0x44U, 6U, kWordMask);
        set_logic_word_flags(registers, 6U);

        const auto bit_index = read_byte(host, a5 + 0x6dU);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | bit_index;
        set_logic_byte_flags(registers, bit_index);

        auto signal_mask = read_byte(host, 0x00000c06U);
        const auto bit = static_cast<std::uint8_t>(1U << (bit_index & 7U));
        set_bit_zero(registers, (signal_mask & bit) != 0U);
        signal_mask = static_cast<std::uint8_t>(signal_mask | bit);
        write_byte(host, 0x00000c06U, signal_mask);

        const auto global_counter = host.read_memory_word(
            kRegion, 0x00000c10U, kWordMask);
        const auto decremented = static_cast<std::uint16_t>(global_counter - 1U);
        host.write_memory_word(kRegion, 0x00000c10U, decremented, kWordMask);
        set_subtract_word_flags(registers, global_counter, 1U, decremented);
        registers.status = static_cast<std::uint16_t>(
            (registers.status & ~0x001fU) | 0x0001U);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
