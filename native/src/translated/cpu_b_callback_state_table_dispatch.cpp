// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"
#include "cpu_b_initials_entry.h"

namespace gain_ground::translated {
FunctionResult cpu_b_callback_state_table_dispatch(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        if (is_initials_entry_pc(r.program_counter)) return cpu_b_initials_entry(c);
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xee9eU: { // 386d0060 movea.w $60(a5), a4
            next = 0xeea2U;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            r.address[4] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xeea2U: { // 306d006a movea.w $6a(a5), a0
            next = 0xeea6U;
            const auto source_address = r.address[5] + 0x6aU;
            const auto value = m.word(source_address);
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xeea6U: { // 2950008a move.l (a0), $8a(a4)
            next = 0xeeaaU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[4] + 0x8aU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xeeaaU: { // 302d0042 move.w $42(a5), d0
            next = 0xeeaeU;
            m.dw(0U, m.word(r.address[5] + 0x42U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xeeaeU: { // e540 asl.w #$2, d0
            next = 0xeeb0U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xeeb0U: { // 4efb0002 jmp $eeb4(pc, d0.w)
            next = 0xeeb4U;
            next = (0xeeb4U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0xeeb4U: { // 6000000a bra.w $eec0
            next = 0xeeb8U;
            if (true) { next = 0xeec0U; transfer_kind = 1U; }
            break;
        }
        case 0xeeb8U: { // 600004f2 bra.w $f3ac
            next = 0xeebcU;
            if (true) { next = 0xf3acU; transfer_kind = 1U; }
            break;
        }
        case 0xeebcU: { // 6000006e bra.w $ef2c
            next = 0xeec0U;
            if (true) { next = 0xef2cU; transfer_kind = 1U; }
            break;
        }
        case 0xeec0U: { // 0c7800060c16 cmpi.w #$6, $c16.w
            next = 0xeec6U;
            const auto source_value = 0x6U;
            const auto destination_address = 0xc16U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xeec6U: { // 6a0a bpl.b $eed2
            next = 0xeec8U;
            if ((r.status & 8U) == 0U) { next = 0xeed2U; transfer_kind = 1U; }
            break;
        }
        case 0xeec8U: { // 61000afa bsr.w $f9c4
            next = 0xeeccU;
            const auto result = m.call(c, 180U, 0xeec8U, 0xf9c4U, 0xeeccU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xeeccU: { // 61000bc4 bsr.w $fa92
            next = 0xeed0U;
            const auto result = m.call(c, 183U, 0xeeccU, 0xfa92U, 0xeed0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xeed0U: { // 6502 bcs.b $eed4
            next = 0xeed2U;
            if ((r.status & 1U) != 0U) { next = 0xeed4U; transfer_kind = 1U; }
            break;
        }
        case 0xeed2U: { // 4e75 rts 
            next = 0xeed4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xeed2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xeed4U: { // 102d006d move.b $6d(a5), d0
            next = 0xeed8U;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xeed8U: { // d000 add.b d0, d0
            next = 0xeedaU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xeedaU: { // 5200 addq.b #$1, d0
            next = 0xeedcU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xeedcU: { // 13c000d00035 move.b d0, $d00035.l
            next = 0xeee2U;
            const auto value = r.data[0];
            const auto destination_address = 0xd00035U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xeee2U: { // 3b7c00010042 move.w #$1, $42(a5)
            next = 0xeee8U;
            m.word(r.address[5] + 0x42U, 0x1U);
            m.logic(0x1U, 16U);
            break;
        }
        case 0xeee8U: { // 30380c16 move.w $c16.w, d0
            next = 0xeeecU;
            const auto source_address = 0xc16U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xeeecU: { // 0c400001 cmpi.w #$1, d0
            next = 0xeef0U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xeef0U: { // 671e beq.b $ef10
            next = 0xeef2U;
            if ((r.status & 4U) != 0U) { next = 0xef10U; transfer_kind = 1U; }
            break;
        }
        case 0xeef2U: { // 0c400002 cmpi.w #$2, d0
            next = 0xeef6U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xeef6U: { // 6718 beq.b $ef10
            next = 0xeef8U;
            if ((r.status & 4U) != 0U) { next = 0xef10U; transfer_kind = 1U; }
            break;
        }
        case 0xeef8U: { // 426d0044 clr.w $44(a5)
            next = 0xeefcU;
            const auto destination_address = r.address[5] + 0x44U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xeefcU: { // 41f80c0a lea.l $c0a.w, a0
            next = 0xef00U;
            r.address[0] = 0xc0aU;
            break;
        }
        case 0xef00U: { // 0c500064 cmpi.w #$64, (a0)
            next = 0xef04U;
            const auto source_value = 0x64U;
            const auto destination_address = r.address[0];
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xef04U: { // 6a08 bpl.b $ef0e
            next = 0xef06U;
            if ((r.status & 8U) == 0U) { next = 0xef0eU; transfer_kind = 1U; }
            break;
        }
        case 0xef06U: { // 30fc0064 move.w #$64, (a0)+
            next = 0xef0aU;
            const auto value = 0x64U;
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            r.address[0] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0xef0aU: { // 30fc0100 move.w #$100, (a0)+
            next = 0xef0eU;
            const auto value = 0x100U;
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            r.address[0] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0xef0eU: { // 4e75 rts 
            next = 0xef10U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xef0eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xef10U: { // 3b7c000a0044 move.w #$a, $44(a5)
            next = 0xef16U;
            m.word(r.address[5] + 0x44U, 0xaU);
            m.logic(0xaU, 16U);
            break;
        }
        case 0xef16U: { // 296c00000040 move.l $0(a4), $40(a4)
            next = 0xef1cU;
            const auto source_address = r.address[4] + 0x0U;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[4] + 0x40U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xef1cU: { // 196c00040044 move.b $4(a4), $44(a4)
            next = 0xef22U;
            const auto source_address = r.address[4] + 0x4U;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[4] + 0x44U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xef22U: { // 426c0000 clr.w $0(a4)
            next = 0xef26U;
            const auto destination_address = r.address[4] + 0x0U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xef26U: { // 56780c12 addq.w #$3, $c12.w
            next = 0xef2aU;
            const auto source_value = 0x3U;
            const auto destination_address = 0xc12U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xef2aU: { // 4e75 rts 
            next = 0xef2cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xef2aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xef2cU: { // 61000c16 bsr.w $fb44
            next = 0xef30U;
            const auto result = m.call(c, 187U, 0xef2cU, 0xfb44U, 0xef30U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xef30U: { // 302d0044 move.w $44(a5), d0
            next = 0xef34U;
            m.dw(0U, m.word(r.address[5] + 0x44U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xef34U: { // e540 asl.w #$2, d0
            next = 0xef36U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xef36U: { // 4efb0002 jmp $ef3a(pc, d0.w)
            next = 0xef3aU;
            next = (0xef3aU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0xef3aU: { // 6000000a bra.w $ef46
            next = 0xef3eU;
            if (true) { next = 0xef46U; transfer_kind = 1U; }
            break;
        }
        case 0xef3eU: { // 60000154 bra.w $f094
            next = 0xf094U; transfer_kind = 1U; break;
        }
        case 0xef42U: { // 60000116 bra.w $f05a
            next = 0xef46U;
            if (true) { next = 0xf05aU; transfer_kind = 1U; }
            break;
        }
        case 0xef46U: { // 536d0046 subq.w #$1, $46(a5)
            next = 0xef4aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xef4aU: { // 6f16 ble.b $ef62
            next = 0xef4cU;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0xef62U; transfer_kind = 1U; }
            break;
        }
        case 0xef4cU: { // 61000318 bsr.w $f266
            next = 0xef50U;
            const auto result = m.call(c, 176U, 0xef4cU, 0xf266U, 0xef50U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xef50U: { // 0c6d005a0046 cmpi.w #$5a, $46(a5)
            next = 0xef56U;
            const auto source_value = 0x5aU;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xef56U: { // 643c bcc.b $ef94
            next = 0xef58U;
            if ((r.status & 1U) == 0U) { next = 0xef94U; transfer_kind = 1U; }
            break;
        }
        case 0xef58U: { // 102c008c move.b $8c(a4), d0
            next = 0xef5cU;
            const auto source_address = r.address[4] + 0x8cU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xef5cU: { // 02000006 andi.b #$6, d0
            next = 0xef60U;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xef60U: { // 6732 beq.b $ef94
            next = 0xef62U;
            if ((r.status & 4U) != 0U) { next = 0xef94U; transfer_kind = 1U; }
            break;
        }
        case 0xef62U: { // 222c0080 move.l $80(a4), d1
            next = 0xef66U;
            r.data[1] = m.lng(r.address[4] + 0x80U);
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xef66U: { // 41f87800 lea.l $7800.w, a0
            next = 0xef6aU;
            r.address[0] = 0x7800U;
            break;
        }
        case 0xef6aU: { // 7000 moveq #$0, d0
            next = 0xef6cU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xef6cU: { // 7462 moveq #$62, d2
            next = 0xef6eU;
            r.data[2] = 0x62U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xef6eU: { // b290 cmp.l (a0), d1
            next = 0xef70U;
            const auto source_address = r.address[0];
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 32U, true);
            break;
        }
        case 0xef70U: { // 6424 bcc.b $ef96
            next = 0xef72U;
            if ((r.status & 1U) == 0U) { next = 0xef96U; transfer_kind = 1U; }
            break;
        }
        case 0xef72U: { // 5240 addq.w #$1, d0
            next = 0xef74U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xef74U: { // 5048 addq.w #$8, a0
            next = 0xef76U;
            const auto source_value = 0x8U;
            r.address[0] += source_value;
            break;
        }
        case 0xef76U: { // 51cafff6 dbra d2, $ef6e
            next = 0xef7aU;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0xef6eU;
            break;
        }
        case 0xef7aU: { // 43fa1682 lea.l $105fe(pc), a1
            next = 0xef7eU;
            const auto source_address = 0x105feU;
            r.address[1] = source_address;
            break;
        }
        case 0xef7eU: { // 4eb900015fd4 jsr $15fd4.l
            next = 0xef84U;
            const auto result = m.call(c, 290U, 0xef7eU, 0x15fd4U, 0xef84U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xef84U: { // 3b7c00020044 move.w #$2, $44(a5)
            next = 0xef8aU;
            m.word(r.address[5] + 0x44U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xef8aU: { // 3b7c00780046 move.w #$78, $46(a5)
            next = 0xef90U;
            m.word(r.address[5] + 0x46U, 0x78U);
            m.logic(0x78U, 16U);
            break;
        }
        case 0xef90U: { // 4af80836 tas.b $836.w
            next = 0xef94U;
            const auto destination_address = 0x836U;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0xef94U: { // 4e75 rts 
            next = 0xef96U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xef94U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xef96U: { // 0c40000a cmpi.w #$a, d0
            next = 0xef9aU;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xef9aU: { // 653e bcs.b $efda
            next = 0xef9cU;
            if ((r.status & 1U) != 0U) { next = 0xefdaU; transfer_kind = 1U; }
            break;
        }
        case 0xef9cU: { // 7261 moveq #$61, d1
            next = 0xef9eU;
            r.data[1] = 0x61U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xef9eU: { // 9240 sub.w d0, d1
            next = 0xefa0U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xefa0U: { // 3f01 move.w d1, -(a7)
            next = 0xefa2U;
            const auto value = r.data[1];
            r.address[7] -= 2U;
            const auto destination_address = r.address[7];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xefa2U: { // 31c00836 move.w d0, $836.w
            next = 0xefa6U;
            const auto value = r.data[0];
            const auto destination_address = 0x836U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xefa6U: { // 5240 addq.w #$1, d0
            next = 0xefa8U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xefa8U: { // 4eb900016234 jsr $16234.l
            next = 0xefaeU;
            const auto result = m.call(c, 299U, 0xefa8U, 0x16234U, 0xefaeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xefaeU: { // 2400 move.l d0, d2
            next = 0xefb0U;
            const auto value = r.data[0];
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0xefb0U: { // e09a ror.l #$8, d2
            next = 0xefb2U;
            m.rotate_right(2U, 8U, 32U);
            break;
        }
        case 0xefb2U: { // 43fa167c lea.l $10630(pc), a1
            next = 0xefb6U;
            const auto source_address = 0x10630U;
            r.address[1] = source_address;
            break;
        }
        case 0xefb6U: { // 4eb90001610e jsr $1610e.l
            next = 0xefbcU;
            const auto result = m.call(c, 296U, 0xefb6U, 0x1610eU, 0xefbcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xefbcU: { // 43fa165a lea.l $10618(pc), a1
            next = 0xefc0U;
            const auto source_address = 0x10618U;
            r.address[1] = source_address;
            break;
        }
        case 0xefc0U: { // 4eb900015fd4 jsr $15fd4.l
            next = 0xefc6U;
            const auto result = m.call(c, 290U, 0xefc0U, 0x15fd4U, 0xefc6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xefc6U: { // 321f move.w (a7)+, d1
            next = 0xefc8U;
            const auto source_address = r.address[7];
            const auto value = m.word(source_address);
            r.address[7] += 2U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xefc8U: { // 202c0080 move.l $80(a4), d0
            next = 0xefccU;
            r.data[0] = m.lng(r.address[4] + 0x80U);
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xefccU: { // 42ac0098 clr.l $98(a4)
            next = 0xefd0U;
            const auto destination_address = r.address[4] + 0x98U;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0xefd0U: { // 19780c03009b move.b $c03.w, $9b(a4)
            next = 0xefd6U;
            const auto source_address = 0xc03U;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[4] + 0x9bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xefd6U: { // 60000266 bra.w $f23e
            next = 0xefdaU;
            if (true) { next = 0xf23eU; transfer_kind = 1U; }
            break;
        }
        case 0xefdaU: { // 41f900200082 lea.l $200082.l, a0
            next = 0xefe0U;
            r.address[0] = 0x200082U;
            break;
        }
        case 0xefe0U: { // 0c400009 cmpi.w #$9, d0
            next = 0xefe4U;
            const auto source_value = 0x9U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xefe4U: { // 6716 beq.b $effc
            next = 0xefe6U;
            if ((r.status & 4U) != 0U) { next = 0xeffcU; transfer_kind = 1U; }
            break;
        }
        case 0xefe6U: { // e540 asl.w #$2, d0
            next = 0xefe8U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xefe8U: { // d07c80c4 add.w #$80c4, d0
            next = 0xefecU;
            const auto source_value = 0x80c4U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xefecU: { // 806d0062 or.w $62(a5), d0
            next = 0xeff0U;
            const auto source_address = r.address[5] + 0x62U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xeff0U: { // d0ed007a adda.w $7a(a5), a0
            next = 0xeff4U;
            const auto source_address = r.address[5] + 0x7aU;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xeff4U: { // 4eb9000161ea jsr $161ea.l
            next = 0xeffaU;
            const auto result = m.call(c, 298U, 0xeff4U, 0x161eaU, 0xeffaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xeffaU: { // 600c bra.b $f008
            next = 0xeffcU;
            if (true) { next = 0xf008U; transfer_kind = 1U; }
            break;
        }
        case 0xeffcU: { // 43fa1636 lea.l $10634(pc), a1
            next = 0xf000U;
            const auto source_address = 0x10634U;
            r.address[1] = source_address;
            break;
        }
        case 0xf000U: { // 7401 moveq #$1, d2
            next = 0xf002U;
            r.data[2] = 0x1U;
            m.logic(r.data[2], 32U);
            break;
        }
        case 0xf002U: { // 4eb900015fde jsr $15fde.l
            next = 0xf008U;
            const auto result = m.call(c, 540U, 0xf002U, 0x15fdeU, 0xf008U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf008U: { // 41f900200182 lea.l $200182.l, a0
            next = 0xf00eU;
            r.address[0] = 0x200182U;
            break;
        }
        case 0xf00eU: { // d0ed007a adda.w $7a(a5), a0
            next = 0xf012U;
            const auto source_address = r.address[5] + 0x7aU;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf012U: { // 4290 clr.l (a0)
            next = 0xf014U;
            const auto destination_address = r.address[0];
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0xf014U: { // 3b7c00010044 move.w #$1, $44(a5)
            next = 0xf01aU;
            m.word(r.address[5] + 0x44U, 0x1U);
            m.logic(0x1U, 16U);
            break;
        }
        case 0xf01aU: { // 397c00000096 move.w #$0, $96(a4)
            next = 0xf020U;
            m.word(r.address[4] + 0x96U, 0x0U);
            m.logic(0x0U, 16U);
            break;
        }
        case 0xf020U: { // 397c001a0098 move.w #$1a, $98(a4)
            next = 0xf026U;
            m.word(r.address[4] + 0x98U, 0x1aU);
            m.logic(0x1aU, 16U);
            break;
        }
        case 0xf026U: { // 397c1a1a009a move.w #$1a1a, $9a(a4)
            next = 0xf02cU;
            m.word(r.address[4] + 0x9aU, 0x1a1aU);
            m.logic(0x1a1aU, 16U);
            break;
        }
        case 0xf02cU: { // 397c001e009c move.w #$1e, $9c(a4)
            next = 0xf032U;
            m.word(r.address[4] + 0x9cU, 0x1eU);
            m.logic(0x1eU, 16U);
            break;
        }
        case 0xf032U: { // 397c0020009e move.w #$20, $9e(a4)
            next = 0xf038U;
            m.word(r.address[4] + 0x9eU, 0x20U);
            m.logic(0x20U, 16U);
            break;
        }
        case 0xf038U: { // 61000266 bsr.w $f2a0
            next = 0xf03cU;
            const auto result = m.call(c, 526U, 0xf038U, 0xf2a0U, 0xf03cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf03cU: { // 397c001900a0 move.w #$19, $a0(a4)
            next = 0xf042U;
            m.word(r.address[4] + 0xa0U, 0x19U);
            m.logic(0x19U, 16U);
            break;
        }
        case 0xf042U: { // 306d0068 movea.w $68(a5), a0
            next = 0xf046U;
            const auto source_address = r.address[5] + 0x68U;
            const auto value = m.word(source_address);
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xf046U: { // 1010 move.b (a0), d0
            next = 0xf048U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf048U: { // 80380404 or.b $404.w, d0
            next = 0xf04cU;
            const auto source_address = 0x404U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xf04cU: { // 6706 beq.b $f054
            next = 0xf04eU;
            if ((r.status & 4U) != 0U) { next = 0xf054U; transfer_kind = 1U; }
            break;
        }
        case 0xf04eU: { // 197c001c0098 move.b #$1c, $98(a4)
            next = 0xf054U;
            m.byte(r.address[4] + 0x98U, 0x1cU);
            m.logic(0x1cU, 8U);
            break;
        }
        case 0xf054U: { // 6100025e bsr.w $f2b4
            next = 0xf058U;
            const auto result = m.call(c, 527U, 0xf054U, 0xf2b4U, 0xf058U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf058U: { // 4e75 rts 
            next = 0xf05aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf058U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf05aU: { // 536d0046 subq.w #$1, $46(a5)
            next = 0xf05eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xf05eU: { // 6f12 ble.b $f072
            next = 0xf060U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0xf072U; transfer_kind = 1U; }
            break;
        }
        case 0xf060U: { // 0c6d005a0046 cmpi.w #$5a, $46(a5)
            next = 0xf066U;
            const auto source_value = 0x5aU;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf066U: { // 642a bcc.b $f092
            next = 0xf068U;
            if ((r.status & 1U) == 0U) { next = 0xf092U; transfer_kind = 1U; }
            break;
        }
        case 0xf068U: { // 102c008d move.b $8d(a4), d0
            next = 0xf06cU;
            const auto source_address = r.address[4] + 0x8dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf06cU: { // 02000006 andi.b #$6, d0
            next = 0xf070U;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xf070U: { // 6720 beq.b $f092
            next = 0xf072U;
            if ((r.status & 4U) != 0U) { next = 0xf092U; transfer_kind = 1U; }
            break;
        }
        case 0xf072U: { // 102d006d move.b $6d(a5), d0
            next = 0xf076U;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf076U: { // 01b80c04 bclr.b d0, $c04.w
            next = 0xf07aU;
            const auto destination_address = 0xc04U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (r.data[0] & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0xf07aU: { // 426d0042 clr.w $42(a5)
            next = 0xf07eU;
            const auto destination_address = r.address[5] + 0x42U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf07eU: { // 61000a9e bsr.w $fb1e
            next = 0xf082U;
            const auto result = m.call(c, 185U, 0xf07eU, 0xfb1eU, 0xf082U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf082U: { // 61000aee bsr.w $fb72
            next = 0xf086U;
            const auto result = m.call(c, 188U, 0xf082U, 0xfb72U, 0xf086U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf086U: { // 102d006d move.b $6d(a5), d0
            next = 0xf08aU;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf08aU: { // d000 add.b d0, d0
            next = 0xf08cU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0xf08cU: { // 13c000d00035 move.b d0, $d00035.l
            next = 0xf092U;
            const auto value = r.data[0];
            const auto destination_address = 0xd00035U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf092U: { // 4e75 rts 
            next = 0xf094U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf092U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf23eU: { // 41f87b18 lea.l $7b18.w, a0
            next = 0xf242U;
            r.address[0] = 0x7b18U;
            break;
        }
        case 0xf242U: { // 43e8fff8 lea.l -$8(a0), a1
            next = 0xf246U;
            const auto source_address = r.address[0] - 0x8U;
            r.address[1] = source_address;
            break;
        }
        case 0xf246U: { // 4a41 tst.w d1
            next = 0xf248U;
            const auto value = r.data[1];
            m.logic(value, 16U);
            break;
        }
        case 0xf248U: { // 6b08 bmi.b $f252
            next = 0xf24aU;
            if ((r.status & 8U) != 0U) { next = 0xf252U; transfer_kind = 1U; }
            break;
        }
        case 0xf24aU: { // 2121 move.l -(a1), -(a0)
            next = 0xf24cU;
            r.address[1] -= 4U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[0] -= 4U;
            const auto destination_address = r.address[0];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf24cU: { // 2121 move.l -(a1), -(a0)
            next = 0xf24eU;
            r.address[1] -= 4U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[0] -= 4U;
            const auto destination_address = r.address[0];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf24eU: { // 51c9fffa dbra d1, $f24a
            next = 0xf252U;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0xf24aU;
            break;
        }
        case 0xf252U: { // 22c0 move.l d0, (a1)+
            next = 0xf254U;
            const auto value = r.data[0];
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0xf254U: { // 22ac0098 move.l $98(a4), (a1)
            next = 0xf258U;
            const auto source_address = r.address[4] + 0x98U;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf258U: { // 3b7c00020044 move.w #$2, $44(a5)
            next = 0xf25eU;
            m.word(r.address[5] + 0x44U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf25eU: { // 3b7c00780046 move.w #$78, $46(a5)
            next = 0xf264U;
            m.word(r.address[5] + 0x46U, 0x78U);
            m.logic(0x78U, 16U);
            break;
        }
        case 0xf264U: { // 4e75 rts 
            next = 0xf266U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf264U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf3acU: { // 083800030833 btst.b #$3, $833.w
            next = 0xf3b2U;
            const auto destination_address = 0x833U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0xf3b2U: { // 6704 beq.b $f3b8
            next = 0xf3b4U;
            if ((r.status & 4U) != 0U) { next = 0xf3b8U; transfer_kind = 1U; }
            break;
        }
        case 0xf3b4U: { // 61000670 bsr.w $fa26
            const auto result = m.call(c, 181U, pc, 0xfa26U, 0xf3b8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; break;
        }
        case 0xf3b8U: { // 6100078a bsr.w $fb44
            next = 0xf3bcU;
            const auto result = m.call(c, 187U, 0xf3b8U, 0xfb44U, 0xf3bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf3bcU: { // 61000048 bsr.w $f406
            next = 0xf3c0U;
            const auto result = m.call(c, 179U, 0xf3bcU, 0xf406U, 0xf3c0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (next == 0xf3c0U) return c.host->call_function(178U, m.cpu, m.state, transfer_kind, pc, next, c);
        switch (next) {
        case 0xee9eU:
        case 0xeea2U:
        case 0xeea6U:
        case 0xeeaaU:
        case 0xeeaeU:
        case 0xeeb0U:
        case 0xeeb4U:
        case 0xeeb8U:
        case 0xeebcU:
        case 0xeec0U:
        case 0xeec6U:
        case 0xeec8U:
        case 0xeeccU:
        case 0xeed0U:
        case 0xeed2U:
        case 0xeed4U:
        case 0xeed8U:
        case 0xeedaU:
        case 0xeedcU:
        case 0xeee2U:
        case 0xeee8U:
        case 0xeeecU:
        case 0xeef0U:
        case 0xeef2U:
        case 0xeef6U:
        case 0xeef8U:
        case 0xeefcU:
        case 0xef00U:
        case 0xef04U:
        case 0xef06U:
        case 0xef0aU:
        case 0xef0eU:
        case 0xef10U:
        case 0xef16U:
        case 0xef1cU:
        case 0xef22U:
        case 0xef26U:
        case 0xef2aU:
        case 0xef2cU:
        case 0xef30U:
        case 0xef34U:
        case 0xef36U:
        case 0xef3aU:
        case 0xef3eU:
        case 0xef42U:
        case 0xef46U:
        case 0xef4aU:
        case 0xef4cU:
        case 0xef50U:
        case 0xef56U:
        case 0xef58U:
        case 0xef5cU:
        case 0xef60U:
        case 0xef62U:
        case 0xef66U:
        case 0xef6aU:
        case 0xef6cU:
        case 0xef6eU:
        case 0xef70U:
        case 0xef72U:
        case 0xef74U:
        case 0xef76U:
        case 0xef7aU:
        case 0xef7eU:
        case 0xef84U:
        case 0xef8aU:
        case 0xef90U:
        case 0xef94U:
        case 0xef96U:
        case 0xef9aU:
        case 0xef9cU:
        case 0xef9eU:
        case 0xefa0U:
        case 0xefa2U:
        case 0xefa6U:
        case 0xefa8U:
        case 0xefaeU:
        case 0xefb0U:
        case 0xefb2U:
        case 0xefb6U:
        case 0xefbcU:
        case 0xefc0U:
        case 0xefc6U:
        case 0xefc8U:
        case 0xefccU:
        case 0xefd0U:
        case 0xefd6U:
        case 0xefdaU:
        case 0xefe0U:
        case 0xefe4U:
        case 0xefe6U:
        case 0xefe8U:
        case 0xefecU:
        case 0xeff0U:
        case 0xeff4U:
        case 0xeffaU:
        case 0xeffcU:
        case 0xf000U:
        case 0xf002U:
        case 0xf008U:
        case 0xf00eU:
        case 0xf012U:
        case 0xf014U:
        case 0xf01aU:
        case 0xf020U:
        case 0xf026U:
        case 0xf02cU:
        case 0xf032U:
        case 0xf038U:
        case 0xf03cU:
        case 0xf042U:
        case 0xf046U:
        case 0xf048U:
        case 0xf04cU:
        case 0xf04eU:
        case 0xf054U:
        case 0xf058U:
        case 0xf05aU:
        case 0xf05eU:
        case 0xf060U:
        case 0xf066U:
        case 0xf068U:
        case 0xf06cU:
        case 0xf070U:
        case 0xf072U:
        case 0xf076U:
        case 0xf07aU:
        case 0xf07eU:
        case 0xf082U:
        case 0xf086U:
        case 0xf08aU:
        case 0xf08cU:
        case 0xf092U:
        case 0xf094U:
        case 0xf23eU:
        case 0xf242U:
        case 0xf246U:
        case 0xf248U:
        case 0xf24aU:
        case 0xf24cU:
        case 0xf24eU:
        case 0xf252U:
        case 0xf254U:
        case 0xf258U:
        case 0xf25eU:
        case 0xf264U:
        case 0xf3acU:
        case 0xf3b2U:
        case 0xf3b4U:
        case 0xf3b8U:
        case 0xf3bcU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
