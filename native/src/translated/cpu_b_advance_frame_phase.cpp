// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_advance_frame_phase(FunctionContext &c) noexcept {
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
        case 0xdba0U: { // 41f80832: lea
            next = 0xdba4U;
            r.address[0] = 0x832U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdba4U: { // 1010: move-to-data-register
            next = 0xdba6U;
            const auto source = r.address[0];
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdba6U: { // 5c10: quick-memory
            next = 0xdba8U;
            const auto address = r.address[0];
            const auto old = t.byte(address);
            const auto value = m.add(old, 6U, 8U);
            t.prefetch(pc + 4U);
            t.byte(address, value);
            break;
        }
        case 0xdba8U: { // 1418: move-to-data-register
            next = 0xdbaaU;
            const auto source = r.address[0];
            r.address[0] += 1U;
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(2U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdbaaU: { // 1210: move-to-data-register
            next = 0xdbacU;
            const auto source = r.address[0];
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(1U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdbacU: { // 02010002: immediate-and
            next = 0xdbb0U;
            t.prefetch(pc + 4U);
            const auto value = r.data[1] & 0x2U;
            m.logic(value, 8U);
            m.db(1U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdbb0U: { // b500: eor-byte-register
            next = 0xdbb2U;
            const auto value = r.data[0] ^ r.data[2];
            m.logic(value, 8U); m.db(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdbb2U: { // 6a08: bcc
            next = 0xdbb4U;
            const bool taken = scene_timing::condition(r.status, 10U);
            next = t.branch(pc, 0xdbbcU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xdbb4U: { // 7203: moveq
            next = 0xdbb6U;
            r.data[1] = 0x3U;
            m.logic(0x3U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdbb6U: { // 4a02: tst-register
            next = 0xdbb8U;
            m.logic(r.data[2], 8U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdbb8U: { // 6b02: bcc
            next = 0xdbbaU;
            const bool taken = scene_timing::condition(r.status, 11U);
            next = t.branch(pc, 0xdbbcU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xdbbaU: { // 7204: moveq
            next = 0xdbbcU;
            r.data[1] = 0x4U;
            m.logic(0x4U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdbbcU: { // 4a380411: tst-memory
            next = 0xdbc0U;
            t.prefetch(pc + 4U);
            const auto address = 0x411U;
            const auto value = t.byte(address);
            m.logic(value, 8U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdbc0U: { // 6708: bcc
            next = 0xdbc2U;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0xdbcaU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xdbc2U: { // 00010008: immediate-or
            next = 0xdbc6U;
            t.prefetch(pc + 4U);
            const auto value = r.data[1] | 0x8U;
            m.logic(value, 8U);
            m.db(1U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdbc6U: { // 42380411: clear-memory
            next = 0xdbcaU;
            const auto address = 0x411U;
            t.prefetch(pc + 4U);
            (void)t.byte(address);
            m.logic(0U, 8U);
            t.prefetch(pc + 6U);
            t.byte(address, 0U);
            break;
        }
        case 0xdbcaU: { // 1081: move-to-indirect
            next = 0xdbccU;
            const auto destination = r.address[0];
            m.logic(r.data[1], 8U);
            t.byte(destination, r.data[1]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdbccU: { // 4a380417: tst-memory
            next = 0xdbd0U;
            t.prefetch(pc + 4U);
            const auto address = 0x417U;
            const auto value = t.byte(address);
            m.logic(value, 8U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdbd0U: { // 670e: bcc
            next = 0xdbd2U;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0xdbe0U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xdbd2U: { // 303c0036: move-to-data-register
            next = 0xdbd6U;
            t.prefetch(pc + 4U);
            m.logic(0x36U, 16U);
            m.dw(0U, 0x36U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdbd6U: { // 4eb90001700c: jsr
            next = 0xdbdcU;
            const auto target = 0x1700cU;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_absolute);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xdbdcU: { // 42380417: clear-memory
            next = 0xdbe0U;
            const auto address = 0x417U;
            t.prefetch(pc + 4U);
            (void)t.byte(address);
            m.logic(0U, 8U);
            t.prefetch(pc + 6U);
            t.byte(address, 0U);
            break;
        }
        case 0xdbe0U: { // 4e75: rts
            next = 0xdbe2U;
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
        case 0xdba0U:
        case 0xdba4U:
        case 0xdba6U:
        case 0xdba8U:
        case 0xdbaaU:
        case 0xdbacU:
        case 0xdbb0U:
        case 0xdbb2U:
        case 0xdbb4U:
        case 0xdbb6U:
        case 0xdbb8U:
        case 0xdbbaU:
        case 0xdbbcU:
        case 0xdbc0U:
        case 0xdbc2U:
        case 0xdbc6U:
        case 0xdbcaU:
        case 0xdbccU:
        case 0xdbd0U:
        case 0xdbd2U:
        case 0xdbd6U:
        case 0xdbdcU:
        case 0xdbe0U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
