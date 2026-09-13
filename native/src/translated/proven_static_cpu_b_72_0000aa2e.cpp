// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000aa2e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xaa2eU: { // 4eb90000b6e0 jsr $b6e0.l
            next = 0xaa34U;
            const auto result = m.call(c, 630U, 0xaa2eU, 0xb6e0U, 0xaa34U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xaa34U: { // 303c0010 move.w #$10, d0
            next = 0xaa38U;
            m.dw(0U, 0x10U);
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xaa38U: { // 4eb90001703a jsr $1703a.l
            next = 0xaa3eU;
            const auto result = m.call(c, 310U, 0xaa38U, 0x1703aU, 0xaa3eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xaa3eU: { // 4e75 rts 
            next = 0xaa40U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xaa3eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xaa2eU:
        case 0xaa34U:
        case 0xaa38U:
        case 0xaa3eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
