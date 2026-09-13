// Implemented but unverified. Instruction phases from the retained 68000 source.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017c3a(FunctionContext &) noexcept;
FunctionResult proven_static_cpu_b_72_00017bde(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17bdeU: case 0x17beaU: {
            const auto reg = pc == 0x17bdeU ? 7U : 0U;
            const auto value = pc == 0x17bdeU ? 7U : 0x11U;
            t.prefetch(pc + 4U); m.dw(reg, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17be2U: case 0x17c1eU: {
            const auto child = t.bsr(pc, pc == 0x17be2U ? 492U : 482U,
                pc == 0x17be2U ? 0x1814eU : 0x17ca2U, pc + 4U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x17be6U: next = t.dbf(pc, 0x17be2U, 7U); break;
        case 0x17bfeU: next = t.dbf(pc, 0x17bf6U); break;
        case 0x17beeU: case 0x17bf0U: case 0x17c06U: case 0x17c08U:
            r.data[pc == 0x17bf0U ? 4U : pc == 0x17c08U ? 0U : 1U] = 0U;
            m.logic(0U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17bf2U: case 0x17c02U:
            r.address[0] = r.address[6] + (pc == 0x17bf2U ? 0x180U : 0U);
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17bf6U: {
            const auto index = static_cast<std::uint32_t>(static_cast<std::int32_t>(
                static_cast<std::int16_t>(r.data[4])));
            const auto address = r.address[0] + index;
            t.clocks(2U); t.prefetch(pc + 4U); m.logic(r.data[1], 16U);
            t.word(address, static_cast<std::uint16_t>(r.data[1]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17bfaU:
            t.prefetch(pc + 4U); m.dw(4U, m.add(r.data[4], 0x50U, 16U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17c0aU: case 0x17c1aU: {
            const bool subtract = pc == 0x17c0aU;
            const auto address = r.address[0] + (subtract ? 0x20U : 0x24U);
            t.prefetch(pc + 4U);
            const auto old = t.word(address);
            const auto value = subtract ? m.sub(old, 1U, 16U) : m.add(old, 2U, 16U);
            t.prefetch(pc + 6U); t.word(address, static_cast<std::uint16_t>(value));
            next = pc + 4U; break;
        }
        case 0x17c0eU: {
            t.prefetch(pc + 4U);
            const auto value = t.word(r.address[0] + 0x24U); m.dw(0U, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17c12U:
            t.prefetch(pc + 4U); m.dw(0U, r.data[0] & 0x1eU); m.logic(r.data[0], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17c16U: case 0x17c22U: case 0x17c2aU: {
            const bool long_index = pc == 0x17c22U;
            const auto index = long_index ? r.data[1] :
                static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
            const auto address = r.address[long_index ? 5U : 0U] + index;
            t.clocks(2U); t.prefetch(pc + 4U);
            const auto value = t.word(address); m.dw(long_index ? 1U : 0U, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17c26U: {
            const auto address = r.address[5] + r.data[1];
            t.clocks(2U); t.prefetch(pc + 4U); r.address[0] = address;
            t.clocks(2U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17c2eU:
            next = t.branch_word(pc, 0x17ca0U, (r.status & 4U) != 0U);
            if (next == 0x17ca0U) return t.transfer(pc, 546U, next);
            break;
        case 0x17c32U:
            // ANDI.L operates low word first; the high-word phase follows
            // the last prefetch. X is preserved throughout.
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); m.logic(r.data[0], 16U);
            t.prefetch(pc + 8U); m.logic(r.data[0] & 0xffffU, 32U);
            t.clocks(2U); r.data[0] &= 0xffffU; t.clocks(2U);
            next = pc + 6U; break;
        case 0x17c38U: {
            const auto value = r.address[0] + r.data[0];
            r.address[0] = (r.address[0] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[0] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        // Sequential ownership boundary, not an original branch or BSR:
        // preserve the direct handoff without adding a call event or cycles.
        if (next == 0x17c3aU) return proven_static_cpu_b_72_00017c3a(c);
        t.begin(next);
    }
}
} // namespace gain_ground::translated
