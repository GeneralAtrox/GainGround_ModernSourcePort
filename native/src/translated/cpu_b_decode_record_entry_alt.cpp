#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
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

[[nodiscard]] std::uint8_t read_private_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kPrivateRegion, location.offset, location.mask) >> location.shift);
}

[[nodiscard]] std::uint8_t read_stream_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        address <= kAddressMask ? kPrivateRegion : kSharedRegion,
        location.offset, location.mask) >> location.shift);
}

void write_private_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kPrivateRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

[[nodiscard]] std::uint32_t read_private_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = host.read_memory_word(
        kPrivateRegion, address & kAddressMask, kWordMask);
    const auto low = host.read_memory_word(
        kPrivateRegion, (address + 2U) & kAddressMask, kWordMask);
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

void set_sub_word_flags(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if (left < right) flags |= 0x0011U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint16_t arithmetic_shift_right_two(
    CpuRegisters &r, std::uint16_t value)
{
    auto shifted = value;
    bool carry{};
    for (unsigned count = 0; count != 2U; ++count) {
        carry = (shifted & 1U) != 0U;
        shifted = static_cast<std::uint16_t>(
            (shifted >> 1U) | (shifted & 0x8000U));
    }
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if ((shifted & 0x8000U) != 0U) flags |= 0x0008U;
    if (shifted == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return shifted;
}

void set_bit_zero(CpuRegisters &r, bool bit_was_set)
{
    if (bit_was_set) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kPrivateRegion, r.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kPrivateRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto value = read_private_long(host, r.address[7]);
    r.address[7] += 4U;
    return value;
}
} // namespace

FunctionResult cpu_b_decode_record_entry_alt(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    const auto descriptor_word = host.read_memory_word(
        kPrivateRegion, r.address[5] + 0x36U, kWordMask);
    r.address[6] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(descriptor_word)));
    const auto state = host.read_memory_word(
        kPrivateRegion, r.address[6] + 0x52U, kWordMask);
    set_logic_flags(r, state, 0x8000U, 0xffffU);
    if (static_cast<std::int16_t>(state) < 0) {
        (void)host.read_memory_word(kPrivateRegion, r.address[5], kWordMask);
        host.write_memory_word(kPrivateRegion, r.address[5], 0U, kWordMask);
        set_logic_flags(r, 0U, 0x8000U, 0xffffU);
        const auto target = pop_return(host, r);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    r.address[4] = read_private_long(host, r.address[6] + 0x54U) + 6U;
    const auto stream_offset = host.read_memory_word(
        kPrivateRegion, r.address[5] + 0x38U, kWordMask);
    r.address[4] += static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(stream_offset)));
    const auto stream_region = r.address[4] <= kAddressMask ? kPrivateRegion : kSharedRegion;
    const auto marker = host.read_memory_word(
        stream_region, r.address[4] & kAddressMask, kWordMask);
    r.address[4] += 2U;
    host.write_memory_word(kPrivateRegion, r.address[5] + 6U, marker, kWordMask);
    set_logic_flags(r, marker, 0x8000U, 0xffffU);

    if (static_cast<std::int16_t>(marker) >= 0) {
        auto value = read_stream_byte(host, r.address[4]++);
        r.data[0] = (r.data[0] & 0xffffff00U) | value;
        set_logic_flags(r, value, 0x80U, 0xffU);
        auto d0 = static_cast<std::uint16_t>(
            static_cast<std::int16_t>(static_cast<std::int8_t>(value)));
        r.data[0] = (r.data[0] & 0xffff0000U) | d0;
        set_logic_flags(r, d0, 0x8000U, 0xffffU);
        r.data[1] = (r.data[1] & 0xffff0000U) | d0;
        set_logic_flags(r, d0, 0x8000U, 0xffffU);
        auto d1 = arithmetic_shift_right_two(r, d0);
        r.data[1] = (r.data[1] & 0xffff0000U) | d1;
        d0 = static_cast<std::uint16_t>(d0 - d1);
        r.data[0] = (r.data[0] & 0xffff0000U) | d0;
        set_sub_word_flags(r, static_cast<std::uint16_t>(d0 + d1), d1, d0);
        auto addend = host.read_memory_word(kPrivateRegion, r.address[6] + 0x0cU, kWordMask);
        auto result_word = static_cast<std::uint16_t>(d0 + addend);
        r.data[0] = (r.data[0] & 0xffff0000U) | result_word;
        set_add_word_flags(r, d0, addend, result_word);
        host.write_memory_word(kPrivateRegion, r.address[5] + 0x0cU, result_word, kWordMask);
        set_logic_flags(r, result_word, 0x8000U, 0xffffU);

        value = read_stream_byte(host, r.address[4]++);
        r.data[0] = (r.data[0] & 0xffffff00U) | value;
        set_logic_flags(r, value, 0x80U, 0xffU);
        d0 = static_cast<std::uint16_t>(
            static_cast<std::int16_t>(static_cast<std::int8_t>(value)));
        r.data[0] = (r.data[0] & 0xffff0000U) | d0;
        set_logic_flags(r, d0, 0x8000U, 0xffffU);
        r.data[1] = (r.data[1] & 0xffff0000U) | d0;
        set_logic_flags(r, d0, 0x8000U, 0xffffU);
        d1 = arithmetic_shift_right_two(r, d0);
        r.data[1] = (r.data[1] & 0xffff0000U) | d1;
        const auto pre_subtract = d0;
        d0 = static_cast<std::uint16_t>(d0 - d1);
        r.data[0] = (r.data[0] & 0xffff0000U) | d0;
        set_sub_word_flags(r, pre_subtract, d1, d0);
        addend = host.read_memory_word(kPrivateRegion, r.address[6] + 0x0eU, kWordMask);
        result_word = static_cast<std::uint16_t>(d0 + addend);
        r.data[0] = (r.data[0] & 0xffff0000U) | result_word;
        set_add_word_flags(r, d0, addend, result_word);
        host.write_memory_word(kPrivateRegion, r.address[5] + 0x0eU, result_word, kWordMask);
        set_logic_flags(r, result_word, 0x8000U, 0xffffU);

        const auto attribute = host.read_memory_word(
            kPrivateRegion, r.address[6] + 0x6aU, kWordMask);
        r.data[0] = (r.data[0] & 0xffff0000U) | attribute;
        set_logic_flags(r, attribute, 0x8000U, 0xffffU);
        auto low = static_cast<std::uint8_t>(r.data[0]);
        value = read_stream_byte(host, r.address[4]++);
        const auto sum = static_cast<std::uint8_t>(low + value);
        r.data[0] = (r.data[0] & 0xffffff00U) | sum;
        set_add_byte_flags(r, low, value, sum);
        host.write_memory_word(kPrivateRegion, r.address[5] + 8U,
            static_cast<std::uint16_t>(r.data[0]), kWordMask);
        set_logic_flags(r, static_cast<std::uint16_t>(r.data[0]), 0x8000U, 0xffffU);

        value = read_stream_byte(host, r.address[4]++);
        r.data[0] = (r.data[0] & 0xffffff00U) | value;
        set_logic_flags(r, value, 0x80U, 0xffU);
        value = static_cast<std::uint8_t>(value | 0x04U);
        r.data[0] = (r.data[0] & 0xffffff00U) | value;
        set_logic_flags(r, value, 0x80U, 0xffU);
        write_private_byte(host, r.address[5] + 1U, value);
        set_logic_flags(r, value, 0x80U, 0xffU);
        const auto position = host.read_memory_word(
            kPrivateRegion, r.address[6] + 0x1aU, kWordMask);
        host.write_memory_word(kPrivateRegion, r.address[5] + 0x1aU,
            position, kWordMask);
        set_logic_flags(r, position, 0x8000U, 0xffffU);

        push_return(host, r, 0x00013a80U);
        r.program_counter = 0x00015df2U;
        const auto child = host.call_function(282U, 1U, 0x72U, 2U,
            0x00013a7aU, 0x00015df2U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }

    const auto old_state = read_private_byte(host, r.address[5]);
    write_private_byte(host, r.address[5], static_cast<std::uint8_t>(old_state & ~0x01U));
    set_bit_zero(r, (old_state & 0x01U) != 0U);
    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
