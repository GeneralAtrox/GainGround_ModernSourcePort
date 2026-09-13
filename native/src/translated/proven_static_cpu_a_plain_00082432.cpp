#include "gain_ground/contract_types.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint16_t kPaletteRegion = 9U;
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kMask = 0xffffU;

void pf(ExecutionHost &h, std::uint32_t address)
{
    (void)h.read_memory_word(kSharedRegion, address & 0x0003ffffU, kMask);
}

void pfp(ExecutionHost &h, std::uint32_t address)
{
    (void)h.read_memory_word(kProgramRegion, address, kMask);
}

void write_shared(ExecutionHost &h, std::uint32_t address,
    std::uint16_t value)
{
    h.write_memory_word(kSharedRegion, address & 0x0003ffffU, value, kMask);
}

void logic_long(CpuRegisters &r, std::uint32_t value)
{
    std::uint16_t flags = r.status & 0x0010U;
    if ((value & 0x80000000U) != 0U) flags |= 0x0008U;
    if (value == 0U) flags |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void sub_long(CpuRegisters &r, std::uint32_t source)
{
    const auto destination = r.data[0];
    const auto result = destination - source;
    std::uint16_t flags{};
    if (source > destination) flags |= 0x0011U;
    if (((destination ^ source) & (destination ^ result) & 0x80000000U) != 0U)
        flags |= 0x0002U;
    if ((result & 0x80000000U) != 0U) flags |= 0x0008U;
    if (result == 0U) flags |= 0x0004U;
    r.data[0] = result;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | flags);
}

void write_palette_long(ExecutionHost &h, std::uint32_t address,
    std::uint32_t value)
{
    const auto offset = address - 0x00400000U;
    h.write_memory_word(kPaletteRegion, offset,
        static_cast<std::uint16_t>(value >> 16U), kMask);
    h.write_memory_word(kPaletteRegion, offset + 2U,
        static_cast<std::uint16_t>(value), kMask);
}

FunctionResult irq3(FunctionContext &c, std::uint32_t resume,
    std::uint32_t lookahead)
{
    auto &h = *c.host;
    auto &r = c.registers;
    const auto pending = h.consume_pending_interrupt(0U, 0xffU, 0x0008243eU);
    if (!pending.asserted || pending.level <= ((r.status >> 8U) & 7U))
        return FunctionResult::complete(0U, resume);
    if (pending.level != 3U)
        return {TranslationStatus::contract_violation, 0U, 0x0008243eU};

    pf(h, lookahead);
    const auto saved = r.status;
    r.address[7] -= 4U;
    write_shared(h, r.address[7] + 2U, static_cast<std::uint16_t>(resume));
    r.address[7] -= 2U;
    write_shared(h, r.address[7], saved);
    write_shared(h, r.address[7] + 2U,
        static_cast<std::uint16_t>(resume >> 16U));
    r.status = static_cast<std::uint16_t>((saved & 0x38ffU) | 0x2300U);
    pfp(h, 0x0000006cU); pfp(h, 0x0000006eU);
    pf(h, 0x00000042U); pf(h, 0x00000044U);
    r.program_counter = 0x00080042U;
    const auto child = h.call_function(46U, 0U, 0xffU, 1U,
        0x0008243eU, 0x00080042U, c);
    if (child.status != TranslationStatus::complete) return child;
    r.program_counter = resume;
    return FunctionResult::complete(1U, resume);
}
} // namespace

FunctionResult proven_static_cpu_a_plain_00082432(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};

    auto &h = *context.host;
    auto &r = context.registers;
    pf(h, 0x00082436U);
    r.data[3] = (r.data[3] & 0xffff0000U) | 1U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x000fU));

    for (;;) {
        pf(h, 0x00082438U);
        pf(h, 0x0008243aU);
        r.data[2] = (r.data[2] & 0xffff0000U) | 7U;
        r.status = static_cast<std::uint16_t>((r.status & ~0x000fU));
        bool pf3c_prefetched = false;
        for (;;) {
            if (!pf3c_prefetched) pf(h, 0x0008243cU);
            pf3c_prefetched = false;
            write_palette_long(h, r.address[0], r.data[0]);
            r.address[0] += 4U;
            logic_long(r, r.data[0]);
            pf(h, 0x0008243eU); pf(h, 0x00082440U);
            sub_long(r, r.data[1]);
            const auto inner = static_cast<std::uint16_t>(r.data[2] - 1U);
            r.data[2] = (r.data[2] & 0xffff0000U) | inner;
            pf(h, 0x0008243aU);
            if (inner != 0xffffU) {
                const auto interrupt = irq3(context, 0x0008243aU, 0x0008243cU);
                if (interrupt.status != TranslationStatus::complete) return interrupt;
                pf3c_prefetched = interrupt.control != 0U;
                continue;
            }
            pf(h, 0x00082442U);
            const auto interrupt = irq3(context, 0x00082442U, 0x00082444U);
            if (interrupt.status != TranslationStatus::complete) return interrupt;
            if (interrupt.control == 0U) pf(h, 0x00082444U);
            break;
        }

        r.address[1] += 0x400U;
        pf(h, 0x00082446U);
        r.address[0] = r.address[1];
        pf(h, 0x00082448U); pf(h, 0x0008244aU);
        const auto outer = static_cast<std::uint16_t>(r.data[3] - 1U);
        r.data[3] = (r.data[3] & 0xffff0000U) | outer;
        pf(h, 0x00082436U);
        if (outer != 0xffffU) continue;
        pf(h, 0x0008244cU); pf(h, 0x0008244eU);
        const auto high = h.read_memory_word(kSharedRegion,
            r.address[7] & 0x0003ffffU, kMask);
        const auto low = h.read_memory_word(kSharedRegion,
            (r.address[7] + 2U) & 0x0003ffffU, kMask);
        r.address[7] += 4U;
        const auto target = (static_cast<std::uint32_t>(high) << 16U) | low;
        pf(h, target); pf(h, target + 2U);
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }
}
} // namespace gain_ground::translated
