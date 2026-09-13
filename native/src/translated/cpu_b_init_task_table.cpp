// Implemented but unverified. Original task-table clear at 8850..886c.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_init_task_table(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x8850U:
            t.prefetch(pc + 4U); (void)t.word(0x834U); m.logic(0U, 16U);
            t.prefetch(pc + 6U); t.word(0x834U, 0U); next = pc + 4U; break;
        case 0x8854U:
            t.prefetch(pc + 4U); m.logic(0x7fU, 8U); t.prefetch(pc + 6U);
            t.byte(0x832U, 0x7fU); t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x885aU:
            // ST preserves CCR and retains its discarded destination read.
            t.prefetch(pc + 4U); (void)t.byte(0x411U);
            t.prefetch(pc + 6U); t.byte(0x411U, 0xffU); next = pc + 4U; break;
        case 0x885eU:
            r.address[0] = 0xd00U; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = pc + 4U; break;
        case 0x8862U:
            t.prefetch(pc + 4U); m.dw(0U, 0x3fU); m.logic(0x3fU, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x8866U: {
            // CLR.L (A0)+: high read, source increment, low read, prefetch,
            // low/high zero writes. The discarded reads remain observable.
            const auto destination = r.address[0]; (void)t.word(destination);
            r.address[0] += 4U; (void)t.word(destination + 2U);
            m.logic(0U, 16U); t.prefetch(pc + 4U); m.logic(0U, 32U);
            t.word(destination + 2U, 0U); t.word(destination, 0U);
            next = pc + 2U; break;
        }
        case 0x8868U: next = t.dbf(pc, 0x8866U); break;
        case 0x886cU:
            r.address[1] = (r.address[1] & 0xffffU) | 0x20000U;
            t.prefetch(pc + 4U); r.address[1] = 0x24824U;
            t.prefetch(pc + 6U); t.prefetch(pc + 8U); next = pc + 6U; break;
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x8872U)
            return c.host->call_function(126U, 1U, 0x72U, 0U, pc, next, c);
        t.begin(next);
    }
}
} // namespace gain_ground::translated
