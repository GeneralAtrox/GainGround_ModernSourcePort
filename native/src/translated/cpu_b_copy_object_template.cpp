// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_copy_object_template(FunctionContext &c) noexcept {
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
        case 0xdc5eU: { // d0fc000c: adda-word
            next = 0xdc62U;
            t.prefetch(pc + 4U);
            scene_timing::address_add(t, r, 0U, 0xcU, pc + 6U);
            break;
        }
        case 0xdc62U: { // 20d9: move-long-postincrement-copy
            next = 0xdc64U;
            const auto source = r.address[1];
            const auto high = t.word(source);
            r.address[1] += 4U;
            const auto low = t.word(source + 2U);
            const auto destination = r.address[0];
            m.logic(low, 16U); t.word(destination, high);
            m.logic((std::uint32_t(high) << 16U) | low, 32U);
            t.word(destination + 2U, low);
            r.address[0] += 4U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdc64U: { // 20d9: move-long-postincrement-copy
            next = 0xdc66U;
            const auto source = r.address[1];
            const auto high = t.word(source);
            r.address[1] += 4U;
            const auto low = t.word(source + 2U);
            const auto destination = r.address[0];
            m.logic(low, 16U); t.word(destination, high);
            m.logic((std::uint32_t(high) << 16U) | low, 32U);
            t.word(destination + 2U, low);
            r.address[0] += 4U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdc66U: { // 30d9: move-to-indirect
            next = 0xdc68U;
            const auto source = r.address[1];
            r.address[1] += 2U;
            const auto value = t.word(source);
            const auto destination = r.address[0];
            m.logic(value, 16U);
            t.word(destination, value);
            r.address[0] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdc68U: { // d0fc000e: adda-word
            next = 0xdc6cU;
            t.prefetch(pc + 4U);
            scene_timing::address_add(t, r, 0U, 0xeU, pc + 6U);
            break;
        }
        case 0xdc6cU: { // 7015: moveq
            next = 0xdc6eU;
            r.data[0] = 0x15U;
            m.logic(0x15U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdc6eU: { // 20d9: move-long-postincrement-copy
            next = 0xdc70U;
            const auto source = r.address[1];
            const auto high = t.word(source);
            r.address[1] += 4U;
            const auto low = t.word(source + 2U);
            const auto destination = r.address[0];
            m.logic(low, 16U); t.word(destination, high);
            m.logic((std::uint32_t(high) << 16U) | low, 32U);
            t.word(destination + 2U, low);
            r.address[0] += 4U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdc70U: { // 51c8fffc: dbf
            next = 0xdc74U;
            next = t.dbf(pc, 0xdc6eU, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xdc74U: { // 5848: quick-word-address
            next = 0xdc76U;
            scene_timing::address_add(t, r, 0U, 0x4U, pc + 4U);
            break;
        }
        case 0xdc76U: { // 4e75: rts
            next = 0xdc78U;
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
        case 0xdc5eU:
        case 0xdc62U:
        case 0xdc64U:
        case 0xdc66U:
        case 0xdc68U:
        case 0xdc6cU:
        case 0xdc6eU:
        case 0xdc70U:
        case 0xdc74U:
        case 0xdc76U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
