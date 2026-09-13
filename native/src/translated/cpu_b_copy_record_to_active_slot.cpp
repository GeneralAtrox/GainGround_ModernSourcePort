#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {(address & kAddressMask) & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

[[nodiscard]] std::uint8_t read_stream_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        address <= kAddressMask ? kRegion : kSharedRegion,
        location.offset, location.mask) >> location.shift);
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
    const auto low = host.read_memory_word(kRegion, (address + 2U) & kAddressMask, kWordMask);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_byte_flags(CpuRegisters &r,
    std::uint8_t left, std::uint8_t right, std::uint8_t result)
{
    std::uint16_t flags{};
    if (static_cast<unsigned>(left) + right > 0xffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x80U) != 0U) flags |= 0x0002U;
    if ((result & 0x80U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (static_cast<std::uint32_t>(left) + right > 0xffffU) flags |= 0x0011U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_bit_zero(CpuRegisters &r, bool bit_was_set)
{
    if (bit_was_set) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kRegion, r.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_long(host, r.address[7]);
    r.address[7] += 4U;
    return value;
}
} // namespace

FunctionResult cpu_b_copy_record_to_active_slot(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    r.data[0] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    const auto descriptor = host.read_memory_word(kRegion, r.address[5] + 0x36U, kWordMask);
    r.data[0] = descriptor;
    set_logic_flags(r, descriptor, 0x8000U, 0xffffU);
    r.address[6] = r.data[0];

    const auto active = read_byte(host, r.address[6] + 0x3fU);
    set_logic_flags(r, active, 0x80U, 0xffU);
    if (active != 0U) {
        (void)host.read_memory_word(kRegion, r.address[5], kWordMask);
        host.write_memory_word(kRegion, r.address[5], 0U, kWordMask);
        set_logic_flags(r, 0U, 0x8000U, 0xffffU);
        const auto target = pop_return(host, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    const bool inhibit = (read_byte(host, r.address[6] + 0x41U) & 0x80U) != 0U;
    set_bit_zero(r, inhibit);
    if (inhibit) {
        const auto target = pop_return(host, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    r.address[0] = read_long(host, r.address[6] + 0x4cU);
    const auto stream_offset = host.read_memory_word(kRegion, r.address[5] + 0x38U, kWordMask);
    r.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(r.address[0]) + static_cast<std::int16_t>(stream_offset));
    const auto stream_region = r.address[0] <= kAddressMask ? kRegion : kSharedRegion;
    const auto marker = host.read_memory_word(
        stream_region, r.address[0] & kAddressMask, kWordMask);
    r.address[0] += 2U;
    host.write_memory_word(kRegion, r.address[5] + 6U, marker, kWordMask);
    set_logic_flags(r, marker, 0x8000U, 0xffffU);
    if (static_cast<std::int16_t>(marker) < 0) {
        const auto target = pop_return(host, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    auto value = read_stream_byte(host, r.address[0]++);
    r.data[0] = (r.data[0] & 0xffffff00U) | value;
    set_logic_flags(r, value, 0x80U, 0xffU);
    auto extended = static_cast<std::uint16_t>(
        static_cast<std::int16_t>(static_cast<std::int8_t>(value)));
    r.data[0] = (r.data[0] & 0xffff0000U) | extended;
    set_logic_flags(r, extended, 0x8000U, 0xffffU);
    auto addend = host.read_memory_word(kRegion, r.address[6] + 0x0cU, kWordMask);
    auto result_word = static_cast<std::uint16_t>(extended + addend);
    r.data[0] = (r.data[0] & 0xffff0000U) | result_word;
    set_add_word_flags(r, extended, addend, result_word);
    host.write_memory_word(kRegion, r.address[5] + 0x0cU, result_word, kWordMask);
    set_logic_flags(r, result_word, 0x8000U, 0xffffU);

    value = read_stream_byte(host, r.address[0]++);
    r.data[0] = (r.data[0] & 0xffffff00U) | value;
    set_logic_flags(r, value, 0x80U, 0xffU);
    extended = static_cast<std::uint16_t>(
        static_cast<std::int16_t>(static_cast<std::int8_t>(value)));
    r.data[0] = (r.data[0] & 0xffff0000U) | extended;
    set_logic_flags(r, extended, 0x8000U, 0xffffU);
    addend = host.read_memory_word(kRegion, r.address[6] + 0x0eU, kWordMask);
    result_word = static_cast<std::uint16_t>(extended + addend);
    r.data[0] = (r.data[0] & 0xffff0000U) | result_word;
    set_add_word_flags(r, extended, addend, result_word);
    host.write_memory_word(kRegion, r.address[5] + 0x0eU, result_word, kWordMask);
    set_logic_flags(r, result_word, 0x8000U, 0xffffU);

    r.data[0] = (r.data[0] & 0xffff0000U) | 0x0c00U;
    set_logic_flags(r, 0x0c00U, 0x8000U, 0xffffU);
    const auto left = static_cast<std::uint8_t>(r.data[0]);
    value = read_stream_byte(host, r.address[0]++);
    const auto result = static_cast<std::uint8_t>(left + value);
    r.data[0] = (r.data[0] & 0xffffff00U) | result;
    set_add_byte_flags(r, left, value, result);
    host.write_memory_word(kRegion, r.address[5] + 8U,
        static_cast<std::uint16_t>(r.data[0]), kWordMask);
    set_logic_flags(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U, 0xffffU);

    const bool fixed_attribute = (read_byte(host, r.address[6] + 0x78U) & 0x01U) != 0U;
    set_bit_zero(r, fixed_attribute);
    if (fixed_attribute) {
        const auto attribute = host.read_memory_word(kRegion, r.address[6] + 8U, kWordMask);
        host.write_memory_word(kRegion, r.address[5] + 8U, attribute, kWordMask);
        set_logic_flags(r, attribute, 0x8000U, 0xffffU);
    }

    auto flags = read_byte(host, r.address[5] + 1U);
    r.data[0] = (r.data[0] & 0xffffff00U) | flags;
    set_logic_flags(r, flags, 0x80U, 0xffU);
    flags = static_cast<std::uint8_t>(flags & 0xfcU);
    r.data[0] = (r.data[0] & 0xffffff00U) | flags;
    set_logic_flags(r, flags, 0x80U, 0xffU);
    value = read_stream_byte(host, r.address[0]++);
    flags = static_cast<std::uint8_t>(flags | value);
    r.data[0] = (r.data[0] & 0xffffff00U) | flags;
    set_logic_flags(r, flags, 0x80U, 0xffU);
    write_byte(host, r.address[5] + 1U, flags);
    set_logic_flags(r, flags, 0x80U, 0xffU);

    const auto position = host.read_memory_word(kRegion, r.address[6] + 0x1aU, kWordMask);
    host.write_memory_word(kRegion, r.address[5] + 0x1aU, position, kWordMask);
    set_logic_flags(r, position, 0x8000U, 0xffffU);
    write_byte(host, r.address[5], 0x80U);
    set_logic_flags(r, 0x80U, 0x80U, 0xffU);

    push_return(host, r, 0x0001efdaU);
    r.program_counter = 0x00015df2U;
    const auto child = host.call_function(282U, 1U, 0x72U, 2U,
        0x0001efd4U, 0x00015df2U, context);
    if (child.status != TranslationStatus::complete || child.control != 1U)
        return child;

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
