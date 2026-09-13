// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000ee4a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xee4aU: { // 4a380c18 tst.b $c18.w
            next = 0xee4eU;
            const auto destination_address = 0xc18U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0xee4eU: { // 6702 beq.b $ee52
            next = 0xee50U;
            if ((r.status & 4U) != 0U) { next = 0xee52U; transfer_kind = 1U; }
            break;
        }
        case 0xee50U: { // 4e75 rts 
            next = 0xee52U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xee50U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0xee52U) return c.host->call_function(581U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xee4aU:
        case 0xee4eU:
        case 0xee50U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
