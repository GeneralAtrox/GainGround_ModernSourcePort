// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_a_plain_00081198(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x81198U: { // 7400 moveq #$0, d2
            next = 0x8119aU;
            r.data[2] = 0x0U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0x8119aU: { // 1418 move.b (a0)+, d2
            next = 0x8119cU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x8119cU: { // 7200 moveq #$0, d1
            next = 0x8119eU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x8119eU: { // 1218 move.b (a0)+, d1
            next = 0x811a0U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x811a0U) return c.host->call_function(495U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0x81198U:
        case 0x8119aU:
        case 0x8119cU:
        case 0x8119eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
