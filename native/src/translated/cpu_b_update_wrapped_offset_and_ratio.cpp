#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kX = 0x0010U;
constexpr std::uint16_t kN = 0x0008U;
constexpr std::uint16_t kZ = 0x0004U;
constexpr std::uint16_t kV = 0x0002U;
constexpr std::uint16_t kC = 0x0001U;

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

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{ return host.read_memory_word(kRegion, address, kWordMask); }

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{ host.write_memory_word(kRegion, address, value, kWordMask); }

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void set_logic(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & kX;
    if ((value & sign) != 0U) flags |= kN;
    if ((value & mask) == 0U) flags |= kZ;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_zero_only(CpuRegisters &r, bool zero)
{
    if (zero) r.status = static_cast<std::uint16_t>(r.status | kZ);
    else r.status = static_cast<std::uint16_t>(r.status & ~kZ);
}

void set_add(CpuRegisters &r, std::uint32_t left, std::uint32_t right,
    std::uint32_t result, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags{};
    if ((result & sign) != 0U) flags |= kN;
    if ((result & mask) == 0U) flags |= kZ;
    if (((~(left ^ right)) & (left ^ result) & sign) != 0U) flags |= kV;
    if (left + right > mask) flags |= static_cast<std::uint16_t>(kX | kC);
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub(CpuRegisters &r, std::uint32_t left, std::uint32_t right,
    std::uint32_t result, std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags{};
    if ((result & sign) != 0U) flags |= kN;
    if ((result & mask) == 0U) flags |= kZ;
    if (((left ^ right) & (left ^ result) & sign) != 0U) flags |= kV;
    if ((right & mask) > (left & mask)) flags |= static_cast<std::uint16_t>(kX | kC);
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint8_t add_byte(CpuRegisters &r,
    std::uint8_t left, std::uint8_t right)
{
    const auto result = static_cast<std::uint8_t>(left + right);
    set_add(r, left, right, result, 0x80U, 0xffU);
    return result;
}

[[nodiscard]] std::uint16_t add_word(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left + right);
    set_add(r, left, right, result, 0x8000U, 0xffffU);
    return result;
}

[[nodiscard]] std::uint8_t subtract_byte(CpuRegisters &r,
    std::uint8_t left, std::uint8_t right)
{
    const auto result = static_cast<std::uint8_t>(left - right);
    set_sub(r, left, right, result, 0x80U, 0xffU);
    return result;
}

[[nodiscard]] std::uint32_t subtract_long(CpuRegisters &r,
    std::uint32_t left, std::uint32_t right)
{
    const auto result = left - right;
    set_sub(r, left, right, result, 0x80000000U, 0xffffffffU);
    return result;
}

[[nodiscard]] std::uint32_t negate_long(CpuRegisters &r, std::uint32_t value)
{
    const auto result = 0U - value;
    set_sub(r, 0U, value, result, 0x80000000U, 0xffffffffU);
    return result;
}

[[nodiscard]] std::uint16_t shift_left_word(CpuRegisters &r,
    std::uint16_t value, unsigned count)
{
    const bool carry = (value & (1U << (16U - count))) != 0U;
    const auto result = static_cast<std::uint16_t>(value << count);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= kN;
    if (result == 0U) flags |= kZ;
    if (carry) flags |= static_cast<std::uint16_t>(kX | kC);
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

[[nodiscard]] std::uint32_t shift_right_long(CpuRegisters &r,
    std::uint32_t value, unsigned count)
{
    const bool carry = (value & (1U << (count - 1U))) != 0U;
    const auto result = value >> count;
    std::uint16_t flags{};
    if ((result & 0x80000000U) != 0U) flags |= kN;
    if (result == 0U) flags |= kZ;
    if (carry) flags |= static_cast<std::uint16_t>(kX | kC);
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
    return result;
}

[[nodiscard]] bool bit_test(CpuRegisters &r, std::uint8_t value, unsigned bit)
{
    const bool set = (value & (1U << bit)) != 0U;
    set_zero_only(r, !set);
    return set;
}

void change_memory_bit(ExecutionHost &host, CpuRegisters &r,
    std::uint32_t address, unsigned bit, bool set)
{
    const auto old = read_byte(host, address);
    set_zero_only(r, (old & (1U << bit)) == 0U);
    const auto mask = static_cast<std::uint8_t>(1U << bit);
    write_byte(host, address, set
        ? static_cast<std::uint8_t>(old | mask)
        : static_cast<std::uint8_t>(old & ~mask));
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    write_word(host, r.address[7], static_cast<std::uint16_t>(value >> 16U));
    write_word(host, r.address[7] + 2U, static_cast<std::uint16_t>(value));
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

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    auto &host = *context.host;
    push_return(host, context.registers, continuation);
    context.registers.program_counter = target;
    return host.call_function(id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] bool child_complete(const FunctionResult &result)
{ return result.status == TranslationStatus::complete && result.control == 1U; }

[[nodiscard]] FunctionResult tail_child(FunctionContext &context,
    std::uint32_t id, std::uint8_t kind,
    std::uint32_t callsite, std::uint32_t target)
{
    context.registers.program_counter = target;
    return context.host->call_function(id, 1U, 0x72U, kind,
        callsite, target, context);
}

[[nodiscard]] FunctionResult decrement_and_return(FunctionContext &context)
{
    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    const auto old = read_byte(host, base + 0x58U);
    const auto value = subtract_byte(r, old, 1U);
    write_byte(host, base + 0x58U, value);
    if (value != 0U) return finish(context);
    change_memory_bit(host, r, base + 0x41U, 5U, false);
    return finish(context);
}
} // namespace

FunctionResult cpu_b_update_wrapped_offset_and_ratio_common(
    FunctionContext &context,
    std::uint32_t entry_pc,
    std::uint32_t initial_child_id,
    std::uint32_t initial_callsite,
    std::uint32_t initial_target,
    std::uint32_t initial_continuation,
    std::uint32_t zero_callsite,
    std::uint32_t zero_continuation,
    std::uint32_t gate_callsite,
    std::uint32_t gate_continuation) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    const auto base = r.address[5];
    if (r.program_counter == 0x0001d3b2U)
        return decrement_and_return(context);
    if (r.program_counter != entry_pc)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};

    const auto flags_address = base + 0x41U;
    if (!bit_test(r, read_byte(host, flags_address), 5U)) {
        const auto child = call_child(context, initial_child_id,
            initial_callsite, initial_target, initial_continuation);
        if (!child_complete(child)) return child;
        if (!bit_test(r, read_byte(host, flags_address), 5U))
            return finish(context);
        change_memory_bit(host, r, base + 0x40U, 7U, false);
        write_byte(host, base + 0x59U, 0U);
        set_logic(r, 0U, 0x80U, 0xffU);
        return finish(context);
    }

    const auto countdown = read_byte(host, base + 0x58U);
    set_logic(r, countdown, 0x80U, 0xffU);
    if (countdown != 0U) {
        if (entry_pc == 0x0001d304U)
            return tail_child(context, 548U, 1U,
                0x0001d32eU, 0x0001d3b2U);
        return decrement_and_return(context);
    }

    r.data[0] = 0U;
    set_logic(r, 0U, 0x80000000U, 0xffffffffU);
    const auto selector_source = read_byte(host, base + 0x60U);
    r.data[0] = selector_source;
    set_logic(r, selector_source, 0x80U, 0xffU);
    const auto selector = add_byte(r, selector_source, 9U);
    r.data[0] = selector;
    r.address[4] = 0x00003400U;
    const auto table_value = read_byte(host,
        r.address[4] + static_cast<std::int16_t>(static_cast<std::uint16_t>(r.data[0])));
    set_logic(r, table_value, 0x80U, 0xffU);
    if (table_value == 0U) {
        change_memory_bit(host, r, flags_address, 5U, false);
        return finish(context);
    }

    auto child = call_child(context, 347U,
        zero_callsite, 0x0001da58U, zero_continuation);
    if (!child_complete(child)) return child;
    r.address[0] = read_long(host, base + 0x6eU);
    r.data[0] = 0U;
    set_logic(r, 0U, 0x80000000U, 0xffffffffU);
    const auto step = read_byte(host, r.address[0] + 5U);
    r.data[0] = step;
    set_logic(r, step, 0x80U, 0xffU);
    const auto scaled = shift_left_word(r, static_cast<std::uint16_t>(r.data[0]), 4U);
    r.data[0] = scaled;

    auto mode = read_word(host, base + 0x5cU);
    r.data[1] = (r.data[1] & 0xffff0000U) | mode;
    set_logic(r, mode, 0x8000U, 0xffffU);
    mode = add_word(r, mode, scaled);
    r.data[1] = (r.data[1] & 0xffff0000U) | mode;
    mode = static_cast<std::uint16_t>(mode & 0x07ffU);
    r.data[1] = (r.data[1] & 0xffff0000U) | mode;
    set_logic(r, mode, 0x8000U, 0xffffU);
    write_word(host, base + 0x5cU, mode);
    set_logic(r, mode, 0x8000U, 0xffffU);
    change_memory_bit(host, r, base + 0x40U, 2U, true);
    const auto source = read_byte(host, base + 0x37U);
    write_byte(host, base + 0x36U, source);
    set_logic(r, source, 0x80U, 0xffU);

    child = call_child(context, 352U,
        gate_callsite, 0x0001dbc4U, gate_continuation);
    if (!child_complete(child)) return child;
    r.data[0] = 0U;
    set_logic(r, 0U, 0x80000000U, 0xffffffffU);
    r.address[0] = read_long(host, base + 0x6eU);
    const auto numerator_byte = read_byte(host, r.address[0] + 1U);
    r.data[0] = numerator_byte;
    set_logic(r, numerator_byte, 0x80U, 0xffU);
    r.data[0] = (r.data[0] << 16U) | (r.data[0] >> 16U);
    set_logic(r, r.data[0], 0x80000000U, 0xffffffffU);

    auto selector_word = read_word(host, base + 0x5cU);
    r.data[2] = (r.data[2] & 0xffff0000U) | selector_word;
    set_logic(r, selector_word, 0x8000U, 0xffffU);
    selector_word = add_word(r, selector_word, 0x0100U);
    r.data[2] = (r.data[2] & 0xffff0000U) | selector_word;
    selector_word = static_cast<std::uint16_t>(selector_word & 0x0200U);
    r.data[2] = (r.data[2] & 0xffff0000U) | selector_word;
    set_logic(r, selector_word, 0x8000U, 0xffffU);

    if (selector_word == 0U) {
        r.data[1] = subtract_long(r, r.data[1], read_long(host, base + 0x1eU));
        if (static_cast<std::int32_t>(r.data[1]) < 0)
            r.data[1] = negate_long(r, r.data[1]);
    } else {
        r.data[1] = read_long(host, base + 0x26U);
        set_logic(r, r.data[1], 0x80000000U, 0xffffffffU);
        if (static_cast<std::int32_t>(r.data[1]) < 0)
            r.data[1] = negate_long(r, r.data[1]);
    }

    r.data[0] = shift_right_long(r, r.data[0], 4U);
    r.data[1] = shift_right_long(r, r.data[1], 4U);
    const auto divisor = static_cast<std::uint16_t>(r.data[1]);
    if (divisor == 0U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    const auto dividend = r.data[0];
    const auto quotient = dividend / divisor;
    const auto remainder = dividend % divisor;
    if (quotient > 0xffffU)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    r.data[0] = (remainder << 16U) | quotient;
    set_logic(r, static_cast<std::uint16_t>(quotient), 0x8000U, 0xffffU);
    const auto ratio = static_cast<std::uint8_t>(r.data[0] & 0xffU);
    r.data[0] = (r.data[0] & 0xffffff00U) | ratio;
    set_logic(r, ratio, 0x80U, 0xffU);
    write_byte(host, base + 0x58U, ratio);
    set_logic(r, ratio, 0x80U, 0xffU);
    if (entry_pc == 0x0001d304U)
        return tail_child(context, 548U, 0U,
            0x0001d3aeU, 0x0001d3b2U);
    return decrement_and_return(context);
}

FunctionResult cpu_b_update_wrapped_offset_and_ratio(FunctionContext &context) noexcept
{
    return cpu_b_update_wrapped_offset_and_ratio_common(context,
        0x0001d304U,
        341U, 0x0001d30eU, 0x0001d6d8U, 0x0001d312U,
        0x0001d348U, 0x0001d34cU,
        0x0001d372U, 0x0001d376U);
}

} // namespace gain_ground::translated
