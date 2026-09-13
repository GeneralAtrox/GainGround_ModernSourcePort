// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00022558(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22558U: { // 61001288 bsr.w $237e2
            next = 0x2255cU;
            const auto result = m.call(c, 412U, 0x22558U, 0x237e2U, 0x2255cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2255cU: { // 302d001c move.w $1c(a5), d0
            next = 0x22560U;
            m.dw(0U, m.word(r.address[5] + 0x1cU));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x22560U: { // e540 asl.w #$2, d0
            next = 0x22562U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x22562U: { // 4efb0002 jmp $22566(pc, d0.w)
            next = 0x22566U;
            next = (0x22566U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x22566U: { // 60000008 bra.w $22570
            next = 0x2256aU;
            if (true) { next = 0x22570U; transfer_kind = 1U; }
            break;
        }
        case 0x2256aU: { // 6000001e bra.w $2258a
            next = 0x2256eU;
            if (true) { next = 0x2258aU; transfer_kind = 1U; }
            break;
        }
        case 0x22570U: { // 202d000a move.l $a(a5), d0
            next = 0x22574U;
            r.data[0] = m.lng(r.address[5] + 0xaU);
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22574U: { // 028001010100 andi.l #$1010100, d0
            next = 0x2257aU;
            r.data[0] &= 0x1010100U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2257aU: { // 6602 bne.b $2257e
            next = 0x2257cU;
            if ((r.status & 4U) == 0U) { next = 0x2257eU; transfer_kind = 1U; }
            break;
        }
        case 0x2257cU: { // 4e75 rts 
            next = 0x2257eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2257cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2257eU: { // 1b7c00020022 move.b #$2, $22(a5)
            next = 0x22584U;
            m.byte(r.address[5] + 0x22U, 0x2U);
            m.logic(0x2U, 8U);
            break;
        }
        case 0x22584U: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x22588U;
            const auto address = r.address[5] + 0x1cU;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0x22588U: { // 4e75 rts 
            next = 0x2258aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22588U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2258aU: { // 45f900023d24 lea.l $23d24.l, a2
            next = 0x22590U;
            r.address[2] = 0x23d24U;
            break;
        }
        case 0x22590U: { // 61001398 bsr.w $2392a
            next = 0x22594U;
            const auto result = m.call(c, 419U, 0x22590U, 0x2392aU, 0x22594U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22594U: { // 4e75 rts 
            next = 0x22596U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22594U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22558U:
        case 0x2255cU:
        case 0x22560U:
        case 0x22562U:
        case 0x22566U:
        case 0x2256aU:
        case 0x22570U:
        case 0x22574U:
        case 0x2257aU:
        case 0x2257cU:
        case 0x2257eU:
        case 0x22584U:
        case 0x22588U:
        case 0x2258aU:
        case 0x22590U:
        case 0x22594U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
