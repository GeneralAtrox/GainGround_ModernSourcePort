// Implemented but unverified. Original four-operator sound reset loop.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0001814e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x1814eU: case 0x18150U: {
            const auto reg = pc == 0x1814eU ? 2U : 0U;
            r.data[reg] = pc == 0x1814eU ? 3U : 0x60U; m.logic(r.data[reg], 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x18152U:
            m.dw(0U, m.add(r.data[0], r.data[7], 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x18154U:
            t.prefetch(pc + 4U); m.db(1U, 0xffU); m.logic(0xffU, 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x18158U: case 0x18160U: {
            const auto child = t.bsr(pc, 320U, 0x18010U, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x1815cU: case 0x18164U:
            t.prefetch(pc + 4U);
            m.db(0U, pc == 0x1815cU ? r.data[0] | 0x80U : r.data[0] & 0x7fU);
            m.logic(r.data[0], 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x18168U:
            t.prefetch(pc + 4U); m.db(0U, m.add(r.data[0], 8U, 8U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1816cU: next = t.dbf(pc, 0x18158U, 2U); break;
        case 0x18170U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
