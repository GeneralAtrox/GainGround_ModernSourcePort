// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017ebc(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        bool returned_from_child = false;
        switch (pc) {
        case 0x17ebcU: { // 44fc0001: move-immediate-ccr
            next = 0x17ec0U;
            t.prefetch(pc + 4U);
            t.clocks(2U);
            r.status = static_cast<std::uint16_t>((r.status & 0xa700U) | 0x1U);
            t.clocks(2U);
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17ec0U: { // 4e75: rts
            next = 0x17ec2U;
            return t.rts(pc);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x17ebcU:
        case 0x17ec0U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
