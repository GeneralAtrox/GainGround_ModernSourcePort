// Implemented but unverified. Original sound-record destination selection.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017472(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17472U:
            r.address[1] = r.address[6]; t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17474U: {
            const auto address = r.address[0]; r.address[0] += 2U;
            m.logic(t.word(address), 16U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17476U:
            r.data[1] = 0U; m.logic(0U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17478U:
            m.db(1U, r.data[7]); m.logic(r.data[1], 8U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x1747aU:
            t.prefetch(pc + 4U); m.dw(1U, r.data[1] | 0x10U); m.logic(r.data[1], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1747eU:
            m.dw(1U, m.add(r.data[1], r.data[1], 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17480U:
            r.address[2] = 0x17834U; t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17484U: {
            const auto address = r.address[2] + static_cast<std::int16_t>(r.data[1]);
            t.clocks(2U); t.prefetch(pc + 4U); const auto offset = t.word(address);
            const auto value = r.address[1] + static_cast<std::int16_t>(offset);
            r.address[1] = (r.address[1] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 6U); t.clocks(2U); r.address[1] = value; t.clocks(2U);
            next = pc + 4U; break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        // Original sequential partition marker: no branch clocks or BSR push.
        if (next == 0x17488U)
            return c.host->call_function(479U, 1U, 0x72U, 0U, pc, next, c);
        t.begin(next);
    }
}
} // namespace gain_ground::translated
