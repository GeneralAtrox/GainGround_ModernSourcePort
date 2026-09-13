#include "gain_ground/contract_types.h"
#include "gain_ground/cpu_b_interrupt.h"

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

void set_sub_word_flags(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (left < right) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
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

[[nodiscard]] std::uint16_t sub_word(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    set_sub_word_flags(r, left, right, result);
    return result;
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

[[nodiscard]] std::uint16_t negate_word(CpuRegisters &r, std::uint16_t value)
{
    return sub_word(r, 0U, value);
}

void set_zero_only(CpuRegisters &r, bool zero)
{
    if (zero) r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
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
    const auto high = host.read_memory_word(kRegion, r.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_record_block_scan(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    if (r.program_counter == 0x1d73eU) return finish(context);
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
        const auto index = static_cast<std::uint16_t>(r.data[0]);
        const auto active = read_byte(host, r.address[4]
            + static_cast<std::int16_t>(index));
        set_logic_flags(r, active, 0x80U, 0xffU);
        bool rejected = active == 0U;

        if (!rejected) {
            r.data[2] = 0U;
            set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
            r.address[0] = (static_cast<std::uint32_t>(
                host.read_memory_word(kRegion, base + 0x6eU, kWordMask)) << 16U)
                | host.read_memory_word(kRegion, base + 0x70U, kWordMask);
            const auto threshold = read_byte(host, r.address[0]);
            r.data[2] = threshold;
            set_logic_flags(r, threshold, 0x80U, 0xffU);

            r.data[3] = (r.data[3] & 0xffff0000U) | index;
            set_logic_flags(r, index, 0x8000U, 0xffffU);
            auto offset = add_word(r, index, index);
            r.data[3] = (r.data[3] & 0xffff0000U) | offset;
            offset = add_word(r, offset, offset);
            r.data[3] = (r.data[3] & 0xffff0000U) | offset;
            offset = add_word(r, offset, 0x000cU);
            r.data[3] = (r.data[3] & 0xffff0000U) | offset;
            r.address[3] = static_cast<std::uint32_t>(0x00003400U
                + static_cast<std::int16_t>(offset));

            auto x = host.read_memory_word(kRegion, r.address[3], kWordMask);
            r.address[3] += 2U;
            r.data[3] = (r.data[3] & 0xffff0000U) | x;
            set_logic_flags(r, x, 0x8000U, 0xffffU);
            x = sub_word(r, x,
                host.read_memory_word(kRegion, base + 0x12U, kWordMask));
            r.data[3] = (r.data[3] & 0xffff0000U) | x;
            if ((x & 0x8000U) != 0U) {
                x = negate_word(r, x);
                r.data[3] = (r.data[3] & 0xffff0000U) | x;
            }

            auto y = host.read_memory_word(kRegion, r.address[3], kWordMask);
            r.address[3] += 2U;
            r.data[4] = (r.data[4] & 0xffff0000U) | y;
            set_logic_flags(r, y, 0x8000U, 0xffffU);
            y = sub_word(r, y,
                host.read_memory_word(kRegion, base + 0x1aU, kWordMask));
            r.data[4] = (r.data[4] & 0xffff0000U) | y;
            if ((y & 0x8000U) != 0U) {
                y = negate_word(r, y);
                r.data[4] = (r.data[4] & 0xffff0000U) | y;
            }

            set_cmp_word_flags(r, x, threshold);
            rejected = static_cast<std::int16_t>(x)
                > static_cast<std::int16_t>(threshold);
            if (!rejected) {
                set_cmp_word_flags(r, y, threshold);
                rejected = static_cast<std::int16_t>(y)
                    > static_cast<std::int16_t>(threshold);
            }
        }

        if (!rejected) {
            write_byte(host, base + 0x60U,
                static_cast<std::uint8_t>(r.data[0]));
            set_logic_flags(r, r.data[0] & 0xffU, 0x80U, 0xffU);
            push_return(host, r, 0x0001d748U);
            r.program_counter = 0x0001da58U;
            const auto child = host.call_function(347U, 1U, 0x72U, 2U,
                0x0001d744U, 0x0001da58U, context);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
            r.address[0] = (static_cast<std::uint32_t>(
                host.read_memory_word(kRegion, base + 0x6eU, kWordMask)) << 16U)
                | host.read_memory_word(kRegion, base + 0x70U, kWordMask);
            const auto mode = read_byte(host, r.address[0] + 2U);
            write_byte(host, base + 0x59U, mode);
            set_logic_flags(r, mode, 0x80U, 0xffU);
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
            const auto flag_address = base + 0x41U;
            const auto flags = read_byte(host, flag_address);
            write_byte(host, flag_address, static_cast<std::uint8_t>(flags & ~0x20U));
            set_zero_only(r, (flags & 0x20U) == 0U);
            if (host.resumes_interrupts_inline()) {
                if (const auto result = cpu_b_interrupt_boundary(context, 0x1d738U, 0x1d73eU))
                    return *result;
                return finish(context);
            }
            const auto interrupt = host.consume_pending_interrupt(1U, 0x72U, 0x1d738U);
            if (interrupt.asserted && interrupt.level > ((r.status >> 8U) & 7U)) {
                const auto saved = r.status;
                r.address[7] -= 4U;
                host.write_memory_word(kRegion, r.address[7] + 2U, 0xd73eU, kWordMask);
                r.address[7] -= 2U;
                host.write_memory_word(kRegion, r.address[7], saved, kWordMask);
                host.write_memory_word(kRegion, r.address[7] + 2U, 1U, kWordMask);
                r.status = static_cast<std::uint16_t>((saved & 0x38ffU) | 0x2000U | (interrupt.level << 8U));
                const auto vector = (24U + interrupt.level) * 4U;
                const auto high = host.read_memory_word(kRegion, vector, kWordMask);
                const auto low = host.read_memory_word(kRegion, vector + 2U, kWordMask);
                const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
                r.program_counter = target; context.state = 0x04U;
                (void)host.call_function(96U + interrupt.level, 1U, 0x04U, 6U, 0x1d738U, target, context);
                return FunctionResult::complete(5U, target);
            }
            return finish(context);
        }
    }
}

} // namespace gain_ground::translated
