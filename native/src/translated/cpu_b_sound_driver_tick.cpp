// Implemented but unverified. Source-backed driver timing; no new validation.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sound_driver_tick(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x1707eU: case 0x17082U: {
            t.prefetch(pc + 4U);
            const auto reg = pc == 0x1707eU ? 0U : 1U;
            const auto value = pc == 0x1707eU ? 0x14U : 0x15U;
            m.dw(reg, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17086U: case 0x170aaU: case 0x170b4U: case 0x170d0U: {
            const auto id = pc == 0x17086U ? 320U : pc == 0x170aaU ? 480U :
                pc == 0x170b4U ? 481U : 497U;
            const auto target = pc == 0x17086U ? 0x18010U : pc == 0x170aaU ? 0x17b86U :
                pc == 0x170b4U ? 0x17bdeU : 0x170deU;
            const auto child = t.bsr(pc, id, target, pc + (pc == 0x170d0U ? 2U : 4U));
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x1708aU: case 0x17096U: case 0x170a4U: case 0x170aeU: {
            const auto offset = pc == 0x1708aU ? 0x8aU : pc == 0x17096U ? 0x86U :
                pc == 0x170a4U ? 0x80U : 0x20U;
            t.prefetch(pc + 4U);
            const auto value = t.word(r.address[6] + offset); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1708eU: next = t.branch(pc, 0x17096U, (r.status & 4U) != 0U); break;
        case 0x1709aU: next = t.branch(pc, 0x170a4U, (r.status & 4U) != 0U); break;
        case 0x170a8U: next = t.branch(pc, 0x170aeU, (r.status & 4U) != 0U); break;
        case 0x170b2U: next = t.branch(pc, 0x170b8U, (r.status & 4U) != 0U); break;
        case 0x17090U: case 0x1709cU: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto address = r.address[6] + (pc == 0x17090U ? 0x8aU : 0x86U);
            const auto old = t.word(address);
            const auto value = m.sub(old, 1U, 16U);
            t.prefetch(pc + 8U); t.word(address, static_cast<std::uint16_t>(value));
            next = pc + 6U; break;
        }
        case 0x170a2U: next = t.branch(pc, 0x170aeU, true); break;
        case 0x170b8U: case 0x170d2U:
            r.address[3] = pc == 0x170b8U ? r.address[6] + 0x180U : r.address[3] + 0x50U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x170bcU:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            m.logic(0x12U, 8U); t.byte(r.address[6] + 0xf0U, 0x12U);
            t.prefetch(pc + 8U); next = 0x170c2U; break;
        case 0x170c2U: case 0x170c8U: {
            t.prefetch(pc + 4U);
            const auto value = t.byte(r.address[3] + (pc == 0x170c2U ? 0U : 2U));
            if (pc == 0x170c8U) m.db(7U, value);
            m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x170c6U: next = t.branch(pc, 0x170d2U, (r.status & 8U) == 0U); break;
        case 0x170ccU: {
            t.prefetch(pc + 4U);
            const auto value = r.data[7] & 7U; m.logic(value, 8U); m.db(7U, value);
            t.prefetch(pc + 6U); next = 0x170d0U; break;
        }
        case 0x170d6U: {
            t.prefetch(pc + 4U);
            const auto address = r.address[6] + 0xf0U;
            const auto old = t.byte(address); const auto value = m.sub(old, 1U, 8U);
            t.prefetch(pc + 6U); t.byte(address, static_cast<std::uint8_t>(value));
            next = 0x170daU; break;
        }
        case 0x170daU: next = t.branch(pc, 0x170c2U, (r.status & 4U) == 0U); break;
        case 0x170dcU: return t.rts(pc);
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
