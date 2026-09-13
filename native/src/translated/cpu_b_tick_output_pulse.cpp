// Implemented but unverified. Original coin-output pulse countdown.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_tick_output_pulse(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U}; CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter; auto next = pc;
        switch (pc) {
        case 0x8382U:
            r.data[1] = 1U; m.logic(1U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x8384U: case 0x83baU: {
            const auto value = t.byte(r.address[1]); m.db(pc == 0x8384U ? 0U : 1U, value);
            m.logic(value, 8U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8386U: next = t.branch(pc, 0x839aU, (r.status & 4U) != 0U); break;
        case 0x8388U: next = t.branch(pc, 0x83b4U, (r.status & 8U) != 0U); break;
        case 0x838aU:
            t.prefetch(pc + 4U); m.logic(t.byte(r.address[1] + 1U), 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x838eU: next = t.branch(pc, 0x839cU, !(r.status & 4U)); break;
        case 0x8390U: case 0x83a4U: {
            t.prefetch(pc + 4U); const auto old = t.byte(0x40fU);
            const auto value = static_cast<std::uint8_t>(pc == 0x8390U ? old | r.data[1] : old & r.data[1]);
            m.logic(value, 8U); t.prefetch(pc + 6U); t.byte(0x40fU, value); next = pc + 4U; break;
        }
        case 0x8394U: case 0x83a8U:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(6U, 8U);
            t.byte(r.address[1] + 1U, 6U); t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x839aU: case 0x83b2U: case 0x83c4U: return t.rts(pc);
        case 0x839cU: case 0x83b4U: {
            t.prefetch(pc + 4U); const auto address = r.address[1] + 1U;
            const auto value = m.sub(t.byte(address), 1U, 8U);
            t.prefetch(pc + 6U); t.byte(address, static_cast<std::uint8_t>(value)); next = pc + 4U; break;
        }
        case 0x83a0U: case 0x83b8U: next = t.branch(pc, 0x839aU, !(r.status & 4U)); break;
        case 0x83a2U:
            m.dw(1U, ~r.data[1]); m.logic(r.data[1], 16U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x83aeU: {
            t.prefetch(pc + 4U); const auto address = r.address[1]; const auto old = t.byte(address);
            t.prefetch(pc + 6U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x80U) ? 0U : 4U));
            t.byte(address, static_cast<std::uint8_t>(old | 0x80U)); next = pc + 4U; break;
        }
        case 0x83bcU:
            t.prefetch(pc + 4U); m.db(1U, r.data[1] & 0x7fU); m.logic(r.data[1], 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x83c0U:
            m.db(1U, m.sub(r.data[1], 1U, 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x83c2U:
            m.logic(r.data[1], 8U); t.byte(r.address[1], static_cast<std::uint8_t>(r.data[1]));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
