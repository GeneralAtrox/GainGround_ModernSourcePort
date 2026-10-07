#include "cpu_b_dispatch_state_step_detail.h"

namespace gain_ground::translated {
namespace cpu_b_dispatch_state_step_detail {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint16_t kX = 0x0010U;
constexpr std::uint16_t kN = 0x0008U;
constexpr std::uint16_t kZ = 0x0004U;
constexpr std::uint16_t kV = 0x0002U;
constexpr std::uint16_t kC = 0x0001U;

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
{
    return host.read_memory_word(kRegion, address, kWordMask);
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t value)
{
    host.write_memory_word(kRegion, address, value, kWordMask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    return (static_cast<std::uint32_t>(read_word(host, address)) << 16U)
        | read_word(host, address + 2U);
}

void push_return(ExecutionHost &host, CpuRegisters &registers, std::uint32_t value)
{
    registers.address[7] -= 4U;
    write_word(host, registers.address[7], static_cast<std::uint16_t>(value >> 16U));
    write_word(host, registers.address[7] + 2U, static_cast<std::uint16_t>(value));
}

void set_logic_flags(CpuRegisters &registers, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = registers.status & kX;
    if ((value & sign) != 0U) flags |= kN;
    if ((value & mask) == 0U) flags |= kZ;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_bit_test_zero(CpuRegisters &registers, bool bit_set)
{
    if (bit_set) registers.status = static_cast<std::uint16_t>(registers.status & ~kZ);
    else registers.status = static_cast<std::uint16_t>(registers.status | kZ);
}

void set_add_flags(CpuRegisters &registers, std::uint32_t left,
    std::uint32_t right, std::uint32_t result, std::uint32_t sign,
    std::uint32_t mask)
{
    const auto wide = left + right;
    std::uint16_t flags{};
    if ((result & sign) != 0U) flags |= kN;
    if ((result & mask) == 0U) flags |= kZ;
    if (((~(left ^ right)) & (left ^ result) & sign) != 0U) flags |= kV;
    if (wide > mask) flags |= static_cast<std::uint16_t>(kX | kC);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

void set_sub_flags(CpuRegisters &registers, std::uint16_t left,
    std::uint16_t right, std::uint16_t result, bool update_extend)
{
    std::uint16_t flags = update_extend ? 0U : registers.status & kX;
    if ((result & 0x8000U) != 0U) flags |= kN;
    if (result == 0U) flags |= kZ;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= kV;
    if (right > left) flags |= kC;
    if (update_extend && right > left) flags |= kX;
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint8_t add_byte(CpuRegisters &registers,
    std::uint8_t left, std::uint8_t right)
{
    const auto result = static_cast<std::uint8_t>(left + right);
    set_add_flags(registers, left, right, result, 0x80U, 0xffU);
    return result;
}

[[nodiscard]] std::uint16_t add_word(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left + right);
    set_add_flags(registers, left, right, result, 0x8000U, 0xffffU);
    return result;
}

[[nodiscard]] std::uint16_t sub_word(CpuRegisters &registers,
    std::uint16_t left, std::uint16_t right)
{
    const auto result = static_cast<std::uint16_t>(left - right);
    set_sub_flags(registers, left, right, result, true);
    return result;
}

void compare_word(CpuRegisters &registers, std::uint16_t left, std::uint16_t right)
{
    set_sub_flags(registers, left, right,
        static_cast<std::uint16_t>(left - right), false);
}

[[nodiscard]] std::uint16_t shift_left_three(CpuRegisters &registers,
    std::uint16_t value)
{
    const bool carry = (value & 0x2000U) != 0U;
    const auto result = static_cast<std::uint16_t>(value << 3U);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= kN;
    if (result == 0U) flags |= kZ;
    if (carry) flags |= static_cast<std::uint16_t>(kX | kC);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return result;
}

[[nodiscard]] std::uint16_t shift_right_seven(CpuRegisters &registers,
    std::uint16_t value)
{
    const bool carry = (value & 0x0040U) != 0U;
    const auto result = static_cast<std::uint16_t>(value >> 7U);
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= kN;
    if (result == 0U) flags |= kZ;
    if (carry) flags |= static_cast<std::uint16_t>(kX | kC);
    registers.status = static_cast<std::uint16_t>(
        (registers.status & ~0x001fU) | flags);
    return result;
}

[[nodiscard]] bool bit_test(CpuRegisters &registers,
    std::uint8_t value, unsigned bit)
{
    const bool set = (value & (1U << bit)) != 0U;
    set_bit_test_zero(registers, set);
    return set;
}

void set_memory_bit(ExecutionHost &host, CpuRegisters &registers,
    std::uint32_t address, unsigned bit, bool set)
{
    const auto old = read_byte(host, address);
    set_bit_test_zero(registers, (old & (1U << bit)) != 0U);
    const auto mask = static_cast<std::uint8_t>(1U << bit);
    write_byte(host, address, set
        ? static_cast<std::uint8_t>(old | mask)
        : static_cast<std::uint8_t>(old & ~mask));
}

[[nodiscard]] FunctionResult finish(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto target = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
    registers.program_counter = target;
    return FunctionResult::complete(1U, target);
}

[[nodiscard]] FunctionResult call_child(FunctionContext &context,
    std::uint32_t function_id, std::uint32_t callsite,
    std::uint32_t target, std::uint32_t continuation)
{
    auto &host = *context.host;
    push_return(host, context.registers, continuation);
    context.registers.program_counter = target;
    return host.call_function(function_id, 1U, 0x72U, 2U,
        callsite, target, context);
}

[[nodiscard]] bool child_complete(const FunctionResult &result)
{
    return result.status == TranslationStatus::complete && result.control == 1U;
}

void return_from_embedded_helper(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    registers.program_counter = read_long(host, registers.address[7]);
    registers.address[7] += 4U;
}

void run_embedded_helper(FunctionContext &context)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];
    const auto flags_address = base + 0x41U;

    if (!bit_test(registers, read_byte(host, flags_address), 6U)) {
        registers.address[0] = read_long(host, base + 0x66U);
        registers.address[0] = read_long(host, registers.address[0]);
        registers.address[0] += 1U;

        auto selector = read_word(host, base + 0x5cU);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | selector;
        set_logic_flags(registers, selector, 0x8000U, 0xffffU);
        selector = static_cast<std::uint16_t>(selector & 0x0600U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | selector;
        set_logic_flags(registers, selector, 0x8000U, 0xffffU);
        selector = shift_right_seven(registers, selector);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | selector;

        std::uint32_t source_offset{};
        std::uint32_t coordinate_offset{};
        bool accept_less_equal{};
        switch (selector) {
        case 0U:
            source_offset = 1U;
            coordinate_offset = 0x12U;
            accept_less_equal = true;
            break;
        case 4U:
            source_offset = 3U;
            coordinate_offset = 0x1aU;
            accept_less_equal = true;
            break;
        case 8U:
            source_offset = 0U;
            coordinate_offset = 0x12U;
            accept_less_equal = false;
            break;
        default:
            source_offset = 2U;
            coordinate_offset = 0x1aU;
            accept_less_equal = false;
            break;
        }

        registers.data[0] = 0U;
        set_logic_flags(registers, 0U, 0x80000000U, 0xffffffffU);
        const auto source = read_byte(host, registers.address[0] + source_offset);
        registers.data[0] = source;
        set_logic_flags(registers, source, 0x80U, 0xffU);
        const auto shifted = shift_left_three(registers,
            static_cast<std::uint16_t>(registers.data[0]));
        registers.data[0] = shifted;
        const auto coordinate = read_word(host, base + coordinate_offset);
        compare_word(registers, shifted, coordinate);
        const auto signed_shifted = static_cast<std::int16_t>(shifted);
        const auto signed_coordinate = static_cast<std::int16_t>(coordinate);
        const bool accepted = accept_less_equal
            ? signed_shifted <= signed_coordinate
            : signed_shifted >= signed_coordinate;
        if (!accepted) {
            return_from_embedded_helper(context);
            return;
        }

        set_memory_bit(host, registers, flags_address, 6U, true);
        write_byte(host, base + 0x60U, 0U);
        set_logic_flags(registers, 0U, 0x80U, 0xffU);
        auto mode = read_word(host, base + 0x5cU);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | mode;
        set_logic_flags(registers, mode, 0x8000U, 0xffffU);
        mode = add_word(registers, mode, 0x0400U);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | mode;
        mode = static_cast<std::uint16_t>(mode & 0x07ffU);
        registers.data[0] = (registers.data[0] & 0xffff0000U) | mode;
        set_logic_flags(registers, mode, 0x8000U, 0xffffU);
        write_word(host, base + 0x5cU, mode);
        set_logic_flags(registers, mode, 0x8000U, 0xffffU);
        return_from_embedded_helper(context);
        return;
    }

    auto value = read_word(host, base + 0x12U);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    set_logic_flags(registers, value, 0x8000U, 0xffffU);
    value = sub_word(registers, value, read_word(host, base + 0x62U));
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    value = add_word(registers, value, 0x000aU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    compare_word(registers, value, 0x0014U);
    if (static_cast<std::int16_t>(value) > 0x14) {
        return_from_embedded_helper(context);
        return;
    }

    value = read_word(host, base + 0x1aU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    set_logic_flags(registers, value, 0x8000U, 0xffffU);
    value = sub_word(registers, value, read_word(host, base + 0x64U));
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    value = add_word(registers, value, 0x000aU);
    registers.data[0] = (registers.data[0] & 0xffff0000U) | value;
    compare_word(registers, value, 0x0014U);
    if (static_cast<std::int16_t>(value) > 0x14) {
        return_from_embedded_helper(context);
        return;
    }

    set_memory_bit(host, registers, flags_address, 5U, false);
    set_memory_bit(host, registers, flags_address, 6U, false);
    return_from_embedded_helper(context);
}

[[nodiscard]] FunctionResult run_tail(FunctionContext &context,
    bool clear_accumulator_first)
{
    auto &host = *context.host;
    auto &registers = context.registers;
    const auto base = registers.address[5];
    if (clear_accumulator_first) {
        const auto child = call_child(context, 347U,
            0x0001d5caU, 0x0001da58U, 0x0001d5ceU);
        if (!child_complete(child)) return child;
    }

    registers.address[0] = read_long(host, base + 0x6eU);
    const auto adjustment = read_byte(host, registers.address[0] + 3U);
    registers.data[0] = (registers.data[0] & 0xffffff00U) | adjustment;
    set_logic_flags(registers, adjustment, 0x80U, 0xffU);
    const auto source = read_byte(host, base + 0x37U);
    write_byte(host, base + 0x36U, source);
    set_logic_flags(registers, source, 0x80U, 0xffU);
    const auto sum = add_byte(registers, read_byte(host, base + 0x36U), adjustment);
    write_byte(host, base + 0x36U, sum);

    const auto decode_callsite = clear_accumulator_first ? 0x0001d5e0U : 0x0001d60aU;
    const auto decode_continuation = clear_accumulator_first ? 0x0001d5e4U : 0x0001d60eU;
    const auto child = call_child(context, 364U,
        decode_callsite, 0x0001ec00U, decode_continuation);
    if (!child_complete(child)) return child;
    const auto decoded = static_cast<std::uint16_t>(registers.data[1]);
    write_word(host, base + 0x5cU, decoded);
    set_logic_flags(registers, decoded, 0x8000U, 0xffffU);
    return finish(context);
}
} // namespace cpu_b_dispatch_state_step_detail

} // namespace gain_ground::translated
