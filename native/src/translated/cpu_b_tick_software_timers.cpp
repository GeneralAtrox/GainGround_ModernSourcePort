// Implemented but unverified. Original IRQ software timer updates.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_tick_software_timers(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x81eaU: case 0x820eU:
            r.address[0] = pc == 0x81eaU ? 0x7b6cU : 0x7b32U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x81eeU: case 0x8238U: {
            t.prefetch(pc + 4U); const auto value = t.byte(pc == 0x81eeU ? 0x820U : 0x821U);
            m.db(0U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x81f2U: case 0x81f4U: case 0x8236U: {
            const auto reg = pc == 0x81f2U ? 1U : pc == 0x81f4U ? 2U : 0U;
            r.data[reg] = pc == 0x81f4U ? 2U : 0U; m.logic(r.data[reg], 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x81f6U: {
            const auto mask = 1U << (r.data[1] & 31U); const auto value = r.data[0];
            t.prefetch(pc + 4U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((value & mask) ? 0U : 4U));
            t.clocks(2U); next = pc + 2U; break;
        }
        case 0x81f8U: next = t.branch(pc, 0x8206U, (r.status & 4U) != 0U); break;
        case 0x81faU: case 0x8212U: case 0x8228U: case 0x823eU: {
            const auto address = r.address[0]; const auto old = t.word(address);
            const auto value = m.sub(old, pc == 0x823eU ? r.data[0] : 2U, 16U);
            t.prefetch(pc + 4U); t.word(address, static_cast<std::uint16_t>(value)); next = pc + 2U; break;
        }
        case 0x81fcU: case 0x8214U: case 0x822aU: case 0x8240U: {
            const auto target = pc == 0x81fcU ? 0x8206U : pc == 0x8214U ? 0x821eU :
                pc == 0x822aU ? 0x8234U : 0x824aU;
            const bool greater = !(r.status & 4U) && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U;
            next = t.branch(pc, target, greater); break;
        }
        case 0x81feU: case 0x8216U: case 0x822cU: case 0x8242U: {
            t.prefetch(pc + 4U); const auto address = r.address[0];
            const auto value = m.add(t.word(address), 0x73U, 16U);
            t.prefetch(pc + 6U); t.word(address, static_cast<std::uint16_t>(value)); next = pc + 4U; break;
        }
        case 0x8202U: {
            t.prefetch(pc + 4U); const auto address = r.address[0] + 2U;
            const auto value = m.add(t.word(address), 1U, 16U);
            t.prefetch(pc + 6U); t.word(address, static_cast<std::uint16_t>(value)); next = pc + 4U; break;
        }
        case 0x8206U: case 0x8226U: case 0x8234U: {
            const auto value = r.address[0] + (pc == 0x8206U ? 4U : 6U);
            r.address[0] = (r.address[0] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[0] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x8208U:
            m.dw(1U, m.add(r.data[1], 1U, 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x820aU: next = t.dbf(pc, 0x81f6U, 2U); break;
        case 0x821aU: case 0x8230U: case 0x8246U: {
            t.prefetch(pc + 4U); const auto address = r.address[0] + 2U;
            const auto old = t.lng(address);
            (void)m.add(old, 1U, 16U); t.prefetch(pc + 6U);
            // morl2 merges the high-word flags before writing the low word.
            const auto value = m.add(old, 1U, 32U);
            t.word(address + 2U, static_cast<std::uint16_t>(value));
            t.word(address, static_cast<std::uint16_t>(value >> 16U)); next = pc + 4U; break;
        }
        case 0x821eU: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); const auto value = t.byte(0x820U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((value & 8U) ? 0U : 4U));
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x8224U: next = t.branch(pc, 0x824aU, (r.status & 4U) != 0U); break;
        case 0x823cU:
            m.dw(0U, m.add(r.data[0], r.data[0], 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x824aU: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
