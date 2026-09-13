// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_init_runtime_mode_state(FunctionContext &c) noexcept {
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
        case 0x80236U: { // 303900800008: move-to-data-register
            next = 0x8023cU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto source = 0x800008U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x8023cU: { // 02400060: immediate-and
            next = 0x80240U;
            t.prefetch(pc + 4U);
            const auto value = r.data[0] & 0x60U;
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x80240U: { // 6726: bcc
            next = 0x80242U;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0x80268U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x80242U: { // 70ff: moveq
            next = 0x80244U;
            r.data[0] = 0xffffffffU;
            m.logic(0xffffffffU, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x80244U: { // 4e48: supervisor-trap
            next = 0x80246U;
            const auto child = t.supervisor_trap(pc, 40U, next);
            if (child.status != TranslationStatus::complete || child.control != 2U) return child;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x80246U: { // 41f8d000: lea
            next = 0x8024aU;
            r.address[0] = 0xffffd000U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x8024aU: { // 43f9fff07b20: lea-absolute-long
            next = 0x80250U;
            const auto value = 0xfff07b20U;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x80250U: { // 223c0000004c: move-long-immediate-to-register
            next = 0x80256U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            m.logic(0x4cU, 32U);
            r.data[1] = 0x4cU;
            t.prefetch(pc + 8U);
            break;
        }
        case 0x80256U: { // 4e46: supervisor-trap
            next = 0x80258U;
            const auto child = t.supervisor_trap(pc, 38U, next);
            if (child.status != TranslationStatus::complete || child.control != 2U) return child;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x80258U: { // 6506: bcc
            next = 0x8025aU;
            const bool taken = scene_timing::condition(r.status, 5U);
            next = t.branch(pc, 0x80260U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x8025aU: { // 7000: moveq
            next = 0x8025cU;
            r.data[0] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x8025cU: { // 4e48: supervisor-trap
            next = 0x8025eU;
            const auto child = t.supervisor_trap(pc, 40U, next);
            if (child.status != TranslationStatus::complete || child.control != 2U) return child;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x8025eU: { // 4e75: rts
            next = 0x80260U;
            return t.rts(pc);
            break;
        }
        case 0x80260U: { // 7000: moveq
            next = 0x80262U;
            r.data[0] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x80262U: { // 4e48: supervisor-trap
            next = 0x80264U;
            const auto child = t.supervisor_trap(pc, 40U, next);
            if (child.status != TranslationStatus::complete || child.control != 2U) return child;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x80264U: { // 50f88000: set-true-absolute-byte
            next = 0x80268U;
            t.prefetch(pc + 4U);
            const auto address = 0xffff8000U;
            (void)t.byte(address);
            t.prefetch(pc + 6U);
            t.byte(address, 0xffU);
            break;
        }
        case 0x80268U: { // 41f9fff07b20: lea-absolute-long
            next = 0x8026eU;
            const auto value = 0xfff07b20U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x8026eU: { // 203c0000004c: move-long-immediate-to-register
            next = 0x80274U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            m.logic(0x4cU, 32U);
            r.data[0] = 0x4cU;
            t.prefetch(pc + 8U);
            break;
        }
        case 0x80274U: { // e448: shift-word
            next = 0x80276U;
            scene_timing::shift_word(t, m, pc, 0U, 2U, false, false);
            break;
        }
        case 0x80276U: { // 7200: moveq
            next = 0x80278U;
            r.data[1] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x80278U: { // 20c1: move-long-register-or-immediate-to-memory
            next = 0x8027aU;
            const auto value = r.data[1];
            const auto destination = r.address[0];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[0] += 4U;
            m.logic(value, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x8027aU: { // 51c8fffc: dbf
            next = 0x8027eU;
            next = t.dbf(pc, 0x80278U, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x8027eU: { // 70ff: moveq
            next = 0x80280U;
            r.data[0] = 0xffffffffU;
            m.logic(0xffffffffU, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x80280U: { // 23c0fff07b48: move-register-or-immediate-to-absolute-long
            next = 0x80286U;
            const auto value = r.data[0];
            const auto destination = 0xfff07b48U;
            t.prefetch(pc + 4U);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            m.logic(value, 32U);
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination + 2U, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0x80286U: { // 33c0fff07b46: move-register-or-immediate-to-absolute-long
            next = 0x8028cU;
            const auto value = r.data[0];
            const auto destination = 0xfff07b46U;
            t.prefetch(pc + 4U);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0x8028cU: { // 33fc003cfff07b32: move-immediate-word-absolute-long
            next = 0x80294U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0xfff07b32U;
            m.logic(0x3cU, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0x3cU);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x80294U: { // 33fc003cfff07b38: move-immediate-word-absolute-long
            next = 0x8029cU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0xfff07b38U;
            m.logic(0x3cU, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0x3cU);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x8029cU: { // 33fc003cfff07b3e: move-immediate-word-absolute-long
            next = 0x802a4U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0xfff07b3eU;
            m.logic(0x3cU, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0x3cU);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x802a4U: { // 50f9fff00410: set-true-absolute-byte
            next = 0x802aaU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = 0xfff00410U;
            (void)t.byte(address);
            t.prefetch(pc + 8U);
            t.byte(address, 0xffU);
            break;
        }
        case 0x802aaU: { // 4e75: rts
            next = 0x802acU;
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
        case 0x80236U:
        case 0x8023cU:
        case 0x80240U:
        case 0x80242U:
        case 0x80244U:
        case 0x80246U:
        case 0x8024aU:
        case 0x80250U:
        case 0x80256U:
        case 0x80258U:
        case 0x8025aU:
        case 0x8025cU:
        case 0x8025eU:
        case 0x80260U:
        case 0x80262U:
        case 0x80264U:
        case 0x80268U:
        case 0x8026eU:
        case 0x80274U:
        case 0x80276U:
        case 0x80278U:
        case 0x8027aU:
        case 0x8027eU:
        case 0x80280U:
        case 0x80286U:
        case 0x8028cU:
        case 0x80294U:
        case 0x8029cU:
        case 0x802a4U:
        case 0x802aaU:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
