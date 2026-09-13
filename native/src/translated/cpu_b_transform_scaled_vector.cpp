#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_flags(CpuRegisters &r, std::uint32_t value,
    std::uint32_t sign, std::uint32_t mask)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if ((value & mask) == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void add_long(CpuRegisters &r, std::uint32_t &left, std::uint32_t right)
{
    const auto original = left;
    const auto result = original + right;
    std::uint16_t flags{};
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(original ^ right)) & (original ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (result < original) flags |= 0x0011U;
    left = result;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void subtract_long(CpuRegisters &r, std::uint32_t &left, std::uint32_t right)
{
    const auto original = left;
    const auto result = original - right;
    std::uint16_t flags{};
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((original ^ right) & (original ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (original < right) flags |= 0x0011U;
    left = result;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void arithmetic_shift_right(CpuRegisters &r, std::uint32_t &value,
    unsigned count)
{
    const bool carry = ((value >> (count - 1U)) & 1U) != 0U;
    value = static_cast<std::uint32_t>(static_cast<std::int32_t>(value) >> count);
    std::uint16_t flags{};
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    if (carry) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

[[nodiscard]] std::uint8_t read_byte(ExecutionHost &host, std::uint32_t address)
{
    const bool odd = (address & 1U) != 0U;
    const auto word = host.read_memory_word(
        kRegion, address & ~1U, odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? word : word >> 8U);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto high = host.read_memory_word(kRegion, r.address[7], kWordMask);
    const auto low = host.read_memory_word(kRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void push_return(ExecutionHost &host, CpuRegisters &r, std::uint32_t value)
{
    r.address[7] -= 4U;
    host.write_memory_word(kRegion, r.address[7],
        static_cast<std::uint16_t>(value >> 16U), kWordMask);
    host.write_memory_word(kRegion, r.address[7] + 2U,
        static_cast<std::uint16_t>(value), kWordMask);
}

[[nodiscard]] bool a5_at_least_5400(const CpuRegisters &r)
{
    return static_cast<std::int32_t>(r.address[5]) >= 0x5400;
}

void copy_pair(CpuRegisters &r)
{
    r.data[2] = r.data[0];
    set_logic_flags(r, r.data[2], 0x80000000U, 0xffffffffU);
    r.data[3] = r.data[1];
    set_logic_flags(r, r.data[3], 0x80000000U, 0xffffffffU);
}

void shift_pair(CpuRegisters &r, unsigned count)
{
    arithmetic_shift_right(r, r.data[2], count);
    arithmetic_shift_right(r, r.data[3], count);
}

void add_pair(CpuRegisters &r)
{
    add_long(r, r.data[0], r.data[2]);
    add_long(r, r.data[1], r.data[3]);
}

void subtract_pair(CpuRegisters &r)
{
    subtract_long(r, r.data[0], r.data[2]);
    subtract_long(r, r.data[1], r.data[3]);
}

void compare_a5_5400(CpuRegisters &r)
{
    const auto left = r.address[5];
    constexpr std::uint32_t right = 0x00005400U;
    const auto result = left - right;
    std::uint16_t flags = r.status & 0x0010U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if (left < right) flags |= 0x0001U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}
} // namespace

FunctionResult cpu_b_transform_scaled_vector(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;

    r.data[0] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
    set_logic_flags(r, r.data[0], 0x80000000U, 0xffffffffU);
    arithmetic_shift_right(r, r.data[0], 3U);
    r.data[1] = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[1])));
    set_logic_flags(r, r.data[1], 0x80000000U, 0xffffffffU);
    arithmetic_shift_right(r, r.data[1], 3U);

    r.data[2] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    r.data[3] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    r.data[5] = 7U;
    set_logic_flags(r, 7U, 0x80000000U, 0xffffffffU);

    for (unsigned bit = 0U; bit < 8U; ++bit) {
        add_long(r, r.data[0], r.data[0]);
        add_long(r, r.data[1], r.data[1]);
        const auto byte = static_cast<std::uint8_t>(r.data[4]);
        const bool carry = (byte & 1U) != 0U;
        const auto shifted = static_cast<std::uint8_t>(byte >> 1U);
        r.data[4] = (r.data[4] & 0xffffff00U) | shifted;
        std::uint16_t flags{};
        if (shifted == 0U) flags |= 0x0004U;
        if (carry) flags |= 0x0011U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
        if (carry) {
            add_long(r, r.data[2], r.data[0]);
            add_long(r, r.data[3], r.data[1]);
        }
        auto counter = static_cast<std::uint16_t>(r.data[5]);
        --counter;
        r.data[5] = (r.data[5] & 0xffff0000U) | counter;
    }

    r.data[0] = r.data[2];
    set_logic_flags(r, r.data[0], 0x80000000U, 0xffffffffU);
    r.data[1] = r.data[3];
    set_logic_flags(r, r.data[1], 0x80000000U, 0xffffffffU);
    shift_pair(r, 2U);

    r.data[4] = 0U;
    set_logic_flags(r, 0U, 0x80000000U, 0xffffffffU);
    const auto difficulty = read_byte(host, 0x00000403U);
    r.data[4] = difficulty;
    set_logic_flags(r, difficulty, 0x80U, 0xffU);
    const auto first_mode = static_cast<std::uint8_t>(difficulty & 0x1cU);
    r.data[4] = first_mode;
    set_logic_flags(r, first_mode, 0x80U, 0xffU);

    switch (first_mode) {
    case 0x08U:
        add_pair(r);
        break;
    case 0x0cU:
    case 0x14U:
    case 0x1cU:
        subtract_pair(r);
        break;
    case 0x18U:
        add_pair(r);
        compare_a5_5400(r);
        if (a5_at_least_5400(r)) {
            shift_pair(r, 1U);
            add_pair(r);
        }
        break;
    default:
        break;
    }

    r.address[0] = 0x00003400U;
    const auto second_mode = host.read_memory_word(kRegion, 0x00003478U, kWordMask);
    r.data[0] = (r.data[0] & 0xffff0000U) | second_mode;
    set_logic_flags(r, second_mode, 0x8000U, 0xffffU);

    switch (second_mode) {
    case 0U:
        compare_a5_5400(r);
        if (!a5_at_least_5400(r)) {
            copy_pair(r);
            shift_pair(r, 2U);
            subtract_pair(r);
        }
        break;
    case 4U:
        compare_a5_5400(r);
        if (!a5_at_least_5400(r)) {
            copy_pair(r);
            shift_pair(r, 3U);
            subtract_pair(r);
        }
        break;
    case 8U:
        break;
    case 12U:
        compare_a5_5400(r);
        if (!a5_at_least_5400(r)) {
            copy_pair(r);
            shift_pair(r, 3U);
            add_pair(r);
        }
        break;
    case 16U:
        compare_a5_5400(r);
        if (!a5_at_least_5400(r)) {
            copy_pair(r);
            shift_pair(r, 2U);
            add_pair(r);
        }
        break;
    case 20U:
        compare_a5_5400(r);
        if (!a5_at_least_5400(r)) {
            push_return(host, r, 0x0002057eU);
            r.program_counter = 0x0002055cU;
            host.observe_inline_call(1U, 0x72U,
                0x0002057aU, 0x0002055cU, context);
            compare_a5_5400(r);
            copy_pair(r);
            shift_pair(r, 2U);
            add_pair(r);
            r.program_counter = pop_return(host, r);
            shift_pair(r, 2U);
            add_pair(r);
        }
        break;
    case 24U:
        copy_pair(r);
        shift_pair(r, 3U);
        add_pair(r);
        compare_a5_5400(r);
        if (!a5_at_least_5400(r)) {
            add_pair(r);
            shift_pair(r, 1U);
            add_pair(r);
        }
        break;
    case 28U:
        copy_pair(r);
        shift_pair(r, 2U);
        add_pair(r);
        compare_a5_5400(r);
        if (!a5_at_least_5400(r)) {
            shift_pair(r, 2U);
            add_pair(r);
        }
        break;
    case 32U:
        copy_pair(r);
        shift_pair(r, 2U);
        add_pair(r);
        shift_pair(r, 2U);
        add_pair(r);
        compare_a5_5400(r);
        if (!a5_at_least_5400(r)) add_pair(r);
        break;
    case 36U:
        copy_pair(r);
        shift_pair(r, 2U);
        add_pair(r);
        shift_pair(r, 1U);
        add_pair(r);
        break;
    case 40U:
        push_return(host, r, 0x00020602U);
        r.program_counter = 0x000205eaU;
        host.observe_inline_call(1U, 0x72U,
            0x00020600U, 0x000205eaU, context);
        copy_pair(r);
        shift_pair(r, 2U);
        add_pair(r);
        shift_pair(r, 1U);
        add_pair(r);
        r.program_counter = pop_return(host, r);
        shift_pair(r, 1U);
        add_pair(r);
        break;
    default:
        return {TranslationStatus::contract_violation, 0U, 0x000204e8U};
    }

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
