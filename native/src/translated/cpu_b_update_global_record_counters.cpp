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

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host,
    std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
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

void set_subtract_byte_flags(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source, std::uint8_t result,
    bool update_extend)
{
    std::uint16_t flags = update_extend ? 0U
        : static_cast<std::uint16_t>(registers.status & 0x0010U);
    if (destination < source) flags |= update_extend ? 0x0011U : 0x0001U;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
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

void set_add_byte_flags(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint16_t>(left) + right > 0xffU)
        flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU)
        flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint8_t abcd(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source)
{
    const auto extend = static_cast<std::uint16_t>(
        (registers.status & 0x0010U) != 0U);
    const auto binary = static_cast<std::uint16_t>(destination + source + extend);
    auto adjusted = binary;
    if (static_cast<std::uint16_t>((destination & 0x0fU)
        + (source & 0x0fU) + extend) > 9U)
        adjusted = static_cast<std::uint16_t>(adjusted + 6U);
    const bool carry = adjusted > 0x99U;
    if (carry) adjusted = static_cast<std::uint16_t>(adjusted + 0x60U);
    const auto result = static_cast<std::uint8_t>(adjusted);

    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if ((registers.status & 0x0004U) != 0U && result == 0U)
        flags |= 0x0004U;
    if (((~binary) & adjusted & 0x80U) != 0U) flags |= 0x0002U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return result;
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

FunctionResult cpu_b_update_global_record_counters(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;

    const auto global_count = host.read_memory_word(
        kRegion, 0x00000c10U, kWordMask);
    const auto decremented = static_cast<std::uint16_t>(global_count - 1U);
    host.write_memory_word(kRegion, 0x00000c10U, decremented, kWordMask);
    set_subtract_word_flags(registers, global_count, 1U, decremented);

    const auto roster = registers.address[4];
    const auto count = read_byte(host, roster + 0x40U);
    const auto compared = static_cast<std::uint8_t>(count - 0x3eU);
    set_subtract_byte_flags(registers, count, 0x3eU, compared, false);

    if (count < 0x3eU) {
        const auto accepted = host.read_memory_word(
            kRegion, 0x00000c12U, kWordMask);
        const auto incremented = static_cast<std::uint16_t>(accepted + 1U);
        host.write_memory_word(kRegion, 0x00000c12U, incremented, kWordMask);
        set_add_word_flags(registers, accepted, 1U, incremented);

        const auto count_before_increment = read_byte(host, roster + 0x40U);
        const auto new_count = static_cast<std::uint8_t>(
            count_before_increment + 1U);
        write_byte(host, roster + 0x40U, new_count);
        set_add_byte_flags(registers, count_before_increment, 1U, new_count);

        auto d0 = read_byte(host, roster + 0x41U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | d0;
        set_logic_byte_flags(registers, d0);

        registers.data[1] = 1U;
        set_logic_byte_flags(registers, 1U);
        registers.status = static_cast<std::uint16_t>(
            registers.status & ~0x001fU);

        d0 = abcd(registers, d0, 1U);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | d0;
        write_byte(host, roster + 0x41U, d0);
        set_logic_byte_flags(registers, d0);

        const auto current_count = read_byte(host, roster + 0x40U);
        const auto d1 = static_cast<std::uint8_t>(
            static_cast<std::uint8_t>(registers.data[1]) + current_count);
        set_add_byte_flags(registers,
            static_cast<std::uint8_t>(registers.data[1]), current_count, d1);
        registers.data[1] = (registers.data[1] & 0xffffff00U) | d1;

        const auto record_value = read_byte(host, registers.address[5] + 0x4bU);
        write_byte(host, roster + 0x40U
            + static_cast<std::int16_t>(registers.data[1]), record_value);
        set_logic_byte_flags(registers, record_value);
    }

    const auto target = pop_return(host, registers);
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
