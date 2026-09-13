#include "gain_ground/contract_types.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kRegion = 2U;
constexpr std::uint16_t kWordMask = 0xffffU;

void set_logic_word(CpuRegisters &r, std::uint16_t value)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((value & 0x8000U) != 0U) f |= 0x0008U;
    if (value == 0U) f |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

void set_add_word(CpuRegisters &r, std::uint16_t a, std::uint16_t b, std::uint16_t q)
{
    std::uint16_t f{};
    if ((q & 0x8000U) != 0U) f |= 0x0008U;
    if (q == 0U) f |= 0x0004U;
    if (((~(a ^ b)) & (a ^ q) & 0x8000U) != 0U) f |= 0x0002U;
    if (static_cast<std::uint32_t>(a) + b > 0xffffU) f |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

void set_sub_word(CpuRegisters &r, std::uint16_t a, std::uint16_t b, std::uint16_t q)
{
    std::uint16_t f{};
    if ((q & 0x8000U) != 0U) f |= 0x0008U;
    if (q == 0U) f |= 0x0004U;
    if (((a ^ b) & (a ^ q) & 0x8000U) != 0U) f |= 0x0002U;
    if (b > a) f |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

void push_return(ExecutionHost &h, CpuRegisters &r, std::uint32_t pc)
{
    r.address[7] -= 4U;
    h.write_memory_word(kRegion, r.address[7], static_cast<std::uint16_t>(pc >> 16U), kWordMask);
    h.write_memory_word(kRegion, r.address[7] + 2U, static_cast<std::uint16_t>(pc), kWordMask);
}

[[nodiscard]] std::uint32_t pop_return(ExecutionHost &h, CpuRegisters &r)
{
    const auto hi = h.read_memory_word(kRegion, r.address[7], kWordMask);
    const auto lo = h.read_memory_word(kRegion, r.address[7] + 2U, kWordMask);
    r.address[7] += 4U;
    return (static_cast<std::uint32_t>(hi) << 16U) | lo;
}
} // namespace

FunctionResult cpu_b_update_object_record_batch(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    const auto source = h.read_memory_word(kRegion, 0x00000838U, kWordMask);
    r.data[7] = (r.data[7] & 0xffff0000U) | source;
    set_logic_word(r, source);
    const auto added = static_cast<std::uint16_t>(source + 0x000bU);
    r.data[7] = (r.data[7] & 0xffff0000U) | added;
    set_add_word(r, source, 0x000bU, added);
    r.address[4] = 0x00204404U;
    r.data[6] = 0x0000000fU;
    set_logic_word(r, 0x000fU);
    for (;;) {
        const auto d7 = static_cast<std::uint16_t>(r.data[7]);
        r.data[5] = (r.data[5] & 0xffff0000U) | d7;
        set_logic_word(r, d7);
        const auto compared = static_cast<std::uint16_t>(d7 - 0x0063U);
        std::uint16_t f = r.status & 0x0010U;
        if ((compared & 0x8000U) != 0U) f |= 0x0008U;
        if (compared == 0U) f |= 0x0004U;
        if (((d7 ^ 0x0063U) & (d7 ^ compared) & 0x8000U) != 0U) f |= 0x0002U;
        if (d7 < 0x0063U) f |= 0x0001U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x000fU) | f);
        if (d7 >= 0x0063U) {
            r.data[5] = (r.data[5] & 0xffff0000U) | 0x0062U;
            set_logic_word(r, 0x0062U);
        }
        push_return(h, r, 0x00009dfcU);
        r.program_counter = 0x00009e06U;
        const auto child = h.call_function(136U, 1U, 0x72U, 2U,
            0x00009df8U, 0x00009e06U, context);
        if (child.status != TranslationStatus::complete || child.control != 1U)
            return child;
        r.address[4] += 8U;
        const auto before = static_cast<std::uint16_t>(r.data[7]);
        const auto after = static_cast<std::uint16_t>(before - 1U);
        r.data[7] = (r.data[7] & 0xffff0000U) | after;
        set_sub_word(r, before, 1U, after);
        const auto counter = static_cast<std::uint16_t>(r.data[6] - 1U);
        r.data[6] = (r.data[6] & 0xffff0000U) | counter;
        if (counter == 0xffffU) break;
    }

    const auto return_address = pop_return(h, r);
    r.program_counter = return_address;
    return FunctionResult::complete(1U, return_address);
}

} // namespace gain_ground::translated
