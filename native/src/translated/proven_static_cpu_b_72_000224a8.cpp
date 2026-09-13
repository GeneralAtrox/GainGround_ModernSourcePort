// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_000224a8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x224a8U: { // 6100141e bsr.w $238c8
            next = 0x224acU;
            const auto result = m.call(c, 416U, 0x224a8U, 0x238c8U, 0x224acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x224acU: { // 2b7c000224cc0002 move.l #$224cc, $2(a5)
            next = 0x224b4U;
            m.lng(r.address[5] + 0x2U, 0x224ccU);
            m.logic(0x224ccU, 32U);
            break;
        }
        case 0x224b4U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x224b8U;
            r.address[6] = r.address[5] + 0x22U;
            break;
        }
        case 0x224b8U: { // 41f900023cbe lea.l $23cbe.l, a0
            next = 0x224beU;
            r.address[0] = 0x23cbeU;
            break;
        }
        case 0x224beU: { // 7007 moveq #$7, d0
            next = 0x224c0U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x224c0U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x224c2U;
            const auto value = m.lng(r.address[0]);
            r.address[0] += 4U;
            m.lng(r.address[6], value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x224c2U: { // 51c8fffc dbra d0, $224c0
            next = 0x224c6U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x224c0U;
            break;
        }
        case 0x224c6U: { // 61001364 bsr.w $2382c
            next = 0x224caU;
            const auto result = m.call(c, 413U, 0x224c6U, 0x2382cU, 0x224caU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x224caU: { // 4e75 rts 
            next = 0x224ccU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x224caU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x224a8U:
        case 0x224acU:
        case 0x224b4U:
        case 0x224b8U:
        case 0x224beU:
        case 0x224c0U:
        case 0x224c2U:
        case 0x224c6U:
        case 0x224caU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
