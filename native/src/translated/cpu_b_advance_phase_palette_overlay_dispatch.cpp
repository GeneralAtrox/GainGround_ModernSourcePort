// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"
#include "cpu_b_advance_phase_palette_overlay_dispatch_detail.h"

namespace gain_ground::translated {
FunctionResult cpu_b_advance_phase_palette_overlay_dispatch(FunctionContext &c) noexcept {
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
        case 0x9bc4U: { // 4eb900017054: jsr
            next = 0x9bcaU;
            const auto target = 0x17054U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_absolute);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x9bcaU: { // 33fc00010040401a: move-immediate-word-absolute-long
            next = 0x9bd2U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = 0x40401aU;
            m.logic(0x1U, 16U);
            t.prefetch(pc + 8U);
            t.word(destination, 0x1U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0x9bd2U: { // 4eb9000085ac: jsr
            next = 0x9bd8U;
            const auto target = 0x85acU;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_absolute);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x9bd8U: { // 4eb9000085f2: jsr
            next = 0x9bdeU;
            const auto target = 0x85f2U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_absolute);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x9bdeU: { // 4eb900008622: jsr
            next = 0x9be4U;
            const auto target = 0x8622U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_absolute);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x9be4U: { // 4eb900008654: jsr
            next = 0x9beaU;
            const auto target = 0x8654U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_absolute);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x9beaU: { // 61000024: bsr
            next = 0x9beeU;
            const auto target = 0x9c10U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x9beeU: { // 4eb90000a618: jsr
            next = 0x9bf4U;
            const auto target = 0xa618U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_absolute);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x9bf4U: { // 41f87402: lea
            next = 0x9bf8U;
            r.address[0] = 0x7402U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x9bf8U: { // 30fc0060: move-to-indirect
            next = 0x9bfcU;
            t.prefetch(pc + 4U);
            const auto destination = r.address[0];
            m.logic(0x60U, 16U);
            t.word(destination, 0x60U);
            r.address[0] += 2U;
            t.prefetch(pc + 6U);
            break;
        }
        case 0x9bfcU: { // 30fc0080: move-to-indirect
            next = 0x9c00U;
            t.prefetch(pc + 4U);
            const auto destination = r.address[0];
            m.logic(0x80U, 16U);
            t.word(destination, 0x80U);
            r.address[0] += 2U;
            t.prefetch(pc + 6U);
            break;
        }
        case 0x9c00U: { // 30fc0805: move-to-indirect
            next = 0x9c04U;
            t.prefetch(pc + 4U);
            const auto destination = r.address[0];
            m.logic(0x805U, 16U);
            t.word(destination, 0x805U);
            r.address[0] += 2U;
            t.prefetch(pc + 6U);
            break;
        }
        case 0x9c04U: { // 20fc00024dea: move-long-register-or-immediate-to-memory
            next = 0x9c0aU;
            const auto value = 0x24deaU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = r.address[0];
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            r.address[0] += 4U;
            m.logic(value, 32U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x9c0aU: { // 4ef900008572: jmp-absolute-long
            next = 0x9c10U;
            t.prefetch(pc + 4U);
            t.prefetch(0x8572U); t.prefetch(0x8572U + 2U);
            next = 0x8572U; transfer_kind = 1U;
            break;
        }
        case 0xd394U: { // 6100080a: bsr
            next = 0xd398U;
            const auto target = 0xdba0U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xd398U: { // 61000848: bsr
            next = 0xd39cU;
            const auto target = 0xdbe2U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xd39cU: { // 61000892: bsr
            next = 0xd3a0U;
            const auto target = 0xdc30U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xd3d6U: { // 61000886: bsr
            next = 0xd3daU;
            const auto target = 0xdc5eU;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xd3f6U: { // 61000866: bsr
            next = 0xd3faU;
            const auto target = 0xdc5eU;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xd430U: { // 4eb90001700c: jsr
            next = 0xd436U;
            const auto target = 0x1700cU;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_absolute);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xd436U: { // 4e75: rts
            next = 0xd438U;
            return t.rts(pc);
            break;
        }
        case 0xd456U: { // 6100083c: bsr
            next = 0xd45aU;
            const auto target = 0xdc94U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xd470U: { // 4e75: rts
            next = 0xd472U;
            return t.rts(pc);
            break;
        }
        default:
            if (cpu_b_advance_phase_palette_overlay_dispatch_detail::dispatch_lookup_setup(
                    c, r, m, t, pc, next, transfer_kind))
                break;
            if (cpu_b_advance_phase_palette_overlay_dispatch_detail::dispatch_overlay_updates(
                    c, r, m, t, pc, next, transfer_kind))
                break;
            return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x9bc4U:
        case 0x9bcaU:
        case 0x9bd2U:
        case 0x9bd8U:
        case 0x9bdeU:
        case 0x9be4U:
        case 0x9beaU:
        case 0x9beeU:
        case 0x9bf4U:
        case 0x9bf8U:
        case 0x9bfcU:
        case 0x9c00U:
        case 0x9c04U:
        case 0x9c0aU:
        case 0xd394U:
        case 0xd398U:
        case 0xd39cU:
        case 0xd3a0U:
        case 0xd3a4U:
        case 0xd3a6U:
        case 0xd3aaU:
        case 0xd3aeU:
        case 0xd3b2U:
        case 0xd3b6U:
        case 0xd3baU:
        case 0xd3bcU:
        case 0xd3c2U:
        case 0xd3c6U:
        case 0xd3c8U:
        case 0xd3ccU:
        case 0xd3d2U:
        case 0xd3d4U:
        case 0xd3d6U:
        case 0xd3daU:
        case 0xd3deU:
        case 0xd3e2U:
        case 0xd3e4U:
        case 0xd3e6U:
        case 0xd3ecU:
        case 0xd3eeU:
        case 0xd3f4U:
        case 0xd3f6U:
        case 0xd3faU:
        case 0xd3feU:
        case 0xd402U:
        case 0xd404U:
        case 0xd408U:
        case 0xd40aU:
        case 0xd40eU:
        case 0xd412U:
        case 0xd416U:
        case 0xd41aU:
        case 0xd420U:
        case 0xd422U:
        case 0xd428U:
        case 0xd42cU:
        case 0xd430U:
        case 0xd436U:
        case 0xd438U:
        case 0xd43cU:
        case 0xd440U:
        case 0xd442U:
        case 0xd444U:
        case 0xd44aU:
        case 0xd44eU:
        case 0xd450U:
        case 0xd456U:
        case 0xd45aU:
        case 0xd45eU:
        case 0xd462U:
        case 0xd468U:
        case 0xd46aU:
        case 0xd470U:
            t.begin(next); break;
        default: return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
