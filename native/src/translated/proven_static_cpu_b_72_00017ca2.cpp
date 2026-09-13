// Implemented but unverified. Original sound counter/stream-index correction.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017ca2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17ca2U: case 0x17cb0U: case 0x17cb4U: {
            const auto offset = pc == 0x17ca2U ? 0x20U : pc == 0x17cb0U ? 0x24U : 0x22U;
            t.prefetch(pc + 4U); const auto value = t.word(r.address[0] + offset);
            m.dw(pc == 0x17cb4U ? 4U : 3U, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17ca6U:
            t.prefetch(pc + 4U); (void)m.sub(r.data[3], 0U, 16U, true);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17caaU: next = t.branch_word(pc, 0x17cb0U, (r.status & 4U) != 0U); break;
        case 0x17cb8U:
            (void)m.sub(r.data[4], r.data[3], 16U, true);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17cbaU: next = t.branch_word(pc, 0x17cc0U, (r.status & 4U) == 0U); break;
        case 0x17cc0U:
            t.prefetch(pc + 4U); m.logic(r.data[4], 16U);
            t.word(r.address[0] + 0x24U, static_cast<std::uint16_t>(r.data[4]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17caeU: case 0x17cbeU: case 0x17cc4U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
