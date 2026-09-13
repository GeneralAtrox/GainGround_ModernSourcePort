#include "gain_ground/contract_types.h"
#include "gain_ground/m68000_interrupt_entry.h"
#include "gain_ground/sound_caller_timing.h"
#include "gground_functions.h"

#include <cstdint>

namespace gain_ground::translated {
namespace {
constexpr std::uint16_t kProgramRegion = 1U;
constexpr std::uint16_t kSharedRegion = 3U;
constexpr std::uint32_t kSharedMask = 0x0003ffffU;

void pf(ExecutionHost &h, std::uint32_t a) { (void)h.read_memory_word(kSharedRegion, a & kSharedMask, 0xffffU); }
void pfp(ExecutionHost &h, std::uint32_t a) { (void)h.read_memory_word(kProgramRegion, a, 0xffffU); }
std::uint16_t rw(ExecutionHost &h, std::uint32_t a, std::uint16_t m = 0xffffU) { return h.read_memory_word(kSharedRegion, a & kSharedMask, m); }
void ww(ExecutionHost &h, std::uint32_t a, std::uint16_t v, std::uint16_t m = 0xffffU) { h.write_memory_word(kSharedRegion, a & kSharedMask, v, m); }

void logic8(CpuRegisters &r, std::uint8_t v)
{
    std::uint16_t f = r.status & 0x0010U;
    if ((v & 0x80U) != 0U) f |= 0x0008U;
    if (v == 0U) f |= 0x0004U;
    r.status = static_cast<std::uint16_t>((r.status & ~0x001fU) | f);
}

FunctionResult irq(FunctionContext &c, std::uint32_t pc, std::uint32_t resume,
                   std::uint8_t kind, std::uint32_t lookahead,
                   const PendingInterrupt *sampled = nullptr)
{
    auto &h = *c.host;
    auto &r = c.registers;
    const auto p = sampled ? *sampled : h.consume_pending_interrupt(0U, 0xffU, pc);
    if (!p.asserted || p.level <= ((r.status >> 8U) & 7U))
        return FunctionResult::complete(0U, resume);
    if (lookahead != 0U) pf(h, lookahead);
    if (h.resumes_interrupts_inline()) {
        const auto called = service_cpu_a_autovector(c, p.level, pc, resume);
        if (called.status != TranslationStatus::complete) return called;
        return FunctionResult::complete(1U, resume);
    }
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
    const auto stub = i3 ? 0x42U : (i4 ? 0x48U : 0x4eU);
    const auto target = i3 ? 0x80042U : (i4 ? 0x80048U : 0x8004eU);
    const auto id = i3 ? 46U : (i4 ? 47U : 48U);
    pfp(h, vector); pfp(h, vector + 2U);
    (void)rw(h, stub); (void)rw(h, stub + 2U);
    r.program_counter = target;
    const auto called = h.call_function(id, 0U, 0xffU, kind, pc, target, c);
    if (called.status != TranslationStatus::complete) return called;
    if (kind == 0U) {
        const auto trampoline = i3 ? cpu_a_irq3_vector_trampoline(c)
            : (i4 ? cpu_a_irq4_vector_trampoline(c)
                  : cpu_a_irq5_vector_trampoline(c));
        if (trampoline.status != TranslationStatus::complete) return trampoline;
    }
    r.program_counter = resume;
    return FunctionResult::complete(1U, resume);
}
} // namespace

FunctionResult cpu_a_sprite_frame_gate_wait(FunctionContext &context) noexcept
{
    if (context.host == nullptr)
        return {TranslationStatus::contract_violation, 0U,
                context.registers.program_counter};
    auto &h = *context.host;
    auto &r = context.registers;
    SoundCallerTiming timing(context);
    PendingInterrupt sampled{};
    auto sample = [&](std::uint32_t pc) {
        // mmrw3/morw2 latch IPL before the final bus cycle. An edge during
        // that cycle belongs to the next instruction, not this completion.
        if (h.resumes_interrupts_inline())
            sampled = h.consume_pending_interrupt(0U, 0xffU, pc);
    };
    auto begin = [&](std::uint32_t pc) {
        timing.begin(pc, "Frame wait phases implemented; complete IPL observations remain unverified");
    };
    auto prefetch = [&](std::uint32_t address) {
        (void)timing.read_memory_word(kSharedRegion, address & kSharedMask, 0xffffU);
    };
    auto read = [&](std::uint32_t address, std::uint16_t mask = 0xffffU) {
        return timing.read_memory_word(kSharedRegion, address & kSharedMask, mask);
    };
    auto boundary = [&](std::uint32_t pc, std::uint32_t resume,
                        std::uint8_t kind, std::uint32_t lookahead) {
        timing.stop(); // The ISR owns its clocks; restart at its actual return time.
        const auto result = irq(context, pc, resume, kind, lookahead,
            h.resumes_interrupts_inline() ? &sampled : nullptr);
        begin(resume);
        return result;
    };

    for (;;) {
        begin(0x0008034aU);
        // tst_b_adr16_df: extension prefetch, byte read, flags/final prefetch.
        prefetch(0x0008034eU);
        const auto gate = static_cast<std::uint8_t>(
            read(0xffff8400U, 0xff00U) >> 8U);
        logic8(r, gate);
        sample(0x0008034aU);
        prefetch(0x00080350U);
        auto interrupt = boundary(0x0008034aU, 0x0008034eU, 0U, 0U);
        if (interrupt.status != TranslationStatus::complete) return interrupt;

        bool branch_already_serviced = false;
        if ((r.status & 0x0004U) != 0U) {
            timing.clocks(2U); // Taken BEQ: internal phase then two prefetches.
            prefetch(0x0008034aU);
            sample(0x0008034eU);
            prefetch(0x0008034cU);
            interrupt = boundary(0x0008034eU, 0x0008034aU, 1U, 0U);
            if (interrupt.status != TranslationStatus::complete) return interrupt;
            if (interrupt.control != 0U && (r.status & 0x0004U) == 0U) {
                branch_already_serviced = true;
            } else {
                r.program_counter = 0x0008034aU;
                if (h.consume_self_continuation_boundary(61U, 0U, 0xffU, 1U,
                        0x0008034eU, 0x0008034aU, context))
                    return FunctionResult::complete(4U, 0x0008034aU);
                continue;
            }
        }

        if (!branch_already_serviced) timing.clocks(4U); // Untaken BEQ: 8 clocks.
        sample(0x0008034eU);
        prefetch(0x00080352U);
        if (!branch_already_serviced) {
            interrupt = boundary(0x0008034eU, 0x00080350U, 1U,
                h.resumes_interrupts_inline() ? 0U : 0x00080354U);
            if (interrupt.status != TranslationStatus::complete) return interrupt;
            if (interrupt.control == 0U || h.resumes_interrupts_inline()) prefetch(0x00080354U);
        } else {
            prefetch(0x00080354U);
        }
        (void)read(0xffff8400U, 0xff00U);
        // clr_b_adr16_df exposes zero flags before its final prefetch/write.
        logic8(r, 0U);
        prefetch(0x00080356U);
        sample(0x00080350U);
        timing.write_memory_word(kSharedRegion, 0xffff8400U & kSharedMask, 0U, 0xff00U);
        if (h.resumes_interrupts_inline()) {
            interrupt = boundary(0x00080350U, 0x00080354U, 0U, 0U);
            if (interrupt.status != TranslationStatus::complete) return interrupt;
        }
        // RTS: two stack reads and two target prefetches, 16 clocks.
        const auto high = read(r.address[7]);
        const auto target = (std::uint32_t(high) << 16U) | read(r.address[7] + 2U);
        r.address[7] += 4U;
        prefetch(target);
        sample(0x00080354U);
        prefetch(target + 2U);
        timing.stop();
        if (h.resumes_interrupts_inline()) {
            interrupt = irq(context, 0x00080354U, target, 0U, 0U, &sampled);
            if (interrupt.status != TranslationStatus::complete) return interrupt;
        }
        r.program_counter = target;
        return FunctionResult::complete(1U, target);
    }
}

} // namespace gain_ground::translated
