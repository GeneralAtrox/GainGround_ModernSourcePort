// Implemented but unverified. Original operator attenuation and tail writer.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0001813e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x1813eU: {
            const auto address = r.address[3] + static_cast<std::int16_t>(r.data[3]);
            t.clocks(2U); t.prefetch(pc + 4U);
            const auto value = t.byte(address); m.db(1U, value); m.logic(value, 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x18142U:
            m.db(1U, m.add(r.data[1], r.data[2], 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x18144U: next = t.branch(pc, 0x18148U, (r.status & 8U) == 0U); break;
        case 0x18146U:
            r.data[1] = 0x7fU; m.logic(0x7fU, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x18148U:
            next = t.branch_word(pc, 0x18010U, true);
            return t.transfer(pc, 320U, next);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
