#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kPrivateMask = 0x0003ffffU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const auto offset = address & kPrivateMask;
    const bool odd = (offset & 1U) != 0U;
    return {offset & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U),
        odd ? 0U : 8U};
}

[[nodiscard]] std::uint16_t read_word(
    ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(
        kPrivateRegion, address & kPrivateMask, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address,
    std::uint16_t value)
{
    host.write_memory_word(
        kPrivateRegion, address & kPrivateMask, value, kWordMask);
}

[[nodiscard]] std::uint8_t read_byte(
    ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(kPrivateRegion,
        location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address,
    std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint32_t read_long(
    ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U));
    write_word(host, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value));
}

void set_logic_flags(CpuRegisters &registers,
    std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = registers.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_byte_flags(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source, std::uint8_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint16_t>(destination) + source > 0xffU)
        flags |= 0x0011U;
    if (((~(destination ^ source)) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(destination) + source > 0xffffU)
        flags |= 0x0011U;
    if (((~(destination ^ source)) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_byte_flags(CpuRegisters &registers,
    std::uint8_t destination, std::uint8_t source, std::uint8_t result)
{
    std::uint16_t flags{};
    if (source > destination) flags |= 0x0011U;
    if (((destination ^ source) & (destination ^ result) & 0x80U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_word_flags(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source, std::uint16_t result)
{
    std::uint16_t flags{};
    if (source > destination) flags |= 0x0011U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
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
    const auto binary = static_cast<std::uint16_t>(
        destination + source + extend);
    auto adjusted = binary;
    if (static_cast<std::uint16_t>((destination & 0x0fU)
        + (source & 0x0fU) + extend) > 9U)
        adjusted = static_cast<std::uint16_t>(adjusted + 6U);
    if (adjusted > 0x009fU)
        adjusted = static_cast<std::uint16_t>(adjusted + 0x0060U);
    const bool carry = (adjusted & 0x0300U) != 0U;
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

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &registers = context.registers;
    const auto target = read_long(*context.host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult proven_static_cpu_b_72_0000fd40(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &registers = context.registers;
    if (registers.program_counter != 0x0000fd40U)
        return {TranslationStatus::contract_violation, 0U,
            registers.program_counter};

    registers.data[0] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    const auto source_count = read_byte(host, registers.address[4]);
    registers.data[0] = source_count;
    set_logic_flags(registers, source_count, 0x80U);

    auto word = read_word(host, 0x00000c10U);
    auto word_result = static_cast<std::uint16_t>(word - source_count);
    write_word(host, 0x00000c10U, word_result);
    set_sub_word_flags(registers, word, source_count, word_result);

    const auto stored_count = read_byte(host, registers.address[4] + 0x40U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | stored_count;
    set_logic_flags(registers, stored_count, 0x80U);
    auto byte_result = static_cast<std::uint8_t>(stored_count + source_count);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | byte_result;
    set_add_byte_flags(registers, stored_count, source_count, byte_result);
    const auto adjusted_count = static_cast<std::uint8_t>(byte_result - 0x3eU);
    registers.data[1] =
        (registers.data[1] & 0xffffff00U) | adjusted_count;
    set_sub_byte_flags(registers, byte_result, 0x3eU, adjusted_count);

    const bool less_or_equal = (registers.status & 0x0004U) != 0U
        || ((registers.status & 0x0008U) != 0U)
            != ((registers.status & 0x0002U) != 0U);
    if (!less_or_equal) {
        const auto d0 = static_cast<std::uint8_t>(registers.data[0]);
        byte_result = static_cast<std::uint8_t>(d0 - adjusted_count);
        registers.data[0] = (registers.data[0] & 0xffffff00U) | byte_result;
        set_sub_byte_flags(registers, d0, adjusted_count, byte_result);

        registers.data[2] = (registers.data[2] & 0xffff0000U)
            | static_cast<std::uint16_t>(registers.data[0]);
        set_logic_flags(registers,
            static_cast<std::uint16_t>(registers.data[2]), 0x8000U);

        push_return(host, registers, 0x0000fd60U);
        registers.program_counter = 0x00016234U;
        const auto child = host.call_function(299U, 1U, 0x72U, 2U,
            0x0000fd5aU, 0x00016234U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;

        const auto decimal = static_cast<std::uint8_t>(registers.data[0]);
        write_byte(host, registers.address[4] + 1U, decimal);
        set_logic_flags(registers, decimal, 0x80U);
        const auto saved_count = static_cast<std::uint16_t>(registers.data[2]);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | saved_count;
        set_logic_flags(registers, saved_count, 0x8000U);
    }

    word = read_word(host, 0x00000c12U);
    word_result = static_cast<std::uint16_t>(
        word + static_cast<std::uint16_t>(registers.data[0]));
    write_word(host, 0x00000c12U, word_result);
    set_add_word_flags(registers, word,
        static_cast<std::uint16_t>(registers.data[0]), word_result);
    const auto copy_count = static_cast<std::uint16_t>(registers.data[0]);
    set_logic_flags(registers, copy_count, 0x8000U);
    if (copy_count == 0U)
        return finish(context);

    registers.data[1] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U);
    const auto destination_index =
        read_byte(host, registers.address[4] + 0x40U);
    registers.data[1] = destination_index;
    set_logic_flags(registers, destination_index, 0x80U);
    registers.address[0] = registers.address[4] + 0x42U
        + static_cast<std::int16_t>(
            static_cast<std::uint16_t>(registers.data[1]));

    const auto old_total = read_byte(host, registers.address[4] + 0x40U);
    const auto new_total = static_cast<std::uint8_t>(
        old_total + static_cast<std::uint8_t>(registers.data[0]));
    write_byte(host, registers.address[4] + 0x40U, new_total);
    set_add_byte_flags(registers, old_total,
        static_cast<std::uint8_t>(registers.data[0]), new_total);

    const auto low_decimal = read_byte(host, registers.address[4] + 1U);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | low_decimal;
    set_logic_flags(registers, low_decimal, 0x80U);
    const auto high_decimal = read_byte(host, registers.address[4] + 0x41U);
    registers.data[2] = (registers.data[2] & 0xffffff00U) | high_decimal;
    set_logic_flags(registers, high_decimal, 0x80U);
    registers.status = static_cast<std::uint16_t>(registers.status & ~0x001fU);
    const auto combined = abcd(registers, low_decimal, high_decimal);
    registers.data[1] = (registers.data[1] & 0xffffff00U) | combined;
    write_byte(host, registers.address[4] + 0x41U, combined);
    set_logic_flags(registers, combined, 0x80U);

    (void)read_word(host, registers.address[4]);
    write_word(host, registers.address[4], 0U);
    set_logic_flags(registers, 0U, 0x8000U);

    const auto before_subq = static_cast<std::uint16_t>(registers.data[0]);
    auto counter = static_cast<std::uint16_t>(before_subq - 1U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
    set_sub_word_flags(registers, before_subq, 1U, counter);
    for (;;) {
        const auto signed_index = static_cast<std::int16_t>(counter);
        const auto value = read_byte(host,
            registers.address[4] + 2U + signed_index);
        write_byte(host, registers.address[0] + signed_index, value);
        set_logic_flags(registers, value, 0x80U);
        counter = static_cast<std::uint16_t>(counter - 1U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    return finish(context);
}

} // namespace gain_ground::translated
