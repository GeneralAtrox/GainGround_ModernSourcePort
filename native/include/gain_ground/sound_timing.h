#pragma once
#include "gground_sound_timing.h"
#include "gain_ground/cpu_b_interrupt.h"
#include "gain_ground/m68000_interrupt_entry.h"
#include <optional>

namespace gain_ground::sound_timing {
inline bool instruction_interrupt(FunctionContext &context, std::uint32_t site,
                                  PendingInterrupt pending,
                                  std::optional<FunctionResult> *transfer = nullptr) {
    auto &host = *context.host;
    // Retained fixture hosts own their captured interrupt boundaries. Live
    // execution delivers the real ISR and resumes this instruction sequence.
    if (!host.resumes_interrupts_inline()) return true;
    auto &r = context.registers;
    if (!pending.asserted) return true;
    const auto next = r.program_counter;
    const auto saved_sp = r.address[7];
    const auto saved_sr = r.status;
    const auto saved_state = context.state;
    const auto child = context.cpu == 0U
        ? service_cpu_a_autovector(context, pending.level, site, next)
        : service_cpu_b_autovector(context, pending.level, site, next);
    if (child.status == TranslationStatus::complete && child.control == 3U) {
        // CPU-B IRQ4 can replace the saved PC. Surface that real transfer;
        // the writer must not issue another write or finish a pending RTS.
        if (transfer) *transfer = child;
        return false;
    }
    return child.status == TranslationStatus::complete && child.control == 2U &&
        child.exit_program_counter == next && r.program_counter == next &&
        r.address[7] == saved_sp && r.status == saved_sr && context.state == saved_state;
}
inline bool instruction(FunctionContext &context, TimingCpuPosition &position,
                        SoundInstructionOps::Operation operation, std::uint32_t pc,
                        unsigned reg = 0U, std::uint32_t port = 0U,
                        std::optional<FunctionResult> *transfer = nullptr) {
    if (transfer) transfer->reset();
    auto &host = *context.host;
    SoundInstructionOps native{host, context.registers, context.cpu, context.state, pc, operation,
                               port, context.registers.address[7], reg};
    const auto opcode = static_cast<std::uint16_t>(
        operation == SoundInstructionOps::Operation::btst ? 0x0839U :
        operation == SoundInstructionOps::Operation::bne ? 0x66f6U :
        operation == SoundInstructionOps::Operation::move ? 0x13c0U | reg : 0x4e75U);
    context.registers.program_counter = pc;
    bool completed = false;
    {
        TimingInstruction timing(host, position, {context.cpu, context.state, pc, opcode});
        switch (operation) {
        case SoundInstructionOps::Operation::btst: completed = sound_btst(timing, native); break;
        case SoundInstructionOps::Operation::bne: completed = sound_bne(timing, native); break;
        case SoundInstructionOps::Operation::move: completed = sound_move(timing, native); break;
        case SoundInstructionOps::Operation::rts: completed = sound_rts(timing, native); break;
        default: return timing.unsupported("unknown sound instruction");
        }
    }
    // The instruction must be closed before its ISR uses this CPU's timing
    // position. In particular RTS has already popped the caller's return PC.
    return completed && instruction_interrupt(context, pc, native.sampled_interrupt, transfer);
}

inline bool poll(FunctionContext &context, TimingCpuPosition &position, std::uint32_t pc) {
    return instruction(context, position, SoundInstructionOps::Operation::btst, pc) &&
           instruction(context, position, SoundInstructionOps::Operation::bne, pc + 8U);
}
inline bool write_pair(FunctionContext &context, TimingCpuPosition &position, std::uint32_t pc) {
    return instruction(context, position, SoundInstructionOps::Operation::move, pc, 0U, 0x800101U) &&
           instruction(context, position, SoundInstructionOps::Operation::move, pc + 6U, 1U, 0x800103U);
}
inline FunctionResult writer(FunctionContext &context, std::uint32_t function_id,
                             std::uint32_t entry_pc) {
    if (!context.host)
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    auto &host = *context.host;
    TimingCpuPosition fallback;
    auto *position = host.instruction_timing_position(context.cpu);
    if (!position) position = &fallback;
    if (host.resumes_interrupts_inline()) {
        for (;;) {
            const auto pc = context.registers.program_counter;
            SoundInstructionOps::Operation operation;
            unsigned reg = 0U;
            std::uint32_t port = 0U;
            if (pc == entry_pc) operation = SoundInstructionOps::Operation::btst;
            else if (pc == entry_pc + 8U) operation = SoundInstructionOps::Operation::bne;
            else if (pc == entry_pc + 10U || pc == entry_pc + 16U) {
                operation = SoundInstructionOps::Operation::move;
                reg = pc == entry_pc + 10U ? 0U : 1U;
                port = reg == 0U ? 0x800101U : 0x800103U;
            } else if (pc == entry_pc + 22U) operation = SoundInstructionOps::Operation::rts;
            else return {TranslationStatus::contract_violation, 0U, pc};
            std::optional<FunctionResult> transfer;
            if (!instruction(context, *position, operation, pc, reg, port, &transfer)) {
                if (transfer) return *transfer;
                return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
            }
            if (operation == SoundInstructionOps::Operation::rts)
                return FunctionResult::complete(1U, context.registers.program_counter);
            // BNE is an instruction backedge, not a recursive native call.
            // The actual post-instruction PC also preserves entries after RTE.
        }
    }
    if (!poll(context, *position, entry_pc))
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    if ((context.registers.status & 4U) == 0U) {
        const auto loop = host.call_function(function_id, context.cpu, context.state, 1U,
                                             entry_pc + 8U, entry_pc, context);
        if (loop.status == TranslationStatus::complete && loop.control == 3U)
            return FunctionResult::complete(4U, entry_pc);
        return loop;
    }
    if (!write_pair(context, *position, entry_pc + 10U) ||
        !instruction(context, *position, SoundInstructionOps::Operation::rts, entry_pc + 22U))
        return {TranslationStatus::contract_violation, 0U, context.registers.program_counter};
    return FunctionResult::complete(1U, context.registers.program_counter);
}
} // namespace gain_ground::sound_timing
