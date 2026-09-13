// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00021412(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21412U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21416U;
            m.logic(m.byte(r.address[5] + 0x3fU), 8U);
            break;
        }
        case 0x21416U: { // 6716 beq.b $2142e
            next = 0x21418U;
            if ((r.status & 4U) != 0U) { next = 0x2142eU; transfer_kind = 1U; }
            break;
        }
        case 0x21418U: { // 4a6d001a tst.w $1a(a5)
            next = 0x2141cU;
            m.logic(m.word(r.address[5] + 0x1aU), 16U);
            break;
        }
        case 0x2141cU: { // 6a08 bpl.b $21426
            next = 0x2141eU;
            if ((r.status & 8U) == 0U) { next = 0x21426U; transfer_kind = 1U; }
            break;
        }
        case 0x2141eU: { // 1b7c0000003f move.b #$0, $3f(a5)
            next = 0x21424U;
            m.byte(r.address[5] + 0x3fU, 0x0U);
            m.logic(0x0U, 8U);
            break;
        }
        case 0x21424U: { // 6008 bra.b $2142e
            next = 0x21426U;
            if (true) { next = 0x2142eU; transfer_kind = 1U; }
            break;
        }
        case 0x21426U: { // 4df83400 lea.l $3400.w, a6
            next = 0x2142aU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x2142aU: { // 522e0026 addq.b #$1, $26(a6)
            next = 0x2142eU;
            const auto address = r.address[6] + 0x26U;
            const auto value = m.add(m.byte(address), 0x1U, 8U);
            m.byte(address, value);
            break;
        }
        case 0x2142eU: { // 1b7c0009000b move.b #$9, $b(a5)
            next = 0x21434U;
            m.byte(r.address[5] + 0xbU, 0x9U);
            m.logic(0x9U, 8U);
            break;
        }
        case 0x21434U: { // 6100aae6 bsr.w $1bf1c
            next = 0x21438U;
            const auto result = m.call(c, 328U, 0x21434U, 0x1bf1cU, 0x21438U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21438U: { // 6100c78a bsr.w $1dbc4
            next = 0x2143cU;
            const auto result = m.call(c, 352U, 0x21438U, 0x1dbc4U, 0x2143cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2143cU: { // 6100c7b2 bsr.w $1dbf0
            next = 0x21440U;
            const auto result = m.call(c, 353U, 0x2143cU, 0x1dbf0U, 0x21440U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21440U: { // 6100c9ee bsr.w $1de30
            next = 0x21444U;
            const auto result = m.call(c, 355U, 0x21440U, 0x1de30U, 0x21444U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21444U: { // 6100cca8 bsr.w $1e0ee
            next = 0x21448U;
            const auto result = m.call(c, 356U, 0x21444U, 0x1e0eeU, 0x21448U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21448U: { // 6100ccda bsr.w $1e124
            next = 0x2144cU;
            const auto result = m.call(c, 357U, 0x21448U, 0x1e124U, 0x2144cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2144cU: { // 6100cd32 bsr.w $1e180
            next = 0x21450U;
            const auto result = m.call(c, 359U, 0x2144cU, 0x1e180U, 0x21450U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21450U: { // 4e75 rts 
            next = 0x21452U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21450U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21412U:
        case 0x21416U:
        case 0x21418U:
        case 0x2141cU:
        case 0x2141eU:
        case 0x21424U:
        case 0x21426U:
        case 0x2142aU:
        case 0x2142eU:
        case 0x21434U:
        case 0x21438U:
        case 0x2143cU:
        case 0x21440U:
        case 0x21444U:
        case 0x21448U:
        case 0x2144cU:
        case 0x21450U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
