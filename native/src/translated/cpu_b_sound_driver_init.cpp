// Implemented but unverified. Original CPU-B state-72 opcodes 0x17054..0x1706d.
#include "gain_ground/cpu_b_init_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sound_driver_init(FunctionContext &c) noexcept {
    if (!c.host) return {TranslationStatus::contract_violation, 0U, c.registers.program_counter};
    CpuBInitTiming m(c);
    auto &r = c.registers;
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next;
        bool returning = false;
        switch (pc) {
        case 0x17054U: case 0x17056U: { // 2f0d/2f0e MOVE.L A5/A6,-(SP)
            const auto value = r.address[pc == 0x17054U ? 5U : 6U];
            const auto stack = r.address[7] - 4U;
            m.logic(value, 16U); // rmml1 exposes low-word NZVC before prefetch.
            m.prefetch(pc + 4U); // rmml1 prefetch precedes low/high writes.
            m.logic(value, 32U); // rmml2 merges high-word N/Z before low write.
            m.write_word(2U, stack + 2U, static_cast<std::uint16_t>(value));
            r.address[7] = stack;
            m.write_word(2U, stack, static_cast<std::uint16_t>(value >> 16U));
            next = pc + 2U; break;
        }
        case 0x17058U: // 4df9ffffc000
            m.lea_absolute_long(pc, 6U, 0xffffc000U); next = 0x1705eU; break;
        case 0x1705eU: // 4bf900fb0000
            m.lea_absolute_long(pc, 5U, 0xfb0000U); next = 0x17064U; break;
        case 0x17064U: { // 61000014 BSR.W $1707a
            const auto child = m.bsr(314U, pc, 0x1707aU);
            if (child.status != TranslationStatus::complete || child.control != 1U) return child;
            continue;
        }
        case 0x17068U: case 0x1706aU: { // 2c5f/2a5f MOVEA.L (SP)+,A6/A5
            const auto stack = r.address[7];
            const auto high = m.read_word(2U, stack);
            r.address[7] += 4U;
            const auto low = m.read_word(2U, stack + 2U);
            r.address[pc == 0x17068U ? 6U : 5U] = (std::uint32_t(high) << 16U) | low;
            m.prefetch(pc + 4U); next = pc + 2U; break; // MOVEA leaves CCR unchanged.
        }
        case 0x1706cU: next = m.rts(); returning = true; break;
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (auto result = m.boundary(pc, next)) return *result;
        if (returning) return FunctionResult::complete(1U, next);
    }
}
} // namespace gain_ground::translated
