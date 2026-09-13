// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_ym2151_reset_table(FunctionContext &c) noexcept {
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
        case 0x3464U: { // 13fc00800080000f: move-byte-to-absolute-long
            next = 0x346cU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0x80000fU;
            m.logic(0x80U, 8U);
            t.prefetch(pc + 8U);
            m.byte(destination, 0x80U);
            c.host->write_hardware(4U, 0U, 0xffU, pc, 0x80000eU, 0x80U, 0xffU);
            t.clocks(4U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x346cU: { // 41fa00ce: lea
            next = 0x3470U;
            r.address[0] = 0x353cU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x3470U: { // 60000006: bcc
            next = 0x3474U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0x3478U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x3478U: { // 1018: move-to-data-register
            next = 0x347aU;
            const auto source = r.address[0];
            r.address[0] += 1U;
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x347aU: { // 6602: bcc
            next = 0x347cU;
            const bool taken = scene_timing::condition(r.status, 6U);
            next = t.branch(pc, 0x347eU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x347cU: { // 4e75: rts
            next = 0x347eU;
            return t.rts(pc);
            break;
        }
        case 0x347eU: { // 1218: move-to-data-register
            next = 0x3480U;
            const auto source = r.address[0];
            r.address[0] += 1U;
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(1U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x3480U: { // 6164: bsr
            next = 0x3482U;
            const auto target = 0x34e6U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x3482U: { // 60f4: bcc
            next = 0x3484U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch(pc, 0x3478U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x3464U:
        case 0x346cU:
        case 0x3470U:
        case 0x3478U:
        case 0x347aU:
        case 0x347cU:
        case 0x347eU:
        case 0x3480U:
        case 0x3482U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
