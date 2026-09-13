// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_enqueue_palette_block_copy(FunctionContext &c) noexcept {
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
        case 0x15e4eU: { // 41f874ea: lea
            next = 0x15e52U;
            r.address[0] = 0x74eaU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x15e52U: { // 4ad8: tas-postincrement
            next = 0x15e54U;
            const auto address = r.address[0];
            r.address[0] += 1U;
            const auto old = t.byte(address); t.clocks(2U);
            m.logic(old, 8U); t.byte(address, old | 0x80U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x15e54U: { // 7200: moveq
            next = 0x15e56U;
            r.data[1] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x15e56U: { // 1210: move-to-data-register
            next = 0x15e58U;
            const auto source = r.address[0];
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(1U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x15e58U: { // 5218: quick-memory
            next = 0x15e5aU;
            const auto address = r.address[0];
            r.address[0] += 1U;
            const auto old = t.byte(address);
            const auto value = m.add(old, 1U, 8U);
            t.prefetch(pc + 4U);
            t.byte(address, value);
            break;
        }
        case 0x15e5aU: { // e541: shift-word
            next = 0x15e5cU;
            scene_timing::shift_word(t, m, pc, 1U, 2U, true, true);
            break;
        }
        case 0x15e5cU: { // 21801000: move-long-register-or-immediate-to-memory
            next = 0x15e60U;
            const auto value = r.data[0];
            t.clocks(2U);
            const auto destination = (r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[1])) + 0x0U);
            t.prefetch(pc + 4U);
            scene_timing::nz_high(r, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            m.logic(value, 32U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x15e60U: { // 4228fffe: clear-memory
            next = 0x15e64U;
            const auto address = (r.address[0] + 0xfffffffeU);
            t.prefetch(pc + 4U);
            (void)t.byte(address);
            m.logic(0U, 8U);
            t.prefetch(pc + 6U);
            t.byte(address, 0U);
            break;
        }
        case 0x15e64U: { // 4e75: rts
            next = 0x15e66U;
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
        case 0x15e4eU:
        case 0x15e52U:
        case 0x15e54U:
        case 0x15e56U:
        case 0x15e58U:
        case 0x15e5aU:
        case 0x15e5cU:
        case 0x15e60U:
        case 0x15e64U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
