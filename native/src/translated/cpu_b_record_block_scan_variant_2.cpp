#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {address & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
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

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint16_t add_word(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    const auto result = static_cast<std::uint16_t>(wide);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

void set_cmp_word_flags(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    std::uint16_t flags = r.status & 0x0010U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (left < right) flags |= 0x0001U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint16_t asl_word(CpuRegisters &r,
    std::uint16_t value, unsigned count)
{
    bool carry{};
    bool overflow{};
    auto result = value;
    for (unsigned index = 0; index != count; ++index) {
        const bool old_sign = (result & 0x8000U) != 0U;
        carry = old_sign;
        result = static_cast<std::uint16_t>(result << 1U);
        overflow = overflow || (old_sign != ((result & 0x8000U) != 0U));
    }
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

void set_zero_only(CpuRegisters &r, bool zero)
{
    if (zero) r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(
        host.read_memory_word(kRegion, address, kWordMask)) << 16U)
        | host.read_memory_word(kRegion, address + 2U, kWordMask);
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kRegion, r.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto target = read_long(host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_record_block_scan_variant_common(FunctionContext &context,
    bool clear_selector_on_exhaustion, std::uint32_t child_callsite,
    std::uint32_t child_continuation) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    r.address[4] = 0x00003409U;
    r.data[0] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    const auto initial = read_byte(host, base + 0x61U);
    r.data[0] = initial;
    set_logic_flags(r, initial, 0x80U, 0xffU);
    r.data[1] = 2U;
    set_logic_flags(r, 2U, 0x80000000U, 0xffffffffU);

    while (true) {
        const auto selector = static_cast<std::uint16_t>(r.data[0]);
        const auto active = read_byte(host, r.address[4]
            + static_cast<std::int16_t>(selector));
        set_logic_flags(r, active, 0x80U, 0xffU);
        bool rejected = active == 0U;

        if (!rejected) {
            r.address[0] = read_long(host, base + 0x6eU);
            auto table_offset = host.read_memory_word(
                kRegion, r.address[0] + 8U, kWordMask);
            r.data[2] = (r.data[2] & 0xffff0000U) | table_offset;
            set_logic_flags(r, table_offset, 0x8000U, 0xffffU);
            table_offset = add_word(r, table_offset, table_offset);
            r.data[2] = (r.data[2] & 0xffff0000U) | table_offset;
            table_offset = add_word(r, table_offset, table_offset);
            r.data[2] = (r.data[2] & 0xffff0000U) | table_offset;
            r.address[0] = static_cast<std::uint32_t>(0x00032decU
                + static_cast<std::int16_t>(table_offset));

            r.data[3] = (r.data[3] & 0xffff0000U) | selector;
            set_logic_flags(r, selector, 0x8000U, 0xffffU);
            auto record_offset = add_word(r, selector, selector);
            r.data[3] = (r.data[3] & 0xffff0000U) | record_offset;
            record_offset = add_word(r, record_offset, record_offset);
            r.data[3] = (r.data[3] & 0xffff0000U) | record_offset;
            record_offset = add_word(r, record_offset, 0x000cU);
            r.data[3] = (r.data[3] & 0xffff0000U) | record_offset;
            r.address[3] = static_cast<std::uint32_t>(0x00003400U
                + static_cast<std::int16_t>(record_offset));

            const auto x = host.read_memory_word(kRegion, r.address[3], kWordMask);
            r.address[3] += 2U;
            r.data[3] = (r.data[3] & 0xffff0000U) | x;
            set_logic_flags(r, x, 0x8000U, 0xffffU);
            const auto y = host.read_memory_word(kRegion, r.address[3], kWordMask);
            r.address[3] += 2U;
            r.data[4] = (r.data[4] & 0xffff0000U) | y;
            set_logic_flags(r, y, 0x8000U, 0xffffU);

            const auto threshold = [&]() {
                const auto byte = read_byte(host, r.address[0]++);
                r.data[2] = (r.data[2] & 0xffffff00U) | byte;
                set_logic_flags(r, byte, 0x80U, 0xffU);
                const auto extended = static_cast<std::uint16_t>(
                    static_cast<std::int16_t>(static_cast<std::int8_t>(byte)));
                r.data[2] = (r.data[2] & 0xffff0000U) | extended;
                set_logic_flags(r, extended, 0x8000U, 0xffffU);
                const auto scaled = asl_word(r, extended, 3U);
                r.data[2] = (r.data[2] & 0xffff0000U) | scaled;
                return scaled;
            };

            auto bound = threshold();
            set_cmp_word_flags(r, x, bound);
            rejected = static_cast<std::int16_t>(x)
                < static_cast<std::int16_t>(bound);
            if (!rejected) {
                bound = threshold();
                set_cmp_word_flags(r, x, bound);
                rejected = static_cast<std::int16_t>(x)
                    >= static_cast<std::int16_t>(bound);
            }
            if (!rejected) {
                bound = threshold();
                set_cmp_word_flags(r, y, bound);
                rejected = static_cast<std::int16_t>(y)
                    < static_cast<std::int16_t>(bound);
            }
            if (!rejected) {
                bound = threshold();
                set_cmp_word_flags(r, y, bound);
                rejected = static_cast<std::int16_t>(y)
                    >= static_cast<std::int16_t>(bound);
            }
        }

        if (!rejected) {
            write_byte(host, base + 0x60U,
                static_cast<std::uint8_t>(r.data[0]));
            set_logic_flags(r, r.data[0] & 0xffU, 0x80U, 0xffU);
            push_return(host, r, child_continuation);
            r.program_counter = 0x0001da58U;
            const auto child = host.call_function(347U, 1U, 0x72U, 2U,
                child_callsite, 0x0001da58U, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
            const auto flag_address = base + 0x41U;
            const auto flags = read_byte(host, flag_address);
            write_byte(host, flag_address, static_cast<std::uint8_t>(flags | 0x20U));
            set_zero_only(r, (flags & 0x20U) == 0U);
            return finish(context);
        }

        auto next = add_word(r, static_cast<std::uint16_t>(r.data[0]), 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | next;
        set_cmp_word_flags(r, next, 4U);
        if (static_cast<std::int16_t>(next) >= 4) {
            next = add_word(r, next, 1U);
            r.data[0] = (r.data[0] & 0xffff0000U) | next;
        }
        next = static_cast<std::uint16_t>(next & 3U);
        r.data[0] = (r.data[0] & 0xffff0000U) | next;
        set_logic_flags(r, next, 0x8000U, 0xffffU);

        const auto counter = static_cast<std::uint16_t>(r.data[1]);
        r.data[1] = (r.data[1] & 0xffff0000U)
            | static_cast<std::uint16_t>(counter - 1U);
        if (counter == 0U) {
            if (clear_selector_on_exhaustion)
                write_byte(host, base + 0x60U, 0U);
            const auto flag_address = base + 0x41U;
            const auto flags = read_byte(host, flag_address);
            write_byte(host, flag_address, static_cast<std::uint8_t>(flags & ~0x20U));
            set_zero_only(r, (flags & 0x20U) == 0U);
            return finish(context);
        }
    }
}

FunctionResult cpu_b_record_block_scan_variant_2(FunctionContext &context) noexcept
{
    return cpu_b_record_block_scan_variant_common(
        context, true, 0x0001d8f6U, 0x0001d8faU);
}

} // namespace gain_ground::translated
