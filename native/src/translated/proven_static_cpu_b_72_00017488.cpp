// Implemented but unverified. Original sound-record copy and initialization.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017488(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17488U: {
            const auto source = r.address[0]; r.address[0] += 2U;
            const auto value = t.word(source); const auto destination = r.address[1];
            m.logic(value, 16U); t.word(destination, value); r.address[1] = destination + 2U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x1748aU: {
            const auto source = r.address[0]++;
            const auto value = t.byte(source); m.db(1U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x1748cU:
            m.db(1U, r.data[7]); m.logic(r.data[1], 8U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x1748eU:
            t.prefetch(pc + 4U); m.db(1U, r.data[1] | 0x90U); m.logic(r.data[1], 8U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17492U: case 0x174aaU: case 0x174aeU: {
            const auto destination = r.address[1]; m.logic(r.data[1], 8U);
            t.byte(destination, static_cast<std::uint8_t>(r.data[1])); r.address[1] = destination + 1U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17494U: {
            const auto source = r.address[0]++;
            const auto value = t.byte(source); const auto destination = r.address[1];
            m.logic(value, 8U); t.byte(destination, value); r.address[1] = destination + 1U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x17496U:
            r.data[1] = r.address[0]; m.logic(r.data[1], 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17498U: {
            const auto source = r.address[0]; const auto high = t.word(source);
            r.address[0] = source + 4U; const auto low = t.word(source + 2U);
            const auto addend = (std::uint32_t(high) << 16U) | low;
            const auto value = m.add(r.data[1], addend, 32U);
            // roml2 commits D1.L's low word and full flags before prefetch;
            // roml3 commits its high word after that bus cycle.
            m.dw(1U, value); t.prefetch(pc + 4U); r.data[1] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x1749aU: {
            const auto left = r.data[1]; const auto right = r.address[5];
            m.dw(1U, m.sub(left, right, 16U)); t.prefetch(pc + 4U);
            const auto value = m.sub(left, right, 32U); t.clocks(2U);
            r.data[1] = value; t.clocks(2U); next = pc + 2U; break;
        }
        case 0x1749cU: case 0x174b2U: {
            const auto destination = r.address[1]; const auto value = r.data[1];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[1] = destination + 4U; m.logic(value, 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x1749eU: {
            const auto source = r.address[0]; const auto high = t.word(source);
            r.address[0] = source + 4U; const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            const auto destination = r.address[1];
            m.logic(low, 16U); t.word(destination, high);
            m.logic(value, 32U); t.word(destination + 2U, low); r.address[1] = destination + 4U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x174a0U: {
            t.prefetch(pc + 4U); const auto destination = r.address[1];
            m.logic(0x5001U, 16U); t.word(destination, 0x5001U); r.address[1] = destination + 2U;
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x174a4U: {
            t.prefetch(pc + 4U); const auto value = t.byte(r.address[0] - 9U);
            m.db(1U, value); m.logic(value, 8U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x174a8U:
            m.db(1U, m.sub(r.data[1], 1U, 8U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x174acU: case 0x174b0U: {
            const auto reg = pc == 0x174acU ? 1U : 2U;
            r.data[reg] = pc == 0x174acU ? 0U : 0xfU; m.logic(r.data[reg], 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x174b4U: next = t.dbf(pc, 0x174b2U, 2U); break;
        case 0x174b8U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
