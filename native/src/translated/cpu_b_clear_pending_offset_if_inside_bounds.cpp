#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {

constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kFullWordMask = 0xffffU;
constexpr std::uint16_t kStatusMask = 0x001fU;
constexpr std::uint16_t kExtendBit = 0x0010U;
constexpr std::uint16_t kNegativeBit = 0x0008U;
constexpr std::uint16_t kZeroBit = 0x0004U;
constexpr std::uint16_t kOverflowBit = 0x0002U;
constexpr std::uint16_t kCarryBit = 0x0001U;

struct ConditionCodes {
    bool carry{};
    bool overflow{};
    bool zero{};
    bool negative{};
};

[[nodiscard]] std::uint16_t read_word(ExecutionHost &host, std::uint32_t address)
{
    return host.read_memory_word(kRegion, address, kFullWordMask);
}

[[nodiscard]] std::uint32_t read_long(ExecutionHost &host, std::uint32_t address)
{
    const auto high = read_word(host, address);
    const auto low = read_word(host, address + 2U);
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}

void write_word(ExecutionHost &host, std::uint32_t address, std::uint16_t data)
{
    host.write_memory_word(kRegion, address, data, kFullWordMask);
}

[[nodiscard]] ConditionCodes move_long(std::uint32_t value, std::uint32_t &destination) noexcept
{
    destination = value;
    return {false, false, value == 0U, (value >> 31U) != 0U};
}

[[nodiscard]] ConditionCodes load_word(std::uint16_t value, std::uint32_t &destination) noexcept
{
    destination = (destination & 0xffff0000U) | value;
    return {false, false, value == 0U, (value >> 15U) != 0U};
}

[[nodiscard]] ConditionCodes add_long(std::uint32_t addend, std::uint32_t &destination) noexcept
{
    const std::uint64_t wide = static_cast<std::uint64_t>(destination) + addend;
    const auto result = static_cast<std::uint32_t>(wide);
    const bool carry = wide > 0xffffffffULL;
    const bool overflow = ((~(destination ^ addend)) & (destination ^ result) & 0x80000000U) != 0U;
    destination = result;
    return {carry, overflow, result == 0U, (result >> 31U) != 0U};
}

[[nodiscard]] ConditionCodes swap_halves(std::uint32_t &destination) noexcept
{
    destination = (destination >> 16U) | (destination << 16U);
    return {false, false, destination == 0U, (destination >> 31U) != 0U};
}

[[nodiscard]] ConditionCodes subtract_quick_word(
    std::uint16_t amount, std::uint32_t &destination) noexcept
{
    const auto source = static_cast<std::uint16_t>(destination & 0xffffU);
    const auto result = static_cast<std::uint16_t>(static_cast<std::uint16_t>(source - amount));
    const bool carry = source < amount;
    const bool overflow = ((source ^ amount) & (source ^ result) & 0x8000U) != 0U;
    destination = (destination & 0xffff0000U) | result;
    return {carry, overflow, result == 0U, (result >> 15U) != 0U};
}

[[nodiscard]] ConditionCodes add_immediate_word(
    std::uint16_t amount, std::uint32_t &destination) noexcept
{
    const auto source = static_cast<std::uint16_t>(destination & 0xffffU);
    const std::uint32_t wide = static_cast<std::uint32_t>(source) + amount;
    const auto result = static_cast<std::uint16_t>(wide);
    const bool carry = wide > 0xffffU;
    const bool overflow = ((~(source ^ amount)) & (source ^ result) & 0x8000U) != 0U;
    destination = (destination & 0xffff0000U) | result;
    return {carry, overflow, result == 0U, (result >> 15U) != 0U};
}

[[nodiscard]] ConditionCodes compare_word(std::uint16_t against, std::uint32_t destination) noexcept
{
    const auto source = static_cast<std::uint16_t>(destination & 0xffffU);
    const auto difference = static_cast<std::uint16_t>(static_cast<std::uint16_t>(source - against));
    const bool carry = source < against;
    const bool overflow = ((source ^ against) & (source ^ difference) & 0x8000U) != 0U;
    return {carry, overflow, difference == 0U, (difference >> 15U) != 0U};
}

[[nodiscard]] ConditionCodes clear_long_at(
    ExecutionHost &host, std::uint32_t address)
{
    static_cast<void>(read_word(host, address));
    static_cast<void>(read_word(host, address + 2U));
    write_word(host, address + 2U, 0U);
    write_word(host, address, 0U);
    return {false, false, true, false};
}

} // namespace

