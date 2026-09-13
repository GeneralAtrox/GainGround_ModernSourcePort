// Implemented but unverified. Original priority merge and nonlocal return.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017cc6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x17cc6U:
            r.data[2] = 0U; m.logic(0U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17cc8U: {
            t.prefetch(pc + 4U); const auto value = t.word(r.address[6] + 0x32U);
            m.dw(2U, value); m.logic(value, 16U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17cd0U: case 0x17cd8U: case 0x17ce0U: case 0x17ce8U: case 0x17cf0U:
        case 0x17d1cU: case 0x17d24U: case 0x17d2cU: case 0x17d34U: {
            const auto bit = pc <= 0x17cf0U ? 15U - (pc - 0x17cd0U) / 8U :
                10U - (pc - 0x17d1cU) / 8U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            // bcsr4 samples/tests only after the second prefetch.
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((r.data[2] & (1U << bit)) ? 0U : 4U));
            t.clocks(2U); next = pc + 4U; break;
        }
        case 0x17cf8U: case 0x17d00U: case 0x17d04U: case 0x17d12U:
        case 0x17d3cU: case 0x17d40U: case 0x17d4cU: case 0x17d54U:
        case 0x17d5cU: case 0x17d64U: case 0x17d6cU: case 0x17d74U:
        case 0x17d7eU: case 0x17d88U: case 0x17d92U: {
            const auto reg = pc == 0x17d00U || pc == 0x17d3cU ? 2U :
                pc == 0x17d04U || pc == 0x17d40U ? 0U : 3U;
            const auto mask = pc == 0x17d00U || pc == 0x17d40U || pc == 0x17d92U ? 0x780U :
                pc == 0x17d4cU ? 0x8000U : pc == 0x17d54U ? 0xc000U :
                pc == 0x17d5cU ? 0xe000U : pc == 0x17d64U ? 0xf000U :
                pc == 0x17d74U ? 0x400U : pc == 0x17d7eU ? 0x600U : pc == 0x17d88U ? 0x700U : 0xf800U;
            t.prefetch(pc + 4U); m.dw(reg, r.data[reg] & mask); m.logic(r.data[reg], 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x17d08U: case 0x17d44U:
            m.dw(2U, r.data[2] | r.data[0]); m.logic(r.data[2], 16U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17d0aU: case 0x17d46U:
            t.prefetch(pc + 4U); m.logic(r.data[2], 16U);
            t.word(r.address[6] + 0x32U, static_cast<std::uint16_t>(r.data[2]));
            t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17d10U: case 0x17d1aU:
            m.dw(3U, r.data[0]); m.logic(r.data[3], 16U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x17cccU: next = t.branch_word(pc, 0x17d08U, (r.status & 4U) != 0U); break;
        case 0x17cd4U: case 0x17cdcU: case 0x17ce4U: case 0x17cecU: case 0x17cf4U:
            next = t.branch_word(pc, pc + 0x78U, (r.status & 4U) == 0U); break;
        case 0x17cfcU: next = t.branch_word(pc, 0x17d1aU, (r.status & 4U) != 0U); break;
        case 0x17d16U: next = t.branch_word(pc, 0x17d98U, (r.status & 4U) == 0U); break;
        case 0x17d20U: next = t.branch_word(pc, 0x17d74U, (r.status & 4U) == 0U); break;
        case 0x17d28U: next = t.branch_word(pc, 0x17d7eU, (r.status & 4U) == 0U); break;
        case 0x17d30U: next = t.branch_word(pc, 0x17d88U, (r.status & 4U) == 0U); break;
        case 0x17d38U: next = t.branch_word(pc, 0x17d92U, (r.status & 4U) == 0U); break;
        case 0x17d50U: case 0x17d58U: case 0x17d60U: case 0x17d68U: case 0x17d70U:
            next = t.branch(pc, 0x17d00U, (r.status & 4U) == 0U); break;
        case 0x17d52U: case 0x17d5aU: case 0x17d62U: case 0x17d6aU: case 0x17d72U:
            next = t.branch(pc, 0x17d10U, true); break;
        case 0x17d78U: case 0x17d82U: case 0x17d8cU: case 0x17d96U:
            next = t.branch(pc, 0x17d3cU, (r.status & 4U) == 0U); break;
        case 0x17d7aU: case 0x17d84U: case 0x17d8eU:
            next = t.branch_word(pc, 0x17d98U, true); break;
        case 0x17d98U:
            // LEA discards the BSR return without reading it. PC identifies
            // the nonlocal RTS even if an IRQ resumes between these two steps.
            r.address[7] += 4U; t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x17d9cU: {
            const auto result = t.rts(pc);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            return FunctionResult::complete(8U, result.exit_program_counter);
        }
        case 0x17d0eU: case 0x17d4aU: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
