// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000a8b6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xa8b6U: { // 4eb900017054 jsr $17054.l
            next = 0xa8bcU;
            const auto result = m.call(c, 311U, 0xa8b6U, 0x17054U, 0xa8bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8bcU: { // 33fc00010040401a move.w #$1, $40401a.l
            next = 0xa8c4U;
            const auto value = 0x1U;
            const auto destination_address = 0x40401aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xa8c4U: { // 4eb9000085ac jsr $85ac.l
            next = 0xa8caU;
            const auto result = m.call(c, 117U, 0xa8c4U, 0x85acU, 0xa8caU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8caU: { // 61000060 bsr.w $a92c
            next = 0xa8ceU;
            const auto result = m.call(c, 617U, 0xa8caU, 0xa92cU, 0xa8ceU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8ceU: { // 4eb900008622 jsr $8622.l
            next = 0xa8d4U;
            const auto result = m.call(c, 122U, 0xa8ceU, 0x8622U, 0xa8d4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8d4U: { // 4eb900008654 jsr $8654.l
            next = 0xa8daU;
            const auto result = m.call(c, 123U, 0xa8d4U, 0x8654U, 0xa8daU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8daU: { // 6100007e bsr.w $a95a
            next = 0xa8deU;
            const auto result = m.call(c, 618U, 0xa8daU, 0xa95aU, 0xa8deU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8deU: { // 610000f8 bsr.w $a9d8
            next = 0xa8e2U;
            const auto result = m.call(c, 620U, 0xa8deU, 0xa9d8U, 0xa8e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8e2U: { // 7000 moveq #$0, d0
            next = 0xa8e4U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xa8e4U: { // 80380419 or.b $419.w, d0
            next = 0xa8e8U;
            const auto source_address = 0x419U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xa8e8U: { // 23c000404018 move.l d0, $404018.l
            next = 0xa8eeU;
            const auto value = r.data[0];
            const auto destination_address = 0x404018U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xa8eeU: { // 4e75 rts 
            next = 0xa8f0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xa8eeU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xa8b6U:
        case 0xa8bcU:
        case 0xa8c4U:
        case 0xa8caU:
        case 0xa8ceU:
        case 0xa8d4U:
        case 0xa8daU:
        case 0xa8deU:
        case 0xa8e2U:
        case 0xa8e4U:
        case 0xa8e8U:
        case 0xa8eeU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
