#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kWordMask = 0xffffU;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void pf(ExecutionHost &h, std::uint32_t a)
{
    (void)h.read_memory_word(kSharedRegion, a & kSharedMask, kWordMask);
}

void pfp(ExecutionHost &h, std::uint32_t a)
{
    (void)h.read_memory_word(kProgramRegion, a, kWordMask);
}

std::uint16_t rw(ExecutionHost &h, std::uint32_t a)
{
    return h.read_memory_word(kSharedRegion, a & kSharedMask, kWordMask);
}

void ww(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{
    h.write_memory_word(kSharedRegion, a & kSharedMask, v, kWordMask);
}

std::uint32_t rl(ExecutionHost &h, std::uint32_t a)
{
    return (static_cast<std::uint32_t>(rw(h, a)) << 16U) | rw(h, a + 2U);
}

void wl(ExecutionHost &h, std::uint32_t a, std::uint32_t v)
{
    ww(h, a, static_cast<std::uint16_t>(v >> 16U));
    ww(h, a + 2U, static_cast<std::uint16_t>(v));
}

void move_word_flags(CpuRegisters &r, std::uint16_t v)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((v & 0x8000U) != 0U) f |= 0x0008U;
    if (v == 0U) f |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

void move_long_flags(CpuRegisters &r, std::uint32_t v)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((v & 0x80000000U) != 0U) f |= 0x0008U;
    if (v == 0U) f |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

FunctionResult irq(FunctionContext &c, std::uint32_t pc, std::uint32_t resume,
                   std::uint8_t kind, std::uint32_t lookahead)
{
    auto &h = *c.host;
    auto &r = c.registers;
    const auto p = h.consume_pending_interrupt(0U, 0xffU, pc);
    if (!p.asserted || p.level <= ((r.status >> 8U) & 7U))
        return FunctionResult::complete(0U, resume);
    if (lookahead != 0U) pf(h, lookahead);

    const auto sr = r.status;
    r.address[7] -= 4U;
    ww(h, r.address[7] + 2U, static_cast<std::uint16_t>(resume));
    r.address[7] -= 2U;
    ww(h, r.address[7], sr);
    ww(h, r.address[7] + 2U, static_cast<std::uint16_t>(resume >> 16U));
    r.status = static_cast<std::uint16_t>((sr & 0x38ffU) | 0x2000U
        | (static_cast<std::uint16_t>(p.level) << 8U));

    const bool i3 = p.level == 3U;
    const bool i4 = p.level == 4U;
    const auto vector = i3 ? 0x6cU : (i4 ? 0x70U : 0x74U);
    const auto target = i3 ? 0x80042U : (i4 ? 0x80048U : 0x8004eU);
    const auto stub = i3 ? 0x42U : (i4 ? 0x48U : 0x4eU);
    const auto id = i3 ? 46U : (i4 ? 47U : 48U);
    pfp(h, vector);
    pfp(h, vector + 2U);
    (void)rw(h, stub);
    (void)rw(h, stub + 2U);
    r.program_counter = target;
    const auto result = h.call_function(id, 0U, 0xffU, kind, pc, target, c);
    if (result.status != TranslationStatus::complete) return result;
    if (kind == 0U) {
        const auto tr = i3 ? cpu_a_irq3_vector_trampoline(c)
            : (i4 ? cpu_a_irq4_vector_trampoline(c) : cpu_a_irq5_vector_trampoline(c));
        if (tr.status != TranslationStatus::complete) return tr;
    }
    r.program_counter = resume;
    return FunctionResult::complete(1U, resume);
}
} // namespace

FunctionResult cpu_a_asset_metatile_crop_to_62_columns(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    pf(h, 0x000805f2U);
    pf(h, 0x000805f4U);
    r.address[0] = 0xffff985eU;
    pf(h, 0x000805f6U);
    pf(h, 0x000805f8U);
    r.address[1] = 0xffff9862U;
    pf(h, 0x000805faU);
    pf(h, 0x000805fcU);
    r.data[0] = (r.data[0] & 0xffff0000U) | 0x002eU;
    move_word_flags(r, 0x002eU);

    bool outer_prefetched = false;
    for (;;) {
        if (!outer_prefetched) pf(h, 0x000805feU);
        outer_prefetched = false;
        pf(h, 0x00080600U);
        pf(h, 0x00080602U);
        r.data[1] = (r.data[1] & 0xffff0000U) | 0x001eU;
        move_word_flags(r, 0x001eU);
        const auto setup_irq = irq(context, 0x000805feU, 0x00080602U, 0U, 0x00080604U);
        if (setup_irq.status != TranslationStatus::complete) return setup_irq;

        bool move_prefetched = true;
        bool dbra_prefetched = setup_irq.control != 0U;
        for (;;) {
            if (!move_prefetched) pf(h, 0x00080602U);
            move_prefetched = false;
            if (!dbra_prefetched) pf(h, 0x00080604U);
            dbra_prefetched = false;
            const auto value = rl(h, r.address[1]);
            r.address[1] += 4U;
            wl(h, r.address[0], value);
            r.address[0] += 4U;
            move_long_flags(r, value);
            const auto move_irq = irq(context, 0x00080602U, 0x00080604U, 0U, 0x00080606U);
            if (move_irq.status != TranslationStatus::complete) return move_irq;

            if (move_irq.control == 0U) pf(h, 0x00080606U);
            const auto inner = static_cast<std::uint16_t>(r.data[1] - 1U);
            r.data[1] = (r.data[1] & 0xffff0000U) | inner;
            if (inner != 0xffffU) {
                pf(h, 0x00080602U);
                const auto loop_irq = irq(context, 0x00080604U, 0x00080602U, 1U, 0x00080604U);
                if (loop_irq.status != TranslationStatus::complete) return loop_irq;
                move_prefetched = true;
                dbra_prefetched = loop_irq.control != 0U;
                continue;
            }
            pf(h, 0x00080602U);
            pf(h, 0x00080608U);
            break;
        }

        pf(h, 0x0008060aU);
        r.address[1] += 4U;
        pf(h, 0x0008060cU);
        pf(h, 0x0008060eU);
        const auto outer = static_cast<std::uint16_t>(r.data[0] - 1U);
        r.data[0] = (r.data[0] & 0xffff0000U) | outer;
        if (outer != 0xffffU) {
            pf(h, 0x000805feU);
            outer_prefetched = true;
            continue;
        }
        pf(h, 0x000805feU);
        pf(h, 0x00080610U);
        pf(h, 0x00080612U);
        const auto target = rl(h, r.address[7]);
        r.address[7] += 4U;
        pf(h, target);
        pf(h, target + 2U);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }
}

} // namespace gain_ground::translated
