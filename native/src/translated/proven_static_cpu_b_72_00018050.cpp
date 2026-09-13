// Implemented but unverified. Original instrument stream and register writes.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00018050(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x18050U: case 0x1805aU: case 0x1805eU: case 0x1806aU: case 0x18070U: {
            const auto reg = pc == 0x1805aU ? 1U : pc == 0x1806aU ? 3U : pc == 0x18070U ? 2U : 0U;
            const auto value = pc == 0x1805aU ? 6U : pc == 0x1805eU ? 0x20U : pc == 0x18070U ? 0x17U : 0U;
            r.data[reg] = value; m.logic(value, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x18052U: {
            t.prefetch(pc + 4U); const auto value = t.byte(r.address[3] + 0x16U);
            m.db(0U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x18056U:
            m.dw(0U, m.sub(r.data[0], 1U, 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x18058U:
            m.dw(0U, m.add(r.data[0], r.data[0], 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x1805cU: {
            const auto child = t.bsr(pc, 485U, 0x18028U, pc + 2U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x18060U:
            m.db(0U, m.add(r.data[0], r.data[7], 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x18062U: case 0x18072U: {
            const auto source = r.address[0]++;
            const auto value = t.byte(source); m.db(1U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x18064U:
            t.prefetch(pc + 4U); m.logic(r.data[1], 8U);
            t.byte(r.address[3] + 0x17U, static_cast<std::uint8_t>(r.data[1]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x18068U: case 0x18078U: {
            const auto child = t.bsr(pc, 320U, 0x18010U, pc + 2U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x1806cU: case 0x1807eU:
            t.prefetch(pc + 4U); m.db(0U, m.add(r.data[0], pc == 0x1806cU ? 0x20U : 8U, 8U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x18074U: {
            const auto destination = r.address[3] + static_cast<std::int16_t>(r.data[3]) + 0x18U;
            t.clocks(2U); t.prefetch(pc + 4U); m.logic(r.data[1], 8U);
            t.byte(destination, static_cast<std::uint8_t>(r.data[1]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1807aU:
            t.prefetch(pc + 4U); m.dw(3U, m.add(r.data[3], 1U, 16U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x18082U: next = t.dbf(pc, 0x18072U, 2U); break;
        case 0x18086U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
