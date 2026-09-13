// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_b_sound_compare_command_limit(FunctionContext &c) noexcept {
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
        case 0x17decU: { // 0c4000ff: immediate-cmp
            next = 0x17df0U;
            t.prefetch(pc + 4U);
            const auto value = m.sub(r.data[0], 0xffU, 16U, true);
            (void)value; // CMPI updates flags without storing a result.
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17df0U: { // 6400feae: bcc
            next = 0x17df4U;
            const bool taken = scene_timing::condition(r.status, 4U);
            next = t.branch_word(pc, 0x17ca0U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x17df4U: { // 024000fe: immediate-and
            next = 0x17df8U;
            t.prefetch(pc + 4U);
            const auto value = r.data[0] & 0xfeU;
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17df8U: { // 41fa00fe: lea
            next = 0x17dfcU;
            r.address[0] = 0x17ef8U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17dfcU: { // 322e0088: move-to-data-register
            next = 0x17e00U;
            t.prefetch(pc + 4U);
            const auto source = (r.address[6] + 0x88U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(1U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17e00U: { // b240: register-cmp
            next = 0x17e02U;
            const auto value = m.sub(r.data[1], r.data[0], 16U, true);
            (void)value; // CMP updates flags without storing a result.
            t.prefetch(pc + 4U);
            break;
        }
        case 0x17e02U: { // 6612: bcc
            next = 0x17e04U;
            const bool taken = scene_timing::condition(r.status, 6U);
            next = t.branch(pc, 0x17e16U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x17e04U: { // 083000070000: immediate-bit-memory
            next = 0x17e0aU;
            t.prefetch(pc + 4U);
            t.clocks(2U);
            t.prefetch(pc + 6U);
            const auto address = (r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x0U);
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x80U) ? 0U : 4U));
            t.prefetch(pc + 8U);
            break;
        }
        case 0x17e0aU: { // 6700fe94: bcc
            next = 0x17e0eU;
            const bool taken = scene_timing::condition(r.status, 7U);
            next = t.branch_word(pc, 0x17ca0U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x17e0eU: { // 4a6e008a: tst-memory
            next = 0x17e12U;
            t.prefetch(pc + 4U);
            const auto address = (r.address[6] + 0x8aU);
            const auto value = t.word(address);
            m.logic(value, 16U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17e12U: { // 6600fe8c: bcc
            next = 0x17e16U;
            const bool taken = scene_timing::condition(r.status, 6U);
            next = t.branch_word(pc, 0x17ca0U, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x17e16U: { // 32300000: move-memory-to-register
            next = 0x17e1aU;
            t.clocks(2U);
            const auto source = (r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])) + 0x0U);
            t.prefetch(pc + 4U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(1U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17e1aU: { // 024100ff: immediate-and
            next = 0x17e1eU;
            t.prefetch(pc + 4U);
            const auto value = r.data[1] & 0xffU;
            m.logic(value, 16U);
            m.dw(1U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17e1eU: { // 3d41008a: move-to-displacement
            next = 0x17e22U;
            const auto destination = (r.address[6] + 0x8aU);
            t.prefetch(pc + 4U);
            m.logic(r.data[1], 16U);
            t.word(destination, r.data[1]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17e22U: { // 41ee0060: lea
            next = 0x17e26U;
            r.address[0] = (r.address[6] + 0x60U);
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17e26U: { // 31400028: move-to-displacement
            next = 0x17e2aU;
            const auto destination = (r.address[0] + 0x28U);
            t.prefetch(pc + 4U);
            m.logic(r.data[0], 16U);
            t.word(destination, r.data[0]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17e2aU: { // 6000006e: bcc
            next = 0x17e2eU;
            const bool taken = scene_timing::condition(r.status, 0U);
            next = t.branch_word(pc, 0x17e9aU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next; t.stop();
        if (!returned_from_child)
            if (const auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x17decU:
        case 0x17df0U:
        case 0x17df4U:
        case 0x17df8U:
        case 0x17dfcU:
        case 0x17e00U:
        case 0x17e02U:
        case 0x17e04U:
        case 0x17e0aU:
        case 0x17e0eU:
        case 0x17e12U:
        case 0x17e16U:
        case 0x17e1aU:
        case 0x17e1eU:
        case 0x17e22U:
        case 0x17e26U:
        case 0x17e2aU:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
