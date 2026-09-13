// Implemented but unverified. Original runtime setup at 8806..884a.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_runtime_init(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x8806U: case 0x8814U: case 0x881aU:
        case 0x8820U: case 0x8826U: case 0x8830U: {
            const auto id = pc == 0x8806U ? 311U : pc == 0x8814U ? 117U :
                pc == 0x881aU ? 121U : pc == 0x8820U ? 122U : pc == 0x8826U ? 123U : 322U;
            const auto target = pc == 0x8806U ? 0x17054U : pc == 0x8814U ? 0x85acU :
                pc == 0x881aU ? 0x85f2U : pc == 0x8820U ? 0x8622U : pc == 0x8826U ? 0x8654U : 0x1826eU;
            const auto child = t.jsr_absolute(pc, id, target, pc + 6U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x882cU: case 0x8836U: case 0x883aU: {
            const auto id = pc == 0x882cU ? 125U : pc == 0x8836U ? 127U : 128U;
            const auto target = pc == 0x882cU ? 0x8850U : pc == 0x8836U ? 0x8892U : 0x891aU;
            const auto child = t.bsr(pc, id, target, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x880cU:
            // MOVE.W #1,abs.l: immediate/address fetches, flags, next fetch,
            // destination write and final prefetch (20 ordinary clocks).
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(1U, 16U);
            t.prefetch(pc + 8U); t.word(0x40401aU, 1U);
            t.prefetch(pc + 10U); next = pc + 8U; break;
        case 0x883eU:
            r.data[0] = 0U; m.logic(0U, 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x8840U: {
            t.prefetch(pc + 4U); const auto value = t.byte(0x419U);
            m.db(0U, r.data[0] | value); m.logic(r.data[0], 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x8844U: {
            // rall1/2/3: low-word NZVC precedes the second address fetch;
            // merged longword N/Z precedes both ordered destination writes.
            const auto value = r.data[0];
            t.prefetch(pc + 4U); m.logic(value, 16U); t.prefetch(pc + 6U);
            m.logic(value, 32U); t.word(0x404018U, static_cast<std::uint16_t>(value >> 16U));
            t.word(0x40401aU, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x884aU:
            // JMP abs.l uses the actual frame-loop entry and pushes no return.
            t.prefetch(pc + 4U); t.prefetch(0x8572U); t.prefetch(0x8574U);
            return t.transfer(pc, 116U, 0x8572U);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
