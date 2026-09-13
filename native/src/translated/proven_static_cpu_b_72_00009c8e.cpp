// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_00009c8e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x9c8eU: { // 4eb900017054 jsr $17054.l
            next = 0x9c94U;
            const auto result = m.call(c, 311U, 0x9c8eU, 0x17054U, 0x9c94U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x9c94U: { // 4af80418 tas.b $418.w
            next = 0x9c98U;
            const auto destination_address = 0x418U;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x9c98U: { // 33fc00010040401a move.w #$1, $40401a.l
            next = 0x9ca0U;
            const auto value = 0x1U;
            const auto destination_address = 0x40401aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x9ca0U: { // 4eb9000085ac jsr $85ac.l
            next = 0x9ca6U;
            const auto result = m.call(c, 117U, 0x9ca0U, 0x85acU, 0x9ca6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x9ca6U: { // 4eb9000085f2 jsr $85f2.l
            next = 0x9cacU;
            const auto result = m.call(c, 121U, 0x9ca6U, 0x85f2U, 0x9cacU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x9cacU: { // 4eb900008622 jsr $8622.l
            next = 0x9cb2U;
            const auto result = m.call(c, 122U, 0x9cacU, 0x8622U, 0x9cb2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x9cb2U: { // 4eb900008654 jsr $8654.l
            next = 0x9cb8U;
            const auto result = m.call(c, 123U, 0x9cb2U, 0x8654U, 0x9cb8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x9cb8U: { // 61000022 bsr.w $9cdc
            next = 0x9cbcU;
            const auto result = m.call(c, 131U, 0x9cb8U, 0x9cdcU, 0x9cbcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x9cbcU: { // 4eb90000a7ca jsr $a7ca.l
            next = 0x9cc2U;
            const auto result = m.call(c, 143U, 0x9cbcU, 0xa7caU, 0x9cc2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x9cc2U: { // 61000266 bsr.w $9f2a
            next = 0x9cc6U;
            const auto result = m.call(c, 137U, 0x9cc2U, 0x9f2aU, 0x9cc6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x9cc6U: { // 33fc000200404018 move.w #$2, $404018.l
            next = 0x9cceU;
            const auto value = 0x2U;
            const auto destination_address = 0x404018U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x9cceU: { // 13f804190040401b move.b $419.w, $40401b.l
            next = 0x9cd6U;
            const auto source_address = 0x419U;
            const auto value = m.byte(source_address);
            const auto destination_address = 0x40401bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x9cd6U: { // 4ef900008572 jmp $8572.l
            next = 0x9cdcU;
            next = 0x8572U;
            transfer_kind = 1U;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x8572U) return c.host->call_function(116U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0x9c8eU:
        case 0x9c94U:
        case 0x9c98U:
        case 0x9ca0U:
        case 0x9ca6U:
        case 0x9cacU:
        case 0x9cb2U:
        case 0x9cb8U:
        case 0x9cbcU:
        case 0x9cc2U:
        case 0x9cc6U:
        case 0x9cceU:
        case 0x9cd6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
