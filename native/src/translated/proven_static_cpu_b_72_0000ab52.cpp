// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0000ab52(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xab52U: { // 301a move.w (a2)+, d0
            next = 0xab54U;
            const auto source_address = r.address[2];
            const auto value = m.word(source_address);
            r.address[2] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xab54U: { // 41f900204000 lea.l $204000.l, a0
            next = 0xab5aU;
            r.address[0] = 0x204000U;
            break;
        }
        case 0xab5aU: { // d0da adda.w (a2)+, a0
            next = 0xab5cU;
            const auto source_address = r.address[2];
            const auto source_value = m.word(source_address);
            r.address[2] += 2U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xab5cU: { // 7403 moveq #$3, d2
            next = 0xab5eU;
            r.data[2] = 0x3U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xab5eU: { // 43d0 lea.l (a0), a1
            next = 0xab60U;
            const auto source_address = r.address[0];
            r.address[1] = source_address;
            break;
        }
        case 0xab60U: { // 7207 moveq #$7, d1
            next = 0xab62U;
            r.data[1] = 0x7U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xab62U: { // 32c0 move.w d0, (a1)+
            next = 0xab64U;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.word(destination_address, value);
            r.address[1] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0xab64U: { // 5240 addq.w #$1, d0
            next = 0xab66U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xab66U: { // 51c9fffa dbra d1, $ab62
            next = 0xab6aU;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0xab62U;
            break;
        }
        case 0xab6aU: { // 41e80080 lea.l $80(a0), a0
            next = 0xab6eU;
            r.address[0] = r.address[0] + 0x80U;
            break;
        }
        case 0xab6eU: { // 51caffee dbra d2, $ab5e
            next = 0xab72U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0xab5eU;
            break;
        }
        case 0xab72U: { // 4e75 rts 
            next = 0xab74U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xab72U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xab52U:
        case 0xab54U:
        case 0xab5aU:
        case 0xab5cU:
        case 0xab5eU:
        case 0xab60U:
        case 0xab62U:
        case 0xab64U:
        case 0xab66U:
        case 0xab6aU:
        case 0xab6eU:
        case 0xab72U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
