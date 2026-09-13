// Implemented but unverified. Wait on the original IRQ5-owned countdown byte.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_wait_for_irq5_frame(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x85b0U: {
            t.prefetch(pc + 4U); const auto value = t.byte(0x502U);
            m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x85b4U:
            next = t.branch(pc, 0x85b0U, (r.status & 8U) == 0U); break;
        case 0x85b6U:
            t.prefetch(pc + 4U); m.logic(1U, 8U); t.prefetch(pc + 6U);
            t.byte(0x502U, 1U); t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x85bcU: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        // Preserve the captured self-continuation boundary after the taken BPL.
        if (pc == 0x85b4U && next == 0x85b0U &&
            c.host->consume_self_continuation_boundary(118U, 1U, 0x72U, 1U, pc, next, c))
            return FunctionResult::complete(4U, next);
        t.begin(next);
    }
}
} // namespace gain_ground::translated
