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
std::uint8_t rb(ExecutionHost &h, std::uint32_t a)
{
    const bool odd = (a & 1U) != 0U;
    const auto value = h.read_memory_word(kSharedRegion, a & (kSharedMask & ~1U),
        odd ? 0x00ffU : 0xff00U);
    return static_cast<std::uint8_t>(odd ? value : (value >> 8U));
}
void ww(ExecutionHost &h, std::uint32_t a, std::uint16_t v)
{
    h.write_memory_word(kSharedRegion, a & kSharedMask, v, kWordMask);
}
std::uint32_t rl(ExecutionHost &h, std::uint32_t a)
{
    return (static_cast<std::uint32_t>(rw(h, a)) << 16U) | rw(h, a + 2U);
}
void logic8(CpuRegisters &r, std::uint8_t v)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((v & 0x80U) != 0U) f |= 0x0008U;
    if (v == 0U) f |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
void logic16(CpuRegisters &r, std::uint16_t v)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((v & 0x8000U) != 0U) f |= 0x0008U;
    if (v == 0U) f |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
void logic32(CpuRegisters &r, std::uint32_t v)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((v & 0x80000000U) != 0U) f |= 0x0008U;
    if (v == 0U) f |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}
void lsl5_word(CpuRegisters &r)
{
    const auto input = static_cast<std::uint16_t>(r.data[0]);
    const auto result = static_cast<std::uint16_t>(input << 5U);
    const bool carry = (input & 0x0800U) != 0U;
    std::uint16_t f = 0U;
    if ((result & 0x8000U) != 0U) f |= 0x0008U;
    if (result == 0U) f |= 0x0004U;
    if (carry) f |= 0x0011U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
    r.data[0] = (r.data[0] & 0xffff0000U) | result;
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
    pfp(h, vector); pfp(h, vector + 2U); (void)rw(h, stub); (void)rw(h, stub + 2U);
    r.program_counter = target;
    const auto called = h.call_function(id, 0U, 0xffU, kind, pc, target, c);
    if (called.status != TranslationStatus::complete) return called;
    if (kind == 0U) {
        const auto tr = i3 ? cpu_a_irq3_vector_trampoline(c)
            : (i4 ? cpu_a_irq4_vector_trampoline(c) : cpu_a_irq5_vector_trampoline(c));
        if (tr.status != TranslationStatus::complete) return tr;
    }
    r.program_counter = resume;
    return FunctionResult::complete(1U, resume);
}
} // namespace

