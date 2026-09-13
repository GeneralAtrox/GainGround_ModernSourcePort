// Implemented but unverified. Original opcodes 0x8654..0x8665.
#include "gain_ground/cpu_b_init_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_clear_overlay_descriptors(FunctionContext &c) noexcept {
    if (!c.host) return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    CpuBInitTiming m(c);
    auto &r = c.registers;
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next;
        bool returning = false;
        switch (pc) {
        case 0x8654U: // 41f87402 LEA abs.w,A0
            r.address[0] = 0x7402U; m.sequential(pc, 2U); next = 0x8658U; break;
        case 0x8658U: // 303c0027 MOVE.W #$27,D0
            m.move_immediate_word(pc, 0U, 0x27U); next = 0x865cU; break;
        case 0x865cU: // 7200 MOVEQ #0,D1
            r.data[1] = 0U; m.logic(0U, 32U); m.sequential(pc, 1U); next = 0x865eU; break;
        case 0x865eU: // 20c1 MOVE.L D1,(A0)+
            m.store_postincrement(2U, 0U, pc); next = 0x8660U; break;
        case 0x8660U: next = m.dbf(pc, 0x865eU); break; // 51c8fffc
        case 0x8664U: next = m.rts(); returning = true; break; // 4e75
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (auto result = m.boundary(pc, next)) return *result;
        if (returning) return FunctionResult::complete(1U, next);
    }
}
} // namespace gain_ground::translated
