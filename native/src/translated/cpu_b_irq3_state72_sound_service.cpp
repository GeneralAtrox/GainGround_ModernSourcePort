// Implemented but unverified. Source-backed IRQ3 timing; no new validation.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_irq3_state72_sound_service(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x801eU: t.save(pc, false); next = 0x8022U; break;
        case 0x8022U: case 0x8024U: case 0x8026U:
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x8028U:
            r.data[0] = 0x7fU; m.logic(r.data[0], 32U);
            t.prefetch(pc + 4U); next = 0x802aU; break;
        case 0x802aU: next = t.dbf(pc, 0x802aU); break;
        case 0x802eU: t.restore(pc, false); next = 0x8032U; break;
        case 0x8032U: case 0x806cU: return t.rte();
        case 0x8042U: {
            t.prefetch(pc + 4U);
            const auto value = t.byte(0xffff8001U); m.logic(value, 8U);
            t.prefetch(pc + 6U); next = 0x8046U; break;
        }
        case 0x8046U: next = t.branch(pc, 0x801eU, (r.status & 4U) != 0U); break;
        case 0x8048U: {
            t.prefetch(pc + 4U);
            const auto value = t.byte(0xffff800aU);
            t.clocks(2U); m.logic(value, 8U);
            t.byte(0xffff800aU, static_cast<std::uint8_t>(value | 0x80U));
            t.prefetch(pc + 6U); next = 0x804cU; break;
        }
        case 0x804cU: next = t.branch(pc, 0x801eU, (r.status & 4U) == 0U); break;
        case 0x804eU: t.save(pc, true); next = 0x8052U; break;
        case 0x8052U: case 0x8058U:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            r.address[pc == 0x8052U ? 5U : 6U] = pc == 0x8052U ? 0x00fb0000U : 0xffffc000U;
            t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x805eU: {
            const auto child = t.jsr_absolute(pc, 315U, 0x1707eU, 0x8064U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            // JSR/child already handled its boundary; resume after the RTS.
            continue;
        }
        case 0x8064U: t.restore(pc, true); next = 0x8068U; break;
        case 0x8068U:
            t.prefetch(pc + 4U); (void)t.byte(0xffff800aU);
            m.logic(0U, 8U); t.prefetch(pc + 6U); t.byte(0xffff800aU, 0U);
            next = 0x806cU; break;
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
}
