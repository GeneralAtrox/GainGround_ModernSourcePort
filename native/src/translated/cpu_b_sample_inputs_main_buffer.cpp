// Implemented but unverified. Main input-bank selection and original BSR calls.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sample_inputs_main_buffer(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x85beU:
            r.address[0] = 0x800U; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = pc + 4U; break;
        case 0x85c2U:
            r.address[1] = (r.address[1] & 0xffffU) | 0x800000U;
            t.prefetch(pc + 4U); r.address[1] = 0x800000U;
            t.prefetch(pc + 6U); t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x85c8U: case 0x85caU: case 0x85ccU: {
            const auto child = t.bsr(pc, 120U, 0x85d0U, pc + 2U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x85ceU: {
            // ADDQ.W An updates the full address and preserves CCR.
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
        // The final sample falls through and uses the enclosing caller's return.
        if (next == 0x85d0U)
            return c.host->call_function(120U, 1U, 0x72U, 0U, pc, next, c);
        t.begin(next);
    }
}
} // namespace gain_ground::translated
