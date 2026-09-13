// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_step_overlay_animation(FunctionContext &c) noexcept {
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
        case 0xdc30U: { // 536d0020: quick-memory
            next = 0xdc34U;
            const auto address = (r.address[5] + 0x20U);
            t.prefetch(pc + 4U);
            const auto old = t.word(address);
            const auto value = m.sub(old, 1U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, value);
            break;
        }
        case 0xdc34U: { // 6e26: bcc
            next = 0xdc36U;
            const bool taken = scene_timing::condition(r.status, 14U);
            next = t.branch(pc, 0xdc5cU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xdc36U: { // 3b7c00030020: move-immediate-16-to-displacement
            next = 0xdc3cU;
            const auto value = 0x3U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x20U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xdc3cU: { // 302d0022: move-to-data-register
            next = 0xdc40U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x22U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdc40U: { // 5240: quick-register
            next = 0xdc42U;
            const auto value = m.add(r.data[0], 1U, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdc42U: { // 0c40000e: immediate-cmp
            next = 0xdc46U;
            t.prefetch(pc + 4U);
            const auto value = m.sub(r.data[0], 0xeU, 16U, true);
            (void)value; // CMPI updates flags without storing a result.
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdc46U: { // 6b02: bcc
            next = 0xdc48U;
            const bool taken = scene_timing::condition(r.status, 11U);
            next = t.branch(pc, 0xdc4aU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xdc48U: { // 7000: moveq
            next = 0xdc4aU;
            r.data[0] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdc4aU: { // 3b400022: move-to-displacement
            next = 0xdc4eU;
            const auto destination = (r.address[5] + 0x22U);
            t.prefetch(pc + 4U);
            m.logic(r.data[0], 16U);
            t.word(destination, r.data[0]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdc4eU: { // d040: register-add
            next = 0xdc50U;
            const auto value = m.add(r.data[0], r.data[0], 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdc50U: { // 41f9000253a6: lea-absolute-long
            next = 0xdc56U;
            const auto value = 0x253a6U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xdc56U: { // 31f000007406: move-memory-to-extended-memory
            next = 0xdc5cU;
            t.clocks(2U);
            const auto source = (r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x0U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            const auto destination = 0x7406U;
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xdc5cU: { // 4e75: rts
            next = 0xdc5eU;
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
        case 0xdc30U:
        case 0xdc34U:
        case 0xdc36U:
        case 0xdc3cU:
        case 0xdc40U:
        case 0xdc42U:
        case 0xdc46U:
        case 0xdc48U:
        case 0xdc4aU:
        case 0xdc4eU:
        case 0xdc50U:
        case 0xdc56U:
        case 0xdc5cU:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
