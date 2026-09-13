// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_initial_fdc_load_batch(FunctionContext &c) noexcept {
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
        case 0x802acU: { // 46fc2700: move-immediate-supervisor-status
            next = 0x802b0U;
            if (!(r.status & 0x2000U)) return {TranslationStatus::contract_violation, 0U, pc};
            t.prefetch(pc + 4U);
            t.clocks(2U);
            r.status = 0x2700U;
            t.clocks(2U);
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x802b0U: { // 50f88001: set-true-absolute-byte
            next = 0x802b4U;
            t.prefetch(pc + 4U);
            const auto address = 0xffff8001U;
            (void)t.byte(address);
            t.prefetch(pc + 6U);
            t.byte(address, 0xffU);
            break;
        }
        case 0x802b4U: { // 70ff: moveq
            next = 0x802b6U;
            r.data[0] = 0xffffffffU;
            m.logic(0xffffffffU, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x802b6U: { // 4e48: supervisor-trap
            next = 0x802b8U;
            const auto child = t.supervisor_trap(pc, 40U, next);
            if (child.status != TranslationStatus::complete || child.control != 2U) return child;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x802b8U: { // 4df9000807dc: lea-absolute-long
            next = 0x802beU;
            const auto value = 0x807dcU;
            r.address[6] = (r.address[6] & 0xffffU) | (value & 0xffff0000U);
            t.prefetch(pc + 4U);
            r.address[6] = value;
            t.prefetch(pc + 6U);
            t.prefetch(pc + 8U);
            break;
        }
        case 0x802beU: { // 3e3c0004: move-to-data-register
            next = 0x802c2U;
            t.prefetch(pc + 4U);
            m.logic(0x4U, 16U);
            m.dw(7U, 0x4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x802c2U: { // 41f8d000: lea
            next = 0x802c6U;
            r.address[0] = 0xffffd000U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x802c6U: { // 225e: move-memory-to-register
            next = 0x802c8U;
            const auto source = r.address[6];
            const auto high = t.word(source);
            r.address[6] += 4U;
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            r.address[1] = value;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x802c8U: { // 201e: move-memory-to-register
            next = 0x802caU;
            const auto source = r.address[6];
            const auto high = t.word(source);
            r.address[6] += 4U;
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            m.logic(value, 32U);
            r.data[0] = value;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x802caU: { // 221e: move-memory-to-register
            next = 0x802ccU;
            const auto source = r.address[6];
            const auto high = t.word(source);
            r.address[6] += 4U;
            const auto low = t.word(source + 2U);
            const auto value = (std::uint32_t(high) << 16U) | low;
            m.logic(value, 32U);
            r.data[1] = value;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x802ccU: { // 3f07: move-word-register-to-predecrement
            next = 0x802ceU;
            const auto value = static_cast<std::uint16_t>(r.data[7]);
            const auto destination = r.address[7] - 2U;
            m.logic(value, 16U);
            t.prefetch(pc + 4U);
            r.address[7] = destination;
            t.word(destination, value);
            break;
        }
        case 0x802ceU: { // 4e4a: supervisor-trap
            next = 0x802d0U;
            const auto child = t.supervisor_trap(pc, 42U, next);
            if (child.status != TranslationStatus::complete || child.control != 2U) return child;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x802d0U: { // 3e1f: move-to-data-register
            next = 0x802d2U;
            const auto source = r.address[7];
            r.address[7] += 2U;
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(7U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x802d2U: { // 51cfffee: dbf
            next = 0x802d6U;
            next = t.dbf(pc, 0x802c2U, 7U);
            if ((r.data[7] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x802d6U: { // 42788002: clear-memory
            next = 0x802daU;
            const auto address = 0xffff8002U;
            t.prefetch(pc + 4U);
            (void)t.word(address);
            m.logic(0U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, 0U);
            break;
        }
        case 0x802daU: { // 42388001: clear-memory
            next = 0x802deU;
            const auto address = 0xffff8001U;
            t.prefetch(pc + 4U);
            (void)t.byte(address);
            m.logic(0U, 8U);
            t.prefetch(pc + 6U);
            t.byte(address, 0U);
            break;
        }
        case 0x802deU: { // 7000: moveq
            next = 0x802e0U;
            r.data[0] = 0x0U;
            m.logic(0x0U, 32U);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x802e0U: { // 4e48: supervisor-trap
            next = 0x802e2U;
            const auto child = t.supervisor_trap(pc, 40U, next);
            if (child.status != TranslationStatus::complete || child.control != 2U) return child;
            next = r.program_counter; returned_from_child = true;
            break;
        }
        case 0x802e2U: { // 46fc2000: move-immediate-supervisor-status
            next = 0x802e6U;
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
        case 0x802acU:
        case 0x802b0U:
        case 0x802b4U:
        case 0x802b6U:
        case 0x802b8U:
        case 0x802beU:
        case 0x802c2U:
        case 0x802c6U:
        case 0x802c8U:
        case 0x802caU:
        case 0x802ccU:
        case 0x802ceU:
        case 0x802d0U:
        case 0x802d2U:
        case 0x802d6U:
        case 0x802daU:
        case 0x802deU:
        case 0x802e0U:
        case 0x802e2U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
