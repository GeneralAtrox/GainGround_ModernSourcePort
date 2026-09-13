#pragma once
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated::unverified {

// Implementation-first correction, not yet replay-verified. CPU-A fixtures
// retain the interrupted instruction's control category for IRQ ownership.
inline std::optional<FunctionResult> diagnostic_irq3(
    FunctionContext &c, Machine &m, std::uint32_t pc,
    std::uint32_t next, std::uint8_t instruction_kind)
{
    auto &r = c.registers;
    const auto pending = m.h.consume_pending_interrupt(0U, 0xffU, pc);
    if (!pending.asserted || pending.level <= ((r.status >> 8U) & 7U))
        return std::nullopt;

    const auto saved_status = r.status;
    const auto saved_sp = r.address[7];
    r.address[7] -= 4U;
    m.word(r.address[7] + 2U, next);
    r.address[7] -= 2U;
    m.word(r.address[7], saved_status);
    m.word(r.address[7] + 2U, next >> 16U);
    r.status = static_cast<std::uint16_t>(
        (saved_status & 0x38ffU) | 0x2000U | (pending.level << 8U));
    const auto target = m.lng((24U + pending.level) * 4U);
    (void)m.word(target);
    (void)m.word(target + 2U);
    r.program_counter = target;
    // Preserve the fetched destination when an unimplemented IRQ route occurs.
    if (m.unresolved_bus || pending.level != 3U || target != 0x80042U)
        return FunctionResult{TranslationStatus::contract_violation, 0U, target};

    auto child = m.h.call_function(46U, 0U, 0xffU, instruction_kind, pc, target, c);
    if (child.status != TranslationStatus::complete)
        return child;
    if (instruction_kind == 0U) {
        // The host records kind-0 ownership without executing that partition.
        child = cpu_a_irq3_vector_trampoline(c);
        if (child.status != TranslationStatus::complete)
            return child;
    }
    // Trampoline return control describes the ownership transfer, so use the
    // architectural RTE state to determine whether the interrupted loop resumes.
    if (r.program_counter != next || r.address[7] != saved_sp || r.status != saved_status)
        return FunctionResult{TranslationStatus::contract_violation, 0U, r.program_counter};
    return std::nullopt;
}
}
