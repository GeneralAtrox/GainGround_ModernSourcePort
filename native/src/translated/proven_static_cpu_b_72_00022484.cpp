// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00022484(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22484U: { // 61001442 bsr.w $238c8
            next = 0x22488U;
            const auto result = m.call(c, 416U, 0x22484U, 0x238c8U, 0x22488U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22488U: { // 2b7c000224cc0002 move.l #$224cc, $2(a5)
            next = 0x22490U;
            m.lng(r.address[5] + 0x2U, 0x224ccU);
            m.logic(0x224ccU, 32U);
            break;
        }
        case 0x22490U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22494U;
            r.address[6] = r.address[5] + 0x22U;
            break;
        }
        case 0x22494U: { // 41f900023cae lea.l $23cae.l, a0
            next = 0x2249aU;
            r.address[0] = 0x23caeU;
            break;
        }
        case 0x2249aU: { // 7007 moveq #$7, d0
            next = 0x2249cU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2249cU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2249eU;
            const auto value = m.lng(r.address[0]);
            r.address[0] += 4U;
            m.lng(r.address[6], value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2249eU: { // 51c8fffc dbra d0, $2249c
            next = 0x224a2U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2249cU;
            break;
        }
        case 0x224a2U: { // 61001388 bsr.w $2382c
            next = 0x224a6U;
            const auto result = m.call(c, 413U, 0x224a2U, 0x2382cU, 0x224a6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x224a6U: { // 4e75 rts 
            next = 0x224a8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x224a6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22484U:
        case 0x22488U:
        case 0x22490U:
        case 0x22494U:
        case 0x2249aU:
        case 0x2249cU:
        case 0x2249eU:
        case 0x224a2U:
        case 0x224a6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
