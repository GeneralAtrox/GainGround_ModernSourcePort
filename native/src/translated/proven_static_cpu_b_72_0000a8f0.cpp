// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000a8f0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xa8f0U: { // 4eb900017054 jsr $17054.l
            next = 0xa8f6U;
            const auto result = m.call(c, 311U, 0xa8f0U, 0x17054U, 0xa8f6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa8f6U: { // 33fc00010040401a move.w #$1, $40401a.l
            next = 0xa8feU;
            const auto value = 0x1U;
            const auto destination_address = 0x40401aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xa8feU: { // 4eb9000085ac jsr $85ac.l
            next = 0xa904U;
            const auto result = m.call(c, 117U, 0xa8feU, 0x85acU, 0xa904U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa904U: { // 4eb900008622 jsr $8622.l
            next = 0xa90aU;
            const auto result = m.call(c, 122U, 0xa904U, 0x8622U, 0xa90aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa90aU: { // 4eb900008654 jsr $8654.l
            next = 0xa910U;
            const auto result = m.call(c, 123U, 0xa90aU, 0x8654U, 0xa910U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa910U: { // 6100011c bsr.w $aa2e
            next = 0xa914U;
            const auto result = m.call(c, 621U, 0xa910U, 0xaa2eU, 0xa914U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa914U: { // 4eb90001826e jsr $1826e.l
            next = 0xa91aU;
            const auto result = m.call(c, 322U, 0xa914U, 0x1826eU, 0xa91aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa91aU: { // 61000124 bsr.w $aa40
            next = 0xa91eU;
            const auto result = m.call(c, 622U, 0xa91aU, 0xaa40U, 0xa91eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xa91eU: { // 7000 moveq #$0, d0
            next = 0xa920U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xa920U: { // 80380419 or.b $419.w, d0
            next = 0xa924U;
            const auto source_address = 0x419U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xa924U: { // 23c000404018 move.l d0, $404018.l
            next = 0xa92aU;
            const auto value = r.data[0];
            const auto destination_address = 0x404018U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xa92aU: { // 4e75 rts 
            next = 0xa92cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xa92aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xa8f0U:
        case 0xa8f6U:
        case 0xa8feU:
        case 0xa904U:
        case 0xa90aU:
        case 0xa910U:
        case 0xa914U:
        case 0xa91aU:
        case 0xa91eU:
        case 0xa920U:
        case 0xa924U:
        case 0xa92aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
