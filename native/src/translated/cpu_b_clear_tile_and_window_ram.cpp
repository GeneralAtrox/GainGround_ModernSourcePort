// Implemented but unverified. Original opcodes 0x85f2..0x8621.
#include "gain_ground/cpu_b_init_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_clear_tile_and_window_ram(FunctionContext &c) noexcept {
    if (!c.host) return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    CpuBInitTiming m(c);
    auto &r = c.registers;
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next;
        bool returning = false;
        switch (pc) {
        case 0x85f2U: // 41f900200000 LEA abs.l,A0
            m.lea_absolute_long(pc, 0U, 0x200000U); next = 0x85f8U; break;
        case 0x85f8U: // 303c23ff MOVE.W #$23ff,D0
            m.move_immediate_word(pc, 0U, 0x23ffU); next = 0x85fcU; break;
        case 0x85fcU: // 7200 MOVEQ #0,D1
            r.data[1] = 0U; m.logic(0U, 32U); m.sequential(pc, 1U); next = 0x85feU; break;
        case 0x85feU: // 20c1 MOVE.L D1,(A0)+; spans tile and scroll RAM.
        case 0x861aU: {
            const auto a = r.address[0];
            if (a >= 0x20c000U) m.store_postincrement(7U, 0x20c000U, pc);
            else if (a >= 0x208000U) m.store_postincrement(6U, 0x208000U, pc);
            else m.store_postincrement(5U, 0x200000U, pc);
            next = pc + 2U; break;
        }
        case 0x8600U: next = m.dbf(pc, 0x85feU); break; // 51c8fffc
        case 0x8604U: // 41f8050e LEA abs.w,A0
            r.address[0] = 0x50eU; m.sequential(pc, 2U); next = 0x8608U; break;
        case 0x8608U: case 0x860aU: case 0x860cU: case 0x860eU: // 20c1
            m.store_postincrement(2U, 0U, pc); next = pc + 2U; break;
        case 0x8610U: // 41f90020c000
            m.lea_absolute_long(pc, 0U, 0x20c000U); next = 0x8616U; break;
        case 0x8616U: // 303c07ff
            m.move_immediate_word(pc, 0U, 0x7ffU); next = 0x861aU; break;
        case 0x861cU: next = m.dbf(pc, 0x861aU); break; // 51c8fffc
        case 0x8620U: next = m.rts(); returning = true; break; // 4e75
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (auto result = m.boundary(pc, next)) return *result;
        if (returning) return FunctionResult::complete(1U, next);
    }
}
} // namespace gain_ground::translated
