#pragma once
#include "gain_ground/sound_caller_timing.h"
#include <array>

namespace gain_ground {
// Original state-04 IRQ3/4/5 prologues. Implemented but unverified;
// nested-IRQ sampling and complete opcode observations remain open.
inline FunctionResult cpu_b_irq_prologue(FunctionContext &c, std::uint32_t entry,
    std::uint32_t compare_pc, std::uint32_t child_id, std::uint32_t target,
    bool save_all) {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x04U ||
        (r.program_counter != entry && r.program_counter != compare_pc))
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    auto &host = *c.host;
    SoundCallerTiming timing(c);
    timing.begin(r.program_counter, "CPU-B IRQ prologue instruction/IRQ observations remain incomplete");
    bool missing_opcode_reported = false;
    auto prefetch = [&](std::uint32_t address) {
        std::uint16_t value{};
        if (!host.read_timing_program_word(1U, c.state, address, value) && !missing_opcode_reported) {
            TimingTraceEvent event;
            event.kind = "unsupported"; event.cpu = 1U; event.state = c.state;
            event.pc = address; event.time = TimingTimestamp::from_ns(host.execution_time_ns());
            if (const auto position = host.instruction_timing_position(1U)) event.clocks = position->clocks;
            event.detail = "CPU-B IRQ prologue opcode view is unavailable";
            host.observe_instruction_timing(event);
            missing_opcode_reported = true;
        }
        timing.clocks(4U);
    };
    if (r.program_counter == entry) {
        if (save_all) {
            // MOVEM.L predecrement: mask/next-word fetch, A6..D0 low/high
            // stores, final prefetch. The reference commits SP after the list.
            const std::array<std::uint32_t, 15> values{
                r.address[6], r.address[5], r.address[4], r.address[3],
                r.address[2], r.address[1], r.address[0], r.data[7],
                r.data[6], r.data[5], r.data[4], r.data[3],
                r.data[2], r.data[1], r.data[0]};
            auto sp = r.address[7];
            prefetch(entry + 4U);
            for (const auto value : values) {
                sp -= 4U;
                timing.write_memory_word(2U, sp + 2U, static_cast<std::uint16_t>(value), 0xffffU);
                timing.write_memory_word(2U, sp, static_cast<std::uint16_t>(value >> 16U), 0xffffU);
            }
            r.address[7] = sp;
            prefetch(entry + 6U);
        } else {
            // MOVE.W #$18,$a00006.l: three fetches, device write, final fetch.
            prefetch(entry + 4U);
            prefetch(entry + 6U);
            r.status = static_cast<std::uint16_t>(r.status & ~0xfU);
            prefetch(entry + 8U);
            host.write_hardware(2U, 1U, 0x04U, entry, 0x00a00006U, 0x0018U, 0xffffU);
            timing.clocks(4U);
            prefetch(entry + 10U);
        }
    }
    r.program_counter = compare_pc;
    // CMPI.L #$0300ffff,D0: the FD1094 callback follows the low-immediate
    // fetch, before fetching the first opcode in the selected state.
    prefetch(compare_pc + 4U);
    c.state = 0x72U;
    prefetch(compare_pc + 6U);
    constexpr std::uint32_t right = 0x0300ffffU;
    const auto left = r.data[0];
    const auto difference = left - right;
    r.status = static_cast<std::uint16_t>((r.status & ~0xfU) |
        ((difference & 0x80000000U) ? 8U : 0U) | (difference == 0U ? 4U : 0U) |
        (((left ^ right) & (left ^ difference) & 0x80000000U) ? 2U : 0U) |
        (left < right ? 1U : 0U));
    prefetch(compare_pc + 8U);
    timing.clocks(2U);
    r.program_counter = target;
    timing.stop();
    const auto child = host.call_function(child_id, 1U, 0x72U, 6U, compare_pc, target, c);
    if (child.status != TranslationStatus::complete) return child;
    if (host.resumes_interrupts_inline()) return child;
    return FunctionResult::complete(5U, target);
}
} // namespace gain_ground
