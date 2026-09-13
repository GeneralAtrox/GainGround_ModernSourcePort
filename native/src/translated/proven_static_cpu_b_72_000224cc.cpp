// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_000224cc(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x224ccU: { // 61001314 bsr.w $237e2
            next = 0x224d0U;
            const auto result = m.call(c, 412U, 0x224ccU, 0x237e2U, 0x224d0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x224d0U: { // 302d001c move.w $1c(a5), d0
            next = 0x224d4U;
            m.dw(0U, m.word(r.address[5] + 0x1cU));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x224d4U: { // e540 asl.w #$2, d0
            next = 0x224d6U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x224d6U: { // 4efb0002 jmp $224da(pc, d0.w)
            next = 0x224daU;
            next = (0x224daU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x224daU: { // 60000004 bra.w $224e0
            next = 0x224deU;
            if (true) { next = 0x224e0U; transfer_kind = 1U; }
            break;
        }
        case 0x224e0U: { // 45f900023cce lea.l $23cce.l, a2
            next = 0x224e6U;
            r.address[2] = 0x23cceU;
            break;
        }
        case 0x224e6U: { // 61001442 bsr.w $2392a
            next = 0x224eaU;
            const auto result = m.call(c, 419U, 0x224e6U, 0x2392aU, 0x224eaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x224eaU: { // 4e75 rts 
            next = 0x224ecU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x224eaU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x224ccU:
        case 0x224d0U:
        case 0x224d4U:
        case 0x224d6U:
        case 0x224daU:
        case 0x224e0U:
        case 0x224e6U:
        case 0x224eaU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
