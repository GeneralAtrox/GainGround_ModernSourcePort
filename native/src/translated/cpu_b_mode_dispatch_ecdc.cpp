// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_mode_dispatch_ecdc(FunctionContext &c) noexcept {
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
        case 0xecdcU: { // 30380834: move-to-data-register
            next = 0xece0U;
            t.prefetch(pc + 4U);
            const auto source = 0x834U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xece0U: { // e540: shift-word
            next = 0xece2U;
            scene_timing::shift_word(t, m, pc, 0U, 2U, true, true);
            break;
        }
        case 0xece2U: { // 4efb0002: jmp-pc-indexed
            next = 0xece6U;
            t.clocks(2U);
            const auto target = (0xece4U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x2U);
            t.clocks(4U); t.prefetch(target); t.prefetch(target + 2U);
            next = target; transfer_kind = 1U;
            break;
        }
        case 0xece6U: { // 6000000e: bcc
            next = 0xeceaU;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xecf6U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xeceaU: { // 60000028: bcc
            next = 0xeceeU;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xed14U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xeceeU: { // 600000ee: bcc
            next = 0xecf2U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xeddeU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xecf2U: { // 60000114: bcc
            next = 0xecf6U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xee08U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xecf6U: { // 2b7c0000ecfe0002: move-long-register-or-immediate-to-memory
            next = 0xecfeU;
            const auto value = 0xecfeU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = (r.address[5] + 0x2U);
            t.prefetch(pc + 8U);
            scene_timing::nz_high(r, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            m.logic(value, 32U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0xed14U: { // 2b7c0000edb80002: move-long-register-or-immediate-to-memory
            next = 0xed1cU;
            const auto value = 0xedb8U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = (r.address[5] + 0x2U);
            t.prefetch(pc + 8U);
            scene_timing::nz_high(r, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            m.logic(value, 32U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0xed1cU: { // 3b7c00050044: move-immediate-16-to-displacement
            next = 0xed22U;
            const auto value = 0x5U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x44U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed22U: { // 7000: moveq
            next = 0xed24U;
            r.data[0] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xed24U: { // 102d004b: move-to-data-register
            next = 0xed28U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x4bU);
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xed28U: { // e940: shift-word
            next = 0xed2aU;
            scene_timing::shift_word(t, m, pc, 0U, 4U, true, true);
            break;
        }
        case 0xed2aU: { // 41f900026f1c: lea-absolute-long
            next = 0xed30U;
            const auto value = 0x26f1cU;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed30U: { // d0c0: adda-word
            next = 0xed32U;
            scene_timing::address_add(t, r, 0U, static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])), pc + 4U);
            break;
        }
        case 0xed32U: { // 2b48004c: move-long-register-or-immediate-to-memory
            next = 0xed36U;
            const auto value = r.address[0];
            const auto destination = (r.address[5] + 0x4cU);
            t.prefetch(pc + 4U);
            scene_timing::nz_high(r, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            m.logic(value, 32U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xed36U: { // 3b6800060036: move-memory-to-extended-memory
            next = 0xed3cU;
            const auto source = (r.address[0] + 0x6U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            const auto destination = (r.address[5] + 0x36U);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed3cU: { // 3b6800080038: move-memory-to-extended-memory
            next = 0xed42U;
            const auto source = (r.address[0] + 0x8U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            const auto destination = (r.address[5] + 0x38U);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed42U: { // 3b68000a003a: move-memory-to-extended-memory
            next = 0xed48U;
            const auto source = (r.address[0] + 0xaU);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            const auto destination = (r.address[5] + 0x3aU);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed48U: { // 42ad003c: clear-memory
            next = 0xed4cU;
            const auto address = (r.address[5] + 0x3cU);
            t.prefetch(pc + 4U);
            (void)t.word(address);
            (void)t.word(address + 2U);
            m.logic(0U, 32U);
            t.prefetch(pc + 6U);
            t.word(address + 2U, 0U);
            t.word(address, 0U);
            break;
        }
        case 0xed4cU: { // 1b7c0000005a: move-immediate-8-to-displacement
            next = 0xed52U;
            const auto value = 0x0U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x5aU);
            t.prefetch(pc + 6U);
            m.logic(value, 8U);
            t.byte(destination, static_cast<std::uint8_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed52U: { // 1b7c0000005b: move-immediate-8-to-displacement
            next = 0xed58U;
            const auto value = 0x0U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x5bU);
            t.prefetch(pc + 6U);
            m.logic(value, 8U);
            t.byte(destination, static_cast<std::uint8_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed58U: { // 3b7c00000048: move-immediate-16-to-displacement
            next = 0xed5eU;
            const auto value = 0x0U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x48U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed5eU: { // 3b6d00700012: move-memory-to-extended-memory
            next = 0xed64U;
            const auto source = (r.address[5] + 0x70U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            const auto destination = (r.address[5] + 0x12U);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            t.word(destination, value);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed64U: { // 3b7c00080016: move-immediate-16-to-displacement
            next = 0xed6aU;
            const auto value = 0x8U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x16U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed6aU: { // 3b7c0180001a: move-immediate-16-to-displacement
            next = 0xed70U;
            const auto value = 0x180U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x1aU);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed70U: { // 3b7c00060058: move-immediate-16-to-displacement
            next = 0xed76U;
            const auto value = 0x6U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x58U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed76U: { // 3b7c00180052: move-immediate-16-to-displacement
            next = 0xed7cU;
            const auto value = 0x18U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0x52U);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed7cU: { // 426d005c: clear-memory
            next = 0xed80U;
            const auto address = (r.address[5] + 0x5cU);
            t.prefetch(pc + 4U);
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, 0U);
            break;
        }
        case 0xed80U: { // 61000f14: bsr
            next = 0xed84U;
            const auto target = 0xfc96U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xed84U: { // 41f90020220a: lea-absolute-long
            next = 0xed8aU;
            const auto value = 0x20220aU;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed8aU: { // 7000: moveq
            next = 0xed8cU;
            r.data[0] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xed8cU: { // 102d004b: move-to-data-register
            next = 0xed90U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x4bU);
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xed90U: { // e940: shift-word
            next = 0xed92U;
            scene_timing::shift_word(t, m, pc, 0U, 4U, true, true);
            break;
        }
        case 0xed92U: { // 43f900026f1c: lea-absolute-long
            next = 0xed98U;
            const auto value = 0x26f1cU;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xed98U: { // 30310000: move-memory-to-register
            next = 0xed9cU;
            t.clocks(2U);
            const auto source = (r.address[1] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x0U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xed9cU: { // 61000eaa: bsr
            next = 0xeda0U;
            const auto target = 0xfc48U;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::bsr);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xeda0U: { // 323c07f0: move-to-data-register
            next = 0xeda4U;
            t.prefetch(pc + 4U);
            m.logic(0x7f0U, 16U);
            m.dw(1U, 0x7f0U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xeda4U: { // 41f90020c100: lea-absolute-long
            next = 0xedaaU;
            const auto value = 0x20c100U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xedaaU: { // d0ed0078: adda-word-memory
            next = 0xedaeU;
            const auto address = (r.address[5] + 0x78U);
            t.prefetch(pc + 4U);
            const auto source = static_cast<std::uint32_t>(static_cast<std::int16_t>(t.word(address)));
            scene_timing::address_add(t, r, 0U, source, pc + 6U);
            break;
        }
        case 0xedaeU: { // 7037: moveq
            next = 0xedb0U;
            r.data[0] = 0x37U;
            m.logic(0x37U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xedb0U: { // 3081: move-to-indirect
            next = 0xedb2U;
            const auto destination = r.address[0];
            m.logic(r.data[1], 16U);
            t.word(destination, r.data[1]);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xedb2U: { // 5048: quick-word-address
            next = 0xedb4U;
            scene_timing::address_add(t, r, 0U, 0x8U, pc + 4U);
            break;
        }
        case 0xedb4U: { // 51c8fffa: dbf
            next = 0xedb8U;
            next = t.dbf(pc, 0xedb0U, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xeddeU: { // 2b7c0000ede60002: move-long-register-or-immediate-to-memory
            next = 0xede6U;
            const auto value = 0xede6U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = (r.address[5] + 0x2U);
            t.prefetch(pc + 8U);
            scene_timing::nz_high(r, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            m.logic(value, 32U);
            t.prefetch(pc + 10U);
            break;
        }
        case 0xee08U: { // 102d006d: move-to-data-register
            next = 0xee0cU;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x6dU);
            const auto value = t.byte(source);
            m.logic(value, 8U);
            m.db(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xee0cU: { // 01380820: btst-register-absolute-word
            next = 0xee10U;
            const auto bit = r.data[0] & 7U;
            t.prefetch(pc + 4U);
            const auto value = t.byte(0x820U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((value & (1U << bit)) ? 0U : 4U));
            t.prefetch(pc + 6U);
            break;
        }
        case 0xee10U: { // 6616: bcc
            next = 0xee12U;
            const bool taken = scene_timing::condition(r.status, 6U);
            next = t.branch(pc, 0xee28U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xee12U: { // 2b7c0000ee1a0002: move-long-register-or-immediate-to-memory
            next = 0xee1aU;
            const auto value = 0xee1aU;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto destination = (r.address[5] + 0x2U);
            t.prefetch(pc + 8U);
            scene_timing::nz_high(r, static_cast<std::uint16_t>(value >> 16U));
            t.word(destination, static_cast<std::uint16_t>(value >> 16U));
            m.logic(value, 16U); t.word(destination + 2U, static_cast<std::uint16_t>(value));
            m.logic(value, 32U);
            t.prefetch(pc + 10U);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xecdcU:
        case 0xece0U:
        case 0xece2U:
        case 0xece6U:
        case 0xeceaU:
        case 0xeceeU:
        case 0xecf2U:
        case 0xecf6U:
        case 0xed14U:
        case 0xed1cU:
        case 0xed22U:
        case 0xed24U:
        case 0xed28U:
        case 0xed2aU:
        case 0xed30U:
        case 0xed32U:
        case 0xed36U:
        case 0xed3cU:
        case 0xed42U:
        case 0xed48U:
        case 0xed4cU:
        case 0xed52U:
        case 0xed58U:
        case 0xed5eU:
        case 0xed64U:
        case 0xed6aU:
        case 0xed70U:
        case 0xed76U:
        case 0xed7cU:
        case 0xed80U:
        case 0xed84U:
        case 0xed8aU:
        case 0xed8cU:
        case 0xed90U:
        case 0xed92U:
        case 0xed98U:
        case 0xed9cU:
        case 0xeda0U:
        case 0xeda4U:
        case 0xedaaU:
        case 0xedaeU:
        case 0xedb0U:
        case 0xedb2U:
        case 0xedb4U:
        case 0xeddeU:
        case 0xee08U:
        case 0xee0cU:
        case 0xee10U:
        case 0xee12U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
