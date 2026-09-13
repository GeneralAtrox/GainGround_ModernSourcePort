// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000a87c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xa87cU: { // 4eb900017054 jsr $17054.l
            next = 0xa882U;
            const auto result = m.call(c, 311U, 0xa87cU, 0x17054U, 0xa882U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa882U: { // 33fc00010040401a move.w #$1, $40401a.l
            next = 0xa88aU;
            const auto value = 0x1U;
            const auto destination_address = 0x40401aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xa88aU: { // 4eb9000085ac jsr $85ac.l
            next = 0xa890U;
            const auto result = m.call(c, 117U, 0xa88aU, 0x85acU, 0xa890U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa890U: { // 6100009a bsr.w $a92c
            next = 0xa894U;
            const auto result = m.call(c, 617U, 0xa890U, 0xa92cU, 0xa894U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa894U: { // 4eb900008622 jsr $8622.l
            next = 0xa89aU;
            const auto result = m.call(c, 122U, 0xa894U, 0x8622U, 0xa89aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa89aU: { // 4eb900008654 jsr $8654.l
            next = 0xa8a0U;
            const auto result = m.call(c, 123U, 0xa89aU, 0x8654U, 0xa8a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8a0U: { // 610000b8 bsr.w $a95a
            next = 0xa8a4U;
            const auto result = m.call(c, 618U, 0xa8a0U, 0xa95aU, 0xa8a4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8a4U: { // 610000d6 bsr.w $a97c
            next = 0xa8a8U;
            const auto result = m.call(c, 619U, 0xa8a4U, 0xa97cU, 0xa8a8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8a8U: { // 7000 moveq #$0, d0
            next = 0xa8aaU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xa8aaU: { // 80380419 or.b $419.w, d0
            next = 0xa8aeU;
            const auto source_address = 0x419U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xa8aeU: { // 23c000404018 move.l d0, $404018.l
            next = 0xa8b4U;
            const auto value = r.data[0];
            const auto destination_address = 0x404018U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xa8b4U: { // 4e75 rts 
            next = 0xa8b6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xa8b4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xa87cU:
        case 0xa882U:
        case 0xa88aU:
        case 0xa890U:
        case 0xa894U:
        case 0xa89aU:
        case 0xa8a0U:
        case 0xa8a4U:
        case 0xa8a8U:
        case 0xa8aaU:
        case 0xa8aeU:
        case 0xa8b4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
