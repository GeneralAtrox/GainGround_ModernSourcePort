#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kAddressMask = 0x0003ffffU;

struct ByteLocation { std::uint32_t offset; std::uint16_t mask; unsigned shift; };
struct ChildSpec { std::uint32_t id, callsite, target, continuation; };

void set_logic(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask);

[[nodiscard]] constexpr ByteLocation locate_byte(std::uint32_t address) noexcept
{
    const bool odd = (address & 1U) != 0U;
    return {(address & kAddressMask) & ~1U,
        static_cast<std::uint16_t>(odd ? 0x00ffU : 0xff00U), odd ? 0U : 8U};
}

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address & kAddressMask, kWordMask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const auto location = locate_byte(address);
    return static_cast<std::uint8_t>(host.read_memory_word(
        kRegion, location.offset, location.mask) >> location.shift);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address & kAddressMask, value, kWordMask);
}

void write_long(ExecutionHost &host, std::uint32_t address, std::uint32_t value)
{
    write_word(host, address, static_cast<std::uint16_t>(value >> 16U));
    write_word(host, address + 2U, static_cast<std::uint16_t>(value));
}

void write_byte(ExecutionHost &host, std::uint32_t address, std::uint8_t value)
{
    const auto location = locate_byte(address);
    host.write_memory_word(kRegion, location.offset,
        static_cast<std::uint16_t>(value) << location.shift, location.mask);
}

void clear_word(ExecutionHost &host, CpuRegisters &r, std::uint32_t address)
{
    (void)read_word(host, address);
    write_word(host, address, 0U);
    set_logic(r, 0U, 0x8000U, 0xffffU);
}

void clear_long(ExecutionHost &host, CpuRegisters &r, std::uint32_t address)
{
    (void)read_word(host, address);
    (void)read_word(host, address + 2U);
    write_word(host, address + 2U, 0U);
    write_word(host, address, 0U);
    set_logic(r, 0U, 0x80000000U, 0xffffffffU);
}

