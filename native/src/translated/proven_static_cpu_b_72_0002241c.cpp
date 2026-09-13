// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0002241c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2241cU: { // 610014aa bsr.w $238c8
            next = 0x22420U;
            const auto result = m.call(c, 416U, 0x2241cU, 0x238c8U, 0x22420U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22420U: { // 2b7c000224400002 move.l #$22440, $2(a5)
            next = 0x22428U;
            m.lng(r.address[5] + 0x2U, 0x22440U);
            m.logic(0x22440U, 32U);
            break;
        }
        case 0x22428U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2242cU;
            r.address[6] = r.address[5] + 0x22U;
            break;
        }
        case 0x2242cU: { // 41f900023c58 lea.l $23c58.l, a0
            next = 0x22432U;
            r.address[0] = 0x23c58U;
            break;
        }
        case 0x22432U: { // 7007 moveq #$7, d0
            next = 0x22434U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22434U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22436U;
            const auto value = m.lng(r.address[0]);
            r.address[0] += 4U;
            m.lng(r.address[6], value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22436U: { // 51c8fffc dbra d0, $22434
            next = 0x2243aU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22434U;
            break;
        }
        case 0x2243aU: { // 610013f0 bsr.w $2382c
            next = 0x2243eU;
            const auto result = m.call(c, 413U, 0x2243aU, 0x2382cU, 0x2243eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2243eU: { // 4e75 rts 
            next = 0x22440U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2243eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2241cU:
        case 0x22420U:
        case 0x22428U:
        case 0x2242cU:
        case 0x22432U:
        case 0x22434U:
        case 0x22436U:
        case 0x2243aU:
        case 0x2243eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
