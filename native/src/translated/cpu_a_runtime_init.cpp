// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_runtime_init(FunctionContext &c) noexcept {
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
        case 0x800c0U: { // 4ff80000: lea
            next = 0x800c4U;
            r.address[7] = 0x0U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x800c4U: { // 46fc2700: move-immediate-supervisor-status
            next = 0x800c8U;
            if (!(r.status & 0x2000U)) return {TranslationStatus::contract_violation, 0U, pc};
            t.prefetch(pc + 4U);
            t.clocks(2U);
            r.status = 0x2700U;
            t.clocks(2U);
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x800c8U: { // 4eba0050: jsr
            next = 0x800ccU;
            const auto target = 0x8011aU;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_pc);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x800ccU: { // 4eba0128: jsr
            next = 0x800d0U;
            const auto target = 0x801f6U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_pc);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x800d0U: { // 4eba0164: jsr
            next = 0x800d4U;
            const auto target = 0x80236U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_pc);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x800d4U: { // 33fc00010040401a: move-immediate-word-absolute-long
            next = 0x800dcU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0x40401aU;
            m.logic(0x1U, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0x1U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x800dcU: { // 4eba01ce: jsr
            next = 0x800e0U;
            const auto target = 0x802acU;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_pc);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x800e0U: { // 33fc000100a00002: move-immediate-word-absolute-long
            next = 0x800e8U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0xa00002U;
            m.logic(0x1U, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0x1U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x800e8U: { // 33fc001800a00004: move-immediate-word-absolute-long
            next = 0x800f0U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0xa00004U;
            m.logic(0x18U, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0x18U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x800f0U: { // 33fc001800a00006: move-immediate-word-absolute-long
            next = 0x800f8U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0xa00006U;
            m.logic(0x18U, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0x18U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x800f8U: { // 33fc00060080001c: move-immediate-word-absolute-long
            next = 0x80100U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0x80001cU;
            m.logic(0x6U, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0x6U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x80100U: { // 46fc2000: move-immediate-supervisor-status
            next = 0x80104U;
            if (!(r.status & 0x2000U)) return {TranslationStatus::contract_violation, 0U, pc};
            t.prefetch(pc + 4U);
            t.clocks(2U);
            r.status = 0x2000U;
            t.clocks(2U);
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x800c0U:
        case 0x800c4U:
        case 0x800c8U:
        case 0x800ccU:
        case 0x800d0U:
        case 0x800d4U:
        case 0x800dcU:
        case 0x800e0U:
        case 0x800e8U:
        case 0x800f0U:
        case 0x800f8U:
        case 0x80100U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
