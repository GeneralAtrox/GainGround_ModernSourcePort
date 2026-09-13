// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00022460(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22460U: { // 61001466 bsr.w $238c8
            next = 0x22464U;
            const auto result = m.call(c, 416U, 0x22460U, 0x238c8U, 0x22464U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22464U: { // 2b7c000224cc0002 move.l #$224cc, $2(a5)
            next = 0x2246cU;
            m.lng(r.address[5] + 0x2U, 0x224ccU);
            m.logic(0x224ccU, 32U);
            break;
        }
        case 0x2246cU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22470U;
            r.address[6] = r.address[5] + 0x22U;
            break;
        }
        case 0x22470U: { // 41f900023c9e lea.l $23c9e.l, a0
            next = 0x22476U;
            r.address[0] = 0x23c9eU;
            break;
        }
        case 0x22476U: { // 7007 moveq #$7, d0
            next = 0x22478U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22478U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2247aU;
            const auto value = m.lng(r.address[0]);
            r.address[0] += 4U;
            m.lng(r.address[6], value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2247aU: { // 51c8fffc dbra d0, $22478
            next = 0x2247eU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22478U;
            break;
        }
        case 0x2247eU: { // 610013ac bsr.w $2382c
            next = 0x22482U;
            const auto result = m.call(c, 413U, 0x2247eU, 0x2382cU, 0x22482U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22482U: { // 4e75 rts 
            next = 0x22484U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22482U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22460U:
        case 0x22464U:
        case 0x2246cU:
        case 0x22470U:
        case 0x22476U:
        case 0x22478U:
        case 0x2247aU:
        case 0x2247eU:
        case 0x22482U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
