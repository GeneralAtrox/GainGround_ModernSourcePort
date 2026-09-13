// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_000223f8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x223f8U: { // 610014ce bsr.w $238c8
            next = 0x223fcU;
            const auto result = m.call(c, 416U, 0x223f8U, 0x238c8U, 0x223fcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x223fcU: { // 2b7c000224400002 move.l #$22440, $2(a5)
            next = 0x22404U;
            m.lng(r.address[5] + 0x2U, 0x22440U);
            m.logic(0x22440U, 32U);
            break;
        }
        case 0x22404U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22408U;
            r.address[6] = r.address[5] + 0x22U;
            break;
        }
        case 0x22408U: { // 41f900023c48 lea.l $23c48.l, a0
            next = 0x2240eU;
            r.address[0] = 0x23c48U;
            break;
        }
        case 0x2240eU: { // 7007 moveq #$7, d0
            next = 0x22410U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22410U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22412U;
            const auto value = m.lng(r.address[0]);
            r.address[0] += 4U;
            m.lng(r.address[6], value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22412U: { // 51c8fffc dbra d0, $22410
            next = 0x22416U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22410U;
            break;
        }
        case 0x22416U: { // 61001414 bsr.w $2382c
            next = 0x2241aU;
            const auto result = m.call(c, 413U, 0x22416U, 0x2382cU, 0x2241aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2241aU: { // 4e75 rts 
            next = 0x2241cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2241aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x223f8U:
        case 0x223fcU:
        case 0x22404U:
        case 0x22408U:
        case 0x2240eU:
        case 0x22410U:
        case 0x22412U:
        case 0x22416U:
        case 0x2241aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
