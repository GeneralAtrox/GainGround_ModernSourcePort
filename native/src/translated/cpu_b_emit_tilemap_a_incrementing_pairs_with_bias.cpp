#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivateRegion = 2U;
constexpr std::uint16_t kTileRegion = 5U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_flags(CpuRegisters &r, std::uint32_t value, std::uint32_t sign)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & sign) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_word_flags(CpuRegisters &r,
    std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void rotate_left_four(CpuRegisters &r)
{
    const auto value = r.data[2];
    const auto result = static_cast<std::uint32_t>((value << 4U) | (value >> 28U));
    r.data[2] = result;
    std::uint16_t flags = r.status & 0x0010U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if ((value & 0x10000000U) != 0U) flags |= 0x0001U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void bset_sign_marker(CpuRegisters &r)
{
    const bool was_set = (r.data[3] & 0x80000000U) != 0U;
    r.data[3] |= 0x80000000U;
    if (was_set) r.status = static_cast<std::uint16_t>(r.status & ~0x0004U);
    else r.status = static_cast<std::uint16_t>(r.status | 0x0004U);
}

void write_d0(ExecutionHost &host, CpuRegisters &r, bool postincrement)
{
    const auto value = static_cast<std::uint16_t>(r.data[0]);
    host.write_memory_word(kTileRegion, r.address[0] - 0x00200000U,
        value, kWordMask);
    if (postincrement) r.address[0] += 2U;
    set_logic_flags(r, value, 0x8000U);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &host, CpuRegisters &r)
{
    const auto stack = r.address[7];
    const auto high = host.read_memory_word(kPrivateRegion, stack, kWordMask);
    const auto low = host.read_memory_word(kPrivateRegion, stack + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_emit_tilemap_a_incrementing_pairs_with_bias_resume_1611e(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};
    auto &host = *context.host;
    auto &r = context.registers;

    const auto entry_pc = r.program_counter;
    if (entry_pc != 0x0001611eU) {
        if (entry_pc != 0x0001610eU)
            return {TranslationStatus::contract_violation, 0U, entry_pc};
        r.address[0] = 0x00200000U;
        const auto first_offset = host.read_memory_word(
            kPrivateRegion, r.address[1], kWordMask);
        r.address[1] += 2U;
        r.address[0] = static_cast<std::uint32_t>(
            static_cast<std::int64_t>(r.address[0])
            + static_cast<std::int16_t>(first_offset));
        r.data[3] = 0U;
        set_logic_flags(r, 0U, 0x80000000U);
        const auto count = host.read_memory_word(
            kPrivateRegion, r.address[1], kWordMask);
        r.data[3] = count;
        set_logic_flags(r, count, 0x8000U);
        const auto bias = host.read_memory_word(
            kPrivateRegion, r.address[5] + 0x62U, kWordMask);
        r.data[1] = (r.data[1] & 0xffff0000U) | bias;
        set_logic_flags(r, bias, 0x8000U);
    }
    const auto second_offset = host.read_memory_word(
        kPrivateRegion, r.address[5] + 0x7aU, kWordMask);
    r.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(r.address[0])
        + static_cast<std::int16_t>(second_offset));

    for (;;) {
        rotate_left_four(r);
        const auto rotated_word = static_cast<std::uint16_t>(r.data[2]);
        r.data[0] = (r.data[0] & 0xffff0000U) | rotated_word;
        set_logic_flags(r, rotated_word, 0x8000U);
        const auto nibble = static_cast<std::uint16_t>(rotated_word & 0x000fU);
        r.data[0] = (r.data[0] & 0xffff0000U) | nibble;
        set_logic_flags(r, nibble, 0x8000U);

        bool emit_zero{};
        if (nibble != 0U) {
            bset_sign_marker(r);
        } else {
            const auto counter = static_cast<std::uint16_t>(r.data[3]);
            set_logic_flags(r, counter, 0x8000U);
            if (counter != 0U) {
                set_logic_flags(r, r.data[3], 0x80000000U);
                if ((r.data[3] & 0x80000000U) == 0U) emit_zero = true;
                else bset_sign_marker(r);
            }
        }

        if (emit_zero) {
            r.data[0] = 0U;
            set_logic_flags(r, 0U, 0x80000000U);
        } else {
            const auto before_add = static_cast<std::uint16_t>(r.data[0]);
            const auto added = static_cast<std::uint16_t>(before_add + 0x0030U);
            r.data[0] = (r.data[0] & 0xffff0000U) | added;
            set_add_word_flags(r, before_add, 0x0030U, added);
            const auto doubled = static_cast<std::uint16_t>(added << 1U);
            r.data[0] = (r.data[0] & 0xffff0000U) | doubled;
            set_add_word_flags(r, added, added, doubled);
        }

        const auto combined = static_cast<std::uint16_t>(r.data[0] | r.data[1]);
        r.data[0] = (r.data[0] & 0xffff0000U) | combined;
        set_logic_flags(r, combined, 0x8000U);
        write_d0(host, r, true);
        const auto incremented = static_cast<std::uint16_t>(combined + 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | incremented;
        set_add_word_flags(r, combined, 1U, incremented);
        write_d0(host, r, false);
        r.address[0] += 0x7eU;

        const auto decremented = static_cast<std::uint16_t>(r.data[3] - 1U);
        r.data[3] = (r.data[3] & 0xffff0000U) | decremented;
        if (decremented == 0xffffU) break;
    }

    const auto target = pop_return(host, r);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

FunctionResult cpu_b_emit_tilemap_a_incrementing_pairs_with_bias(
    FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
            context.registers.program_counter};

    auto &host = *context.host;
    auto &r = context.registers;
    if (r.program_counter != 0x0001610eU)
        return {TranslationStatus::contract_violation, 0U,
            r.program_counter};

    r.address[0] = 0x00200000U;
    const auto first_offset = host.read_memory_word(
        kPrivateRegion, r.address[1], kWordMask);
    r.address[1] += 2U;
    r.address[0] = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(r.address[0])
        + static_cast<std::int16_t>(first_offset));

    r.data[3] = 0U;
    set_logic_flags(r, 0U, 0x80000000U);
    const auto count = host.read_memory_word(
        kPrivateRegion, r.address[1], kWordMask);
    r.data[3] = count;
    set_logic_flags(r, count, 0x8000U);

    const auto bias = host.read_memory_word(
        kPrivateRegion, r.address[5] + 0x62U, kWordMask);
    r.data[1] = (r.data[1] & 0xffff0000U) | bias;
    set_logic_flags(r, bias, 0x8000U);

    r.program_counter = 0x0001611eU;
    return host.call_function(541U, 1U, 0x72U, 0U,
        0x0001611aU, 0x0001611eU, context);
}

} // namespace gain_ground::translated
