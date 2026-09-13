#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kPrivate = 2U;
constexpr std::uint16_t kShared = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;

std::uint32_t shared_offset(std::uint32_t address) { return address & 0x0003ffffU; }

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_sub_word(CpuRegisters &r, std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((left ^ right) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (right > left) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void set_add_word(CpuRegisters &r, std::uint16_t left, std::uint16_t right, std::uint16_t result)
{
    const auto wide = static_cast<std::uint32_t>(left) + right;
    std::uint16_t flags{};
    if ((result & 0x8000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    if (((~(left ^ right)) & (left ^ result) & 0x8000U) != 0U) flags |= 0x0002U;
    if (wide > 0xffffU) flags |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

std::uint32_t pop_return(FunctionContext &context)
{
    auto &r = context.registers;
    const auto high = context.host->read_memory_word(kPrivate, r.address[7], kWordMask);
    const auto low = context.host->read_memory_word(kPrivate, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(high) << 16U) | low;
}
} // namespace

FunctionResult cpu_b_sound_init_record_return(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    auto d0 = static_cast<std::uint16_t>(r.data[0]);
    const auto compared = static_cast<std::uint16_t>(d0 - 0x001aU);
    set_sub_word(r, d0, 0x001aU, compared);
    if (d0 >= 0x001aU) {
        const auto target = pop_return(context);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }

    d0 = static_cast<std::uint16_t>(d0 & 0x00feU);
    r.data[0] = (r.data[0] & 0xffff0000U) | d0;
    set_logic_word(r, d0);
    r.address[0] = r.address[6];

    const auto counter_address = shared_offset(r.address[0] + 0x20U);
    const auto counter = h.read_memory_word(kShared, counter_address, kWordMask);
    const auto next_counter = static_cast<std::uint16_t>(counter + 1U);
    h.write_memory_word(kShared, counter_address, next_counter, kWordMask);
    set_add_word(r, counter, 1U, next_counter);

    const auto control_word = h.read_memory_word(kShared, counter_address, 0x00ffU);
    const bool full = (static_cast<std::uint8_t>(control_word) & 0x10U) != 0U;
    if (full) {
        if (full) r.status &= static_cast<std::uint16_t>(~0x0004U);
        r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | 0x0001U);
    } else {
        r.status |= 0x0004U;
        const auto cursor_address = shared_offset(r.address[0] + 0x22U);
        auto d1 = h.read_memory_word(kShared, cursor_address, kWordMask);
        d1 = static_cast<std::uint16_t>(d1 & 0x001eU);
        r.data[1] = (r.data[1] & 0xffff0000U) | d1;
        set_logic_word(r, d1);
        const auto slot = static_cast<std::uint32_t>(
            static_cast<std::int64_t>(r.address[0]) + static_cast<std::int16_t>(d1));
        h.write_memory_word(kShared, shared_offset(slot), d0, kWordMask);
        const auto cursor = h.read_memory_word(kShared, cursor_address, kWordMask);
        const auto next_cursor = static_cast<std::uint16_t>(cursor + 2U);
        h.write_memory_word(kShared, cursor_address, next_cursor, kWordMask);
        set_add_word(r, cursor, 2U, next_cursor);
        set_logic_word(r, d0);
    }

    const auto target = pop_return(context);
    r.program_counter = target;
    return FunctionResult::complete(1U, target);
}

} // namespace gain_ground::translated
