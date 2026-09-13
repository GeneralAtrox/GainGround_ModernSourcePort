// Implemented but unverified. Original input read and transition-byte updates.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sample_input_word_irq_buffer(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x81c8U: {
            const auto source = r.address[1]; r.address[1] += 2U;
            const auto value = t.word(source); m.dw(0U, value); m.logic(value, 16U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x81caU: {
            t.prefetch(pc + 4U); const auto value = t.byte(r.address[0] + 1U);
            m.db(1U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x81ceU:
            m.logic(r.data[1], 8U); t.byte(r.address[0], static_cast<std::uint8_t>(r.data[1]));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x81d0U:
            m.db(2U, r.data[0]); m.logic(r.data[2], 8U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x81d2U: case 0x81deU: {
            const auto reg = pc == 0x81d2U ? 0U : 1U;
            m.db(reg, ~r.data[reg]); m.logic(r.data[reg], 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x81d4U: case 0x81daU: case 0x81e2U: {
            const auto reg = pc == 0x81daU ? 2U : 0U;
            const auto offset = pc == 0x81d4U ? 1U : pc == 0x81daU ? 3U : 2U;
            t.prefetch(pc + 4U); m.logic(r.data[reg], 8U);
            t.byte(r.address[0] + offset, static_cast<std::uint8_t>(r.data[reg]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x81d8U: case 0x81e0U: {
            const auto reg = pc == 0x81d8U ? 2U : 0U;
            m.db(reg, r.data[reg] & r.data[1]); m.logic(r.data[reg], 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x81e6U: {
            const auto value = r.address[0] + 4U;
            r.address[0] = (r.address[0] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[0] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x81e8U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
