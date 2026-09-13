// Implemented but unverified. Original IRQ input-bank selection and sampling calls.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sample_inputs_irq_buffer(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x81b6U:
            r.address[0] = 0x810U; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = pc + 4U; break;
        case 0x81baU:
            // LEA absolute long commits the high half before the first fetch.
            r.address[1] = (r.address[1] & 0xffffU) | 0x800000U;
            t.prefetch(pc + 4U); r.address[1] = 0x800000U;
            t.prefetch(pc + 6U); t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x81c0U: case 0x81c2U: case 0x81c4U: {
            const auto child = t.bsr(pc, 108U, 0x81c8U, pc + 2U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x81c6U: {
            // ADDQ.W to An carries into the high half and does not change CCR.
            const auto value = r.address[1] + 2U;
            r.address[1] = (r.address[1] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[1] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        // The fourth sample is sequential, using the enclosing caller's return.
        if (next == 0x81c8U)
            return c.host->call_function(108U, 1U, 0x72U, 0U, pc, next, c);
        t.begin(next);
    }
}
} // namespace gain_ground::translated
