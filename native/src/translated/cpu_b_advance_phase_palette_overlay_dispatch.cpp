// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

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
        case 0xd3a0U: { // 302d000c: move-to-data-register
            next = 0xd3a4U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0xcU);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd3a4U: { // e540: shift-word
            next = 0xd3a6U;
            scene_timing::shift_word(t, m, pc, 0U, 2U, true, true);
            break;
        }
        case 0xd3a6U: { // 4efb0002: jmp-pc-indexed
            next = 0xd3aaU;
            t.clocks(2U);
            const auto target = (0xd3a8U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x2U);
            t.clocks(4U); t.prefetch(target); t.prefetch(target + 2U);
            next = target; transfer_kind = 1U;
            break;
        }
        case 0xd3aaU: { // 60000006: bcc
            next = 0xd3aeU;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xd3b2U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd3aeU: { // 60000088: bcc
            next = 0xd3b2U;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0xd438U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd3b2U: { // 342d0010: move-to-data-register
            next = 0xd3b6U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x10U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(2U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd3b6U: { // 0c420018: immediate-cmp
            next = 0xd3baU;
            t.prefetch(pc + 4U);
            const auto value = m.sub(r.data[2], 0x18U, 16U, true);
            (void)value; // CMPI updates flags without storing a result.
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd3baU: { // 6a42: bcc
            next = 0xd3bcU;
            const bool taken = scene_timing::condition(r.status, 10U);
            next = t.branch(pc, 0xd3feU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd3bcU: { // 41f900202000: lea-absolute-long
            next = 0xd3c2U;
            const auto value = 0x202000U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd3c2U: { // 303c0017: move-to-data-register
            next = 0xd3c6U;
            t.prefetch(pc + 4U);
            m.logic(0x17U, 16U);
            m.dw(0U, 0x17U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd3c6U: { // 9042: register-sub
            next = 0xd3c8U;
            const auto value = m.sub(r.data[0], r.data[2], 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd3c8U: { // c0fc0062: multiply-unsigned-immediate
            next = 0xd3ccU;
            scene_timing::multiply_unsigned(t, m, pc, 0U, 0x62U);
            break;
        }
        case 0xd3ccU: { // 43f900008960: lea-absolute-long
            next = 0xd3d2U;
            const auto value = 0x8960U;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd3d2U: { // d2c0: adda-word
            next = 0xd3d4U;
            scene_timing::address_add(t, r, 1U, static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])), pc + 4U);
            break;
        }
        case 0xd3d4U: { // 3202: move-to-data-register
            next = 0xd3d6U;
            m.logic(r.data[2], 16U);
            m.dw(1U, r.data[2]);
            t.prefetch(pc + 4U);
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
        case 0xd3daU: { // 51c9fffa: dbf
            next = 0xd3deU;
            next = t.dbf(pc, 0xd3d6U, 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xd3deU: { // 303c002f: move-to-data-register
            next = 0xd3e2U;
            t.prefetch(pc + 4U);
            m.logic(0x2fU, 16U);
            m.dw(0U, 0x2fU);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd3e2U: { // 9042: register-sub
            next = 0xd3e4U;
            const auto value = m.sub(r.data[0], r.data[2], 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd3e4U: { // ef40: shift-word
            next = 0xd3e6U;
            scene_timing::shift_word(t, m, pc, 0U, 7U, true, true);
            break;
        }
        case 0xd3e6U: { // 41f900202000: lea-absolute-long
            next = 0xd3ecU;
            const auto value = 0x202000U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd3ecU: { // d0c0: adda-word
            next = 0xd3eeU;
            scene_timing::address_add(t, r, 0U, static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])), pc + 4U);
            break;
        }
        case 0xd3eeU: { // 43f900009290: lea-absolute-long
            next = 0xd3f4U;
            const auto value = 0x9290U;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd3f4U: { // 3202: move-to-data-register
            next = 0xd3f6U;
            m.logic(r.data[2], 16U);
            m.dw(1U, r.data[2]);
            t.prefetch(pc + 4U);
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
        case 0xd3faU: { // 51c9fffa: dbf
            next = 0xd3feU;
            next = t.dbf(pc, 0xd3f6U, 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xd3feU: { // 302d0010: move-to-data-register
            next = 0xd402U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x10U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd402U: { // e740: shift-word
            next = 0xd404U;
            scene_timing::shift_word(t, m, pc, 0U, 3U, true, true);
            break;
        }
        case 0xd404U: { // 323c0198: move-to-data-register
            next = 0xd408U;
            t.prefetch(pc + 4U);
            m.logic(0x198U, 16U);
            m.dw(1U, 0x198U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd408U: { // 9240: register-sub
            next = 0xd40aU;
            const auto value = m.sub(r.data[1], r.data[0], 16U);
            m.dw(1U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd40aU: { // 31c1740e: move-to-displacement
            next = 0xd40eU;
            const auto destination = 0x740eU;
            m.logic(r.data[1], 16U);
            t.prefetch(pc + 4U);
            t.word(destination, r.data[1]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd40eU: { // d07cff78: word-source-add
            next = 0xd412U;
            const auto source = 0xff78U;
            t.prefetch(pc + 4U);
            m.dw(0U, m.add(r.data[0], source, 16U));
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd412U: { // 31c07404: move-to-displacement
            next = 0xd416U;
            const auto destination = 0x7404U;
            m.logic(r.data[0], 16U);
            t.prefetch(pc + 4U);
            t.word(destination, r.data[0]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd416U: { // 526d0010: quick-memory
            next = 0xd41aU;
            const auto address = (r.address[5] + 0x10U);
            t.prefetch(pc + 4U);
            const auto old = t.word(address);
            const auto value = m.add(old, 1U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, value);
            break;
        }
        case 0xd41aU: { // 0c6d002d0010: compare-immediate-word-memory
            next = 0xd420U;
            t.prefetch(pc + 4U);
            const auto address = (r.address[5] + 0x10U);
            t.prefetch(pc + 6U);
            const auto old = t.word(address);
            (void)m.sub(old, 0x2dU, 16U, true);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd420U: { // 6f14: bcc
            next = 0xd422U;
            const bool taken = scene_timing::condition(r.status, 15U);
            next = t.branch(pc, 0xd436U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd422U: { // 3b7c0001000c: move-immediate-16-to-displacement
            next = 0xd428U;
            const auto value = 0x1U;
            t.prefetch(pc + 4U);
            const auto destination = (r.address[5] + 0xcU);
            t.prefetch(pc + 6U);
            m.logic(value, 16U);
            t.word(destination, static_cast<std::uint16_t>(value));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd428U: { // 426d0010: clear-memory
            next = 0xd42cU;
            const auto address = (r.address[5] + 0x10U);
            t.prefetch(pc + 4U);
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, 0U);
            break;
        }
        case 0xd42cU: { // 303c0052: move-to-data-register
            next = 0xd430U;
            t.prefetch(pc + 4U);
            m.logic(0x52U, 16U);
            m.dw(0U, 0x52U);
            t.prefetch(pc + 6U);
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
        case 0xd438U: { // 302d0010: move-to-data-register
            next = 0xd43cU;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x10U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd43cU: { // 0c400023: immediate-cmp
            next = 0xd440U;
            t.prefetch(pc + 4U);
            const auto value = m.sub(r.data[0], 0x23U, 16U, true);
            (void)value; // CMPI updates flags without storing a result.
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd440U: { // 6e1c: bcc
            next = 0xd442U;
            const bool taken = scene_timing::condition(r.status, 14U);
            next = t.branch(pc, 0xd45eU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd442U: { // e540: shift-word
            next = 0xd444U;
            scene_timing::shift_word(t, m, pc, 0U, 2U, true, true);
            break;
        }
        case 0xd444U: { // 41f900024f36: lea-absolute-long
            next = 0xd44aU;
            const auto value = 0x24f36U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd44aU: { // 28700000: move-memory-to-register
            next = 0xd44eU;
            t.clocks(2U);
            const auto source = (r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x0U);
            t.prefetch(pc + 4U);
            const auto high = t.word(source);
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            r.address[4] = value;
            t.prefetch(pc + 6U);
            break;
        }
        case 0xd44eU: { // 3e1c: move-to-data-register
            next = 0xd450U;
            const auto source = r.address[4];
            r.address[4] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(7U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xd450U: { // 47f900024ef6: lea-absolute-long
            next = 0xd456U;
            const auto value = 0x24ef6U;
            r.address[3] = (r.address[3] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[3] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
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
        case 0xd45aU: { // 51cffffa: dbf
            next = 0xd45eU;
            next = t.dbf(pc, 0xd456U, 7U);
            if ((r.data[7] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xd45eU: { // 526d0010: quick-memory
            next = 0xd462U;
            const auto address = (r.address[5] + 0x10U);
            t.prefetch(pc + 4U);
            const auto old = t.word(address);
            const auto value = m.add(old, 1U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, value);
            break;
        }
        case 0xd462U: { // 0c6d010b0010: compare-immediate-word-memory
            next = 0xd468U;
            t.prefetch(pc + 4U);
            const auto address = (r.address[5] + 0x10U);
            t.prefetch(pc + 6U);
            const auto old = t.word(address);
            (void)m.sub(old, 0x10bU, 16U, true);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xd468U: { // 6f06: bcc
            next = 0xd46aU;
            const bool taken = scene_timing::condition(r.status, 15U);
            next = t.branch(pc, 0xd470U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xd46aU: { // 4ef900009bc4: jmp-absolute-long
            next = 0xd470U;
            t.prefetch(pc + 4U);
            t.prefetch(0x9bc4U); t.prefetch(0x9bc4U + 2U);
            next = 0x9bc4U; transfer_kind = 1U;
            break;
        }
        case 0xd470U: { // 4e75: rts
            next = 0xd472U;
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
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
