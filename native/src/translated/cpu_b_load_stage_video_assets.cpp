// Implemented but unverified. Original stage palette/sprite load at 1826e..1831a.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_load_stage_video_assets(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x1826eU: case 0x1828aU: case 0x182c6U:
        case 0x182e0U: case 0x182f6U: case 0x18304U: {
            const bool a0 = pc == 0x1828aU || pc == 0x182e0U || pc == 0x18304U;
            const auto value = pc == 0x1826eU ? 0xfffa3750U : pc == 0x1828aU ? 0x400000U :
                pc == 0x182c6U ? 0x18340U : pc == 0x182e0U ? 0x402000U : pc == 0x182f6U ? 0x18358U : 0x608000U;
            auto &address = r.address[a0 ? 0U : 1U];
            address = (address & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U); address = value;
            t.prefetch(pc + 6U); t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x18274U: case 0x18294U: case 0x182ccU: {
            const auto reg = pc == 0x18294U ? 1U : 0U;
            r.data[reg] = 0U; m.logic(0U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x18276U: case 0x182b0U: case 0x182ceU: case 0x182ecU: {
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto value = t.word(pc == 0x18276U || pc == 0x182b0U ? 0xc02U : 0xc00U);
            m.dw(0U, value); m.logic(value, 16U); t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x1827cU: case 0x1827eU: case 0x182d4U: case 0x182d6U: case 0x182f2U: case 0x182f4U:
            m.dw(0U, m.add(r.data[0], r.data[0], 16U));
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x18280U: case 0x182d8U: case 0x182fcU: {
            // Extension 0800 selects D0.L; 0000 selects signed D0.W.
            const auto index = pc == 0x182fcU
                ? static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])))
                : r.data[0];
            const auto source = r.address[1] + index;
            t.clocks(2U); t.prefetch(pc + 4U); const auto value = t.lng(source);
            r.address[1] = value; t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x18284U:
            m.logic(t.word(r.address[1]), 16U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x18286U: next = t.branch_word(pc, 0x182b0U, (r.status & 8U) != 0U); break;
        case 0x18290U: case 0x182a8U: case 0x1830aU: {
            const auto reg = pc == 0x182a8U ? 1U : 0U;
            std::uint32_t operand;
            if (pc == 0x18290U) {
                const auto source = r.address[1]; r.address[1] += 2U;
                operand = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(t.word(source))));
            } else if (pc == 0x182a8U) { t.prefetch(pc + 4U); operand = 0x20U; }
            else operand = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
            const auto value = r.address[reg] + operand;
            r.address[reg] = (r.address[reg] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + (pc == 0x182a8U ? 6U : 4U));
            t.clocks(2U); r.address[reg] = value; t.clocks(2U);
            next = pc + (pc == 0x182a8U ? 4U : 2U); break;
        }
        case 0x18292U: case 0x18300U: {
            const auto source = r.address[1]; r.address[1] += 2U;
            const auto value = t.word(source); m.dw(pc == 0x18292U ? 2U : 0U, value);
            m.logic(value, 16U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x18296U: case 0x182dcU: case 0x1830eU: {
            const auto value = pc == 0x18296U ? 0xfU : pc == 0x182dcU ? 0x7fU : 0x3ffU;
            t.prefetch(pc + 4U); m.dw(0U, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1829aU: {
            const auto source = r.address[1] + static_cast<std::int16_t>(r.data[1]);
            t.clocks(2U); t.prefetch(pc + 4U); const auto value = t.word(source);
            m.logic(value, 16U); t.word(r.address[0], value); r.address[0] += 2U;
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x1829eU:
            m.dw(1U, m.add(r.data[1], 2U, 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x182a0U: next = t.dbf(pc, 0x1829aU); break;
        case 0x182a4U: next = t.dbf(pc, 0x18294U, 2U); break;
        case 0x182acU: next = t.branch_word(pc, 0x18284U, true); break;
        case 0x182b6U:
            m.logic(r.data[0], 16U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x182b8U: next = t.branch_word(pc, 0x182c6U, (r.status & 4U) != 0U); break;
        case 0x182bcU: next = t.branch_word(pc, 0x1831aU, (r.status & 8U) != 0U); break;
        case 0x182c0U:
            t.prefetch(pc + 4U); m.dw(0U, m.sub(r.data[0], 10U, 16U));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x182c4U: next = t.branch(pc, 0x182b6U, true); break;
        case 0x182e6U: case 0x18312U: {
            auto &source_register = r.address[pc == 0x182e6U ? 1U : 2U];
            const auto source = source_register; const auto high = t.word(source);
            source_register += 4U; const auto low = t.word(source + 2U);
            const auto destination = r.address[0];
            m.logic(low, 16U); t.word(destination, high);
            m.logic((std::uint32_t(high) << 16U) | low, 32U);
            t.word(destination + 2U, low); r.address[0] += 4U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x182e8U: next = t.dbf(pc, 0x182e6U); break;
        case 0x18302U: next = t.branch(pc, 0x1831aU, (r.status & 8U) != 0U); break;
        case 0x1830cU: {
            const auto source = r.address[1]; const auto high = t.word(source);
            r.address[1] += 4U; const auto low = t.word(source + 2U);
            r.address[2] = (std::uint32_t(high) << 16U) | low;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x18314U: next = t.dbf(pc, 0x18312U); break;
        case 0x18318U: next = t.branch(pc, 0x18300U, true); break;
        case 0x1831aU: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