FunctionResult cpu_b_clear_pending_offset_if_inside_bounds(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};

    auto &host = *context.host;
    auto &data = context.registers.data;
    bool extend = (context.registers.status & kExtendBit) != 0U;
    const auto commit = [&](const ConditionCodes &codes) {
        context.registers.status = static_cast<std::uint16_t>(
            (context.registers.status & ~kStatusMask) |
            (extend ? kExtendBit : 0U) |
            (codes.negative ? kNegativeBit : 0U) |
            (codes.zero ? kZeroBit : 0U) |
            (codes.overflow ? kOverflowBit : 0U) |
            (codes.carry ? kCarryBit : 0U));
    };
    const std::uint32_t base = context.registers.address[5];

    ConditionCodes codes = move_long(read_long(host, base + 0x26U), data[0]);
    commit(codes);

    if (codes.zero)
        goto y_bounds;

    codes = add_long(read_long(host, base + 0x1aU), data[0]);
    commit(codes);
    codes = swap_halves(data[0]);
    commit(codes);
    codes = subtract_quick_word(6U, data[0]);
    extend = codes.carry;
    commit(codes);
    codes = compare_word(static_cast<std::uint16_t>(data[4] & 0xffffU), data[0]);
    commit(codes);
    if (codes.negative == codes.overflow)
        goto exit_point;
    codes = add_immediate_word(0x000cU, data[0]);
    extend = codes.carry;
    commit(codes);
    codes = compare_word(static_cast<std::uint16_t>(data[3] & 0xffffU), data[0]);
    commit(codes);
    if (codes.zero || (codes.negative != codes.overflow))
        goto exit_point;

    codes = load_word(read_word(host, base + 0x12U), data[0]);
    commit(codes);
    codes = subtract_quick_word(7U, data[0]);
    extend = codes.carry;
    commit(codes);
    codes = compare_word(static_cast<std::uint16_t>(data[2] & 0xffffU), data[0]);
    commit(codes);
    if (codes.negative == codes.overflow)
        goto second_stage;
    codes = add_immediate_word(0x000eU, data[0]);
    extend = codes.carry;
    commit(codes);
    codes = compare_word(static_cast<std::uint16_t>(data[1] & 0xffffU), data[0]);
    commit(codes);
    if (codes.zero || (codes.negative != codes.overflow))
        goto second_stage;
    codes = clear_long_at(host, base + 0x26U);
    commit(codes);

y_bounds:
    codes = load_word(read_word(host, base + 0x1aU), data[0]);
    commit(codes);
    codes = subtract_quick_word(6U, data[0]);
    extend = codes.carry;
    commit(codes);
    codes = compare_word(static_cast<std::uint16_t>(data[4] & 0xffffU), data[0]);
    commit(codes);
    if (codes.negative == codes.overflow)
        goto exit_point;
    codes = add_immediate_word(0x000cU, data[0]);
    extend = codes.carry;
    commit(codes);
    codes = compare_word(static_cast<std::uint16_t>(data[3] & 0xffffU), data[0]);
    commit(codes);
    if (codes.zero || (codes.negative != codes.overflow))
        goto exit_point;

second_stage:
    codes = move_long(read_long(host, base + 0x1eU), data[0]);
    commit(codes);
    if (codes.zero)
        goto exit_point;
    codes = add_long(read_long(host, base + 0x12U), data[0]);
    commit(codes);
    codes = swap_halves(data[0]);
    commit(codes);
    codes = subtract_quick_word(7U, data[0]);
    extend = codes.carry;
    commit(codes);
    codes = compare_word(static_cast<std::uint16_t>(data[2] & 0xffffU), data[0]);
    commit(codes);
    if (codes.negative == codes.overflow)
        goto exit_point;
    codes = add_immediate_word(0x000eU, data[0]);
    extend = codes.carry;
    commit(codes);
    codes = compare_word(static_cast<std::uint16_t>(data[1] & 0xffffU), data[0]);
    commit(codes);
    if (!(codes.zero || (codes.negative != codes.overflow))) {
        codes = clear_long_at(host, base + 0x1eU);
        commit(codes);
    }

exit_point:
    const std::uint32_t return_address = read_long(host, context.registers.address[7]);
    context.registers.address[7] += 4U;
    context.registers.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
