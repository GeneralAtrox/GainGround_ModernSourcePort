// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_upload_initial_palette_blocks(FunctionContext &c) noexcept {
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
        case 0x802e6U: { // 43f900080818: lea-absolute-long
            next = 0x802ecU;
            const auto value = 0x80818U;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x802ecU: { // 4a51: tst-memory
            next = 0x802eeU;
            const auto address = r.address[1];
            const auto value = t.word(address);
            m.logic(value, 16U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x802eeU: { // 6b000028: bcc
            next = 0x802f2U;
            const bool taken = scene_timing::condition(r.status, 11U);
            next = t.branch_word(pc, 0x80318U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x802f2U: { // 41f900400000: lea-absolute-long
            next = 0x802f8U;
            const auto value = 0x400000U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x802f8U: { // d0d9: adda-word-memory
            next = 0x802faU;
            const auto address = r.address[1];
            r.address[1] += 2U;
            const auto source = static_cast<std::uint32_t>(static_cast<std::int16_t>(t.word(address)));
            scene_timing::address_add(t, r, 0U, source, pc + 4U);
            break;
        }
        case 0x802faU: { // 3419: move-to-data-register
            next = 0x802fcU;
            const auto source = r.address[1];
            r.address[1] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(2U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x802fcU: { // 7200: moveq
            next = 0x802feU;
            r.data[1] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x802feU: { // 303c000f: move-to-data-register
            next = 0x80302U;
            t.prefetch(pc + 4U);
            m.logic(0xfU, 16U);
            m.dw(0U, 0xfU);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x80302U: { // 30f11000: move-word-indexed-to-postincrement
            next = 0x80306U;
            t.clocks(2U);
            const auto source = (r.address[1] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[1])) + 0x0U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            const auto destination = r.address[0];
            m.logic(value, 16U);
            t.word(destination, value);
            r.address[0] += 2U;
            t.prefetch(pc + 6U);
            break;
        }
        case 0x80306U: { // 5441: quick-register
            next = 0x80308U;
            const auto value = m.add(r.data[1], 2U, 16U);
            m.dw(1U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x80308U: { // 51c8fff8: dbf
            next = 0x8030cU;
            next = t.dbf(pc, 0x80302U, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x8030cU: { // 51caffee: dbf
            next = 0x80310U;
            next = t.dbf(pc, 0x802fcU, 2U);
            if ((r.data[2] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x80310U: { // d2fc0020: adda-word
            next = 0x80314U;
            t.prefetch(pc + 4U);
            scene_timing::address_add(t, r, 1U, 0x20U, pc + 6U);
            break;
        }
        case 0x80314U: { // 6000ffd6: bcc
            next = 0x80318U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0x802ecU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x80318U: { // 4e75: rts
            next = 0x8031aU;
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
        case 0x802e6U:
        case 0x802ecU:
        case 0x802eeU:
        case 0x802f2U:
        case 0x802f8U:
        case 0x802faU:
        case 0x802fcU:
        case 0x802feU:
        case 0x80302U:
        case 0x80306U:
        case 0x80308U:
        case 0x8030cU:
        case 0x80310U:
        case 0x80314U:
        case 0x80318U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
