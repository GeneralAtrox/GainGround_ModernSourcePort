#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kCcrMask = 0x001fU;

struct ByteLocation {
    std::uint32_t offset;
    std::uint16_t mask;
    unsigned shift;
};

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(
        host.read_memory_word(kRegion, location.offset, location.mask)
        >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = static_cast<std::uint16_t>(registers.status & 0x0010U);
    if ((value & mask) == 0U) flags |= 0x0004U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kCcrMask) | flags);
}

void set_cmp_word_flags(CpuRegisters &registers, std::uint16_t destination,
    std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = static_cast<std::uint16_t>(registers.status & 0x0010U);
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (source > destination) flags |= 0x0001U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kCcrMask) | flags);
}

[[nodiscard]] std::uint16_t add_word(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source)
{
    const auto wide = static_cast<std::uint32_t>(destination) + source;
    const auto result = static_cast<std::uint16_t>(wide);
    std::uint16_t flags = 0U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (((~(destination ^ source)) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kCcrMask) | flags);
    return result;
}

[[nodiscard]] std::uint16_t sub_word(CpuRegisters &registers,
    std::uint16_t destination, std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = 0U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (source > destination) flags |= 0x0011U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kCcrMask) | flags);
    return result;
}

[[nodiscard]] std::uint16_t asl_word(CpuRegisters &registers,
    std::uint16_t value, unsigned count)
{
    bool carry = false;
    bool overflow = false;
    auto result = value;
    for (unsigned index = 0; index != count; ++index) {
        const bool old_sign = (result & 0x8000U) != 0U;
        carry = old_sign;
        result = static_cast<std::uint16_t>(result << 1U);
        overflow = overflow || (old_sign != ((result & 0x8000U) != 0U));
    }
    std::uint16_t flags = 0U;
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~kCcrMask) | flags);
    return result;
}

