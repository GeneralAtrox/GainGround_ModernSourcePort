// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0002095c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2095cU: { // 6100b462 bsr.w $1bdc0
            next = 0x20960U;
            const auto result = m.call(c, 327U, 0x2095cU, 0x1bdc0U, 0x20960U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20960U: { // 6100d28e bsr.w $1dbf0
            next = 0x20964U;
            const auto result = m.call(c, 353U, 0x20960U, 0x1dbf0U, 0x20964U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20964U: { // 6100e896 bsr.w $1f1fc
            next = 0x20968U;
            const auto result = m.call(c, 371U, 0x20964U, 0x1f1fcU, 0x20968U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20968U: { // 6100d914 bsr.w $1e27e
            next = 0x2096cU;
            const auto result = m.call(c, 559U, 0x20968U, 0x1e27eU, 0x2096cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2096cU: { // 4e75 rts 
            next = 0x2096eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2096cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2095cU:
        case 0x20960U:
        case 0x20964U:
        case 0x20968U:
        case 0x2096cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
