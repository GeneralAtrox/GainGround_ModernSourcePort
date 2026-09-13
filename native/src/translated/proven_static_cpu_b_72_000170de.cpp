// Implemented but unverified. Source-backed sound-channel instruction phases.
#include "cpu_b_irq_timing.h"
#include "gain_ground/sound_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_000170de(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    TimingCpuPosition fallback;
    auto *position = c.host->instruction_timing_position(c.cpu);
    if (!position) position = &fallback;
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        // These four original instructions embed the same YM polling/write
        // sequence as the sound writers. Do not invent a child call or RTS.
        if (pc == 0x1714cU || pc == 0x17154U || pc == 0x17156U || pc == 0x1715cU) {
            const auto operation = pc == 0x1714cU ? SoundInstructionOps::Operation::btst :
                pc == 0x17154U ? SoundInstructionOps::Operation::bne : SoundInstructionOps::Operation::move;
            const auto reg = pc == 0x1715cU ? 1U : 0U;
            t.stop();
            std::optional<FunctionResult> transfer;
            if (!sound_timing::instruction(c, *position, operation, pc, reg,
                    reg == 0U ? 0x800101U : 0x800103U, &transfer)) {
                if (transfer) return *transfer;
                return {TranslationStatus::contract_violation, 0U, r.program_counter};
            }
            // The instruction helper already delivered the latched live IRQ.
            // Retained fixture hosts still own their boundary events.
            if (!c.host->resumes_interrupts_inline())
                if (const auto event = m.interrupt(c, pc, r.program_counter)) return *event;
            t.begin(r.program_counter);
            continue;
        }
        switch (pc) {
        case 0x170deU: case 0x170f0U: case 0x17126U: case 0x171a4U:
        case 0x171c0U: case 0x172c4U: case 0x172dcU: {
            const auto offset = pc == 0x170deU ? 0xfU : pc == 0x170f0U ? 0xeU :
                pc == 0x171a4U ? 0xaU : pc == 0x171c0U ? 0x15U : pc == 0x172c4U ? 0x31U : 0x32U;
            const auto reg = pc == 0x17126U ? 2U : pc == 0x172dcU ? 1U : 0U;
            t.prefetch(pc + 4U); const auto value = t.byte(r.address[3] + offset);
            m.db(reg, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x170e2U: case 0x170f4U: case 0x172c8U:
            m.db(0U, m.add(r.data[0], 1U, 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x171b2U:
            m.db(0U, m.sub(r.data[0], 1U, 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x1712cU:
            m.db(2U, m.sub(0U, r.data[2], 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x170e4U: case 0x170f6U: case 0x1712eU: case 0x172d8U: {
            const auto offset = pc == 0x170e4U ? 0xfU : pc == 0x170f6U ? 0xeU : pc == 0x1712eU ? 0x32U : 0x31U;
            const auto reg = pc == 0x1712eU ? 2U : 0U;
            t.prefetch(pc + 4U); m.logic(r.data[reg], 8U);
            t.byte(r.address[3] + offset, static_cast<std::uint8_t>(r.data[reg]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x170e8U: case 0x170faU: case 0x172caU: {
            const auto offset = pc == 0x170e8U ? 0xdU : pc == 0x170faU ? 3U : 0x30U;
            t.prefetch(pc + 4U); const auto value = t.byte(r.address[3] + offset);
            (void)m.sub(r.data[0], value, 8U, true); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17106U: case 0x17198U:
            t.prefetch(pc + 4U); m.logic(t.byte(r.address[3] + (pc == 0x17106U ? 0xfU : 9U)), 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1710eU: case 0x1714aU: case 0x171b6U: case 0x171beU:
        case 0x171c8U: case 0x172c2U: case 0x172d6U: {
            const auto reg = pc == 0x171b6U || pc == 0x171c8U || pc == 0x172c2U ? 1U : 0U;
            r.data[reg] = pc == 0x1714aU ? 8U : pc == 0x171b6U ? 0xaU : 0U;
            m.logic(r.data[reg], 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17110U: case 0x17114U: case 0x17132U: case 0x172eeU: {
            const auto offset = pc == 0x17110U ? 0x14U : pc == 0x17114U ? 0xeU : 0x34U;
            const auto reg = pc == 0x172eeU ? 1U : 0U;
            t.prefetch(pc + 4U); m.logic(r.data[reg], 16U);
            t.word(r.address[3] + offset, static_cast<std::uint16_t>(r.data[reg]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17118U: case 0x17136U: case 0x17140U: case 0x17162U:
        case 0x17178U: case 0x17182U: case 0x1718eU: case 0x17212U: {
            const auto bit = pc == 0x17118U || pc == 0x1718eU ? 5U :
                pc == 0x17136U || pc == 0x17212U ? 2U : pc == 0x17140U || pc == 0x17182U ? 4U : 7U;
            const auto offset = pc == 0x17118U || pc == 0x17182U || pc == 0x1718eU ? 1U :
                pc == 0x17162U || pc == 0x17178U ? 0x10U : 0U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto value = t.byte(r.address[3] + offset);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((value & (1U << bit)) ? 0U : 4U));
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x17120U: {
            t.prefetch(pc + 4U); const auto value = t.byte(r.address[3] + 0x33U);
            m.logic(value, 8U); t.prefetch(pc + 6U); t.byte(r.address[3] + 0x31U, value);
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x17148U: case 0x1722eU: {
            const auto value = r.data[pc == 0x17148U ? 7U : 1U];
            m.db(pc == 0x17148U ? 1U : 2U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17172U: case 0x171f4U: case 0x171faU: case 0x1720aU: {
            const auto offset = pc == 0x17172U ? 0xfU : pc == 0x171f4U ? 0xaU : 0x15U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(0U, 8U);
            t.byte(r.address[3] + offset, 0U); t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x171aeU: case 0x17222U: case 0x17228U: case 0x172e4U: case 0x172f6U: {
            const auto reg = pc == 0x171aeU ? 0U : 1U;
            const auto immediate = pc == 0x171aeU ? 0xffU : pc == 0x17222U ? 0xffc0U :
                pc == 0x17228U ? 0x3fU : pc == 0x172e4U ? 0xff80U : 0x7fffU;
            t.prefetch(pc + 4U);
            const auto value = pc == 0x17222U || pc == 0x172e4U ?
                r.data[reg] | immediate : r.data[reg] & immediate;
            m.dw(reg, value); m.logic(value, 16U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x171b4U: case 0x1722cU: case 0x172ecU: {
            const auto destination = pc == 0x171b4U ? 0U : 1U;
            const auto source = pc == 0x1722cU ? 1U : 0U;
            m.dw(destination, m.add(r.data[destination], r.data[source], 16U));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x171bcU:
            r.address[1] = r.address[0]; t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x171c4U: case 0x171eeU: case 0x172d2U: {
            const auto address = r.address[3] + (pc == 0x172d2U ? 0x32U : 0x15U);
            t.prefetch(pc + 4U); const auto old = t.byte(address);
            const auto value = pc == 0x171c4U ? m.add(old, 1U, 8U) :
                pc == 0x171eeU ? m.sub(old, 1U, 8U) : m.sub(0U, old, 8U);
            t.prefetch(pc + 6U); t.byte(address, static_cast<std::uint8_t>(value));
            next = pc + 4U; break;
        }
        case 0x171caU: {
            const auto address = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            t.clocks(2U); t.prefetch(pc + 4U); const auto value = t.byte(address);
            m.db(1U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x171ceU: case 0x171daU: case 0x171e6U:
            t.prefetch(pc + 4U);
            (void)m.sub(r.data[1], pc == 0x171ceU ? 0x80U : pc == 0x171daU ? 0x83U : 0x81U, 8U, true);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17202U: case 0x17206U: {
            const auto address = r.address[1]++;
            const auto value = t.byte(address); m.db(0U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17204U: {
            auto value = r.data[0] & 0xffffU;
            m.logic(value, 16U); t.prefetch(pc + 4U);
            for (unsigned count = 0U; count < 8U; ++count) {
                const bool carry = (value & 0x8000U) != 0U;
                value = (value << 1U) & 0xffffU; m.logic(value, 16U);
                r.status = static_cast<std::uint16_t>((r.status & ~0x11U) | (carry ? 0x11U : 0U));
                t.clocks(2U);
            }
            m.dw(0U, value); t.clocks(2U); next = pc + 2U; break;
        }
        case 0x17208U: {
            const auto value = r.address[1] + static_cast<std::int16_t>(r.data[0]);
            r.address[1] = (r.address[1] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[1] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x1721cU:
            m.logic(r.data[1], 8U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x172e8U: case 0x172f2U: {
            t.prefetch(pc + 4U);
            const auto value = t.word(r.address[3] + (pc == 0x172e8U ? 0x34U : 0x10U));
            if (pc == 0x172e8U) { m.dw(0U, value); m.logic(value, 16U); }
            else m.dw(1U, m.add(r.data[1], value, 16U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17102U: case 0x1716cU: case 0x1718aU: case 0x171a0U: case 0x171b8U: case 0x172faU: {
            const auto id = pc == 0x17102U ? 477U : pc == 0x1716cU ? 487U : pc == 0x1718aU ? 476U :
                pc == 0x171a0U ? 475U : pc == 0x171b8U ? 485U : 489U;
            const auto target = pc == 0x17102U ? 0x17326U : pc == 0x1716cU ? 0x18046U :
                pc == 0x1718aU ? 0x17302U : pc == 0x171a0U ? 0x17234U : pc == 0x171b8U ? 0x18028U : 0x18088U;
            const auto child = t.bsr(pc, id, target, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x171acU: case 0x171f2U: case 0x17200U: return t.rts(pc);
        case 0x170ecU: next = t.branch_word(pc, 0x17178U, (r.status & 4U) == 0U); break;
        case 0x170feU: next = t.branch_word(pc, 0x17172U, (r.status & 4U) == 0U); break;
        case 0x1710aU: next = t.branch_word(pc, 0x17324U, (r.status & 4U) != 0U); break;
        case 0x1711eU: next = t.branch(pc, 0x17136U, (r.status & 4U) != 0U); break;
        case 0x1712aU: next = t.branch(pc, 0x17132U, (r.status & 8U) == 0U); break;
        case 0x1713cU: case 0x17168U: case 0x1717eU: case 0x17218U:
            next = t.branch_word(pc, 0x17324U, (r.status & 4U) == 0U); break;
        case 0x17146U: next = t.branch(pc, 0x17162U, (r.status & 4U) == 0U); break;
        case 0x17170U: next = t.branch(pc, 0x17182U, true); break;
        case 0x17188U: next = t.branch(pc, 0x1718eU, (r.status & 4U) != 0U); break;
        case 0x17194U: next = t.branch_word(pc, 0x172c2U, (r.status & 4U) == 0U); break;
        case 0x1719cU: next = t.branch_word(pc, 0x171a4U, (r.status & 4U) != 0U); break;
        case 0x171a8U: next = t.branch_word(pc, 0x171aeU, (r.status & 4U) == 0U); break;
        case 0x171d2U: next = t.branch_word(pc, 0x17212U, (r.status & 1U) != 0U); break;
        case 0x171d6U: next = t.branch_word(pc, 0x171f4U, (r.status & 4U) != 0U); break;
        case 0x171deU: next = t.branch_word(pc, 0x17212U, (r.status & 5U) == 0U); break;
        case 0x171e2U: next = t.branch_word(pc, 0x171faU, (r.status & 4U) != 0U); break;
        case 0x171eaU: next = t.branch_word(pc, 0x17202U, (r.status & 4U) != 0U); break;
        case 0x17210U: next = t.branch(pc, 0x171beU, true); break;
        case 0x1721eU: next = t.branch_word(pc, 0x17228U, (r.status & 8U) == 0U); break;
        case 0x17226U: next = t.branch(pc, 0x1722cU, true); break;
        case 0x17230U: next = t.branch_word(pc, 0x180a8U, true); break;
        case 0x172ceU: next = t.branch_word(pc, 0x172d8U, (r.status & 4U) == 0U); break;
        case 0x172e0U: next = t.branch_word(pc, 0x172e8U, (r.status & 8U) == 0U); break;
        case 0x172feU: next = t.branch_word(pc, 0x171a4U, true); break;
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (next == 0x17324U) return t.transfer(pc, 544U, next);
        if (next == 0x180a8U) return t.transfer(pc, 490U, next);
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
