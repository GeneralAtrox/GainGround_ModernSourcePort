// Implemented but unverified. Original note-on setup and tail branch.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00018046(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x18046U:
            m.db(1U, r.data[7]); m.logic(r.data[1], 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x18048U:
            t.prefetch(pc + 4U); m.db(1U, r.data[1] | 0x78U); m.logic(r.data[1], 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1804cU:
            r.data[0] = 8U; m.logic(8U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x1804eU:
            next = t.branch(pc, 0x18010U, true);
            return t.transfer(pc, 320U, next);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