FunctionResult cpu_a_asset_expand_stage_plane_metatiles(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    r.data[2] = 5U; logic32(r, r.data[2]);
    pf(h, 0x0008055aU);
    r.data[2] = (r.data[2] << 16U) | (r.data[2] >> 16U); logic32(r, r.data[2]);
    pf(h, 0x0008055cU); pf(h, 0x0008055eU);
    r.data[2] = (r.data[2] & 0xffff0000U) | 7U; logic16(r, 7U);

    bool plane_entry_prefetched = true;
    for (;;) {
        if (!plane_entry_prefetched) pf(h, 0x0008055eU);
        plane_entry_prefetched = false;
        r.data[0] = 0U; logic32(r, 0U);
        pf(h, 0x00080560U);
        auto q = irq(context, 0x0008055eU, 0x00080560U, 0U, 0x00080562U);
        if (q.status != TranslationStatus::complete) return q;
        if (q.control == 0U) pf(h, 0x00080562U);
        const auto index = rb(h, r.address[3]);
        r.address[3] += 1U;
        r.data[0] = (r.data[0] & 0xffffff00U) | index; logic8(r, index);
        q = irq(context, 0x00080560U, 0x00080562U, 0U, 0x00080564U);
        if (q.status != TranslationStatus::complete) return q;
        if (q.control == 0U) pf(h, 0x00080564U);
        pf(h, 0x00080566U);
        r.address[2] = 0x00080f72U;
        pf(h, 0x00080568U); pf(h, 0x0008056aU);
        pf(h, 0x0008056cU);
        r.address[2] = rl(h, r.address[2] + r.data[7]);
        lsl5_word(r);
        pf(h, 0x0008056eU);
        r.address[2] += static_cast<std::int16_t>(r.data[0]);
        pf(h, 0x00080570U);
        r.data[1] = 3U; logic32(r, r.data[1]);

        bool row_entry_prefetched = false;
        bool row_next_prefetched = false;
        bool row_irq_lookahead_prefetched = false;
        bool row_exit_lookahead_prefetched = false;
        for (;;) {
            if (!row_entry_prefetched) pf(h, 0x00080572U);
            row_entry_prefetched = false;
            r.data[1] = (r.data[1] << 16U) | (r.data[1] >> 16U); logic32(r, r.data[1]);
            if (!row_next_prefetched) pf(h, 0x00080574U);
            row_next_prefetched = false;
            const bool resumed_from_row_backedge = row_irq_lookahead_prefetched;
            if (resumed_from_row_backedge) pf(h, 0x00080576U);
            q = irq(context, 0x00080572U, 0x00080574U, 0U,
                resumed_from_row_backedge ? 0U : 0x00080576U);
            row_irq_lookahead_prefetched = false;
            if (q.status != TranslationStatus::complete) return q;
            if (!resumed_from_row_backedge && q.control == 0U) pf(h, 0x00080576U);
            pf(h, 0x00080578U);
            r.data[1] = (r.data[1] & 0xffff0000U) | 3U; logic16(r, 3U);
            q = irq(context, 0x00080574U, 0x00080578U, 0U, 0x0008057aU);
            if (q.status != TranslationStatus::complete) return q;

            bool copy_prefetched = true;
            bool copy_dbra_prefetched = q.control != 0U;
            bool copy_exit_lookahead_prefetched = false;
            for (;;) {
                if (!copy_prefetched) pf(h, 0x00080578U);
                copy_prefetched = false;
                if (!copy_dbra_prefetched) pf(h, 0x0008057aU);
                copy_dbra_prefetched = false;
                const auto value = rw(h, r.address[2]); r.address[2] += 2U;
                ww(h, r.address[0], value); r.address[0] += 2U; logic16(r, value);
                q = irq(context, 0x00080578U, 0x0008057aU, 0U, 0x0008057cU);
                if (q.status != TranslationStatus::complete) return q;
                if (q.control == 0U) pf(h, 0x0008057cU);
                const auto inner = static_cast<std::uint16_t>(r.data[1] - 1U);
                r.data[1] = (r.data[1] & 0xffff0000U) | inner;
                if (inner != 0xffffU) {
                    pf(h, 0x00080578U);
                    pf(h, 0x0008057aU);
                    q = irq(context, 0x0008057aU, 0x00080578U, 1U, 0U);
                    if (q.status != TranslationStatus::complete) return q;
                    copy_prefetched = true; copy_dbra_prefetched = true;
                    continue;
                }
                pf(h, 0x00080578U); pf(h, 0x0008057eU);
                q = irq(context, 0x0008057aU, 0x0008057eU, 1U, 0x00080580U);
                if (q.status != TranslationStatus::complete) return q;
                copy_exit_lookahead_prefetched = q.control != 0U;
                break;
            }
            if (!copy_exit_lookahead_prefetched) pf(h, 0x00080580U);
            r.address[0] += 0x78U;
            pf(h, 0x00080582U);
            r.data[1] = (r.data[1] << 16U) | (r.data[1] >> 16U); logic32(r, r.data[1]);
            pf(h, 0x00080584U); pf(h, 0x00080586U);
            const auto rows = static_cast<std::uint16_t>(r.data[1] - 1U);
            r.data[1] = (r.data[1] & 0xffff0000U) | rows;
            if (rows != 0xffffU) {
                pf(h, 0x00080572U);
                pf(h, 0x00080574U);
                q = irq(context, 0x00080584U, 0x00080572U, 1U, 0U);
                if (q.status != TranslationStatus::complete) return q;
                row_entry_prefetched = true;
                row_next_prefetched = true;
                row_irq_lookahead_prefetched = true;
                continue;
            }
            pf(h, 0x00080572U); pf(h, 0x00080588U);
            q = irq(context, 0x00080584U, 0x00080588U, 1U, 0x0008058aU);
            if (q.status != TranslationStatus::complete) return q;
            row_exit_lookahead_prefetched = q.control != 0U;
            break;
        }
        if (!row_exit_lookahead_prefetched) pf(h, 0x0008058aU);
        r.address[0] -= 0x1f8U;
        pf(h, 0x0008058cU); pf(h, 0x0008058eU);
        auto planes = static_cast<std::uint16_t>(r.data[2] - 1U);
        r.data[2] = (r.data[2] & 0xffff0000U) | planes;
        if (planes != 0xffffU) { pf(h, 0x0008055eU); plane_entry_prefetched = true; continue; }
        pf(h, 0x0008055eU); pf(h, 0x00080590U); pf(h, 0x00080592U);
        r.address[0] += 0x1c0U;
        pf(h, 0x00080594U);
        r.data[2] = (r.data[2] << 16U) | (r.data[2] >> 16U); logic32(r, r.data[2]);
        pf(h, 0x00080596U); pf(h, 0x00080598U);
        const auto groups = static_cast<std::uint16_t>(r.data[2] - 1U);
        r.data[2] = (r.data[2] & 0xffff0000U) | groups;
        if (groups != 0xffffU) {
            pf(h, 0x00080558U);
            r.data[2] = (r.data[2] << 16U) | (r.data[2] >> 16U);
            logic32(r, r.data[2]);
            pf(h, 0x0008055aU); pf(h, 0x0008055cU); pf(h, 0x0008055eU);
            r.data[2] = (r.data[2] & 0xffff0000U) | 7U;
            logic16(r, 7U);
            plane_entry_prefetched = true;
            continue;
        }
        pf(h, 0x00080558U); pf(h, 0x0008059aU); pf(h, 0x0008059cU);
        const auto target = rl(h, r.address[7]); r.address[7] += 4U;
        pf(h, target); pf(h, target + 2U); r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }
}

} // namespace gain_ground::translated
