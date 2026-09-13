// Generated scene candidate. Implemented but unverified.
// Replaces whole body only after integration; do not add to old timing.
#include "cpu_b_scene_timing.h"

namespace gain_ground::translated {
FunctionResult cpu_a_install_shared_vector_stubs(FunctionContext &c) noexcept {
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
        case 0x20ccU: { // 41fa0014: lea
            next = 0x20d0U;
            r.address[0] = 0x20e2U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x20d0U: { // 227c00080000: move-long-immediate-to-register
            next = 0x20d6U;
            t.prefetch(pc + 4U);
            t.prefetch(pc + 6U);
            r.address[1] = 0x80000U;
            t.prefetch(pc + 8U);
            break;
        }
        case 0x20d6U: { // 303c005c: move-to-data-register
            next = 0x20daU;
            t.prefetch(pc + 4U);
            m.logic(0x5cU, 16U);
            m.dw(0U, 0x5cU);
            t.prefetch(pc + 6U);
            break;
        }
        case 0x20daU: { // 32d8: move-to-indirect
            next = 0x20dcU;
            const auto source = r.address[0];
            r.address[0] += 2U;
            const auto value = t.word(source);
            const auto destination = r.address[1];
            m.logic(value, 16U);
            t.word(destination, value);
            r.address[1] += 2U;
            t.prefetch(pc + 4U);
            break;
        }
        case 0x20dcU: { // 51c8fffc: dbf
            next = 0x20e0U;
            next = t.dbf(pc, 0x20daU, 0U);
            if ((r.data[0] & 0xffffU) != 0xffffU) transfer_kind = 1U;
            break;
        }
        case 0x20e0U: { // 4e75: rts
            next = 0x20e2U;
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
        case 0x20ccU:
        case 0x20d0U:
        case 0x20d6U:
        case 0x20daU:
        case 0x20dcU:
        case 0x20e0U:
            t.begin(next); break;
        default:
            return m.dispatch(c, pc, next, transfer_kind, c.state);
        }
    }
}
} // namespace gain_ground::translated
