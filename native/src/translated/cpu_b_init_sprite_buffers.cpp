// Implemented but unverified. Original opcodes 0x8622..0x8653.
#include "gain_ground/cpu_b_init_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_init_sprite_buffers(FunctionContext &c) noexcept {
    if (!c.host) return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    CpuBInitTiming m(c);
    auto &r = c.registers;
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next;
        bool returning = false;
        switch (pc) {
        case 0x8622U: // 41f86c02 LEA abs.w,A0
            r.address[0] = 0x6c02U; m.sequential(pc, 2U); next = 0x8626U; break;
        case 0x8626U: // 303c01ff MOVE.W #$1ff,D0
            m.move_immediate_word(pc, 0U, 0x1ffU); next = 0x862aU; break;
        case 0x862aU: // 7200 MOVEQ #0,D1
            r.data[1] = 0U; m.logic(0U, 32U); m.sequential(pc, 1U); next = 0x862cU; break;
        case 0x862cU: // 20c1 MOVE.L D1,(A0)+
            m.store_postincrement(2U, 0U, pc); next = 0x862eU; break;
        case 0x862eU: next = m.dbf(pc, 0x862cU); break; // 51c8fffc
        case 0x8632U: // 41f900600000 LEA abs.l,A0
            m.lea_absolute_long(pc, 0U, 0x600000U); next = 0x8638U; break;
        case 0x8638U: // 303c1fff
            m.move_immediate_word(pc, 0U, 0x1fffU); next = 0x863cU; break;
        case 0x863cU: // 72ff MOVEQ #-1,D1
            r.data[1] = 0xffffffffU; m.logic(r.data[1], 32U); m.sequential(pc, 1U); next = 0x863eU; break;
        case 0x863eU: // 20c1
            m.store_postincrement(11U, 0x600000U, pc); next = 0x8640U; break;
        case 0x8640U: next = m.dbf(pc, 0x863eU); break; // 51c8fffc
        case 0x8644U: // 41f80504
            r.address[0] = 0x504U; m.sequential(pc, 2U); next = 0x8648U; break;
        case 0x8648U: // 43fa01ae LEA $1ae(PC),A1
            r.address[1] = pc + 2U + 0x1aeU; m.sequential(pc, 2U); next = 0x864cU; break;
        case 0x864cU: // 30d9 MOVE.W (A1)+,(A0)+
            m.copy_postincrement(pc, false); next = 0x864eU; break;
        case 0x864eU: case 0x8650U: // 20d9 MOVE.L (A1)+,(A0)+
            m.copy_postincrement(pc, true); next = pc + 2U; break;
        case 0x8652U: next = m.rts(); returning = true; break; // 4e75
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (auto result = m.boundary(pc, next)) return *result;
        if (returning) return FunctionResult::complete(1U, next);
    }
}
} // namespace gain_ground::translated
