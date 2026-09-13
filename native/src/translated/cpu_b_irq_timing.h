#pragma once
#include "unverified_cpu_b_machine.h"
#include "gain_ground/sound_caller_timing.h"
#include <array>

namespace gain_ground::translated {
// Ordinary IRQ instruction phases from the pinned m68000-sdf.cpp.
// Implemented but unverified; complete opcode/IPL observations remain open.
struct CpuBIrqTiming {
    FunctionContext &c;
    unverified::Machine &m;
    SoundCallerTiming timing;
    CpuBIrqTiming(FunctionContext &context, unverified::Machine &machine)
        : c(context), m(machine), timing(context) { begin(c.registers.program_counter); }
    void begin(std::uint32_t pc) { timing.begin(pc, "Translated instruction observations remain incomplete"); }
    void stop() { timing.stop(); }
    void clocks(std::uint32_t n) { timing.clocks(n); }
    void prefetch(std::uint32_t address) {
        std::uint16_t value{};
        // The scope is explicitly unobserved. Do not invent an opcode value
        // or substitute encrypted data RAM when the opcode view is absent.
        (void)c.host->read_timing_program_word(c.cpu, c.state, address, value);
        clocks(4U);
    }
    std::uint16_t word(std::uint32_t a) { const auto v = m.word(a); clocks(4U); return v; }
    void word(std::uint32_t a, std::uint16_t v) { m.word(a, v); clocks(4U); }
    std::uint8_t byte(std::uint32_t a) { const auto v = m.byte(a); clocks(4U); return v; }
    void byte(std::uint32_t a, std::uint8_t v) { m.byte(a, v); clocks(4U); }
    std::uint32_t lng(std::uint32_t a) {
        const auto high = word(a); const auto low = word(a + 2U);
        return (std::uint32_t(high) << 16U) | low;
    }
    void save(std::uint32_t pc, bool all) {
        auto &r = c.registers;
        const std::array<std::uint32_t, 15> values{r.data[0], r.data[1], r.data[2], r.data[3],
            r.data[4], r.data[5], r.data[6], r.data[7], r.address[0], r.address[1],
            r.address[2], r.address[3], r.address[4], r.address[5], r.address[6]};
        prefetch(pc + 4U);
        auto sp = r.address[7];
        for (unsigned i = all ? 15U : 1U; i != 0U; --i) {
            sp -= 4U;
            word(sp + 2U, static_cast<std::uint16_t>(values[i - 1U]));
            word(sp, static_cast<std::uint16_t>(values[i - 1U] >> 16U));
        }
        r.address[7] = sp;
        prefetch(pc + 6U);
    }
    void restore(std::uint32_t pc, bool all) {
        auto &r = c.registers;
        prefetch(pc + 4U);
        auto sp = r.address[7];
        auto high = word(sp);
        for (unsigned i = 0U; i != (all ? 15U : 1U); ++i) {
            auto &value = i < 8U ? r.data[i] : r.address[i - 8U];
            // popm3 commits the high half before the low-word bus cycle.
            value = (value & 0xffffU) | (std::uint32_t(high) << 16U);
            const auto low = word(sp + 2U);
            value = (value & 0xffff0000U) | low;
            sp += 4U;
            // popm5 commits the low half before the next high-word read,
            // including the read beyond the final restored register.
            high = word(sp);
        }
        r.address[7] = sp;
        prefetch(pc + 6U);
    }
    std::uint32_t branch(std::uint32_t pc, std::uint32_t target, bool taken) {
        if (taken) { clocks(2U); prefetch(target); prefetch(target + 2U); return target; }
        clocks(4U); prefetch(pc + 4U); return pc + 2U;
    }
    std::uint32_t branch_word(std::uint32_t pc, std::uint32_t target, bool taken) {
        if (taken) { clocks(2U); prefetch(target); prefetch(target + 2U); return target; }
        clocks(4U); prefetch(pc + 4U); prefetch(pc + 6U); return pc + 4U;
    }
    std::uint32_t dbf(std::uint32_t pc, std::uint32_t target, unsigned reg = 0U) {
        clocks(2U); prefetch(target);
        m.dw(reg, c.registers.data[reg] - 1U);
        if ((c.registers.data[reg] & 0xffffU) != 0xffffU) {
            prefetch(target + 2U); return target;
        }
        prefetch(pc + 4U); prefetch(pc + 6U); return pc + 4U;
    }
    using ChildCompletion = FunctionResult (*)(FunctionContext &, FunctionResult);
    // The branch instruction has already charged its prefetches. A tail
    // transfer does not push another return or charge the child's execution.
    FunctionResult transfer(std::uint32_t pc, std::uint32_t id, std::uint32_t target) {
        c.registers.program_counter = target;
        stop();
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        if (const auto event = m.interrupt(c, pc, target)) return *event;
        return c.host->call_function(id, 1U, 0x72U, 1U, pc, target, c);
    }
    FunctionResult jsr(std::uint32_t pc, std::uint32_t id,
                       std::uint32_t target, std::uint32_t next,
                       bool pc_relative, ChildCompletion complete_child = nullptr) {
        auto &r = c.registers;
        const auto sp = r.address[7];
        if (pc_relative) clocks(2U); else prefetch(pc + 4U);
        prefetch(target);
        r.address[7] -= 4U;
        word(r.address[7], static_cast<std::uint16_t>(next >> 16U));
        word(r.address[7] + 2U, static_cast<std::uint16_t>(next));
        prefetch(target + 2U);
        return invoke_pushed(pc, id, target, next, sp, complete_child);
    }
    FunctionResult invoke_pushed(std::uint32_t pc, std::uint32_t id,
        std::uint32_t target, std::uint32_t next, std::uint32_t sp,
        ChildCompletion complete_child = nullptr) {
        auto &r = c.registers;
        r.program_counter = target;
        stop();
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        // Preserve the pushed return and pending call across an inline IRQ.
        if (const auto event = m.interrupt(c, pc, target)) return *event;
        auto result = c.host->call_function(id, 1U, 0x72U, 2U, pc, target, c);
        if (complete_child && result.status == TranslationStatus::complete)
            result = complete_child(c, result);
        if (result.status != TranslationStatus::complete || result.control != 1U) return result;
        if (r.program_counter != next || result.exit_program_counter != next ||
            r.address[7] != sp || c.state != 0x72U)
            return {TranslationStatus::contract_violation, result.control, r.program_counter};
        begin(next);
        return result;
    }
    FunctionResult jsr_absolute(std::uint32_t pc, std::uint32_t id,
                                std::uint32_t target, std::uint32_t next) {
        return jsr(pc, id, target, next, false);
    }
    FunctionResult jsr_pc_relative(std::uint32_t pc, std::uint32_t id,
        std::uint32_t target, std::uint32_t next, ChildCompletion complete_child = nullptr) {
        return jsr(pc, id, target, next, true, complete_child);
    }
    FunctionResult bsr(std::uint32_t pc, std::uint32_t id,
                       std::uint32_t target, std::uint32_t next,
                       ChildCompletion complete_child = nullptr) {
        auto &r = c.registers;
        const auto sp = r.address[7];
        clocks(2U);
        r.address[7] -= 4U;
        word(r.address[7], static_cast<std::uint16_t>(next >> 16U));
        word(r.address[7] + 2U, static_cast<std::uint16_t>(next));
        prefetch(target); prefetch(target + 2U);
        return invoke_pushed(pc, id, target, next, sp, complete_child);
    }
    FunctionResult supervisor_trap(std::uint32_t pc, unsigned vector, std::uint32_t next) {
        auto &r = c.registers;
        // These supervisor callers have no separate USP/SSP model. Do not invent a
        // user-mode stack switch or an odd-address exception implementation.
        if (!((c.cpu == 1U && c.state == 0x72U) || (c.cpu == 0U && c.state == 0xffU)) ||
            !(r.status & 0x2000U) || (r.address[7] & 1U) ||
            vector < 32U || vector > 47U)
            return {TranslationStatus::contract_violation, 0U, pc};
        const auto saved_sp = r.address[7];
        const auto saved_status = r.status;
        const auto frame = saved_sp - 6U;
        // trap_imm4_df: trap1/2, three stack cycles, vector high/low,
        // handler prefetch, trap9 and final prefetch: 34 ordinary clocks.
        clocks(2U);
        r.status = static_cast<std::uint16_t>((saved_status & ~0x8000U) | 0x2000U);
        clocks(2U);
        word(saved_sp - 2U, static_cast<std::uint16_t>(next));
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.address[7] = frame; // trap4 commits SSP after the first stack write.
        word(frame, saved_status);
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        word(frame + 2U, static_cast<std::uint16_t>(next >> 16U));
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        const auto high = word(vector * 4U);
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        const auto low = word(vector * 4U + 2U);
        const auto target = (std::uint32_t(high) << 16U) | low;
        if (m.unresolved_bus || (target & 1U))
            return {TranslationStatus::contract_violation, 0U, target};
        // Software TRAP performs no interrupt acknowledge. FD1094 therefore
        // keeps the current opcode state; STATE_IRQ belongs to an actual IRQ.
        prefetch(target); clocks(2U); prefetch(target + 2U);
        r.program_counter = target;
        stop();
        const auto child = m.dispatch(c, pc, target, 4U, m.state);
        if (child.status != TranslationStatus::complete || child.control != 2U) return child;
        // The handler may deliberately modify the saved SR. Require its real
        // RTE and continuation, without overwriting the status it returned.
        if (child.exit_program_counter != next || r.program_counter != next ||
            r.address[7] != saved_sp || c.state != m.state)
            return {TranslationStatus::contract_violation, child.control, r.program_counter};
        return child;
    }
    FunctionResult rts(std::uint32_t pc) {
        auto &r = c.registers;
        const auto target = lng(r.address[7]);
        if (m.unresolved_bus || (target & 1U))
            return {TranslationStatus::contract_violation, 0U, pc};
        r.address[7] += 4U;
        prefetch(target); prefetch(target + 2U);
        r.program_counter = target;
        stop();
        if (const auto event = m.interrupt(c, pc, target)) return *event;
        return FunctionResult::complete(1U, target);
    }
    FunctionResult rte() {
        auto &r = c.registers;
        if (!(r.status & 0x2000U) || (r.address[7] & 1U))
            return {TranslationStatus::contract_violation, 0U, r.program_counter};
        // RTE reads SR once. The earlier duplicate is owned by MOVEM.
        const auto sp = r.address[7];
        const auto sr = word(sp);
        const auto high = word(sp + 2U);
        r.address[7] = sp + 6U;
        const auto low = word(sp + 4U);
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, r.program_counter};
        r.status = sr;
        const auto target = (std::uint32_t(high) << 16U) | low;
        if (target & 1U) return {TranslationStatus::contract_violation, 0U, target};
        prefetch(target); prefetch(target + 2U);
        r.program_counter = target;
        return FunctionResult::complete(2U, target);
    }
};
} // namespace gain_ground::translated
