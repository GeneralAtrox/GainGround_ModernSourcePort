// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_init_shared_state_and_io(FunctionContext &c) noexcept {
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
        case 0x8011aU: { // 33fcffc600240000: move-immediate-word-absolute-long
            next = 0x80122U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0x240000U;
            m.logic(0xffc6U, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0xffc6U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x80122U: { // 33fcfff000260000: move-immediate-word-absolute-long
            next = 0x8012aU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0x260000U;
            m.logic(0xfff0U, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0xfff0U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x8012aU: { // 427900270000: clear-word-absolute-long
            next = 0x80130U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = 0x270000U;
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 8U);
            t.word(address, 0U);
            break;
        }
        case 0x80130U: { // 427900800006: clear-word-absolute-long
            next = 0x80136U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = 0x800006U;
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 8U);
            t.word(address, 0U);
            break;
        }
        case 0x80136U: { // 23fc000400880080001c: move-register-or-immediate-to-absolute-long
            next = 0x80140U;
            const auto value = 0x40088U;
            const auto destination = 0x80001cU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            m.logic(value, 16U);
            t.prefetch(pc + 10U);
            m.logic(value, 32U);
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination + 2U, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 12U);
            break;
        }
        case 0x80140U: { // 4e40: supervisor-trap
            next = 0x80142U;
            const auto child = t.supervisor_trap(pc, 32U, next);
            if (child.status != TranslationStatus::complete || child.control != 2U) return child;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x80142U: { // 41fa04e0: lea
            next = 0x80146U;
            r.address[0] = 0x80624U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x80146U: { // 227c00080036: move-long-immediate-to-register
            next = 0x8014cU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            r.address[1] = 0x80036U;
            t.prefetch(pc + 8U);
            break;
        }
        case 0x8014cU: { // 303c0008: move-to-data-register
            next = 0x80150U;
            t.prefetch(pc + 4U);
            m.logic(0x8U, 16U);
            m.dw(0U, 0x8U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x80150U: { // 22d8: move-long-postincrement-copy
            next = 0x80152U;
            const auto source = r.address[0];
            const auto high = t.word(source);
            r.address[0] += 4U;
            const auto low = t.word(source + 2U);
            const auto destination = r.address[1];
            m.logic(low, 16U); t.word(destination, high);
            m.logic((std::uint32_t(high) << 16U) | low, 32U);
            t.word(destination + 2U, low);
            r.address[1] += 4U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x80152U: { // 51c8fffc: dbf
            next = 0x80156U;
            next = t.dbf(pc, 0x80150U, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x80156U: { // 427900a00004: clear-word-absolute-long
            next = 0x8015cU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = 0xa00004U;
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 8U);
            t.word(address, 0U);
            break;
        }
        case 0x8015cU: { // 427900a00006: clear-word-absolute-long
            next = 0x80162U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = 0xa00006U;
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 8U);
            t.word(address, 0U);
            break;
        }
        case 0x80162U: { // 41f88000: lea
            next = 0x80166U;
            r.address[0] = 0xffff8000U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x80166U: { // 303c13ff: move-to-data-register
            next = 0x8016aU;
            t.prefetch(pc + 4U);
            m.logic(0x13ffU, 16U);
            m.dw(0U, 0x13ffU);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x8016aU: { // 7200: moveq
            next = 0x8016cU;
            r.data[1] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x8016cU: { // 20c1: move-long-register-or-immediate-to-memory
            next = 0x8016eU;
            const auto value = r.data[1];
            const auto destination = r.address[0];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[0] += 4U;
            m.logic(value, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x8016eU: { // 51c8fffc: dbf
            next = 0x80172U;
            next = t.dbf(pc, 0x8016cU, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x80172U: { // 41f9fff00402: lea-absolute-long
            next = 0x80178U;
            const auto value = 0xfff00402U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x80178U: { // 303c1efe: move-to-data-register
            next = 0x8017cU;
            t.prefetch(pc + 4U);
            m.logic(0x1efeU, 16U);
            m.dw(0U, 0x1efeU);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x8017cU: { // 20c1: move-long-register-or-immediate-to-memory
            next = 0x8017eU;
            const auto value = r.data[1];
            const auto destination = r.address[0];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[0] += 4U;
            m.logic(value, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x8017eU: { // 51c8fffc: dbf
            next = 0x80182U;
            next = t.dbf(pc, 0x8017cU, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x80182U: { // 41f90080000a: lea-absolute-long
            next = 0x80188U;
            const auto value = 0x80000aU;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x80188U: { // 01080001: movep-word-memory-to-register
            next = 0x8018cU;
            const auto address = (r.address[0] + 0x1U);
            t.prefetch(pc + 4U);
            const auto high = t.byte(address);
            const auto low = t.byte(address + 2U);
            m.dw(0U, (std::uint16_t(high) << 8U) | low);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x8018cU: { // 4640: not-register
            next = 0x8018eU;
            const auto value = ~r.data[0];
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x8018eU: { // 33c0fff00402: move-register-or-immediate-to-absolute-long
            next = 0x80194U;
            const auto value = r.data[0];
            const auto destination = 0xfff00402U;
            t.prefetch(pc + 4U);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0x80194U: { // 1039fff00402: move-to-data-register
            next = 0x8019aU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto source = 0xfff00402U;
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x8019aU: { // 0c0000ff: immediate-cmp
            next = 0x8019eU;
            t.prefetch(pc + 4U);
            const auto value = m.sub(r.data[0], 0xffU, 8U, true);
            (void)value; // CMPI updates flags without storing a result.
            t.prefetch(pc + 6U);
            break;
        }
        case 0x8019eU: { // 6606: bcc
            next = 0x801a0U;
            const bool taken = scene_timing::condition(r.status, 6U);
            next = t.branch(pc, 0x801a6U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x801a0U: { // 50f9fff00404: set-true-absolute-byte
            next = 0x801a6U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = 0xfff00404U;
            (void)t.byte(address);
            t.prefetch(pc + 8U);
            t.byte(address, 0xffU);
            break;
        }
        case 0x801a6U: { // 0240000f: immediate-and
            next = 0x801aaU;
            t.prefetch(pc + 4U);
            const auto value = r.data[0] & 0xfU;
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x801aaU: { // d000: register-add
            next = 0x801acU;
            const auto value = m.add(r.data[0], r.data[0], 8U);
            m.db(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x801acU: { // 13c0fff00405: move-byte-to-absolute-long
            next = 0x801b2U;
            t.prefetch(pc + 4U);
            const auto destination = 0xfff00405U;
            m.logic(r.data[0], 8U);
            t.prefetch(pc + 6U);
            t.byte(destination, r.data[0]);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x801b2U: { // 303900800008: move-to-data-register
            next = 0x801b8U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto source = 0x800008U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x801b8U: { // 4600: not-register
            next = 0x801baU;
            const auto value = ~r.data[0];
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x801baU: { // 13c0fff0081d: move-byte-to-absolute-long
            next = 0x801c0U;
            t.prefetch(pc + 4U);
            const auto destination = 0xfff0081dU;
            m.logic(r.data[0], 8U);
            t.prefetch(pc + 6U);
            t.byte(destination, r.data[0]);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x801c0U: { // 4e75: rts
            next = 0x801c2U;
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
        case 0x8011aU:
        case 0x80122U:
        case 0x8012aU:
        case 0x80130U:
        case 0x80136U:
        case 0x80140U:
        case 0x80142U:
        case 0x80146U:
        case 0x8014cU:
        case 0x80150U:
        case 0x80152U:
        case 0x80156U:
        case 0x8015cU:
        case 0x80162U:
        case 0x80166U:
        case 0x8016aU:
        case 0x8016cU:
        case 0x8016eU:
        case 0x80172U:
        case 0x80178U:
        case 0x8017cU:
        case 0x8017eU:
        case 0x80182U:
        case 0x80188U:
        case 0x8018cU:
        case 0x8018eU:
        case 0x80194U:
        case 0x8019aU:
        case 0x8019eU:
        case 0x801a0U:
        case 0x801a6U:
        case 0x801aaU:
        case 0x801acU:
        case 0x801b2U:
        case 0x801b8U:
        case 0x801baU:
        case 0x801c0U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
