// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_a_plain_00081250(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x81250U: { // 6100ff46 bsr.w $81198
            next = 0x81254U;
            const auto result = m.call(c, 457U, 0x81250U, 0x81198U, 0x81254U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x81254U: { // 4a10 tst.b (a0)
            next = 0x81256U;
            const auto destination_address = r.address[0];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x81256U: { // 6af8 bpl.b $81250
            next = 0x81258U;
            if ((r.status & 8U) == 0U) { next = 0x81250U; transfer_kind = 1U; }
            break;
        }
        case 0x81258U: { // 4e75 rts 
            next = 0x8125aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x81258U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x81250U:
        case 0x81254U:
        case 0x81256U:
        case 0x81258U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
