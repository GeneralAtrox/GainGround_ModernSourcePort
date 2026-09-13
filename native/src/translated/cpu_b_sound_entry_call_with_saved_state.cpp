// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sound_entry_call_with_saved_state(FunctionContext &c) noexcept {
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
        case 0x1700cU: { // 2f0d: move-long-register-or-immediate-to-memory
            next = 0x1700eU;
            const auto value = r.address[5];
            const auto destination = r.address[7] - 4U;
            m.logic(value, 16U);
            t.prefetch(pc + 4U);
            m.logic(value, 32U);
            t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[7] = destination;
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            break;
        }
        case 0x1700eU: { // 2f0e: move-long-register-or-immediate-to-memory
            next = 0x17010U;
            const auto value = r.address[6];
            const auto destination = r.address[7] - 4U;
            m.logic(value, 16U);
            t.prefetch(pc + 4U);
            m.logic(value, 32U);
            t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[7] = destination;
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            break;
        }
        case 0x17010U: { // 4df9ffffc000: lea-absolute-long
            next = 0x17016U;
            const auto value = 0xffffc000U;
            r.address[6] = (r.address[6] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[6] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x17016U: { // 4bf900fb0000: lea-absolute-long
            next = 0x1701cU;
            const auto value = 0xfb0000U;
            r.address[5] = (r.address[5] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[5] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x1701cU: { // 61000050: bsr
            next = 0x17020U;
            const auto target = 0x1706eU;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x17020U: { // 2c5f: move-memory-to-register
            next = 0x17022U;
            const auto source = r.address[7];
            const auto high = t.word(source);
            r.address[7] += 4U;
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            r.address[6] = value;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x17022U: { // 2a5f: move-memory-to-register
            next = 0x17024U;
            const auto source = r.address[7];
            const auto high = t.word(source);
            r.address[7] += 4U;
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            r.address[5] = value;
            t.prefetch(pc + 4U);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1700cU:
        case 0x1700eU:
        case 0x17010U:
        case 0x17016U:
        case 0x1701cU:
        case 0x17020U:
        case 0x17022U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
