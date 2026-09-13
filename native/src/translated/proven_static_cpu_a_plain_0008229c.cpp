// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_a_plain_0008229c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x8229cU: { // 41f900082adc lea.l $82adc.l, a0
            next = 0x822a2U;
            r.address[0] = 0x82adcU;
            break;
        }
        case 0x822a2U: { // 4eb900081250 jsr $81250.l
            next = 0x822a8U;
            const auto result = m.call(c, 458U, 0x822a2U, 0x81250U, 0x822a8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x822a8U: { // 41f900082b2c lea.l $82b2c.l, a0
            next = 0x822aeU;
            r.address[0] = 0x82b2cU;
            break;
        }
        case 0x822aeU: { // 5246 addq.w #$1, d6
            next = 0x822b0U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[6];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(6U, value);
            break;
        }
        case 0x822b0U: { // 3e2e0004 move.w $4(a6), d7
            next = 0x822b4U;
            m.dw(7U, m.word(r.address[6] + 0x4U));
            m.logic(r.data[7], 16U);
            break;
        }
        case 0x822b4U: { // 5247 addq.w #$1, d7
            next = 0x822b6U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[7];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x822b6U: { // 5347 subq.w #$1, d7
            next = 0x822b8U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[7];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x822b8U: { // 6622 bne.b $822dc
            next = 0x822baU;
            if ((r.status & 4U) == 0U) { next = 0x822dcU; transfer_kind = 1U; }
            break;
        }
        case 0x822baU: { // 123c0004 move.b #$4, d1
            next = 0x822beU;
            const auto value = 0x4U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x822beU: { // 08060005 btst.b #$5, d6
            next = 0x822c2U;
            const auto old = r.data[6];
            const auto bit_mask = 1U << (0x5U & 31U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x822c2U: { // 6604 bne.b $822c8
            next = 0x822c4U;
            if ((r.status & 4U) == 0U) { next = 0x822c8U; transfer_kind = 1U; }
            break;
        }
        case 0x822c4U: { // 123c0001 move.b #$1, d1
            next = 0x822c8U;
            const auto value = 0x1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x822c8U: { // 143c0004 move.b #$4, d2
            next = 0x822ccU;
            const auto value = 0x4U;
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x822ccU: { // d0fc0002 adda.w #$2, a0
            next = 0x822d0U;
            const auto source_value = 0x2U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x822d0U: { // 4eb9000811a0 jsr $811a0.l
            next = 0x822d6U;
            const auto result = m.call(c, 495U, 0x822d0U, 0x811a0U, 0x822d6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x822d6U: { // 4a10 tst.b (a0)
            next = 0x822d8U;
            const auto destination_address = r.address[0];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x822d8U: { // 6adc bpl.b $822b6
            next = 0x822daU;
            if ((r.status & 8U) == 0U) { next = 0x822b6U; transfer_kind = 1U; }
            break;
        }
        case 0x822daU: { // 4e75 rts 
            next = 0x822dcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x822daU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x822dcU: { // 4eb900081198 jsr $81198.l
            next = 0x822e2U;
            const auto result = m.call(c, 457U, 0x822dcU, 0x81198U, 0x822e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x822e2U: { // 4a10 tst.b (a0)
            next = 0x822e4U;
            const auto destination_address = r.address[0];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x822e4U: { // 6ad0 bpl.b $822b6
            next = 0x822e6U;
            if ((r.status & 8U) == 0U) { next = 0x822b6U; transfer_kind = 1U; }
            break;
        }
        case 0x822e6U: { // 4e75 rts 
            next = 0x822e8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x822e6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x8229cU:
        case 0x822a2U:
        case 0x822a8U:
        case 0x822aeU:
        case 0x822b0U:
        case 0x822b4U:
        case 0x822b6U:
        case 0x822b8U:
        case 0x822baU:
        case 0x822beU:
        case 0x822c2U:
        case 0x822c4U:
        case 0x822c8U:
        case 0x822ccU:
        case 0x822d0U:
        case 0x822d6U:
        case 0x822d8U:
        case 0x822daU:
        case 0x822dcU:
        case 0x822e2U:
        case 0x822e4U:
        case 0x822e6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
