// Implemented but unverified. Original channel sound-register initialization.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017d9e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17d9eU: case 0x17da2U: case 0x17daeU: case 0x17ddcU: {
            const auto reg = pc == 0x17da2U ? 1U : 0U;
            const auto value = pc == 0x17d9eU ? 0x20U : pc == 0x17daeU ? 8U : 0U;
            r.data[reg] = value; m.logic(value, 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17da0U:
            m.db(0U, m.add(r.data[0], r.data[7], 8U));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17da4U: case 0x17db0U: case 0x17dbcU: case 0x17dccU: {
            const auto child = t.bsr(pc, 319U, 0x17ec2U, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x17da8U: {
            const auto child = t.bsr(pc, 492U, 0x1814eU, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x17dacU:
            m.db(1U, r.data[7]); m.logic(r.data[1], 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17db4U: case 0x17db8U: {
            const auto reg = pc == 0x17db4U ? 0U : 1U;
            const auto value = pc == 0x17db4U ? 0xfU : 0U;
            t.prefetch(pc + 4U); m.db(reg, value); m.logic(value, 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17dc0U:
            r.address[1] = 0x17ef2U; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = pc + 4U; break;
        case 0x17dc4U:
            t.prefetch(pc + 4U); m.dw(2U, 2U); m.logic(2U, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17dc8U: case 0x17dcaU: {
            const auto source = r.address[1]++;
            const auto value = t.byte(source);
            m.db(pc == 0x17dc8U ? 0U : 1U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17dd0U: next = t.dbf(pc, 0x17dc8U, 2U); break;
        case 0x17dd4U:
            t.prefetch(pc + 4U); (void)m.sub(r.data[7], 4U, 8U, true);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17dd8U:
            next = t.branch_word(pc, 0x17ca0U, !(r.status & 1U));
            if (next == 0x17ca0U) return t.transfer(pc, 546U, next);
            break;
        case 0x17ddeU: case 0x17de0U:
            m.dw(7U, m.add(r.data[7], r.data[7], 16U));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17de2U: case 0x17de6U: {
            const auto destination = r.address[6] + static_cast<std::int16_t>(r.data[7])
                + (pc == 0x17de2U ? 0x30U : 0x40U);
            const auto value = r.data[0]; const auto high = static_cast<std::uint16_t>(value >> 16U);
            t.clocks(2U); t.prefetch(pc + 4U);
            // rmdl2 merges the high word's N/Z while preserving V/C and old Z.
            r.status = static_cast<std::uint16_t>((r.status & ~8U & (high == 0U ? 0xffffU : ~4U))
                | ((high & 0x8000U) ? 8U : 0U));
            t.word(destination, high); m.logic(value, 16U);
            t.word(destination + 2U, static_cast<std::uint16_t>(value)); m.logic(value, 32U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17deaU: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
