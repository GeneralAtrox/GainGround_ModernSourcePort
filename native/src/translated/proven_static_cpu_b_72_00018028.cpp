// Implemented but unverified. Source-backed sound table lookup timing.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00018028(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x18028U:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(r.data[0], 16U);
            t.prefetch(pc + 8U); m.logic(r.data[0] & 0xffffU, 32U);
            t.clocks(2U); r.data[0] &= 0xffffU; t.clocks(2U); next = pc + 6U; break;
        case 0x1802eU: case 0x18036U: {
            const bool directory = pc == 0x1802eU;
            const auto address = directory ? r.address[5] + r.data[1] :
                r.address[0] + static_cast<std::int16_t>(r.data[0]);
            t.clocks(2U); t.prefetch(pc + 4U);
            const auto value = t.word(address); m.dw(directory ? 1U : 0U, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x18032U: {
            const auto value = r.address[5] + r.data[1];
            t.clocks(2U); t.prefetch(pc + 4U); r.address[0] = value;
            t.clocks(2U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1803aU: {
            const auto value = r.address[0] + r.data[0];
            r.address[0] = (r.address[0] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[0] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x1803cU: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
