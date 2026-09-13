#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kCharacterRegion = 8U;
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

void move_flags(CpuRegisters &r, std::uint32_t v, std::uint32_t sign)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((v & sign) != 0U) f |= 0x0008U;
    if (v == 0U) f |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

void add_word(CpuRegisters &r, std::uint16_t value)
{
    const auto left = static_cast<std::uint16_t>(r.data[0]);
    const auto sum = static_cast<std::uint32_t>(left) + value;
    const auto result = static_cast<std::uint16_t>(sum);
    std::uint16_t f{};
    if (sum > 0xffffU) f |= 0x0011U;
    if (((~(left ^ value)) & (left ^ result) & 0x8000U) != 0U) f |= 0x0002U;
    if ((result & 0x8000U) != 0U) f |= 0x0008U;
    if (result == 0U) f |= 0x0004U;
    r.data[0] = (r.data[0] & 0xffff0000U) | result;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

void add_long(CpuRegisters &r, std::uint32_t value)
{
    const auto left = r.data[0];
    const auto result = left + value;
    std::uint16_t f{};
    if (result < left) f |= 0x0011U;
    if (((~(left ^ value)) & (left ^ result) & 0x80000000U) != 0U) f |= 0x0002U;
    if ((result & 0x80000000U) != 0U) f |= 0x0008U;
    if (result == 0U) f |= 0x0004U;
    r.data[0] = result;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

void swap(CpuRegisters &r, unsigned reg)
{
    const auto v = r.data[reg];
    r.data[reg] = (v << 16U) | (v >> 16U);
    move_flags(r, r.data[reg], 0x80000000U);
}

void write_character_long(ExecutionHost &h, std::uint32_t a, std::uint32_t v)
{
    const auto off = a - 0x00280000U;
    h.write_memory_word(kCharacterRegion, off,
        static_cast<std::uint16_t>(v >> 16U), kWordMask);
    h.write_memory_word(kCharacterRegion, off + 2U,
        static_cast<std::uint16_t>(v), kWordMask);
}

FunctionResult call_451(FunctionContext &c, std::uint32_t site,
    std::uint32_t return_pc)
{
    auto &h = *c.host;
    auto &r = c.registers;
    pf(h, 0x0008031aU);
    r.address[7] -= 4U;
    ww(h, r.address[7], static_cast<std::uint16_t>(return_pc >> 16U));
    ww(h, r.address[7] + 2U, static_cast<std::uint16_t>(return_pc));
    pf(h, 0x0008031cU);
    r.program_counter = 0x0008031aU;
    return h.call_function(451U, 0U, 0xffU, 2U,
        site, 0x0008031aU, c);
}

FunctionResult irq5(FunctionContext &c, std::uint32_t pc,
    std::uint32_t resume)
{
    auto &h = *c.host;
    auto &r = c.registers;
    const auto pending = h.consume_pending_interrupt(0U, 0xffU, pc);
    if (!pending.asserted || pending.level <= ((r.status >> 8U) & 7U))
        return FunctionResult::complete(0U, resume);
    if (pending.level != 5U)
        return {TranslationStatus::contract_violation, 0U, pc};

    pf(h, 0x00082302U);
    const auto saved = r.status;
    r.address[7] -= 4U;
    ww(h, r.address[7] + 2U, static_cast<std::uint16_t>(resume));
    r.address[7] -= 2U;
    ww(h, r.address[7], saved);
    ww(h, r.address[7] + 2U, static_cast<std::uint16_t>(resume >> 16U));
    r.status = static_cast<std::uint16_t>((saved & 0x38ffU) | 0x2500U);
    pfp(h, 0x00000074U);
    pfp(h, 0x00000076U);
    (void)rw(h, 0x0000004eU);
    (void)rw(h, 0x00000050U);
    r.program_counter = 0x0008004eU;
    const auto owner = h.call_function(48U, 0U, 0xffU, 0U,
        pc, 0x0008004eU, c);
    if (owner.status != TranslationStatus::complete) return owner;
    const auto trampoline = cpu_a_irq5_vector_trampoline(c);
    if (trampoline.status != TranslationStatus::complete) return trampoline;
    r.program_counter = resume;
    return FunctionResult::complete(1U, resume);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_000822e8(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;

    pf(h, 0x000822ecU); pf(h, 0x000822eeU);
    r.address[0] = 0x00280000U;
    pf(h, 0x000822f0U); pf(h, 0x000822f2U);
    r.address[1] = 0x00119000U;
    pf(h, 0x000822f4U); pf(h, 0x000822f6U);
    r.data[6] = (r.data[6] & 0xffff0000U) | 0x00ffU;
    move_flags(r, 0x00ffU, 0x8000U);
    pf(h, 0x000822f8U);

    for (;;) {
        pf(h, 0x000822faU); pf(h, 0x000822fcU);
        auto child = call_451(context, 0x000822f8U, 0x000822feU);
        if (child.status != TranslationStatus::complete) return child;
        r.address[1] += 8U;
        const auto interrupt = irq5(context, 0x000822feU, 0x00082300U);
        if (interrupt.status != TranslationStatus::complete) return interrupt;
        if (interrupt.control == 0U) pf(h, 0x00082302U);
        const auto count = static_cast<std::uint16_t>(r.data[6] - 1U);
        r.data[6] = (r.data[6] & 0xffff0000U) | count;
        pf(h, 0x000822f8U);
        if (count != 0xffffU) continue;
        pf(h, 0x00082304U);
        break;
    }

    pf(h, 0x00082306U); pf(h, 0x00082308U);
    r.address[1] = 0x00113400U;
    pf(h, 0x0008230aU); pf(h, 0x0008230cU);
    r.data[6] = (r.data[6] & 0xffff0000U) | 0x01ffU;
    move_flags(r, 0x01ffU, 0x8000U);
    pf(h, 0x0008230eU);
    for (;;) {
        pf(h, 0x00082310U); pf(h, 0x00082312U);
        auto child = call_451(context, 0x0008230eU, 0x00082314U);
        if (child.status != TranslationStatus::complete) return child;
        const auto count = static_cast<std::uint16_t>(r.data[6] - 1U);
        r.data[6] = (r.data[6] & 0xffff0000U) | count;
        pf(h, 0x0008230eU);
        if (count != 0xffffU) continue;
        pf(h, 0x00082318U);
        break;
    }

    pf(h, 0x0008231aU); pf(h, 0x0008231cU);
    r.address[0] = 0x00280000U;
    pf(h, 0x0008231eU);
    r.address[0] += 0x00006000U;
    pf(h, 0x00082320U); pf(h, 0x00082322U); pf(h, 0x00082324U);
    r.address[1] = 0x00082fd6U;
    pf(h, 0x00082326U);
    r.data[6] = 0U;
    move_flags(r, 0U, 0x80000000U);
    pf(h, 0x00082328U); pf(h, 0x0008232aU);
    const auto source_word = rw(h, r.address[1] & ~1U);
    const auto source_byte = static_cast<std::uint8_t>(source_word >> 8U);
    ++r.address[1];
    r.data[6] = (r.data[6] & 0xffffff00U) | source_byte;
    move_flags(r, source_byte, 0x80U);
    for (;;) {
        pf(h, 0x0008232cU); pf(h, 0x0008232eU);
        auto child = call_451(context, 0x0008232aU, 0x00082330U);
        if (child.status != TranslationStatus::complete) return child;
        const auto count = static_cast<std::uint16_t>(r.data[6] - 1U);
        r.data[6] = (r.data[6] & 0xffff0000U) | count;
        pf(h, 0x0008232aU);
        if (count != 0xffffU) continue;
        pf(h, 0x00082334U);
        break;
    }

    pf(h, 0x00082336U); pf(h, 0x00082338U);
    r.address[0] = 0x00280000U;
    pf(h, 0x0008233aU);
    r.address[0] += 0x00007000U;
    pf(h, 0x0008233cU); pf(h, 0x0008233eU);
    r.data[1] = (r.data[1] & 0xffff0000U) | 7U;
    move_flags(r, 7U, 0x8000U);
    pf(h, 0x00082340U);
    r.data[0] = 0U;
    move_flags(r, 0U, 0x80000000U);
    pf(h, 0x00082342U); pf(h, 0x00082344U);

    for (;;) {
        swap(r, 1U);
        pf(h, 0x00082346U); pf(h, 0x00082348U);
        r.data[1] = (r.data[1] & 0xffff0000U) | 2U;
        move_flags(r, 2U, 0x8000U);
        pf(h, 0x0008234aU);
        for (;;) {
            pf(h, 0x0008234cU);
            r.data[2] = (r.data[2] & 0xffff0000U) | 7U;
            move_flags(r, 7U, 0x8000U);
            pf(h, 0x0008234eU);
            for (;;) {
                pf(h, 0x00082350U);
                write_character_long(h, r.address[0], r.data[0]);
                r.address[0] += 4U;
                move_flags(r, r.data[0], 0x80000000U);
                pf(h, 0x00082352U);
                const auto d2 = static_cast<std::uint16_t>(r.data[2] - 1U);
                r.data[2] = (r.data[2] & 0xffff0000U) | d2;
                pf(h, 0x0008234eU);
                if (d2 != 0xffffU) continue;
                pf(h, 0x00082354U);
                break;
            }
            swap(r, 0U);
            pf(h, 0x00082356U); pf(h, 0x00082358U);
            add_word(r, 0x1111U);
            pf(h, 0x0008235aU); pf(h, 0x0008235cU);
            const auto d1 = static_cast<std::uint16_t>(r.data[1] - 1U);
            r.data[1] = (r.data[1] & 0xffff0000U) | d1;
            pf(h, 0x0008234aU);
            if (d1 != 0xffffU) continue;
            pf(h, 0x0008235eU);
            break;
        }
        pf(h, 0x00082360U); pf(h, 0x00082362U);
        add_long(r, 0x11110000U);
        pf(h, 0x00082364U);
        swap(r, 1U);
        pf(h, 0x00082366U); pf(h, 0x00082368U);
        const auto outer = static_cast<std::uint16_t>(r.data[1] - 1U);
        r.data[1] = (r.data[1] & 0xffff0000U) | outer;
        pf(h, 0x00082344U);
        if (outer != 0xffffU) continue;
        pf(h, 0x0008236aU); pf(h, 0x0008236cU);
        const auto high = rw(h, r.address[7]);
        const auto low = rw(h, r.address[7] + 2U);
        r.address[7] += 4U;
        const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
        pf(h, target); pf(h, target + 2U);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }
}

} // namespace gain_ground::translated
