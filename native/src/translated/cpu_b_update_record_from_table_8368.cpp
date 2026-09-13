// Implemented but unverified. Original credit update, clamp and TAS notification.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_update_record_from_table_8368(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U}; CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter; auto next = pc;
        switch (pc) {
        case 0x8368U:
            t.prefetch(pc + 4U); m.logic(0xffffU, 16U); t.word(r.address[5], 0xffffU);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x836cU:
            m.db(1U, m.add(r.data[1], t.byte(r.address[0]), 8U));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x836eU:
            t.prefetch(pc + 4U); (void)m.sub(r.data[1], 9U, 8U, true);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x8372U:
            next = t.branch(pc, 0x837aU, (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U); break;
        case 0x8374U:
            t.prefetch(pc + 4U); m.logic(9U, 8U); t.byte(r.address[0], 9U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x8378U: next = t.branch(pc, 0x837cU, true); break;
        case 0x837aU:
            m.logic(r.data[1], 8U); t.byte(r.address[0], static_cast<std::uint8_t>(r.data[1]));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x837cU: {
            t.prefetch(pc + 4U); const auto old = t.byte(0x417U); t.clocks(2U);
            m.logic(old, 8U); t.byte(0x417U, static_cast<std::uint8_t>(old | 0x80U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x8380U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
