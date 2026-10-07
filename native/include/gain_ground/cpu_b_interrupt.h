#pragma once
#include "gain_ground/sound_caller_timing.h"
#include "gground_functions.h"
#include "gground_memory_map.h"
#include <optional>

namespace gain_ground {
// Accepted supervisor interrupt, using the existing CPU-B stack/vector and
// FD1094 transition contract. Source-backed entry timing, still unverified.
// The caller must end its instruction/timing scope before entering here.
inline FunctionResult service_cpu_b_autovector(FunctionContext &c, std::uint8_t level,
                                                std::uint32_t site, std::uint32_t resume) {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U || level == 0U || level > 7U ||
        !(r.status & 0x2000U) || (r.address[7] & 1U))
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    auto &host = *c.host;
    const auto saved_status = r.status;
    const auto saved_sp = r.address[7];
    SoundCallerTiming timing(c);
    timing.begin(site, "CPU-B interrupt entry has no complete instruction observations");
    auto resolve_stack = [](std::uint32_t address,
                      std::uint16_t &region, std::uint32_t &offset) {
        address &= 0x00ffffffU;
        for (const auto &window : generated::kMemoryWindows) {
            if (window.address_space != "program" || !(window.cpu_mask & 2U)) continue;
            const auto normalized = address & ~window.mirror;
            if (normalized < window.start || normalized > window.end) continue;
            offset = normalized - window.start;
            if (window.backing_store == "subcpu") { region = 2U; return true; }
            if (window.backing_store == "share1") { region = 3U; return true; }
            break;
        }
        return false;
    };
    auto write = [&](std::uint32_t address, std::uint16_t value) {
        std::uint16_t region{};
        std::uint32_t offset{};
        if (!resolve_stack(address, region, offset)) return false;
        timing.write_memory_word(region, offset, value, 0xffffU);
        return true;
    };
    // state_interrupt_df: itlx1/2/3 precede the first stack bus cycle.
    const auto frame = saved_sp - 6U;
    timing.clocks(2U);
    r.status = static_cast<std::uint16_t>((saved_status & 0x38ffU) | 0x2000U | (level << 8U));
    timing.clocks(4U);
    if (!write(frame + 4U, static_cast<std::uint16_t>(resume)))
        return {TranslationStatus::contract_violation, 0U, frame + 4U};

    // FD1094 irq_callback runs at start_interrupt_vector_lookup, before
    // the default CPU-space acknowledge read and its VPA synchronization.
    c.state = 0x04U;
    // Use the runtime's existing 10 MHz epoch. Its alignment with the
    // original CPU-B E-clock phase remains unverified after direct loading.
    const auto phase = (host.execution_time_ns() / 100U) % 10U;
    timing.clocks(static_cast<std::uint32_t>((phase < 7U ? 10U : 20U) - phase));
    timing.clocks(1U + 4U); // VPA after-delay and acknowledge bus cycle.
    timing.clocks(4U); // itlx6/7.
    r.address[7] = frame;
    if (!write(frame, saved_status))
        return {TranslationStatus::contract_violation, 0U, frame};
    if (!write(frame + 2U, static_cast<std::uint16_t>(resume >> 16U)))
        return {TranslationStatus::contract_violation, 0U, frame + 2U};
    const auto vector = std::uint32_t(24U + level) * 4U;
    // Vector words are data, even though handler fetches use IRQ opcodes.
    const auto high = timing.read_memory_word(2U, vector, 0xffffU);
    const auto low = timing.read_memory_word(2U, vector + 2U, 0xffffU);
    const auto target = (std::uint32_t(high) << 16U) | low;
    if (target & 1U) return {TranslationStatus::contract_violation, 0U, target};
    auto prefetch = [&](std::uint32_t address) {
        std::uint16_t value{};
        if (!host.read_timing_program_word(1U, 0x04U, address, value)) {
            TimingTraceEvent event;
            event.kind = "unsupported";
            event.cpu = 1U; event.state = 0x04U; event.pc = address;
            event.time = TimingTimestamp::from_ns(host.execution_time_ns());
            if (const auto position = host.instruction_timing_position(1U))
                event.clocks = position->clocks;
            event.detail = "CPU-B interrupt handler opcode prefetch is unavailable";
            host.observe_instruction_timing(event);
        }
        // An unavailable opcode view must not be replaced by an encrypted
        // data-RAM read. Its ordinary bus duration is still known.
        timing.clocks(4U);
    };
    prefetch(target);
    timing.clocks(2U); // trap9 between the handler prefetches.
    prefetch(target + 2U);
    r.program_counter = target;
    timing.stop(); // The actual ISR owns all subsequent timing.
    // Resolve the actual vector in the IRQ opcode state. An arithmetic function
    // ID (96 + level) would incorrectly map IRQ2 to the reset-state entry.
    for (const auto &function : generated::kFunctions) {
        if (function.cpu != 1U || function.state != 0x04U || function.address != target) continue;
        const auto child = host.call_function(function.id, 1U, 0x04U, 6U, site, target, c);
        if (child.status != TranslationStatus::complete) return child;
        if (child.control == 9U) return child; // Stack reset: unwinding past the ISR.
        if (!host.resumes_interrupts_inline()) return FunctionResult::complete(5U, target);
        if (child.control != 2U || child.exit_program_counter != r.program_counter ||
            r.address[7] != saved_sp ||
            r.status != saved_status || c.state != 0x72U)
            return {TranslationStatus::contract_violation, child.control, r.program_counter};
        if (r.program_counter != resume) {
            // IRQ4's 0x8254 instruction writes 0x85a6 into its saved PC.
            // The real RTE has already restored SP/SR/state. Transfer without
            // executing the interrupted instruction's pending continuation.
            if (level == 4U && target == 0x806eU && r.program_counter == 0x85a6U)
                return FunctionResult::complete(3U, r.program_counter);
            return {TranslationStatus::contract_violation, child.control, r.program_counter};
        }
        return child;
    }
    return {TranslationStatus::contract_violation, 0U, target};
}
// For existing untimed instruction boundaries. Timed callers must close their
// current scope first; this does not supply instruction-level IPL sampling.
inline std::optional<FunctionResult> cpu_b_interrupt_boundary(FunctionContext &c,
    std::uint32_t site, std::uint32_t resume) {
    if (!c.host || !c.host->resumes_interrupts_inline())
        return FunctionResult{TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    const auto pending = c.host->consume_pending_interrupt(c.cpu, c.state, site);
    if (!pending.asserted || pending.level <= ((c.registers.status >> 8U) & 7U))
        return std::nullopt;
    const auto result = service_cpu_b_autovector(c, pending.level, site, resume);
    if (result.status != TranslationStatus::complete || result.control != 2U) return result;
    // service_cpu_b_autovector has checked the real RTE and restored context.
    return std::nullopt;
}
} // namespace gain_ground
