// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0002235e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2235eU: { // 61001568 bsr.w $238c8
            next = 0x22362U;
            const auto result = m.call(c, 416U, 0x2235eU, 0x238c8U, 0x22362U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22362U: { // 2b7c000223820002 move.l #$22382, $2(a5)
            next = 0x2236aU;
            m.lng(r.address[5] + 0x2U, 0x22382U);
            m.logic(0x22382U, 32U);
            break;
        }
        case 0x2236aU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2236eU;
            r.address[6] = r.address[5] + 0x22U;
            break;
        }
        case 0x2236eU: { // 41f900023be2 lea.l $23be2.l, a0
            next = 0x22374U;
            r.address[0] = 0x23be2U;
            break;
        }
        case 0x22374U: { // 7007 moveq #$7, d0
            next = 0x22376U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22376U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22378U;
            const auto value = m.lng(r.address[0]);
            r.address[0] += 4U;
            m.lng(r.address[6], value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22378U: { // 51c8fffc dbra d0, $22376
            next = 0x2237cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22376U;
            break;
        }
        case 0x2237cU: { // 610014ae bsr.w $2382c
            next = 0x22380U;
            const auto result = m.call(c, 413U, 0x2237cU, 0x2382cU, 0x22380U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22380U: { // 4e75 rts 
            next = 0x22382U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22380U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2235eU:
        case 0x22362U:
        case 0x2236aU:
        case 0x2236eU:
        case 0x22374U:
        case 0x22376U:
        case 0x22378U:
        case 0x2237cU:
        case 0x22380U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
