// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00022534(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22534U: { // 61001392 bsr.w $238c8
            next = 0x22538U;
            const auto result = m.call(c, 416U, 0x22534U, 0x238c8U, 0x22538U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22538U: { // 2b7c000225580002 move.l #$22558, $2(a5)
            next = 0x22540U;
            m.lng(r.address[5] + 0x2U, 0x22558U);
            m.logic(0x22558U, 32U);
            break;
        }
        case 0x22540U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22544U;
            r.address[6] = r.address[5] + 0x22U;
            break;
        }
        case 0x22544U: { // 41f900023d14 lea.l $23d14.l, a0
            next = 0x2254aU;
            r.address[0] = 0x23d14U;
            break;
        }
        case 0x2254aU: { // 7007 moveq #$7, d0
            next = 0x2254cU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2254cU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2254eU;
            const auto value = m.lng(r.address[0]);
            r.address[0] += 4U;
            m.lng(r.address[6], value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2254eU: { // 51c8fffc dbra d0, $2254c
            next = 0x22552U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2254cU;
            break;
        }
        case 0x22552U: { // 610012d8 bsr.w $2382c
            next = 0x22556U;
            const auto result = m.call(c, 413U, 0x22552U, 0x2382cU, 0x22556U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22556U: { // 4e75 rts 
            next = 0x22558U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22556U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22534U:
        case 0x22538U:
        case 0x22540U:
        case 0x22544U:
        case 0x2254aU:
        case 0x2254cU:
        case 0x2254eU:
        case 0x22552U:
        case 0x22556U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
