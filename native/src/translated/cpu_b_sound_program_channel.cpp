// Implemented but unverified. Original CPU-B state-72 opcodes 0x18172..0x18199.
#include "gain_ground/cpu_b_init_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sound_program_channel(FunctionContext &c) noexcept {
    if (!c.host) return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    CpuBInitTiming m(c);
    auto &r = c.registers;
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next;
        bool returning = false;
        switch (pc) {
        case 0x18172U: // 41fa005c LEA $5c(PC),A0
            r.address[0] = pc + 2U + 0x5cU; m.sequential(pc, 2U); next = 0x18176U; break;
        case 0x18176U: // 343c0002 MOVE.W #2,D2
            m.move_immediate_word(pc, 2U, 2U); next = 0x1817aU; break;
        case 0x1817aU: case 0x1817cU: { // MOVE.B (A0)+,D0/D1
            const auto source = r.address[0]++;
            const auto value = m.read_byte(2U, source);
            m.immediate_byte(pc == 0x1817aU ? 0U : 1U, value);
            m.sequential(pc, 1U); next = pc + 2U; break;
        }
        case 0x1817eU: case 0x18194U: { // BSR.W $18010
            const auto child = m.bsr(320U, pc, 0x18010U);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x18182U: next = m.dbf(pc, 0x1817aU, 2U); break; // 51cafff6
        case 0x18186U: // 102b0002 MOVE.B $2(A3),D0
            m.prefetch(pc + 4U);
            m.immediate_byte(0U, m.read_byte(2U, r.address[3] + 2U));
            m.prefetch(pc + 6U); next = 0x1818aU; break;
        case 0x1818aU: // 02000007 ANDI.B #7,D0
            m.prefetch(pc + 4U);
            m.immediate_byte(0U, static_cast<std::uint8_t>(r.data[0] & 7U));
            m.prefetch(pc + 6U); next = 0x1818eU; break;
        case 0x1818eU: // 06000038 ADDI.B #$38,D0
            m.prefetch(pc + 4U); m.add(0U, 0x38U, 8U);
            m.prefetch(pc + 6U); next = 0x18192U; break;
        case 0x18192U: // 7200 MOVEQ #0,D1
            r.data[1] = 0U; m.logic(0U, 32U); m.sequential(pc, 1U); next = 0x18194U; break;
        case 0x18198U: next = m.rts(); returning = true; break;
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (auto result = m.boundary(pc, next)) return *result;
        if (returning) return FunctionResult::complete(1U, next);
    }
}
} // namespace gain_ground::translated
