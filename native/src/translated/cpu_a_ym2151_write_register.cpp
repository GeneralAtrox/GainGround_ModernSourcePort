// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_ym2151_write_register(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 0x0U || c.state != 0xffU)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0x0U, 0xffU};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        bool returned_from_child = false;
        switch (pc) {
        case 0x34e6U: { // 0839000700800103: immediate-bit-memory
            next = 0x34eeU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            const auto address = 0x800103U;
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x80U) ? 0U : 4U));
            t.prefetch(pc + 10U);
            break;
        }
        case 0x34eeU: { // 66f6: bcc
            next = 0x34f0U;
            const bool taken = scene_timing::condition(r.status, 6U);
            next = t.branch(pc, 0x34e6U, taken);
            if (taken) transfer_kind = 1U;
            if (taken && !c.host->resumes_interrupts_inline()) {
                r.program_counter = next; t.stop();
                const auto loop = c.host->call_function(40U, 0U, 0xffU, 1U, pc, next, c);
                if (loop.status == TranslationStatus::complete && loop.control == 3U)
                    return FunctionResult::complete(4U, next);
                return loop;
            }
            break;
        }
        case 0x34f0U: { // 13c000800101: move-byte-to-absolute-long
            next = 0x34f6U;
            t.prefetch(pc + 4U);
            const auto destination = 0x800101U;
            m.logic(r.data[0], 8U);
            t.prefetch(pc + 6U);
            t.byte(destination, r.data[0]);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x34f6U: { // 13c100800103: move-byte-to-absolute-long
            next = 0x34fcU;
            t.prefetch(pc + 4U);
            const auto destination = 0x800103U;
            m.logic(r.data[1], 8U);
            t.prefetch(pc + 6U);
            t.byte(destination, r.data[1]);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x34fcU: { // 4e75: rts
            next = 0x34feU;
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
        case 0x34e6U:
        case 0x34eeU:
        case 0x34f0U:
        case 0x34f6U:
        case 0x34fcU:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
