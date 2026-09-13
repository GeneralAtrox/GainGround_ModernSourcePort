// Implemented but unverified. Source-backed IRQ5 timing; no new validation.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_irq5_state72_frame_commit(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x80acU:
            r.data[1] = 0U; m.logic(0U, 32U);
            t.prefetch(pc + 4U); next = 0x80aeU; break;
        case 0x80aeU: {
            t.prefetch(pc + 4U);
            const auto old = t.byte(0x502U);
            const auto value = m.sub(old, 1U, 8U);
            t.prefetch(pc + 6U); t.byte(0x502U, static_cast<std::uint8_t>(value));
            next = 0x80b2U; break;
        }
        case 0x80b2U: next = t.branch(pc, 0x80beU, (r.status & 8U) == 0U); break;
        case 0x80b4U: case 0x80c8U: {
            t.prefetch(pc + 4U);
            const auto reg = pc == 0x80b4U ? 1U : 0U;
            const auto value = pc == 0x80b4U ? 0x100U : 0x4001U;
            m.dw(reg, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x80b8U: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto old = t.word(0x6c00U);
            const auto value = m.add(old, 0x8000U, 16U);
            t.prefetch(pc + 8U); t.word(0x6c00U, static_cast<std::uint16_t>(value));
            next = 0x80beU; break;
        }
        case 0x80beU: case 0x80e4U: case 0x811eU: case 0x8124U: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto reg = pc == 0x811eU ? 5U : pc == 0x8124U ? 6U : 0U;
            r.address[reg] = pc == 0x80beU ? 0x600000U : pc == 0x80e4U ? 0x20a000U :
                pc == 0x811eU ? 0xfb0000U : 0xffffc000U;
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x80c4U: case 0x80eaU:
            t.prefetch(pc + 4U); r.address[1] = pc == 0x80c4U ? 0x504U : 0x50eU;
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x80ccU: {
            t.prefetch(pc + 4U); const auto value = t.word(0x6c00U);
            m.logic(value, 16U); t.prefetch(pc + 6U); next = 0x80d0U; break;
        }
        case 0x80d0U: next = t.branch(pc, 0x80d6U, (r.status & 8U) != 0U); break;
        case 0x80d2U:
            t.prefetch(pc + 4U); m.dw(0U, m.add(r.data[0], 0x400U, 16U));
            t.prefetch(pc + 6U); next = 0x80d6U; break;
        case 0x80d6U: case 0x80e0U:
            m.dw(0U, m.add(r.data[0], pc == 0x80d6U ? r.data[1] : 8U, 16U));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x80d8U: case 0x80e2U:
            m.logic(r.data[0], 16U); t.word(r.address[0], static_cast<std::uint16_t>(r.data[0]));
            r.address[0] += 2U; t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x80daU: case 0x80dcU: case 0x80eeU: case 0x80f0U: case 0x80f2U: case 0x80f4U: {
            // MOVE.L (A1)+,(A0)+: pinl1/3, mmil1/2, mmiw2.
            const auto source = r.address[1];
            const auto high = t.word(source);
            r.address[1] = source + 4U;
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            const auto destination = r.address[0];
            m.logic(low, 16U); t.word(destination, high);
            m.logic(value, 32U); t.word(destination + 2U, low);
            r.address[0] = destination + 4U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x80deU: {
            const auto source = r.address[1]; r.address[1] += 2U;
            const auto value = t.word(source); m.dw(0U, value); m.logic(value, 16U);
            t.prefetch(pc + 4U); next = 0x80e0U; break;
        }
        case 0x80f6U: {
            t.prefetch(pc + 4U); const auto value = t.byte(0xd0eU);
            m.db(0U, value); m.logic(value, 8U);
            t.prefetch(pc + 6U); next = 0x80faU; break;
        }
        case 0x80faU: next = t.branch(pc, 0x8112U, (r.status & 8U) == 0U); break;
        case 0x80fcU: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); t.prefetch(pc + 8U);
            const auto old = t.byte(0x404007U);
            const auto value = static_cast<std::uint8_t>(old ^ 1U);
            t.prefetch(pc + 10U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 1U) ? 0U : 4U));
            t.byte(0x404007U, value); next = 0x8104U; break;
        }
        case 0x8104U:
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[0] & 1U) ? 0U : 4U));
            t.clocks(2U); next = 0x8108U; break;
        case 0x8108U: next = t.branch(pc, 0x8112U, (r.status & 4U) == 0U); break;
        case 0x810aU: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); t.prefetch(pc + 8U);
            const auto old = t.byte(0x404003U);
            const auto value = static_cast<std::uint8_t>(old ^ 7U); m.logic(value, 8U);
            t.prefetch(pc + 10U); t.byte(0x404003U, value); next = 0x8112U; break;
        }
        case 0x8112U: {
            t.prefetch(pc + 4U); const auto value = t.byte(0xffff8001U);
            m.logic(value, 8U); t.prefetch(pc + 6U); next = 0x8116U; break;
        }
        case 0x8116U: next = t.branch(pc, 0x8144U, (r.status & 4U) != 0U); break;
        case 0x8118U: {
            t.prefetch(pc + 4U); const auto old = t.byte(0xffff800aU);
            t.clocks(2U); m.logic(old, 8U);
            t.byte(0xffff800aU, static_cast<std::uint8_t>(old | 0x80U));
            t.prefetch(pc + 6U); next = 0x811cU; break;
        }
        case 0x811cU: next = t.branch(pc, 0x8144U, (r.status & 4U) == 0U); break;
        case 0x812aU: {
            const auto child = t.jsr_absolute(pc, 315U, 0x1707eU, 0x8130U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x8130U:
            t.prefetch(pc + 4U); (void)t.byte(0xffff800aU); m.logic(0U, 8U);
            t.prefetch(pc + 6U); t.byte(0xffff800aU, 0U); next = 0x8134U; break;
        case 0x8134U: case 0x813cU: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto value = static_cast<std::uint16_t>(pc == 0x8134U ? 0xf3dU : 0x1cU);
            m.logic(value, 16U); t.prefetch(pc + 8U);
            t.word(pc == 0x8134U ? 0xa00000U : 0xa00006U, value);
            t.prefetch(pc + 10U); next = pc + 8U; break;
        }
        case 0x8144U: t.restore(pc, true); next = 0x8148U; break;
        case 0x8148U: return t.rte();
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
