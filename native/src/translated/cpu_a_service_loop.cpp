// Source-derived ordinary instruction timing; full parity remains unverified.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_service_loop(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    CpuBIrqTiming t(c, m);
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        bool returned_from_child = false;
        switch (pc) {
        case 0x80104U: { // 4ff80000 lea.l $0.w, a7
            next = 0x80108U;
            r.address[7] = 0x0U;
            t.prefetch(pc + 4U); t.prefetch(pc + 6U);
            break;
        }
        case 0x80108U: { // 4eba0240 jsr $8034a(pc)
            next = 0x8010cU;
            const auto result = scene_timing::call(c, t, m, pc, 0x8034aU, next, scene_timing::CallForm::jsr_pc);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x8010cU: { // 4eba0248 jsr $80356(pc)
            next = 0x80110U;
            const auto result = scene_timing::call(c, t, m, pc, 0x80356U, next, scene_timing::CallForm::jsr_pc);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x80110U: { // 4eba0302 jsr $80414(pc)
            next = 0x80114U;
            const auto result = scene_timing::call(c, t, m, pc, 0x80414U, next, scene_timing::CallForm::jsr_pc);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x80114U: { // 4eba04fc jsr $80612(pc)
            next = 0x80118U;
            const auto result = scene_timing::call(c, t, m, pc, 0x80612U, next, scene_timing::CallForm::jsr_pc);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x80118U: { // 60ea bra.b $80104
            next = 0x8011aU;
            next = t.branch(pc, 0x80104U, true); transfer_kind = 1U;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        // A child already handled its return boundary; do not poll the old JSR.
        if (!returned_from_child)
            if (auto event = m.interrupt(c, pc, next)) return *event;
        if (pc == 0x80118U && c.host->consume_self_continuation_boundary(
                55U, 0U, 0xffU, 1U, pc, next, c))
            return FunctionResult::complete(4U, next);
        switch (next) {
        case 0x80104U:
        case 0x80108U:
        case 0x8010cU:
        case 0x80110U:
        case 0x80114U:
        case 0x80118U:
            t.begin(next); break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
