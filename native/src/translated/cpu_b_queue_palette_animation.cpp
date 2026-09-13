// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_queue_palette_animation(FunctionContext &c) noexcept {
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
        case 0xdbe2U: { // 302d001c: move-to-data-register
            next = 0xdbe6U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x1cU);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdbe6U: { // 02400007: immediate-and
            next = 0xdbeaU;
            t.prefetch(pc + 4U);
            const auto value = r.data[0] & 0x7U;
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdbeaU: { // e540: shift-word
            next = 0xdbecU;
            scene_timing::shift_word(t, m, pc, 0U, 2U, true, true);
            break;
        }
        case 0xdbecU: { // 41f9000252d6: lea-absolute-long
            next = 0xdbf2U;
            const auto value = 0x252d6U;
            r.address[0] = (r.address[0] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[0] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xdbf2U: { // 20300000: move-memory-to-register
            next = 0xdbf6U;
            t.clocks(2U);
            const auto source = (r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x0U);
            t.prefetch(pc + 4U);
            const auto high = t.word(source);
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            m.logic(value, 32U);
            r.data[0] = value;
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdbf6U: { // 4eb900015e4e: jsr
            next = 0xdbfcU;
            const auto target = 0x15e4eU;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_absolute);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xdbfcU: { // 082d0000001d: immediate-bit-memory
            next = 0xdc02U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = (r.address[5] + 0x1dU);
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x1U) ? 0U : 4U));
            t.prefetch(pc + 8U);
            break;
        }
        case 0xdc02U: { // 6726: bcc
            next = 0xdc04U;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch(pc, 0xdc2aU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0xdc04U: { // 526d001e: quick-memory
            next = 0xdc08U;
            const auto address = (r.address[5] + 0x1eU);
            t.prefetch(pc + 4U);
            const auto old = t.word(address);
            const auto value = m.add(old, 1U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, value);
            break;
        }
        case 0xdc08U: { // 302d001e: move-to-data-register
            next = 0xdc0cU;
            t.prefetch(pc + 4U);
            const auto source = (r.address[5] + 0x1eU);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdc0cU: { // 02400007: immediate-and
            next = 0xdc10U;
            t.prefetch(pc + 4U);
            const auto value = r.data[0] & 0x7U;
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdc10U: { // e540: shift-word
            next = 0xdc12U;
            scene_timing::shift_word(t, m, pc, 0U, 2U, true, true);
            break;
        }
        case 0xdc12U: { // 43f9000252f6: lea-absolute-long
            next = 0xdc18U;
            const auto value = 0x252f6U;
            r.address[1] = (r.address[1] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[1] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0xdc18U: { // 22710000: move-memory-to-register
            next = 0xdc1cU;
            t.clocks(2U);
            const auto source = (r.address[1] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x0U);
            t.prefetch(pc + 4U);
            const auto high = t.word(source);
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            r.address[1] = value;
            t.prefetch(pc + 6U);
            break;
        }
        case 0xdc1cU: { // 3419: move-to-data-register
            next = 0xdc1eU;
            const auto source = r.address[1];
            r.address[1] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(2U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdc1eU: { // 2019: move-memory-to-register
            next = 0xdc20U;
            const auto source = r.address[1];
            const auto high = t.word(source);
            r.address[1] += 4U;
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            m.logic(value, 32U);
            r.data[0] = value;
            t.prefetch(pc + 4U);
            break;
        }
        case 0xdc20U: { // 4eb900015e4e: jsr
            next = 0xdc26U;
            const auto target = 0x15e4eU;
            const auto result = scene_timing::call(c, t, m, pc, target, next, scene_timing::CallForm::jsr_absolute);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0xdc26U: { // 51cafff6: dbf
            next = 0xdc2aU;
            next = t.dbf(pc, 0xdc1eU, 2U);
            if ((r.data[2] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0xdc2aU: { // 526d001c: quick-memory
            next = 0xdc2eU;
            const auto address = (r.address[5] + 0x1cU);
            t.prefetch(pc + 4U);
            const auto old = t.word(address);
            const auto value = m.add(old, 1U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, value);
            break;
        }
        case 0xdc2eU: { // 4e75: rts
            next = 0xdc30U;
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
        case 0xdbe2U:
        case 0xdbe6U:
        case 0xdbeaU:
        case 0xdbecU:
        case 0xdbf2U:
        case 0xdbf6U:
        case 0xdbfcU:
        case 0xdc02U:
        case 0xdc04U:
        case 0xdc08U:
        case 0xdc0cU:
        case 0xdc10U:
        case 0xdc12U:
        case 0xdc18U:
        case 0xdc1cU:
        case 0xdc1eU:
        case 0xdc20U:
        case 0xdc26U:
        case 0xdc2aU:
        case 0xdc2eU:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
