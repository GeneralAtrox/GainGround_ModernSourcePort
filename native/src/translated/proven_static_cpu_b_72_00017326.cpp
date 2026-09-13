// Implemented but unverified. Original sound-command stream instruction timing.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017326(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17326U:
            t.prefetch(pc + 4U); r.address[4] = t.lng(r.address[3] + 4U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1732aU: case 0x173aaU: case 0x173acU: case 0x173b8U:
        case 0x17430U: case 0x17432U: case 0x1743eU: case 0x1744aU: {
            const bool decrement = pc == 0x173aaU || pc == 0x17430U;
            const auto value = pc == 0x1732aU ? r.address[4] + r.address[5] :
                r.address[4] - (decrement ? 1U : r.address[5]);
            r.address[4] = (r.address[4] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[4] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x1732cU: case 0x17370U: case 0x17380U: case 0x173ecU: case 0x173f6U: case 0x17450U: {
            const auto offset = pc == 0x1732cU || pc == 0x17370U ? 1U :
                pc == 0x173f6U || pc == 0x17450U ? 0x320U : 0U;
            const auto bit = pc == 0x1732cU ? 3U : pc == 0x17370U || pc == 0x173ecU ? 4U :
                pc == 0x17380U ? 2U : 7U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto value = t.byte(r.address[3] + offset);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((value & (1U << bit)) ? 0U : 4U));
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x17336U: case 0x17390U: case 0x173c0U: case 0x17414U: {
            const auto address = r.address[4]++;
            const auto value = t.byte(address); m.db(0U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x1733eU: case 0x17350U: case 0x17396U: case 0x173c8U: case 0x173dcU: case 0x1741cU:
            t.prefetch(pc + 4U);
            (void)m.sub(r.data[0], pc == 0x17350U || pc == 0x173dcU ? 0x80U : 0xe0U, 8U, true);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17348U: case 0x173a0U: case 0x173d2U: case 0x17426U:
            t.prefetch(pc + 4U); m.logic(t.byte(r.address[3] + 0xfU), 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17356U: case 0x173e4U: {
            const auto address = r.address[3] + 0x10U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); const auto old = t.byte(address);
            t.prefetch(pc + 8U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x80U) ? 0U : 4U));
            t.byte(address, static_cast<std::uint8_t>(old | 0x80U)); next = pc + 6U; break;
        }
        case 0x1735eU: case 0x173feU:
            t.prefetch(pc + 4U); m.db(0U, m.sub(r.data[0], pc == 0x1735eU ? 0x81U : 0x82U, 8U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17362U: {
            t.prefetch(pc + 4U); const auto value = t.byte(r.address[3] + 8U);
            m.db(0U, m.add(r.data[0], value, 8U)); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17366U: case 0x173b4U: case 0x17406U: case 0x1743aU: case 0x17446U: {
            const auto offset = pc == 0x17366U ? 0x10U : pc == 0x17406U ? 0x16U : 0xdU;
            t.prefetch(pc + 4U); m.logic(r.data[0], 8U);
            t.byte(r.address[3] + offset, static_cast<std::uint8_t>(r.data[0]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1736aU:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(0U, 8U);
            t.byte(r.address[3] + 0x11U, 0U); t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x17378U: case 0x1737cU: {
            const auto address = r.address[4]++;
            const auto value = t.byte(address); m.logic(value, 8U); t.prefetch(pc + 4U);
            t.byte(r.address[3] + (pc == 0x17378U ? 0x12U : 0x13U), value);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17388U: {
            t.prefetch(pc + 4U); const auto value = t.word(r.address[3] + 0x10U);
            m.dw(1U, value); m.logic(value, 16U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x173aeU: case 0x173baU: case 0x17434U: case 0x17440U: case 0x1744cU: {
            const auto value = r.address[4]; const auto high = value >> 16U;
            const auto address = r.address[3] + 4U;
            t.prefetch(pc + 4U);
            // rmdl2 merges high-word N/Z into the previous flags, rmdl3
            // replaces N/Z/V/C from the low word, rmrl3 merges the high again.
            r.status = static_cast<std::uint16_t>((r.status & ~8U & (high == 0U ? 0xffffU : ~4U)) |
                ((high & 0x8000U) ? 8U : 0U));
            t.word(address, static_cast<std::uint16_t>(high)); m.logic(value, 16U);
            t.word(address + 2U, static_cast<std::uint16_t>(value)); m.logic(value, 32U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17402U:
            t.prefetch(pc + 4U); m.dw(0U, r.data[0] & 0x3eU); m.logic(r.data[0], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1740aU: case 0x1745aU: case 0x17460U: {
            const auto reg = pc == 0x1745aU ? 0U : 1U;
            r.data[reg] = pc == 0x1745aU ? 0U : 2U; m.logic(r.data[reg], 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x1745cU: {
            t.prefetch(pc + 4U); const auto value = t.byte(r.address[3] + 0x16U);
            m.db(0U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17466U: {
            const auto address = r.address[0]; r.address[0] += 2U;
            m.logic(t.word(address), 16U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17468U:
            r.address[1] = r.address[3] + 0xa0U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17344U: case 0x1738cU: case 0x1739cU: case 0x173ceU:
        case 0x1740cU: case 0x17410U: case 0x17422U: case 0x17462U: case 0x1746cU: {
            const auto id = pc == 0x1738cU ? 489U : pc == 0x1740cU || pc == 0x17462U ? 485U :
                pc == 0x17410U ? 478U : pc == 0x1746cU ? 479U : 498U;
            const auto target = id == 489U ? 0x18088U : id == 485U ? 0x18028U :
                id == 478U ? 0x17472U : id == 479U ? 0x17488U : 0x174baU;
            const auto child = t.bsr(pc, id, target, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x173b2U: case 0x173beU: case 0x17438U: case 0x17444U: case 0x17470U: return t.rts(pc);
        case 0x17332U: next = t.branch_word(pc, 0x173c0U, (r.status & 4U) == 0U); break;
        case 0x17338U: next = t.branch(pc, 0x1732cU, (r.status & 4U) != 0U); break;
        case 0x1733aU: next = t.branch_word(pc, 0x173b4U, (r.status & 8U) == 0U); break;
        case 0x17342U: next = t.branch(pc, 0x17350U, (r.status & 1U) != 0U); break;
        case 0x1734cU: next = t.branch(pc, 0x17324U, (r.status & 4U) != 0U); break;
        case 0x1734eU: next = t.branch(pc, 0x1732cU, true); break;
        case 0x17354U: next = t.branch(pc, 0x1735eU, (r.status & 4U) == 0U); break;
        case 0x1735cU: next = t.branch(pc, 0x17390U, true); break;
        case 0x17376U: next = t.branch(pc, 0x17380U, (r.status & 4U) != 0U); break;
        case 0x17386U: next = t.branch(pc, 0x17390U, (r.status & 4U) == 0U); break;
        case 0x17392U: next = t.branch(pc, 0x17390U, (r.status & 4U) != 0U); break;
        case 0x17394U: next = t.branch(pc, 0x173b4U, (r.status & 8U) == 0U); break;
        case 0x1739aU: next = t.branch(pc, 0x173aaU, (r.status & 1U) != 0U); break;
        case 0x173a4U: case 0x173d6U: case 0x1742aU:
            next = t.branch_word(pc, 0x17324U, (r.status & 4U) != 0U); break;
        case 0x173a8U: next = t.branch(pc, 0x17390U, true); break;
        case 0x173c2U: next = t.branch(pc, 0x173c0U, (r.status & 4U) != 0U); break;
        case 0x173c4U: next = t.branch_word(pc, 0x17446U, (r.status & 8U) == 0U); break;
        case 0x173ccU: next = t.branch(pc, 0x173dcU, (r.status & 1U) != 0U); break;
        case 0x173daU: next = t.branch(pc, 0x173c0U, true); break;
        case 0x173e0U: next = t.branch_word(pc, 0x173ecU, (r.status & 4U) == 0U); break;
        case 0x173eaU: next = t.branch(pc, 0x17414U, true); break;
        case 0x173f2U: next = t.branch_word(pc, 0x17414U, (r.status & 4U) == 0U); break;
        case 0x173fcU: next = t.branch(pc, 0x17414U, (r.status & 4U) == 0U); break;
        case 0x17416U: next = t.branch(pc, 0x17414U, (r.status & 4U) != 0U); break;
        case 0x17418U: next = t.branch_word(pc, 0x1743aU, (r.status & 8U) == 0U); break;
        case 0x17420U: next = t.branch(pc, 0x17430U, (r.status & 1U) != 0U); break;
        case 0x1742eU: next = t.branch(pc, 0x17414U, true); break;
        case 0x17456U: next = t.branch_word(pc, 0x17324U, (r.status & 4U) == 0U); break;
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
