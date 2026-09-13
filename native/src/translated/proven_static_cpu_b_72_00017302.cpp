// Implemented but unverified. Original pitch accumulator instruction timing.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017302(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17302U: case 0x17306U: {
            t.prefetch(pc + 4U);
            const auto value = t.word(r.address[3] + (pc == 0x17302U ? 0x12U : 0x10U));
            m.dw(pc == 0x17302U ? 1U : 0U, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1730aU: case 0x17314U:
            m.dw(1U, m.add(r.data[1], r.data[0], 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x1730cU: case 0x17310U: {
            const auto reg = pc == 0x1730cU ? 1U : 0U;
            t.prefetch(pc + 4U); m.dw(reg, r.data[reg] & (reg == 1U ? 0x7fffU : 0x8000U));
            m.logic(r.data[reg], 16U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17316U:
            t.prefetch(pc + 4U); m.logic(r.data[1], 16U);
            t.word(r.address[3] + 0x10U, static_cast<std::uint16_t>(r.data[1]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x1731aU: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); const auto value = t.byte(r.address[3]);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((value & 4U) ? 0U : 4U));
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x17320U:
            next = t.branch_word(pc, 0x18088U, (r.status & 4U) != 0U);
            // The untaken BEQ reaches the shared RTS sequentially; do not
            // add taken-branch timing to that existing ownership handoff.
            return t.transfer(pc, next == 0x18088U ? 489U : 544U, next);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
