// Implemented but unverified. Original state-72 startup at 84f2..856c.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_state72_startup(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x84f2U: {
            t.prefetch(pc + 4U); const auto value = t.byte(0x403U);
            m.logic(value, 8U); t.prefetch(pc + 6U);
            t.byte(0x41aU, value); t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x84f8U: {
            if (c.host->replaces_boot_calibration()) {
                // Existing approved direct-loading ABI: omit the speed probe,
                // retain the countdown initialization and boot-mode byte.
                // This is a loading replacement, not a timed original BSR.
                t.stop(); m.byte(0x502U, 1U); m.logic(1U, 8U);
                next = 0x851aU; break;
            }
            const auto child = t.bsr(pc, 117U, 0x85acU, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x84fcU: case 0x8544U: case 0x8550U:
            r.data[0] = pc == 0x8550U ? 0x3bU : 0U; m.logic(r.data[0], 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x84feU: {
            const auto before = r.data[0];
            m.dw(0U, m.add(before, 1U, 16U)); t.prefetch(pc + 4U);
            const auto value = m.add(before, 1U, 32U);
            t.clocks(2U); r.data[0] = value; t.clocks(2U); next = pc + 2U; break;
        }
        case 0x8500U: {
            t.prefetch(pc + 4U); const auto value = t.byte(0x502U);
            m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x8504U: next = t.branch(pc, 0x84feU, (r.status & 8U) == 0U); break;
        case 0x8506U: case 0x8514U:
            t.prefetch(pc + 4U); m.logic(1U, 8U); t.prefetch(pc + 6U);
            t.byte(pc == 0x8506U ? 0x502U : 0x419U, 1U);
            t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x850cU:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            (void)m.sub(r.data[0], 0x2d17U, 32U, true);
            t.prefetch(pc + 8U); t.clocks(2U); next = pc + 6U; break;
        case 0x8512U: next = t.branch(pc, 0x851aU, (r.status & 1U) != 0U); break;
        case 0x851aU:
            r.address[1] = 0x86a2U; t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            next = pc + 4U; break;
        case 0x851eU: {
            const auto child = t.jsr_absolute(pc, 129U, 0x891eU, pc + 6U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x8524U: case 0x8528U: case 0x852cU: case 0x8552U: case 0x8562U: {
            const auto id = pc == 0x8524U ? 121U : pc == 0x8528U ? 122U : pc == 0x852cU ? 123U : 118U;
            const auto target = pc == 0x8524U ? 0x85f2U : pc == 0x8528U ? 0x8622U : pc == 0x852cU ? 0x8654U : 0x85b0U;
            const auto child = t.bsr(pc, id, target, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x8530U: case 0x8532U: case 0x8534U: case 0x8536U: case 0x8538U:
        case 0x853aU: case 0x853cU: case 0x853eU: case 0x8540U: case 0x8542U:
            // Preserve all ten original NOPs as separate instruction boundaries.
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x8546U: {
            t.prefetch(pc + 4U); const auto value = t.byte(0x419U);
            m.db(0U, r.data[0] | value); m.logic(r.data[0], 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x854aU: {
            const auto value = r.data[0];
            t.prefetch(pc + 4U); m.logic(value, 16U); t.prefetch(pc + 6U);
            m.logic(value, 32U); t.word(0x404018U, static_cast<std::uint16_t>(value >> 16U));
            t.word(0x40401aU, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x8556U: next = t.dbf(pc, 0x8552U); break;
        case 0x855aU:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(0x1fU, 8U);
            t.prefetch(pc + 8U); t.byte(0xd00035U, 0x1fU);
            t.prefetch(pc + 10U); next = pc + 8U; break;
        case 0x8566U: {
            t.prefetch(pc + 4U); const auto value = t.word(0xffff8002U);
            m.logic(value, 16U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x856aU: next = t.branch(pc, 0x8562U, (r.status & 4U) == 0U); break;
        case 0x856cU:
            t.prefetch(pc + 4U); t.prefetch(0x8806U); t.prefetch(0x8808U);
            return t.transfer(pc, 124U, 0x8806U);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
