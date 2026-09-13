// Implemented but unverified. Original mixer register loop at 891e..892e.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_write_mixer_priorities(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x891eU:
            r.address[0] = (r.address[0] & 0xffffU) | 0x400000U;
            t.prefetch(pc + 4U); r.address[0] = 0x404000U;
            t.prefetch(pc + 6U); t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x8924U:
            r.data[0] = 11U; m.logic(11U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x8926U: {
            const auto source = r.address[1]++;
            const auto value = t.byte(source); m.db(1U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8928U:
            // The original byte load preserves D1 bits 8..31. Write D1.W
            // exactly, including its preserved high byte, to the mapped mixer.
            m.logic(r.data[1], 16U); t.word(r.address[0], static_cast<std::uint16_t>(r.data[1]));
            r.address[0] += 2U; t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x892aU: next = t.dbf(pc, 0x8926U); break;
        case 0x892eU: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
