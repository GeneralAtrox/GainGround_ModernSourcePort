// Implemented but unverified. Original pitch conversion and YM writes.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00018088(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x18088U:
            m.dw(2U, r.data[1]); m.logic(r.data[2], 16U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x1808aU: {
            auto value = r.data[2] & 0xffffU;
            m.logic(value, 16U); t.prefetch(pc + 4U);
            for (unsigned count = 0U; count < 8U; ++count) {
                const bool carry = (value & 1U) != 0U;
                value >>= 1U; m.logic(value, 16U);
                r.status = static_cast<std::uint16_t>((r.status & ~0x11U) | (carry ? 0x11U : 0U));
                t.clocks(2U);
            }
            m.dw(2U, value); t.clocks(2U); next = pc + 2U; break;
        }
        case 0x1808cU:
            r.data[0] = 0x30U; m.logic(0x30U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x1808eU:
            m.db(0U, m.add(r.data[0], r.data[7], 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x18090U: {
            const auto child = t.bsr(pc, 320U, 0x18010U, 0x18094U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x18094U:
            t.prefetch(pc + 4U); m.dw(2U, r.data[2] & 0xffU); m.logic(r.data[2], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x18098U:
            r.address[0] = 0x181d6U; t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1809cU: {
            const auto address = r.address[0] + static_cast<std::int16_t>(r.data[2]);
            t.clocks(2U); t.prefetch(pc + 4U);
            const auto value = t.byte(address); m.db(1U, value); m.logic(value, 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x180a0U:
            t.prefetch(pc + 4U); m.db(0U, m.sub(r.data[0], 8U, 8U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x180a4U:
            next = t.branch_word(pc, 0x18010U, true);
            return t.transfer(pc, 320U, next);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
