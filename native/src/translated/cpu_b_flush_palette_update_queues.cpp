// Implemented but unverified. Original two-queue palette submission.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_flush_palette_update_queues(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x814aU: case 0x8188U:
            r.address[0] = pc == 0x814aU ? 0x74eaU : 0x766eU;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x814eU: case 0x818cU: {
            const auto source = r.address[0]++;
            m.logic(t.byte(source), 8U); t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8150U: case 0x818eU:
            next = t.branch(pc, pc == 0x8150U ? 0x8188U : 0x81b4U, !(r.status & 4U)); break;
        case 0x8152U: case 0x8190U:
            r.data[0] = 0U; m.logic(0U, 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x8154U: case 0x8192U: {
            const auto value = t.byte(r.address[0]); m.db(0U, value); m.logic(value, 8U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8156U: case 0x8194U:
            next = t.branch(pc, pc == 0x8156U ? 0x8188U : 0x81b4U, (r.status & 4U) != 0U); break;
        case 0x8158U: case 0x8196U: {
            const auto address = r.address[0]++;
            (void)t.byte(address); m.logic(0U, 8U); t.prefetch(pc + 4U);
            t.byte(address, 0U); next = pc + 2U; break;
        }
        case 0x815aU: case 0x8198U:
            m.dw(0U, m.sub(r.data[0], 1U, 16U)); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x815cU: case 0x8162U: case 0x819aU: case 0x81a0U: {
            const auto reg = pc == 0x815cU || pc == 0x819aU ? 1U : 2U;
            const auto value = pc == 0x815cU ? 0x400000U : pc == 0x8162U ? 0x25d74U :
                pc == 0x819aU ? 0x402000U : 0x26f14U;
            r.address[reg] = (r.address[reg] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U); r.address[reg] = value;
            t.prefetch(pc + 6U); t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x8168U: case 0x816aU: case 0x81a6U: case 0x81a8U: {
            const auto reg = pc == 0x8168U || pc == 0x81a6U ? 1U : 2U;
            const auto source = r.address[0]; r.address[0] += 2U;
            const auto value = t.word(source); m.dw(reg, value); m.logic(value, 16U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x816cU: case 0x8170U: {
            const auto base = pc == 0x816cU ? 1U : 2U;
            const auto reg = pc == 0x816cU ? 3U : 4U;
            const auto value = r.address[base] + static_cast<std::int16_t>(r.data[base]);
            t.clocks(2U); t.prefetch(pc + 4U); r.address[reg] = value;
            t.clocks(2U); t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x8174U: case 0x8176U: case 0x8178U: case 0x817aU:
        case 0x817cU: case 0x817eU: case 0x8180U: case 0x8182U: {
            const auto source = r.address[4]; const auto high = t.word(source);
            r.address[4] = source + 4U; const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low; const auto destination = r.address[3];
            m.logic(low, 16U); t.word(destination, high);
            m.logic(value, 32U); t.word(destination + 2U, low); r.address[3] = destination + 4U;
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x8184U: next = t.dbf(pc, 0x8168U); break;
        case 0x81aaU: {
            const auto source = r.address[2] + static_cast<std::int16_t>(r.data[2]);
            t.clocks(2U); t.prefetch(pc + 4U); const auto value = t.word(source);
            t.clocks(2U);
            const auto destination = r.address[1] + static_cast<std::int16_t>(r.data[1]);
            m.logic(value, 16U); t.prefetch(pc + 6U); t.word(destination, value);
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x81b0U: next = t.dbf(pc, 0x81a6U); break;
        case 0x81b4U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
