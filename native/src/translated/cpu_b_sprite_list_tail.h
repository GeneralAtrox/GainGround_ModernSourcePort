#pragma once
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
// Same function-307 instruction stream, not a new emulated call or draw owner.
// Implemented but unverified; the earlier descriptor section remains untimed.
inline FunctionResult cpu_b_sprite_list_tail(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x16ebeU:
            t.prefetch(pc + 4U); m.logic(t.word(0x6c00U), 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x16ec2U: next = t.branch(pc, 0x16ed8U, (r.status & 8U) == 0U); break;
        case 0x16ec4U: case 0x16ec8U: case 0x16ed8U: case 0x16edcU: case 0x16f52U: {
            const auto value = pc == 0x16f52U ? 0xffffU : pc < 0x16ed8U ? 0x8700U : 0x8300U;
            const auto reg = pc == 0x16ec8U || pc == 0x16edcU ? 1U : 5U;
            t.prefetch(pc + 4U); m.logic(value, 16U);
            t.word(r.address[reg], static_cast<std::uint16_t>(value));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x16eccU: case 0x16ee0U:
            r.address[5] = (r.address[5] & 0xffffU) | 0x600000U;
            t.prefetch(pc + 4U); r.address[5] = pc == 0x16eccU ? 0x607000U : 0x603000U;
            t.prefetch(pc + 6U); t.prefetch(pc + 8U); next = pc + 6U; break;
        case 0x16ed2U: case 0x16ee6U: case 0x16ef2U: case 0x16f62U: {
            const auto reg = pc == 0x16f62U ? 0U : pc == 0x16ef2U ? 6U : 7U;
            const auto value = pc == 0x16ed2U ? 0x700U : pc == 0x16ee6U ? 0x300U : pc == 0x16ef2U ? 0xfU : 0xffU;
            t.prefetch(pc + 4U); m.dw(reg, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x16ed6U: next = t.branch(pc, 0x16eeaU, true); break;
        case 0x16eeaU: case 0x16eeeU: case 0x16f56U: case 0x16f5eU:
            r.address[pc == 0x16eeeU ? 4U : 0U] = pc == 0x16eeaU ? 0x16f94U :
                pc == 0x16eeeU ? 0x7402U : pc == 0x16f56U ? 0x6c00U : 0x7002U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x16ef6U: case 0x16f00U: case 0x16f04U: case 0x16f06U: case 0x16f5aU: {
            const auto source_reg = pc == 0x16f5aU ? 0U : pc <= 0x16f00U ? 4U : 6U;
            const auto source = r.address[source_reg]; r.address[source_reg] += 2U;
            const auto value = t.word(source);
            if (pc != 0x16f5aU) m.dw(pc == 0x16ef6U ? 5U : pc == 0x16f00U ? 4U : pc == 0x16f04U ? 3U : 2U, value);
            m.logic(value, 16U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x16ef8U:
            next = t.branch_word(pc, 0x16f4aU,
                (r.status & 4U) != 0U || ((r.status >> 3U) & 1U) != ((r.status >> 1U) & 1U)); break;
        case 0x16efcU: case 0x16f0cU: case 0x16f16U: {
            const auto reg = pc == 0x16efcU ? 5U : pc == 0x16f0cU ? 7U : 1U;
            const auto value = pc == 0x16f16U ? m.sub(r.data[reg], 1U, 16U) :
                m.add(r.data[reg], pc == 0x16efcU ? 8U : 1U, 16U);
            m.dw(reg, value); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x16efeU: {
            const auto source = r.address[4]; r.address[4] += 2U;
            r.address[3] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(t.word(source))));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x16f02U: {
            const auto source = r.address[4]; const auto high = t.word(source);
            r.address[4] += 4U; const auto low = t.word(source + 2U);
            r.address[6] = (std::uint32_t(high) << 16U) | low;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x16f08U:
            r.address[2] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.address[3])));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x16f0aU: case 0x16f22U: case 0x16f2aU: {
            const auto value = pc == 0x16f0aU ? r.data[2] : pc == 0x16f22U ? r.address[2] : r.data[5];
            const auto reg = pc == 0x16f0aU ? 1U : 0U;
            m.dw(reg, value); m.logic(value, 16U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x16f0eU: case 0x16f20U: case 0x16f28U: case 0x16f34U: {
            const auto value = static_cast<std::uint16_t>(r.data[pc == 0x16f0eU ? 7U : pc == 0x16f20U ? 4U : 0U]);
            m.logic(value, 16U); t.word(r.address[5], value); r.address[5] += 2U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x16f10U: case 0x16f12U: case 0x16f18U: {
            const auto source = r.address[pc == 0x16f10U ? 0U : 6U];
            if (pc != 0x16f10U) r.address[6] += 2U;
            const auto value = t.word(source); m.logic(value, 16U);
            if (pc == 0x16f18U) {
                t.prefetch(pc + 4U); t.word(r.address[5] - 2U, value);
                t.prefetch(pc + 6U); next = pc + 4U;
            } else {
                t.word(r.address[5], value); r.address[5] += 2U;
                t.prefetch(pc + 4U); next = pc + 2U;
            }
            break;
        }
        case 0x16f14U: next = t.branch(pc, 0x16f20U, (r.status & 4U) == 0U); break;
        case 0x16f1cU: case 0x16f38U: {
            t.prefetch(pc + 4U);
            const auto operand = static_cast<std::int16_t>(t.word(r.address[0] + 2U));
            const auto value = r.address[2] + operand;
            r.address[2] = (r.address[2] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 6U); t.clocks(2U); r.address[2] = value; t.clocks(2U);
            next = pc + 4U; break;
        }
        case 0x16f24U: case 0x16f2cU:
            t.prefetch(pc + 4U); m.dw(0U, r.data[0] & 0xfffU); m.logic(r.data[0], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x16f30U: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto old = r.data[0]; r.data[0] |= 0x1000U;
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x1000U) ? 0U : 4U));
            t.clocks(2U); next = pc + 4U; break;
        }
        case 0x16f36U: case 0x16f4aU: case 0x16f4cU: {
            const auto reg = pc == 0x16f36U ? 5U : pc == 0x16f4aU ? 4U : 0U;
            const auto value = r.address[reg] + (pc == 0x16f36U ? 4U : pc == 0x16f4aU ? 8U : 6U);
            r.address[reg] = (r.address[reg] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[reg] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x16f3cU: next = t.dbf(pc, 0x16f0cU, 1U); break;
        case 0x16f40U:
            t.prefetch(pc + 4U); m.dw(5U, m.sub(r.data[5], t.word(r.address[0] + 4U), 16U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x16f44U: next = t.dbf(pc, 0x16f08U, 3U); break;
        case 0x16f48U: next = t.branch(pc, 0x16f4cU, true); break;
        case 0x16f4eU: next = t.dbf(pc, 0x16ef6U, 6U); break;
        case 0x16f5cU: next = t.branch(pc, 0x16f62U, (r.status & 8U) != 0U); break;
        case 0x16f66U:
            r.data[1] = 0U; m.logic(0U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x16f68U: {
            const auto value = r.data[1]; const auto destination = r.address[0];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[0] += 4U; m.logic(value, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x16f6aU: next = t.dbf(pc, 0x16f68U); break;
        case 0x16f6eU: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
