// Implemented but unverified. Original pitch-command stream timing.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017234(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17234U: case 0x172a4U: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto value = t.byte(r.address[3] + (pc == 0x17234U ? 1U : 0U));
            const auto bit = pc == 0x17234U ? 0x10U : 4U;
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((value & bit) ? 0U : 4U));
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x1723eU: case 0x17248U: case 0x17250U: case 0x1725aU: {
            const auto reg = pc == 0x17248U || pc == 0x1725aU ? 1U : 0U;
            r.data[reg] = pc == 0x17248U ? 8U : 0U; m.logic(r.data[reg], 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17240U: case 0x17252U: {
            t.prefetch(pc + 4U);
            const auto value = t.byte(r.address[3] + (pc == 0x17240U ? 9U : 0x14U));
            m.db(0U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17244U:
            m.db(0U, m.sub(r.data[0], 1U, 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17246U:
            m.dw(0U, m.add(r.data[0], r.data[0], 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x1724aU: {
            const auto child = t.bsr(pc, 485U, 0x18028U, 0x1724eU);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x1724eU:
            r.address[1] = r.address[0]; t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17256U: case 0x17280U: {
            const auto address = r.address[3] + 0x14U;
            t.prefetch(pc + 4U); const auto old = t.byte(address);
            const auto value = pc == 0x17256U ? m.add(old, 1U, 8U) : m.sub(old, 1U, 8U);
            t.prefetch(pc + 6U); t.byte(address, static_cast<std::uint8_t>(value));
            next = pc + 4U; break;
        }
        case 0x1725cU: {
            const auto address = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            t.clocks(2U); t.prefetch(pc + 4U); const auto value = t.byte(address);
            m.db(1U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17260U: case 0x1726cU: case 0x17278U:
            t.prefetch(pc + 4U);
            (void)m.sub(r.data[1], pc == 0x17260U ? 0x80U : pc == 0x1726cU ? 0x83U : 0x81U, 8U, true);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17286U: case 0x1728cU: case 0x1729cU:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(0U, 8U);
            t.byte(r.address[3] + (pc == 0x17286U ? 9U : 0x14U), 0U);
            t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x17294U: case 0x17298U: {
            const auto address = r.address[1]++;
            const auto value = t.byte(address); m.db(0U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17296U: case 0x172b8U: {
            const auto reg = pc == 0x17296U ? 0U : 1U;
            const auto count = pc == 0x17296U ? 8U : 2U;
            auto value = r.data[reg] & 0xffffU; m.logic(value, 16U); t.prefetch(pc + 4U);
            for (unsigned i = 0U; i < count; ++i) {
                const bool carry = (value & 0x8000U) != 0U;
                value = (value << 1U) & 0xffffU; m.logic(value, 16U);
                r.status = static_cast<std::uint16_t>((r.status & ~0x11U) | (carry ? 0x11U : 0U));
                t.clocks(2U);
            }
            m.dw(reg, value); t.clocks(2U); next = pc + 2U; break;
        }
        case 0x1729aU: {
            const auto value = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            r.address[1] = (r.address[1] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[1] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x172aeU:
            m.logic(r.data[1], 8U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x172b4U:
            t.prefetch(pc + 4U); m.dw(1U, r.data[1] | 0xff00U); m.logic(r.data[1], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x172baU: {
            t.prefetch(pc + 4U); const auto value = t.word(r.address[3] + 0x10U);
            m.dw(1U, m.add(r.data[1], value, 16U)); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1723aU: case 0x172aaU: next = t.branch_word(pc, 0x17324U, (r.status & 4U) == 0U); break;
        case 0x17264U: next = t.branch_word(pc, 0x172a4U, (r.status & 1U) != 0U); break;
        case 0x17268U: next = t.branch_word(pc, 0x17286U, (r.status & 4U) != 0U); break;
        case 0x17270U: next = t.branch_word(pc, 0x172a4U, (r.status & 5U) == 0U); break;
        case 0x17274U: next = t.branch_word(pc, 0x1728cU, (r.status & 4U) != 0U); break;
        case 0x1727cU: next = t.branch_word(pc, 0x17294U, (r.status & 4U) != 0U); break;
        case 0x172a2U: next = t.branch(pc, 0x17250U, true); break;
        case 0x172b0U: next = t.branch_word(pc, 0x172b8U, (r.status & 8U) == 0U); break;
        case 0x172beU:
            next = t.branch_word(pc, 0x18088U, true); return t.transfer(pc, 489U, next);
        case 0x17284U: case 0x17292U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (next == 0x17324U) return t.transfer(pc, 544U, next);
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
