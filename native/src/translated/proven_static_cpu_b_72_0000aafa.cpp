// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000aafa(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xaafaU: { // 41fa0a6a lea.l $b566(pc), a0
            next = 0xaafeU;
            const auto source_address = 0xb566U;
            r.address[0] = source_address;
            break;
        }
        case 0xaafeU: { // 30380c00 move.w $c00.w, d0
            next = 0xab02U;
            const auto source_address = 0xc00U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xab02U: { // e540 asl.w #$2, d0
            next = 0xab04U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xab04U: { // 24700000 movea.l (a0, d0.w), a2
            next = 0xab08U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.lng(source_address);
            r.address[2] = value;
            break;
        }
        case 0xab08U: { // 381a move.w (a2)+, d4
            next = 0xab0aU;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            r.address[2] += 2U;
            m.dw(4U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xab0aU: { // 361a move.w (a2)+, d3
            next = 0xab0cU;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            r.address[2] += 2U;
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xab0cU: { // 3004 move.w d4, d0
            next = 0xab0eU;
            const auto value = r.data[4];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xab0eU: { // 61000022 bsr.w $ab32
            next = 0xab12U;
            const auto result = m.call(c, 626U, 0xab0eU, 0xab32U, 0xab12U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xab12U: { // 51cbfff8 dbra d3, $ab0c
            next = 0xab16U;
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0xab0cU;
            break;
        }
        case 0xab16U: { // 381a move.w (a2)+, d4
            next = 0xab18U;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            r.address[2] += 2U;
            m.dw(4U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xab18U: { // 361a move.w (a2)+, d3
            next = 0xab1aU;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            r.address[2] += 2U;
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xab1aU: { // 3004 move.w d4, d0
            next = 0xab1cU;
            const auto value = r.data[4];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xab1cU: { // 61000014 bsr.w $ab32
            next = 0xab20U;
            const auto result = m.call(c, 626U, 0xab1cU, 0xab32U, 0xab20U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xab20U: { // 51cbfff8 dbra d3, $ab1a
            next = 0xab24U;
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0xab1aU;
            break;
        }
        case 0xab24U: { // 361a move.w (a2)+, d3
            next = 0xab26U;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            r.address[2] += 2U;
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xab26U: { // 6b08 bmi.b $ab30
            next = 0xab28U;
            if ((r.status & 8U) != 0U) { next = 0xab30U; transfer_kind = 1U; }
            break;
        }
        case 0xab28U: { // 61000028 bsr.w $ab52
            next = 0xab2cU;
            const auto result = m.call(c, 627U, 0xab28U, 0xab52U, 0xab2cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xab2cU: { // 51cbfffa dbra d3, $ab28
            next = 0xab30U;
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0xab28U;
            break;
        }
        case 0xab30U: { // 4e75 rts 
            next = 0xab32U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xab30U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xaafaU:
        case 0xaafeU:
        case 0xab02U:
        case 0xab04U:
        case 0xab08U:
        case 0xab0aU:
        case 0xab0cU:
        case 0xab0eU:
        case 0xab12U:
        case 0xab16U:
        case 0xab18U:
        case 0xab1aU:
        case 0xab1cU:
        case 0xab20U:
        case 0xab24U:
        case 0xab26U:
        case 0xab28U:
        case 0xab2cU:
        case 0xab30U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
