// Implemented but unverified. Original video buffer setup at 8892..8918.
#include "cpu_b_irq_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_init_video_buffers(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        switch (pc) {
        case 0x8892U: case 0x88c2U: {
            const auto value = pc == 0x8892U ? 0x80008000U : 0x03ffffffU;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); r.data[0] = value;
            m.logic(value, 32U); t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x8898U: case 0x88caU: case 0x88e2U: {
            const auto reg = pc == 0x88e2U ? 0U : 2U;
            const auto value = pc == 0x8898U ? 0x2fU : pc == 0x88caU ? 0x17fU : 0x2ffU;
            t.prefetch(pc + 4U); m.dw(reg, value); m.logic(value, 16U);
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x889cU: case 0x88ceU: case 0x88dcU: {
            const auto value = pc == 0x889cU ? 0x206000U : pc == 0x88ceU ? 0x20c000U : 0x20d000U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U); r.address[0] = value;
            t.prefetch(pc + 6U); t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x88a2U: case 0x88a4U: case 0x88a6U:
        case 0x88baU: case 0x88bcU: case 0x88beU: case 0x88c0U: {
            const auto destination = r.address[0]; (void)t.word(destination);
            r.address[0] += 4U; (void)t.word(destination + 2U);
            m.logic(0U, 16U); t.prefetch(pc + 4U); m.logic(0U, 32U);
            t.word(destination + 2U, 0U); t.word(destination, 0U);
            next = pc + 2U; break;
        }
        case 0x88a8U: case 0x88c8U:
            r.data[1] = pc == 0x88a8U ? 0x1bU : 0xffffffffU;
            m.logic(r.data[1], 32U); t.prefetch(pc + 4U); next = pc + 2U; break;
        case 0x88aaU: case 0x88d4U: case 0x88d6U: {
            const auto value = r.data[pc == 0x88d6U ? 1U : 0U];
            const auto destination = r.address[0];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[0] += 4U; m.logic(value, 32U);
            t.prefetch(pc + 4U); next = pc + 2U; break;
        }
        case 0x88acU: next = t.dbf(pc, 0x88aaU, 1U); break;
        case 0x88b2U: next = t.dbf(pc, 0x88a2U, 2U); break;
        case 0x88d8U: next = t.dbf(pc, 0x88d4U, 2U); break;
        case 0x88ecU: next = t.dbf(pc, 0x88e6U); break;
        case 0x88b0U: {
            // ADDQ.W An updates all 32 address bits and preserves CCR.
            const auto value = r.address[0] + 2U;
            r.address[0] = (r.address[0] & 0xffff0000U) | (value & 0xffffU);
            t.prefetch(pc + 4U); t.clocks(2U); r.address[0] = value; t.clocks(2U);
            next = pc + 2U; break;
        }
        case 0x88b6U: case 0x88f0U:
            r.address[0] = pc == 0x88b6U ? 0x50eU : 0x7402U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U); next = pc + 4U; break;
        case 0x88e6U: case 0x8900U: case 0x8912U: {
            const auto value = pc == 0x88e6U ? 0xffffffffU : pc == 0x8900U ? 0x24deaU : 0x24d80U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            const auto destination = r.address[0];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[0] += 4U; m.logic(value, 32U);
            t.prefetch(pc + 8U); next = pc + 6U; break;
        }
        case 0x88f4U: case 0x88f8U: case 0x88fcU:
        case 0x8906U: case 0x890aU: case 0x890eU: {
            const auto value = pc == 0x88f4U ? 0x60U : pc == 0x88f8U ? 0xff78U :
                pc == 0x88fcU ? 0x805U : pc == 0x8906U ? 0x7cU : pc == 0x890aU ? 0x198U : 0x804U;
            t.prefetch(pc + 4U); m.logic(value, 16U);
            t.word(r.address[0], static_cast<std::uint16_t>(value)); r.address[0] += 2U;
            t.prefetch(pc + 6U); next = pc + 4U; break;
        }
        case 0x8918U: return t.rts(pc);
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (const auto event = m.interrupt(c, pc, next)) return *event;
        t.begin(next);
    }
}
} // namespace gain_ground::translated
