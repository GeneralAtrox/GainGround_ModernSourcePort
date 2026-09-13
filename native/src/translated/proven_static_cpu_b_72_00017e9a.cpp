// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00017e9a(FunctionContext &c) noexcept {
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
        case 0x17e9aU: { // 52680020: quick-memory
            next = 0x17e9eU;
            const auto address = (r.address[0] + 0x20U);
            t.prefetch(pc + 4U);
            const auto old = t.word(address);
            const auto value = m.add(old, 1U, 16U);
            t.prefetch(pc + 6U);
            t.word(address, value);
            break;
        }
        case 0x17e9eU: { // 082800040021: immediate-bit-memory
            next = 0x17ea4U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            const auto address = (r.address[0] + 0x21U);
            const auto old = t.byte(address);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & 0x10U) ? 0U : 4U));
            t.prefetch(pc + 8U);
            break;
        }
        case 0x17ea4U: { // 6616: bcc
            next = 0x17ea6U;
            const bool taken = scene_timing::condition(r.status, 6U);
            next = t.branch(pc, 0x17ebcU, taken);
            if (taken) transfer_kind = 1U;
            break;
        }
        case 0x17ea6U: { // 32280022: move-to-data-register
            next = 0x17eaaU;
            t.prefetch(pc + 4U);
            const auto source = (r.address[0] + 0x22U);
            const auto value = t.word(source);
            m.logic(value, 16U);
            m.dw(1U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17eaaU: { // 0241001e: immediate-and
            next = 0x17eaeU;
            t.prefetch(pc + 4U);
            const auto value = r.data[1] & 0x1eU;
            m.logic(value, 16U);
            m.dw(1U, value);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17eaeU: { // 31801000: move-word-register-to-indexed
            next = 0x17eb2U;
            t.clocks(2U);
            const auto destination = (r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[1])) + 0x0U);
            t.prefetch(pc + 4U);
            m.logic(r.data[0], 16U);
            t.word(destination, r.data[0]);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x17eb2U: { // 066800020022: add-immediate-word-memory
            next = 0x17eb8U;
            t.prefetch(pc + 4U);
            const auto address = (r.address[0] + 0x22U);
            t.prefetch(pc + 6U);
            const auto old = t.word(address);
            const auto value = m.add(old, 0x2U, 16U);
            t.prefetch(pc + 8U);
            t.word(address, value);
            break;
        }
        case 0x17eb8U: { // 8040: register-or
            next = 0x17ebaU;
            const auto value = r.data[0] | r.data[0];
            m.logic(value, 16U);
            m.dw(0U, value);
            t.prefetch(pc + 4U);
            break;
        }
        case 0x17ebaU: { // 4e75: rts
            next = 0x17ebcU;
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
        case 0x17e9aU:
        case 0x17e9eU:
        case 0x17ea4U:
        case 0x17ea6U:
        case 0x17eaaU:
        case 0x17eaeU:
        case 0x17eb2U:
        case 0x17eb8U:
        case 0x17ebaU:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
