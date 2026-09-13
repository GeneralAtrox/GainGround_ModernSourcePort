// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult proven_static_cpu_b_72_0001e27e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e27eU: { // 61000970 bsr.w $1ebf0
            next = 0x1e282U;
            const auto result = m.call(c, 363U, 0x1e27eU, 0x1ebf0U, 0x1e282U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e282U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1e286U;
            m.logic(m.byte(r.address[5] + 0x3eU), 8U);
            break;
        }
        case 0x1e286U: { // 6722 beq.b $1e2aa
            next = 0x1e288U;
            if ((r.status & 4U) != 0U) { next = 0x1e2aaU; transfer_kind = 1U; }
            break;
        }
        case 0x1e288U: { // 7200 moveq #$0, d1
            next = 0x1e28aU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e28aU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e28eU;
            m.dw(0U, m.word(r.address[5] + 0x5cU));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x1e28eU: { // 06400080 addi.w #$80, d0
            next = 0x1e292U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e292U: { // 02400700 andi.w #$700, d0
            next = 0x1e296U;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e296U: { // e040 asr.w #$8, d0
            next = 0x1e298U;
            m.shift_word(0U, 8U, false, true);
            break;
        }
        case 0x1e298U: { // 5340 subq.w #$1, d0
            next = 0x1e29aU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e29aU: { // 6b0a bmi.b $1e2a6
            next = 0x1e29cU;
            if ((r.status & 8U) != 0U) { next = 0x1e2a6U; transfer_kind = 1U; }
            break;
        }
        case 0x1e29cU: { // 3407 move.w d7, d2
            next = 0x1e29eU;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e29eU: { // d442 add.w d2, d2
            next = 0x1e2a0U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e2a0U: { // d242 add.w d2, d1
            next = 0x1e2a2U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e2a2U: { // 51c8fffc dbra d0, $1e2a0
            next = 0x1e2a6U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e2a0U;
            break;
        }
        case 0x1e2a6U: { // 6000ff4a bra.w $1e1f2
            next = 0x1e2aaU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e2aaU: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1e2b0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e2b0U: { // 6618 bne.b $1e2ca
            next = 0x1e2b2U;
            if ((r.status & 4U) == 0U) { next = 0x1e2caU; transfer_kind = 1U; }
            break;
        }
        case 0x1e2b2U: { // 3207 move.w d7, d1
            next = 0x1e2b4U;
            const auto value = r.data[7];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e2b4U: { // e549 lsl.w #$2, d1
            next = 0x1e2b6U;
            m.shift_word(1U, 2U, true, false);
            break;
        }
        case 0x1e2b6U: { // 526d0056 addq.w #$1, $56(a5)
            next = 0x1e2baU;
            const auto address = r.address[5] + 0x56U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0x1e2baU: { // 302d0056 move.w $56(a5), d0
            next = 0x1e2beU;
            m.dw(0U, m.word(r.address[5] + 0x56U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x1e2beU: { // 02400008 andi.w #$8, d0
            next = 0x1e2c2U;
            const auto source_value = 0x8U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e2c2U: { // 6602 bne.b $1e2c6
            next = 0x1e2c4U;
            if ((r.status & 4U) == 0U) { next = 0x1e2c6U; transfer_kind = 1U; }
            break;
        }
        case 0x1e2c4U: { // e549 lsl.w #$2, d1
            next = 0x1e2c6U;
            m.shift_word(1U, 2U, true, false);
            break;
        }
        case 0x1e2c6U: { // 6000ff2a bra.w $1e1f2
            next = 0x1e2caU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e2caU: { // 7200 moveq #$0, d1
            next = 0x1e2ccU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e2ccU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e2d0U;
            m.dw(0U, m.word(r.address[5] + 0x5cU));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x1e2d0U: { // 06400080 addi.w #$80, d0
            next = 0x1e2d4U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e2d4U: { // 02400700 andi.w #$700, d0
            next = 0x1e2d8U;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e2d8U: { // e048 lsr.w #$8, d0
            next = 0x1e2daU;
            m.shift_word(0U, 8U, false, false);
            break;
        }
        case 0x1e2daU: { // 5340 subq.w #$1, d0
            next = 0x1e2dcU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e2dcU: { // 6b0a bmi.b $1e2e8
            next = 0x1e2deU;
            if ((r.status & 8U) != 0U) { next = 0x1e2e8U; transfer_kind = 1U; }
            break;
        }
        case 0x1e2deU: { // 3407 move.w d7, d2
            next = 0x1e2e0U;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e2e0U: { // d442 add.w d2, d2
            next = 0x1e2e2U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e2e2U: { // d242 add.w d2, d1
            next = 0x1e2e4U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e2e4U: { // 51c8fffc dbra d0, $1e2e2
            next = 0x1e2e8U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e2e2U;
            break;
        }
        case 0x1e2e8U: { // 082d00020041 btst.b #$2, $41(a5)
            next = 0x1e2eeU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e2eeU: { // 6604 bne.b $1e2f4
            next = 0x1e2f0U;
            if ((r.status & 4U) == 0U) { next = 0x1e2f4U; transfer_kind = 1U; }
            break;
        }
        case 0x1e2f0U: { // 6000ff00 bra.w $1e1f2
            next = 0x1e2f4U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e2f4U: { // 0c6d00020074 cmpi.w #$2, $74(a5)
            next = 0x1e2faU;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e2faU: { // 6e00fef6 bgt.w $1e1f2
            next = 0x1e2feU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e2feU: { // d247 add.w d7, d1
            next = 0x1e300U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e300U: { // 6000fef0 bra.w $1e1f2
            next = 0x1e304U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0x1e1f2U) return c.host->call_function(554U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0x1e27eU:
        case 0x1e282U:
        case 0x1e286U:
        case 0x1e288U:
        case 0x1e28aU:
        case 0x1e28eU:
        case 0x1e292U:
        case 0x1e296U:
        case 0x1e298U:
        case 0x1e29aU:
        case 0x1e29cU:
        case 0x1e29eU:
        case 0x1e2a0U:
        case 0x1e2a2U:
        case 0x1e2a6U:
        case 0x1e2aaU:
        case 0x1e2b0U:
        case 0x1e2b2U:
        case 0x1e2b4U:
        case 0x1e2b6U:
        case 0x1e2baU:
        case 0x1e2beU:
        case 0x1e2c2U:
        case 0x1e2c4U:
        case 0x1e2c6U:
        case 0x1e2caU:
        case 0x1e2ccU:
        case 0x1e2d0U:
        case 0x1e2d4U:
        case 0x1e2d8U:
        case 0x1e2daU:
        case 0x1e2dcU:
        case 0x1e2deU:
        case 0x1e2e0U:
        case 0x1e2e2U:
        case 0x1e2e4U:
        case 0x1e2e8U:
        case 0x1e2eeU:
        case 0x1e2f0U:
        case 0x1e2f4U:
        case 0x1e2faU:
        case 0x1e2feU:
        case 0x1e300U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