void push_return(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t value)
{
    registers.address[7] -= 4U;
    host.write_memory_word(kRegion, registers.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, registers.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto high = host.read_memory_word(kRegion, registers.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, registers.address[7] + 2U, kWordMask);
    registers.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_select_matching_table_record(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &registers = context.registers;
    registers.address[4] = 0x00003400U;
    registers.data[7] = 2U;
    set_logic_flags(registers, 2U, 0x80000000U, 0xffffffffU);

    push_return(host, registers, 0x0001faf0U);
    registers.program_counter = 0x0001fb6eU;
    const auto child = host.call_function(
        375U, 1U, 0x72U, 2U, 0x0001faecU, 0x0001fb6eU, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    for (const unsigned data_register : {0U, 1U, 3U, 4U}) {
        registers.data[data_register] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U, 0xffffffffU);
    }
    for (const unsigned data_register : {0U, 1U, 3U, 4U}) {
        const auto value = read_byte(host, registers.address[0]);
        ++registers.address[0];
        registers.data[data_register] =
            (registers.data[data_register] & 0xffffff00U) | value;
        set_logic_flags(registers, value, 0x80U, 0xffU);
    }
    for (const unsigned data_register : {0U, 1U, 3U, 4U}) {
        const auto value = asl_word(registers,
            static_cast<std::uint16_t>(registers.data[data_register]), 3U);
        registers.data[data_register] =
            (registers.data[data_register] & 0xffff0000U) | value;
    }

    registers.data[5] = 0U;
    set_logic_flags(registers, 0U, 0x80000000U, 0xffffffffU);
    const auto initial_selector = read_byte(host, registers.address[5] + 0x61U);
    registers.data[5] = initial_selector;
    set_logic_flags(registers, initial_selector, 0x80U, 0xffU);
    if (initial_selector == 0U) {
        registers.data[5] = 1U;
        set_logic_flags(registers, 1U, 0x80000000U, 0xffffffffU);
    }

    registers.address[2] = registers.address[4];
    registers.address[4] = static_cast<std::uint32_t>(
        registers.address[4] + static_cast<std::int16_t>(9));

    while (true) {
        const auto selector = static_cast<std::uint16_t>(registers.data[5]);
        const auto probe_address = static_cast<std::uint32_t>(registers.address[4]
            + static_cast<std::int16_t>(selector));
        const auto probe = read_byte(host, probe_address);
        set_logic_flags(registers, probe, 0x80U, 0xffU);
        bool rejected = probe == 0U;

        if (!rejected) {
            registers.data[2] = (registers.data[2] & 0xffff0000U) | selector;
            set_logic_flags(registers, selector, 0x8000U, 0xffffU);
            auto offset = sub_word(registers, selector, 1U);
            registers.data[2] = (registers.data[2] & 0xffff0000U) | offset;
            offset = asl_word(registers, offset, 2U);
            registers.data[2] = (registers.data[2] & 0xffff0000U) | offset;
            registers.address[3] = registers.address[2] + 0x10U;
            registers.address[3] = static_cast<std::uint32_t>(registers.address[3]
                + static_cast<std::int16_t>(offset));

            const auto lower_x = host.read_memory_word(
                kRegion, registers.address[3], kWordMask);
            const auto d0 = static_cast<std::uint16_t>(registers.data[0]);
            set_cmp_word_flags(registers, d0, lower_x);
            rejected = static_cast<std::int16_t>(d0)
                > static_cast<std::int16_t>(lower_x);
            if (!rejected) {
                const auto lower_x_again = host.read_memory_word(
                    kRegion, registers.address[3], kWordMask);
                const auto d1 = static_cast<std::uint16_t>(registers.data[1]);
                set_cmp_word_flags(registers, d1, lower_x_again);
                rejected = static_cast<std::int16_t>(d1)
                    < static_cast<std::int16_t>(lower_x_again);
            }
            if (!rejected) {
                const auto lower_y = host.read_memory_word(
                    kRegion, registers.address[3] + 2U, kWordMask);
                const auto d3 = static_cast<std::uint16_t>(registers.data[3]);
                set_cmp_word_flags(registers, d3, lower_y);
                rejected = static_cast<std::int16_t>(d3)
                    > static_cast<std::int16_t>(lower_y);
            }
            if (!rejected) {
                const auto lower_y = host.read_memory_word(
                    kRegion, registers.address[3] + 2U, kWordMask);
                const auto d4 = static_cast<std::uint16_t>(registers.data[4]);
                set_cmp_word_flags(registers, d4, lower_y);
                rejected = static_cast<std::int16_t>(d4)
                    < static_cast<std::int16_t>(lower_y);
            }
        }

        if (!rejected) {
            write_byte(host, registers.address[5] + 0x60U,
                static_cast<std::uint8_t>(registers.data[5]));
            set_logic_flags(registers, registers.data[5], 0x80U, 0xffU);
            const auto first = host.read_memory_word(
                kRegion, registers.address[3], kWordMask);
            registers.address[3] += 2U;
            host.write_memory_word(kRegion, registers.address[5] + 0x6aU,
                first, kWordMask);
            set_logic_flags(registers, first, 0x8000U, 0xffffU);
            const auto second = host.read_memory_word(
                kRegion, registers.address[3], kWordMask);
            registers.address[3] += 2U;
            host.write_memory_word(kRegion, registers.address[5] + 0x6cU,
                second, kWordMask);
            set_logic_flags(registers, second, 0x8000U, 0xffffU);
            registers.status = static_cast<std::uint16_t>(
                (registers.status & ~kCcrMask) | 0x0001U);
            return finish(context);
        }

        auto selector_word = add_word(registers,
            static_cast<std::uint16_t>(registers.data[5]), 1U);
        registers.data[5] = (registers.data[5] & 0xffff0000U) | selector_word;
        set_cmp_word_flags(registers, selector_word, 4U);
        if (static_cast<std::int16_t>(selector_word) >= 4) {
            selector_word = add_word(registers, selector_word, 1U);
            registers.data[5] =
                (registers.data[5] & 0xffff0000U) | selector_word;
        }
        selector_word = static_cast<std::uint16_t>(selector_word & 3U);
        registers.data[5] = (registers.data[5] & 0xffff0000U) | selector_word;
        set_logic_flags(registers, selector_word, 0x8000U, 0xffffU);
        write_byte(host, registers.address[5] + 0x60U, 0U);
        set_logic_flags(registers, 0U, 0x80U, 0xffU);

        const auto counter = static_cast<std::uint16_t>(registers.data[7] - 1U);
        registers.data[7] = (registers.data[7] & 0xffff0000U) | counter;
        if (counter != 0xffffU) continue;

        registers.status = static_cast<std::uint16_t>(registers.status & ~kCcrMask);
        return finish(context);
    }
}
} // namespace gain_ground::translated
