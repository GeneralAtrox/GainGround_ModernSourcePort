// Implemented but unverified. Original task-record copy at 8872..8890.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_initialize_task_record(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x8872U:
            r.data[0] = 0U; m.logic(0U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x8874U: {
            const auto source = r.address[1]; r.address[1] += 2U;
            const auto value = t.word(source); m.dw(0U, value); m.logic(value, 16U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8876U: {
            const auto value = r.data[0];
            m.logic(value, 16U); t.prefetch(pc + 4U); m.logic(value, 32U);
            t.word(0x822U, static_cast<std::uint16_t>(value >> 16U));
            t.word(0x824U, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x887aU:
            r.address[0] = 0x1400U; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = pc + 4U; break;
        case 0x887eU:
            m.dw(0U, m.sub(r.data[0], 1U, 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x8880U: {
            const auto source = r.address[1]; const auto high = t.word(source);
            r.address[1] += 4U; const auto low = t.word(source + 2U);
            r.address[2] = (std::uint32_t(high) << 16U) | low;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8882U:
            t.prefetch(pc + 4U); m.dw(1U, 0x1fU); m.logic(0x1fU, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x8886U: {
            const auto source = r.address[2]; const auto high = t.word(source);
            r.address[2] += 4U; const auto low = t.word(source + 2U);
            const auto destination = r.address[0];
            m.logic(low, 16U); t.word(destination, high);
            m.logic((std::uint32_t(high) << 16U) | low, 32U);
            t.word(destination + 2U, low); r.address[0] += 4U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8888U: next = t.dbf(pc, 0x8886U, 1U); break;
        case 0x888cU: next = t.dbf(pc, 0x8880U); break;
        case 0x8890U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
