// Implemented but unverified. Original IRQ4-directed wait at 85a6..85aa.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_irq_redirect_wait(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x85a6U:
            // MOVE.W #$2000,SR is privileged. There is no USP switch on
            // this supervisor IRQ return path. Unsupported user entry stops.
            if (!(r.status & 0x2000U))
                return {TranslationStatus::contract_violation, 0U, pc};
            // move_i16u_sr_df: e#w1, rstw1, stiw4, malw3, mmrw3.
            // The first and second prefetches intentionally read PC+4 twice.
            t.prefetch(pc + 4U); t.clocks(2U);
            r.status = 0x2000U; t.clocks(2U);
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = 0x85aaU; break;
        case 0x85aaU:
            next = t.branch(pc, pc, true); break;
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        // This is the original interruptible BRA loop, with no RTS or reset
        // of task state. Each iteration advances its own ordinary clocks.
        t.begin(next);
    }
}
} // namespace gain_ground::translated
