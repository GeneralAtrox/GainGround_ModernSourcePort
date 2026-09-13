// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_a_mode_dispatcher(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 0U, 255U};
    for (;;) {
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x80612U: { // 08390007fff00820 btst.b #$7, $fff00820.l
            next = 0x8061aU;
            const auto destination_address = 0xfff00820U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x8061aU: { // 6602 bne.b $8061e
            next = 0x8061cU;
            if ((r.status & 4U) == 0U) { next = 0x8061eU; transfer_kind = 1U; }
            break;
        }
        case 0x8061cU: { // 4e75 rts 
            next = 0x8061eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x8061cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x8061eU: { // 4ef900081264 jmp $81264.l
            next = 0x80624U;
            next = 0x81264U;
            transfer_kind = 1U;
            break;
        }
        case 0x81264U: { // 4eb9000831de jsr $831de.l
            next = 0x8126aU;
            const auto result = m.call(c, 465U, 0x81264U, 0x831deU, 0x8126aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x8126aU: { // 13fc00010040401b move.b #$1, $40401b.l
            next = 0x81272U;
            const auto value = 0x1U;
            const auto destination_address = 0x40401bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x81272U: { // 4eb90008105a jsr $8105a.l
            next = 0x81278U;
            const auto result = m.call(c, 453U, 0x81272U, 0x8105aU, 0x81278U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x81278U: { // 4eb90008108c jsr $8108c.l
            next = 0x8127eU;
            const auto result = m.call(c, 454U, 0x81278U, 0x8108cU, 0x8127eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x8127eU: { // 4eb9000810b0 jsr $810b0.l
            next = 0x81284U;
            const auto result = m.call(c, 455U, 0x8127eU, 0x810b0U, 0x81284U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x81284U: { // 4eb9000810d2 jsr $810d2.l
            next = 0x8128aU;
            const auto result = m.call(c, 456U, 0x81284U, 0x810d2U, 0x8128aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x8128aU: { // 23fc0000000000404018 move.l #$0, $404018.l
            next = 0x81294U;
            const auto value = 0x0U;
            const auto destination_address = 0x404018U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x81294U: { // 61001052 bsr.w $822e8
            next = 0x81298U;
            const auto result = m.call(c, 461U, 0x81294U, 0x822e8U, 0x81298U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x81298U: { // 610010d2 bsr.w $8236c
            next = 0x8129cU;
            const auto result = m.call(c, 462U, 0x81298U, 0x8236cU, 0x8129cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x8129cU: { // 4df9ffff8000 lea.l $ffff8000.l, a6
            next = 0x812a2U;
            r.address[6] = 0xffff8000U;
            break;
        }
        case 0x812a2U: { // 3d7c00000004 move.w #$0, $4(a6)
            next = 0x812a8U;
            m.word(r.address[6] + 0x4U, 0x0U);
            m.logic(0x0U, 16U);
            break;
        }
        case 0x812a8U: { // 4df9ffff8000 lea.l $ffff8000.l, a6
            next = 0x812aeU;
            r.address[6] = 0xffff8000U;
            break;
        }
        case 0x812aeU: { // 4eb90008105a jsr $8105a.l
            next = 0x812b4U;
            const auto result = m.call(c, 453U, 0x812aeU, 0x8105aU, 0x812b4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x812b4U: { // 4eb9000810d2 jsr $810d2.l
            next = 0x812baU;
            const auto result = m.call(c, 456U, 0x812b4U, 0x810d2U, 0x812baU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x812baU: { // 7c00 moveq #$0, d6
            next = 0x812bcU;
            r.data[6] = 0x0U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x812bcU: { // 30390080000c move.w $80000c.l, d0
            next = 0x812c2U;
            const auto source_address = 0x80000cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x812c2U: { // 4640 not.w d0
            next = 0x812c4U;
            const auto old = r.data[0];
            const auto value = ~old;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x812c4U: { // 02400001 andi.w #$1, d0
            next = 0x812c8U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x812c8U: { // d040 add.w d0, d0
            next = 0x812caU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x812caU: { // 33fc00000040401a move.w #$0, $40401a.l
            next = 0x812d2U;
            const auto value = 0x0U;
            const auto destination_address = 0x40401aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x812d2U: { // 81790040401a or.w d0, $40401a.l
            next = 0x812d8U;
            const auto source_value = r.data[0];
            const auto destination_address = 0x40401aU;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x812d8U: { // 4eb900080346 jsr $80346.l
            next = 0x812deU;
            const auto result = m.call(c, 452U, 0x812d8U, 0x80346U, 0x812deU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x812deU: { // 61000fbc bsr.w $8229c
            next = 0x812e2U;
            const auto result = m.call(c, 460U, 0x812deU, 0x8229cU, 0x812e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x812e2U: { // 61000f86 bsr.w $8226a
            next = 0x812e6U;
            const auto result = m.call(c, 459U, 0x812e2U, 0x8226aU, 0x812e6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x812e6U: { // 08390002fff0081e btst.b #$2, $fff0081e.l
            next = 0x812eeU;
            const auto destination_address = 0xfff0081eU;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x812eeU: { // 67cc beq.b $812bc
            next = 0x812f0U;
            if ((r.status & 4U) != 0U) { next = 0x812bcU; transfer_kind = 1U; }
            break;
        }
        case 0x812f0U: { // 302e0004 move.w $4(a6), d0
            next = 0x812f4U;
            m.dw(0U, m.word(r.address[6] + 0x4U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0x812f4U: { // d040 add.w d0, d0
            next = 0x812f6U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x812f6U: { // d040 add.w d0, d0
            next = 0x812f8U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x812f8U: { // 4efb0002 jmp $812fc(pc, d0.w)
            next = 0x812fcU;
            next = (0x812fcU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x80612U:
        case 0x8061aU:
        case 0x8061cU:
        case 0x8061eU:
        case 0x81264U:
        case 0x8126aU:
        case 0x81272U:
        case 0x81278U:
        case 0x8127eU:
        case 0x81284U:
        case 0x8128aU:
        case 0x81294U:
        case 0x81298U:
        case 0x8129cU:
        case 0x812a2U:
        case 0x812a8U:
        case 0x812aeU:
        case 0x812b4U:
        case 0x812baU:
        case 0x812bcU:
        case 0x812c2U:
        case 0x812c4U:
        case 0x812c8U:
        case 0x812caU:
        case 0x812d2U:
        case 0x812d8U:
        case 0x812deU:
        case 0x812e2U:
        case 0x812e6U:
        case 0x812eeU:
        case 0x812f0U:
        case 0x812f4U:
        case 0x812f6U:
        case 0x812f8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
