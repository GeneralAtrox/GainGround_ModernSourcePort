// Implemented but unverified. Original IRQ saved-return redirect.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_redirect_dispatch_on_irq_flag(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x824cU: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); const auto value = t.byte(0x81eU);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((value & 4U) ? 0U : 4U));
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x8252U: next = t.branch(pc, 0x8262U, (r.status & 4U) != 0U); break;
        case 0x8254U: {
            const auto destination = r.address[7] + 0x42U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); t.prefetch(pc + 8U);
            r.status = static_cast<std::uint16_t>(r.status & ~8U); // high-word sr_nz_u, value 0.
            t.word(destination, 0U); m.logic(0x85a6U, 16U);
            t.word(destination + 2U, 0x85a6U); m.logic(0x85a6U, 32U);
            t.prefetch(pc + 10U); next = pc + 8U; break;
        }
        case 0x825cU:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(0x80U, 8U);
            t.byte(0x820U, 0x80U); t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x8262U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
