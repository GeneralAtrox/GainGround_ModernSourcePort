// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_copy_record_grid_to_video_table(FunctionContext &c) noexcept {
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
        case 0xdc94U: { // 43f900206000: lea-absolute-long
            next = 0xdc9aU;
            const auto value = 0x206000U;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xdc9aU: { // d2dc: adda-word-memory
            next = 0xdc9cU;
            const auto address = r.address[4];
            r.address[4] += 2U;
            const auto source = static_cast<std::uint32_t>(static_cast<std::int16_t>(t.word(address)));
            scene_timing::address_add(t, r, 1U, source, pc + 4U);
            break;
        }
        case 0xdc9cU: { // 303cfe00: move-to-data-register
            next = 0xdca0U;
            t.prefetch(pc + 4U);
            m.logic(0xfe00U, 16U);
            m.dw(0U, 0xfe00U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdca0U: { // 3a1c: move-to-data-register
            next = 0xdca2U;
            const auto source = r.address[4];
            r.address[4] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(5U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdca2U: { // 6a02: bcc
            next = 0xdca4U;
            const bool taken = scene_timing::condition(r.status, 10U);
            next = t.branch(pc, 0xdca6U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xdca4U: { // 7000: moveq
            next = 0xdca6U;
            r.data[0] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdca6U: { // 381c: move-to-data-register
            next = 0xdca8U;
            const auto source = r.address[4];
            r.address[4] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(4U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdca8U: { // 361c: move-to-data-register
            next = 0xdcaaU;
            const auto source = r.address[4];
            r.address[4] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(3U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdcaaU: { // 41d1: lea
            next = 0xdcacU;
            r.address[0] = r.address[1];
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdcacU: { // 3404: move-to-data-register
            next = 0xdcaeU;
            m.logic(r.data[4], 16U);
            m.dw(2U, r.data[4]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdcaeU: { // 3205: move-to-data-register
            next = 0xdcb0U;
            m.logic(r.data[5], 16U);
            m.dw(1U, r.data[5]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdcb0U: { // 0240ff00: immediate-and
            next = 0xdcb4U;
            t.prefetch(pc + 4U);
            const auto value = r.data[0] & 0xff00U;
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdcb4U: { // 6704: bcc
            next = 0xdcb6U;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0xdcbaU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xdcb6U: { // 10331000: move-memory-to-register
            next = 0xdcbaU;
            t.clocks(2U);
            const auto source = (r.address[3] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[1])) + 0x0U);
            t.prefetch(pc + 4U);
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdcbaU: { // 30c0: move-to-indirect
            next = 0xdcbcU;
            const auto destination = r.address[0];
            m.logic(r.data[0], 16U);
            t.word(destination, r.data[0]);
            r.address[0] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdcbcU: { // 1001: move-to-data-register
            next = 0xdcbeU;
            m.logic(r.data[1], 8U);
            m.db(0U, r.data[1]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdcbeU: { // 5200: quick-register
            next = 0xdcc0U;
            const auto value = m.add(r.data[0], 1U, 8U);
            m.db(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdcc0U: { // 02000007: immediate-and
            next = 0xdcc4U;
            t.prefetch(pc + 4U);
            const auto value = r.data[0] & 0x7U;
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdcc4U: { // 0241fff8: immediate-and
            next = 0xdcc8U;
            t.prefetch(pc + 4U);
            const auto value = r.data[1] & 0xfff8U;
            m.logic(value, 16U);
            m.dw(1U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdcc8U: { // d200: register-add
            next = 0xdccaU;
            const auto value = m.add(r.data[1], r.data[0], 8U);
            m.db(1U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdccaU: { // 51caffe4: dbf
            next = 0xdcceU;
            next = t.dbf(pc, 0xdcb0U, 2U);
            if ((r.data[2] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xdcceU: { // 43e90080: lea
            next = 0xdcd2U;
            r.address[1] = (r.address[1] + 0x80U);
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdcd2U: { // 5045: quick-register
            next = 0xdcd4U;
            const auto value = m.add(r.data[5], 8U, 16U);
            m.dw(5U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdcd4U: { // 0245003f: immediate-and
            next = 0xdcd8U;
            t.prefetch(pc + 4U);
            const auto value = r.data[5] & 0x3fU;
            m.logic(value, 16U);
            m.dw(5U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdcd8U: { // 51cbffd0: dbf
            next = 0xdcdcU;
            next = t.dbf(pc, 0xdcaaU, 3U);
            if ((r.data[3] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xdcdcU: { // 4e75: rts
            next = 0xdcdeU;
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
        case 0xdc94U:
        case 0xdc9aU:
        case 0xdc9cU:
        case 0xdca0U:
        case 0xdca2U:
        case 0xdca4U:
        case 0xdca6U:
        case 0xdca8U:
        case 0xdcaaU:
        case 0xdcacU:
        case 0xdcaeU:
        case 0xdcb0U:
        case 0xdcb4U:
        case 0xdcb6U:
        case 0xdcbaU:
        case 0xdcbcU:
        case 0xdcbeU:
        case 0xdcc0U:
        case 0xdcc4U:
        case 0xdcc8U:
        case 0xdccaU:
        case 0xdcceU:
        case 0xdcd2U:
        case 0xdcd4U:
        case 0xdcd8U:
        case 0xdcdcU:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