void set_logic(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_compare_word(CpuRegisters &r, std::uint16_t destination,
    std::uint16_t source)
{
    const auto result = static_cast<std::uint16_t>(destination - source);
    std::uint16_t flags = r.status & 0x0010U;
    if (destination < source) flags |= 0x0001U;
    if (((destination ^ source) & (destination ^ result) & 0x8000U) != 0U)
        flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_subtract(CpuRegisters &r, std::uint32_t destination,
    std::uint32_t source, std::uint32_t result, std::uint32_t sign,
    std::uint32_t mask)
{
    std::uint16_t flags{};
    if ((destination & mask) < (source & mask)) flags |= 0x0011U;
    if ((((destination ^ source) & (destination ^ result)) & sign) != 0U)
        flags |= 0x0002U;
    if ((result & mask) == 0U) flags |= 0x0004U;
    if ((result & sign) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add(CpuRegisters &r, std::uint32_t destination,
    std::uint32_t source, std::uint32_t result, std::uint32_t sign,
    std::uint32_t mask)
{
    const auto left = static_cast<std::uint64_t>(destination & mask);
    const auto right = static_cast<std::uint64_t>(source & mask);
    std::uint16_t flags{};
    if (left + right > mask) flags |= 0x0011U;
    if ((((~(destination ^ source)) & (destination ^ result)) & sign) != 0U)
        flags |= 0x0002U;
    if ((result & mask) == 0U) flags |= 0x0004U;
    if ((result & sign) != 0U) flags |= 0x0008U;
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
        overflow |= old_sign != ((result & 0x8000U) != 0U);
    }
    std::uint16_t flags{};
    if (carry) flags |= 0x0011U;
    if (overflow) flags |= 0x0002U;
    if (result == 0U) flags |= 0x0004U;
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

void set_btst(CpuRegisters &r, bool set)
{
    r.status = static_cast<std::uint16_t>(
        (r.status & ~0x0004U) | (set ? 0U : 0x0004U));
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_long(host, r.address[7], value);
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    auto &r = context.registers;
    push_return(*context.host, r, continuation);
    r.program_counter = target;
    return context.host->call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &r = context.registers;
    const auto target = read_long(*context.host, r.address[7]);
    r.address[7] += 4U;
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}
} // namespace

FunctionResult cpu_b_dispatch_descriptor_state(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;
    if (r.program_counter != 0x0001375aU)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};

    const auto base = r.address[5];
    r.address[6] = static_cast<std::uint32_t>(static_cast<std::int32_t>(
        static_cast<std::int16_t>(read_word(host, base + 0x60U))));
    auto d0w = read_word(host, base + 0x42U);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0w;
    set_logic(r, d0w, 0x8000U, 0xffffU);
    d0w = asl_word(r, d0w, 2U);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0w;

    const auto selector = static_cast<unsigned>(d0w >> 2U);
    if (selector == 0U) {
        const auto mode = read_word(host, base + 0x44U);
        set_compare_word(r, mode, 1U);
        if (mode == 1U) {
            write_word(host, base + 0x42U, 1U);
            set_logic(r, 1U, 0x8000U, 0xffffU);
            const auto value = read_word(host, r.address[6] + 0x66U);
            write_word(host, base + 0x6aU, value);
            set_logic(r, value, 0x8000U, 0xffffU);
            goto case_one;
        }
        clear_word(host, r, base + 0x44U);
        write_word(host, base + 0x58U, 2U);
        set_logic(r, 2U, 0x8000U, 0xffffU);
        {
            const auto child = call_child(context, 272U,
                0x0001379aU, 0x00013962U, 0x0001379eU);
            if (child.status != TranslationStatus::complete || child.control != 1U)
                return child;
        }
        goto periodic;
    }
    if (selector == 1U) goto case_one;
    if (selector == 2U) goto case_two;
    if (selector == 3U) goto case_three;
    return {TranslationStatus::contract_violation, 0U, 0x00013764U};

case_one:
    d0w = read_word(host, base + 0x44U);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0w;
    set_logic(r, d0w, 0x8000U, 0xffffU);
    set_btst(r, (r.data[0] & 0x00000002U) != 0U);
    if ((r.data[0] & 0x00000002U) != 0U) {
        write_word(host, base + 0x42U, d0w);
        set_logic(r, d0w, 0x8000U, 0xffffU);
        const auto result = static_cast<std::uint16_t>(d0w - 2U);
        r.data[0] = (r.data[0] & 0xffff0000U) | result;
        set_subtract(r, d0w, 2U, result, 0x8000U, 0xffffU);
        if (result == 0U) goto case_two;
        goto case_three;
    }
    d0w = read_word(host, r.address[6] + 0x58U);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0w;
    set_logic(r, d0w, 0x8000U, 0xffffU);
    d0w = asl_word(r, d0w, 3U);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0w;
    r.address[0] = 0x00013dccU;
    r.address[0] = static_cast<std::uint32_t>(r.address[0]
        + static_cast<std::int16_t>(d0w));
    {
        const auto left = read_long(host, r.address[6] + 0x12U);
        r.data[0] = left;
        set_logic(r, left, 0x80000000U, 0xffffffffU);
        const auto right = read_long(host, r.address[0]);
        r.address[0] += 4U;
        const auto result = left + right;
        r.data[0] = result;
        set_add(r, left, right, result, 0x80000000U, 0xffffffffU);
        write_long(host, base + 0x62U, result);
        set_logic(r, result, 0x80000000U, 0xffffffffU);
    }
    {
        const auto left = read_long(host, r.address[6] + 0x1aU);
        r.data[0] = left;
        set_logic(r, left, 0x80000000U, 0xffffffffU);
        const auto right = read_long(host, r.address[0]);
        r.address[0] += 4U;
        const auto result = left + right;
        r.data[0] = result;
        set_add(r, left, right, result, 0x80000000U, 0xffffffffU);
        write_long(host, base + 0x66U, result);
        set_logic(r, result, 0x80000000U, 0xffffffffU);
    }
    {
        const auto child = call_child(context, 271U,
            0x000137daU, 0x000138eeU, 0x000137deU);
        if (child.status != TranslationStatus::complete)
            return child;
    }
    d0w = read_word(host, base + 0x58U);
    set_logic(r, d0w, 0x8000U, 0xffffU);
    if (static_cast<std::int16_t>(d0w) < 0) {
        const auto value = read_word(host, r.address[6] + 0x58U);
        write_word(host, base + 0x58U, value);
        set_logic(r, value, 0x8000U, 0xffffU);
    }
    goto children;

case_two:
    d0w = read_word(host, base + 0x44U);
    set_compare_word(r, d0w, 1U);
    if (d0w == 1U) {
        write_word(host, base + 0x42U, 1U);
        set_logic(r, 1U, 0x8000U, 0xffffU);
        const auto value = read_word(host, r.address[6] + 0x66U);
        write_word(host, base + 0x6aU, value);
        set_logic(r, value, 0x8000U, 0xffffU);
        goto case_one;
    }
    {
        const auto child = call_child(context, 271U,
            0x00013808U, 0x000138eeU, 0x0001380cU);
        if (child.status != TranslationStatus::complete)
            return child;
    }
    d0w = read_word(host, base + 0x58U);
    set_logic(r, d0w, 0x8000U, 0xffffU);
    if (static_cast<std::int16_t>(d0w) < 0) {
        clear_word(host, r, base + 0x42U);
        write_word(host, base + 0x58U, 2U);
        set_logic(r, 2U, 0x8000U, 0xffffU);
    }
    goto periodic;

case_three:
    {
        const auto child = call_child(context, 271U,
            0x00013822U, 0x000138eeU, 0x00013826U);
        if (child.status != TranslationStatus::complete)
            return child;
    }
    d0w = read_word(host, base + 0x58U);
    set_logic(r, d0w, 0x8000U, 0xffffU);
    if (static_cast<std::int16_t>(d0w) < 0) {
        clear_word(host, r, base);
        clear_long(host, r, base + 0x42U);
        write_word(host, base + 0x52U, 0xffffU);
        set_logic(r, 0xffffU, 0x8000U, 0xffffU);
        return finish(context);
    }
    goto children;

periodic:
    {
        const auto location = locate_byte(base + 0x3eU);
        const auto old_value = read_byte(host, base + 0x3eU);
        const auto value = static_cast<std::uint8_t>(old_value - 1U);
        host.write_memory_word(kRegion, location.offset,
            static_cast<std::uint16_t>(value) << location.shift, location.mask);
        set_subtract(r, old_value, 1U, value, 0x80U, 0xffU);
        const bool greater_than_zero = (r.status & 0x0004U) == 0U
            && (((r.status & 0x0008U) != 0U) == ((r.status & 0x0002U) != 0U));
        if (!greater_than_zero) {
            write_byte(host, base + 0x3eU, 5U);
            set_logic(r, 5U, 0x80U, 0xffU);
            const auto old_word = read_word(host, base + 0x6aU);
            const auto value_word = static_cast<std::uint16_t>(old_word + 0x0100U);
            write_word(host, base + 0x6aU, value_word);
            set_add(r, old_word, 0x0100U, value_word, 0x8000U, 0xffffU);
            const auto compared_word = read_word(host, base + 0x6aU);
            set_compare_word(r, compared_word, 0x0b00U);
            const bool less_or_equal = (r.status & 0x0004U) != 0U
                || (((r.status & 0x0008U) != 0U) != ((r.status & 0x0002U) != 0U));
            if (!less_or_equal) {
                write_word(host, base + 0x6aU, 0x0900U);
                set_logic(r, 0x0900U, 0x8000U, 0xffffU);
            }
        }
    }

children:
    for (const auto child_spec : {
            ChildSpec{273U, 0x0001385cU, 0x00013982U, 0x00013860U},
            ChildSpec{274U, 0x00013860U, 0x00013998U, 0x00013864U},
            ChildSpec{275U, 0x00013864U, 0x000139eaU, 0x00013868U}}) {
        const auto child = call_child(context, child_spec.id, child_spec.callsite,
            child_spec.target, child_spec.continuation);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
    }
    return finish(context);
}

} // namespace gain_ground::translated
