// Implemented but unverified. Code-first pass: no builds or tests performed.
#include "unverified_cpu_b_machine.h"

namespace gain_ground::translated {
FunctionResult cpu_b_state72_dispatch_record_phase(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host) return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 114U};
    for (;;) {
        if (r.program_counter >= 0xfe18U && r.program_counter <= 0xfe3aU) {
            FunctionResult character_result;
            if (c.host->run_character_update(c, character_result)) return character_result;
        }
        const auto pc = r.program_counter;
        std::uint32_t next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0xf3c0U: { // 41f80d06 lea.l $d06.w, a0
            next = 0xf3c4U;
            r.address[0] = 0xd06U;
            break;
        }
        case 0xf3c4U: { // 7200 moveq #$0, d1
            next = 0xf3c6U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xf3c6U: { // 122d006d move.b $6d(a5), d1
            next = 0xf3caU;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf3caU: { // d241 add.w d1, d1
            next = 0xf3ccU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xf3ccU: { // 302d0044 move.w $44(a5), d0
            next = 0xf3d0U;
            m.dw(0U, m.word(r.address[5] + 0x44U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf3d0U: { // 31801000 move.w d0, (a0, d1.w)
            next = 0xf3d4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[0] + static_cast<std::int16_t>(r.data[1]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf3d4U: { // e540 asl.w #$2, d0
            next = 0xf3d6U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0xf3d6U: { // 4efb0002 jmp $f3da(pc, d0.w)
            next = 0xf3daU;
            next = (0xf3daU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0xf3daU: { // 6000013e bra.w $f51a
            next = 0xf3deU;
            if (true) { next = 0xf51aU; transfer_kind = 1U; }
            break;
        }
        case 0xf3deU: { // 60000216 bra.w $f5f6
            next = 0xf3e2U;
            if (true) { next = 0xf5f6U; transfer_kind = 1U; }
            break;
        }
        case 0xf3e2U: { // 60000280 bra.w $f664
            next = 0xf3e6U;
            if (true) { next = 0xf664U; transfer_kind = 1U; }
            break;
        }
        case 0xf3e6U: { // 600003b4 bra.w $f79c
            next = 0xf3eaU;
            if (true) { next = 0xf79cU; transfer_kind = 1U; }
            break;
        }
        case 0xf3eaU: { // 600003d2 bra.w $f7be
            next = 0xf3eeU;
            if (true) { next = 0xf7beU; transfer_kind = 1U; }
            break;
        }
        case 0xf3eeU: { // 60000a28 bra.w $fe18
            next = 0xf3f2U;
            if (true) { next = 0xfe18U; transfer_kind = 1U; }
            break;
        }
        case 0xf3f2U: { // 60000428 bra.w $f81c
            next = 0xf3f6U;
            if (true) { next = 0xf81cU; transfer_kind = 1U; }
            break;
        }
        case 0xf3f6U: { // 600004fe bra.w $f8f6
            next = 0xf3faU;
            if (true) { next = 0xf8f6U; transfer_kind = 1U; }
            break;
        }
        case 0xf3faU: next = 0xf900U; transfer_kind = 1U; break; // 60000504
        case 0xf3feU: next = 0xf902U; transfer_kind = 1U; break; // 60000502
        case 0xf402U: next = 0xf9b6U; transfer_kind = 1U; break; // 600005b2
        case 0xf51aU: { // 102d006d move.b $6d(a5), d0
            next = 0xf51eU;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf51eU: { // 01b80c06 bclr.b d0, $c06.w
            next = 0xf522U;
            const auto destination_address = 0xc06U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (r.data[0] & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0xf522U: { // 4a2c0000 tst.b $0(a4)
            next = 0xf526U;
            m.logic(m.byte(r.address[4] + 0x0U), 8U);
            break;
        }
        case 0xf526U: { // 6730 beq.b $f558
            next = 0xf528U;
            if ((r.status & 4U) != 0U) { next = 0xf558U; transfer_kind = 1U; }
            break;
        }
        case 0xf528U: { // 142c0001 move.b $1(a4), d2
            next = 0xf52cU;
            const auto source_address = r.address[4] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf52cU: { // e09a ror.l #$8, d2
            next = 0xf52eU;
            m.rotate_right(2U, 8U, 32U);
            break;
        }
        case 0xf52eU: { // 43fa111a lea.l $1064a(pc), a1
            next = 0xf532U;
            const auto source_address = 0x1064aU;
            r.address[1] = source_address;
            break;
        }
        case 0xf532U: { // 4eb9000160fc jsr $160fc.l
            next = 0xf538U;
            const auto result = m.call(c, 295U, 0xf532U, 0x160fcU, 0xf538U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf538U: { // 142c0041 move.b $41(a4), d2
            next = 0xf53cU;
            const auto source_address = r.address[4] + 0x41U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf53cU: { // e09a ror.l #$8, d2
            next = 0xf53eU;
            m.rotate_right(2U, 8U, 32U);
            break;
        }
        case 0xf53eU: { // 43fa110e lea.l $1064e(pc), a1
            next = 0xf542U;
            const auto source_address = 0x1064eU;
            r.address[1] = source_address;
            break;
        }
        case 0xf542U: { // 4eb9000160fc jsr $160fc.l
            next = 0xf548U;
            const auto result = m.call(c, 295U, 0xf542U, 0x160fcU, 0xf548U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf548U: { // 610006a2 bsr.w $fbec
            next = 0xf54cU;
            const auto result = m.call(c, 190U, 0xf548U, 0xfbecU, 0xf54cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf54cU: { // 3b7c00010044 move.w #$1, $44(a5)
            next = 0xf552U;
            m.word(r.address[5] + 0x44U, 0x1U);
            m.logic(0x1U, 16U);
            break;
        }
        case 0xf552U: { // 426d0046 clr.w $46(a5)
            next = 0xf556U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf556U: { // 4e75 rts 
            next = 0xf558U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf556U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf558U: { // 4a2c0040 tst.b $40(a4)
            next = 0xf55cU;
            m.logic(m.byte(r.address[4] + 0x40U), 8U);
            break;
        }
        case 0xf55cU: { // 6714 beq.b $f572
            next = 0xf55eU;
            if ((r.status & 4U) != 0U) { next = 0xf572U; transfer_kind = 1U; }
            break;
        }
        case 0xf55eU: // 3b7c00080044 move.w #8,$44(a5)
            next = 0xf564U; m.word(r.address[5] + 0x44U, 8U); m.logic(8U, 16U); break;
        case 0xf564U: // 397c800000a2 move.w #$8000,$a2(a4)
            next = 0xf56aU; m.word(r.address[4] + 0xa2U, 0x8000U); m.logic(0x8000U, 16U); break;
        case 0xf56aU: { // 29780d020086 move.l $d02.w,$86(a4)
            next = 0xf570U;
            const auto value = m.lng(0xd02U);
            m.lng(r.address[4] + 0x86U, value); m.logic(value, 32U); break;
        }
        case 0xf570U: {
            const auto result = m.ret();
            if (auto event = m.interrupt(c, pc, r.program_counter)) return *event;
            return result;
        }
        case 0xf572U: { // 3b7c00020042 move.w #$2, $42(a5)
            next = 0xf578U;
            m.word(r.address[5] + 0x42U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf578U: { // 3b7c00000044 move.w #$0, $44(a5)
            next = 0xf57eU;
            m.word(r.address[5] + 0x44U, 0x0U);
            m.logic(0x0U, 16U);
            break;
        }
        case 0xf57eU: { // 3b7c00780046 move.w #$78, $46(a5)
            next = 0xf584U;
            m.word(r.address[5] + 0x46U, 0x78U);
            m.logic(0x78U, 16U);
            break;
        }
        case 0xf584U: { // 6100fd02 bsr.w $f288
            next = 0xf588U;
            const auto result = m.call(c, 177U, 0xf584U, 0xf288U, 0xf588U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf588U: { // 610005ba bsr.w $fb44
            next = 0xf58cU;
            const auto result = m.call(c, 187U, 0xf588U, 0xfb44U, 0xf58cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf58cU: { // 102d006d move.b $6d(a5), d0
            next = 0xf590U;
            const auto source_address = r.address[5] + 0x6dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf590U: { // 01f80c04 bset.b d0, $c04.w
            next = 0xf594U;
            const auto destination_address = 0xc04U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (r.data[0] & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0xf594U: { // 01b80820 bclr.b d0, $820.w
            next = 0xf598U;
            const auto destination_address = 0x820U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (r.data[0] & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0xf598U: { // 53380821 subq.b #$1, $821.w
            next = 0xf59cU;
            const auto source_value = 0x1U;
            const auto destination_address = 0x821U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0xf59cU: { // 306d006e movea.w $6e(a5), a0
            next = 0xf5a0U;
            const auto source_address = r.address[5] + 0x6eU;
            const auto value = m.word(source_address);
            r.address[0] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xf5a0U: { // 30280002 move.w $2(a0), d0
            next = 0xf5a4U;
            m.dw(0U, m.word(r.address[0] + 0x2U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf5a4U: { // 41f87b44 lea.l $7b44.w, a0
            next = 0xf5a8U;
            r.address[0] = 0x7b44U;
            break;
        }
        case 0xf5a8U: { // b050 cmp.w (a0), d0
            next = 0xf5aaU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf5aaU: { // 6502 bcs.b $f5ae
            next = 0xf5acU;
            if ((r.status & 1U) != 0U) { next = 0xf5aeU; transfer_kind = 1U; }
            break;
        }
        case 0xf5acU: { // 3080 move.w d0, (a0)
            next = 0xf5aeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf5aeU: { // 5448 addq.w #$2, a0
            next = 0xf5b0U;
            const auto source_value = 0x2U;
            r.address[0] += source_value;
            break;
        }
        case 0xf5b0U: { // b050 cmp.w (a0), d0
            next = 0xf5b2U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf5b2U: { // 6402 bcc.b $f5b6
            next = 0xf5b4U;
            if ((r.status & 1U) == 0U) { next = 0xf5b6U; transfer_kind = 1U; }
            break;
        }
        case 0xf5b4U: // 3080 move.w d0,(a0)
            next = 0xf5b6U; m.word(r.address[0], r.data[0]); m.logic(r.data[0], 16U); break;
        case 0xf5b6U: { // 41fa1164 lea.l $1071c(pc), a0
            next = 0xf5baU;
            const auto source_address = 0x1071cU;
            r.address[0] = source_address;
            break;
        }
        case 0xf5baU: { // 43f87b58 lea.l $7b58.w, a1
            next = 0xf5beU;
            r.address[1] = 0x7b58U;
            break;
        }
        case 0xf5beU: { // b058 cmp.w (a0)+, d0
            next = 0xf5c0U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf5c0U: { // 6504 bcs.b $f5c6
            next = 0xf5c2U;
            if ((r.status & 1U) != 0U) { next = 0xf5c6U; transfer_kind = 1U; }
            break;
        }
        case 0xf5c2U: { // 5449 addq.w #$2, a1
            next = 0xf5c4U;
            const auto source_value = 0x2U;
            r.address[1] += source_value;
            break;
        }
        case 0xf5c4U: { // 60f8 bra.b $f5be
            next = 0xf5c6U;
            if (true) { next = 0xf5beU; transfer_kind = 1U; }
            break;
        }
        case 0xf5c6U: { // 5251 addq.w #$1, (a1)
            next = 0xf5c8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[1];
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xf5c8U: { // 202c0080 move.l $80(a4), d0
            next = 0xf5ccU;
            r.data[0] = m.lng(r.address[4] + 0x80U);
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf5ccU: { // 41f87b48 lea.l $7b48.w, a0
            next = 0xf5d0U;
            r.address[0] = 0x7b48U;
            break;
        }
        case 0xf5d0U: { // b090 cmp.l (a0), d0
            next = 0xf5d2U;
            const auto source_address = r.address[0];
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 32U, true);
            break;
        }
        case 0xf5d2U: { // 6402 bcc.b $f5d6
            next = 0xf5d4U;
            if ((r.status & 1U) == 0U) { next = 0xf5d6U; transfer_kind = 1U; }
            break;
        }
        case 0xf5d4U: // 2080 move.l d0,(a0)
            next = 0xf5d6U; m.lng(r.address[0], r.data[0]); m.logic(r.data[0], 32U); break;
        case 0xf5d6U: { // 5848 addq.w #$4, a0
            next = 0xf5d8U;
            const auto source_value = 0x4U;
            r.address[0] += source_value;
            break;
        }
        case 0xf5d8U: { // b090 cmp.l (a0), d0
            next = 0xf5daU;
            const auto source_address = r.address[0];
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 32U, true);
            break;
        }
        case 0xf5daU: { // 6502 bcs.b $f5de
            next = 0xf5dcU;
            if ((r.status & 1U) != 0U) { next = 0xf5deU; transfer_kind = 1U; }
            break;
        }
        case 0xf5dcU: { // 2080 move.l d0, (a0)
            next = 0xf5deU;
            const auto value = r.data[0];
            const auto destination_address = r.address[0];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf5deU: { // 4eb900016252 jsr $16252.l
            next = 0xf5e4U;
            const auto result = m.call(c, 300U, 0xf5deU, 0x16252U, 0xf5e4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf5e4U: { // 41f87b54 lea.l $7b54.w, a0
            next = 0xf5e8U;
            r.address[0] = 0x7b54U;
            break;
        }
        case 0xf5e8U: { // d390 add.l d1, (a0)
            next = 0xf5eaU;
            const auto source_value = r.data[1];
            const auto destination_address = r.address[0];
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0xf5eaU: { // 6404 bcc.b $f5f0
            next = 0xf5ecU;
            if ((r.status & 1U) == 0U) { next = 0xf5f0U; transfer_kind = 1U; }
            break;
        }
        case 0xf5ecU: { // 52a8fffc addq.l #1,-4(a0)
            next = 0xf5f0U;
            const auto address = r.address[0] - 4U;
            const auto value = m.add(m.lng(address), 1U, 32U);
            m.word(address + 2U, value); m.word(address, value >> 16U); break;
        }
        case 0xf5f0U: { // 4af80410 tas.b $410.w
            next = 0xf5f4U;
            const auto destination_address = 0x410U;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0xf5f4U: { // 4e75 rts 
            next = 0xf5f6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf5f4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf5f6U: { // 41f90020c000 lea.l $20c000.l, a0
            next = 0xf5fcU;
            r.address[0] = 0x20c000U;
            break;
        }
        case 0xf5fcU: { // d0ed0078 adda.w $78(a5), a0
            next = 0xf600U;
            const auto source_address = r.address[5] + 0x78U;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf600U: { // 302d0046 move.w $46(a5), d0
            next = 0xf604U;
            m.dw(0U, m.word(r.address[5] + 0x46U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf604U: { // 323cff80 move.w #$ff80, d1
            next = 0xf608U;
            m.dw(1U, 0xff80U);
            m.logic(r.data[1], 16U);
            break;
        }
        case 0xf608U: { // 31810000 move.w d1, (a0, d0.w)
            next = 0xf60cU;
            const auto value = r.data[1];
            const auto destination_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf60cU: { // 31810008 move.w d1, $8(a0, d0.w)
            next = 0xf610U;
            const auto value = r.data[1];
            const auto destination_address = r.address[0] + 0x8U + static_cast<std::int16_t>(r.data[0]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf610U: { // 31810010 move.w d1, $10(a0, d0.w)
            next = 0xf614U;
            const auto value = r.data[1];
            const auto destination_address = r.address[0] + 0x10U + static_cast<std::int16_t>(r.data[0]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf614U: { // 31810018 move.w d1, $18(a0, d0.w)
            next = 0xf618U;
            const auto value = r.data[1];
            const auto destination_address = r.address[0] + 0x18U + static_cast<std::int16_t>(r.data[0]);
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf618U: { // 06400020 addi.w #$20, d0
            next = 0xf61cU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf61cU: { // 3b400046 move.w d0, $46(a5)
            next = 0xf620U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf620U: { // 0c400400 cmpi.w #$400, d0
            next = 0xf624U;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf624U: { // 653c bcs.b $f662
            next = 0xf626U;
            if ((r.status & 1U) != 0U) { next = 0xf662U; transfer_kind = 1U; }
            break;
        }
        case 0xf626U: { // 41f900200090 lea.l $200090.l, a0
            next = 0xf62cU;
            r.address[0] = 0x200090U;
            break;
        }
        case 0xf62cU: { // d0ed007a adda.w $7a(a5), a0
            next = 0xf630U;
            const auto source_address = r.address[5] + 0x7aU;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf630U: { // 43fa0f12 lea.l $10544(pc), a1
            next = 0xf634U;
            const auto source_address = 0x10544U;
            r.address[1] = source_address;
            break;
        }
        case 0xf634U: { // 7006 moveq #$6, d0
            next = 0xf636U;
            r.data[0] = 0x6U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf636U: { // 3219 move.w (a1)+, d1
            next = 0xf638U;
            const auto source_address = r.address[1];
            const auto value = m.word(source_address);
            r.address[1] += 2U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf638U: { // 826d0064 or.w $64(a5), d1
            next = 0xf63cU;
            const auto source_address = r.address[5] + 0x64U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0xf63cU: { // 3081 move.w d1, (a0)
            next = 0xf63eU;
            const auto value = r.data[1];
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf63eU: { // d0fc0080 adda.w #$80, a0
            next = 0xf642U;
            const auto source_value = 0x80U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf642U: { // 51c8fff2 dbra d0, $f636
            next = 0xf646U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xf636U;
            break;
        }
        case 0xf646U: { // 7037 moveq #$37, d0
            next = 0xf648U;
            r.data[0] = 0x37U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf648U: { // 41f90020c040 lea.l $20c040.l, a0
            next = 0xf64eU;
            r.address[0] = 0x20c040U;
            break;
        }
        case 0xf64eU: { // d0ed0078 adda.w $78(a5), a0
            next = 0xf652U;
            const auto source_address = r.address[5] + 0x78U;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf652U: { // 30bcff00 move.w #$ff00, (a0)
            next = 0xf656U;
            const auto value = 0xff00U;
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf656U: { // 5048 addq.w #$8, a0
            next = 0xf658U;
            const auto source_value = 0x8U;
            r.address[0] += source_value;
            break;
        }
        case 0xf658U: { // 51c8fff8 dbra d0, $f652
            next = 0xf65cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xf652U;
            break;
        }
        case 0xf65cU: { // 3b7c00020044 move.w #$2, $44(a5)
            next = 0xf662U;
            m.word(r.address[5] + 0x44U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf662U: { // 4e75 rts 
            next = 0xf664U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf662U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf664U: { // 102c008b move.b $8b(a4), d0
            next = 0xf668U;
            const auto source_address = r.address[4] + 0x8bU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf668U: { // 123c0006 move.b #$6, d1
            next = 0xf66cU;
            const auto value = 0x6U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf66cU: { // c200 and.b d0, d1
            next = 0xf66eU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 8U);
            m.db(1U, value);
            break;
        }
        case 0xf66eU: { // 6648 bne.b $f6b8
            next = 0xf670U;
            if ((r.status & 4U) == 0U) { next = 0xf6b8U; transfer_kind = 1U; }
            break;
        }
        case 0xf670U: { // 0c2c00010000 cmpi.b #$1, $0(a4)
            next = 0xf676U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[4] + 0x0U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0xf676U: { // 673e beq.b $f6b6
            next = 0xf678U;
            if ((r.status & 4U) != 0U) { next = 0xf6b6U; transfer_kind = 1U; }
            break;
        }
        case 0xf678U: { // 08000005 btst.b #$5, d0
            next = 0xf67cU;
            const auto old = r.data[0];
            const auto bit_mask = 1U << (0x5U & 31U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0xf67cU: { // 6612 bne.b $f690
            next = 0xf67eU;
            if ((r.status & 4U) == 0U) { next = 0xf690U; transfer_kind = 1U; }
            break;
        }
        case 0xf67eU: { // 08000004 btst.b #$4, d0
            next = 0xf682U;
            const auto old = r.data[0];
            const auto bit_mask = 1U << (0x4U & 31U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0xf682U: { // 6732 beq.b $f6b6
            next = 0xf684U;
            if ((r.status & 4U) != 0U) { next = 0xf6b6U; transfer_kind = 1U; }
            break;
        }
        case 0xf684U: // 3b7cfffc0050 move.w #$fffc,$50(a5)
            next = 0xf68aU; m.word(r.address[5] + 0x50U, 0xfffcU); m.logic(0xfffcU, 16U); break;
        case 0xf68aU: { // 61000568 bsr.w $fbf4
            const auto result = m.call(c, 469U, pc, 0xfbf4U, 0xf68eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; break;
        }
        case 0xf68eU: next = 0xf69aU; transfer_kind = 1U; break; // 600a
        case 0xf690U: { // 3b7c00040050 move.w #$4, $50(a5)
            next = 0xf696U;
            m.word(r.address[5] + 0x50U, 0x4U);
            m.logic(0x4U, 16U);
            break;
        }
        case 0xf696U: { // 61000574 bsr.w $fc0c
            next = 0xf69aU;
            const auto result = m.call(c, 191U, 0xf696U, 0xfc0cU, 0xf69aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf69aU: { // 426d0046 clr.w $46(a5)
            next = 0xf69eU;
            const auto destination_address = r.address[5] + 0x46U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf69eU: { // 3b7c00030044 move.w #$3, $44(a5)
            next = 0xf6a4U;
            m.word(r.address[5] + 0x44U, 0x3U);
            m.logic(0x3U, 16U);
            break;
        }
        case 0xf6a4U: { // 2f0c move.l a4, -(a7)
            next = 0xf6a6U;
            const auto value = r.address[4];
            r.address[7] -= 4U;
            const auto destination_address = r.address[7];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf6a6U: { // 303c0042 move.w #$42, d0
            next = 0xf6aaU;
            m.dw(0U, 0x42U);
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf6aaU: { // 4eb900016ff8 jsr $16ff8.l
            next = 0xf6b0U;
            const auto result = m.call(c, 308U, 0xf6aaU, 0x16ff8U, 0xf6b0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf6b0U: { // 285f movea.l (a7)+, a4
            next = 0xf6b2U;
            const auto source_address = r.address[7];
            const auto value = m.lng(source_address);
            r.address[7] += 4U;
            r.address[4] = value;
            break;
        }
        case 0xf6b2U: { // 600000e8 bra.w $f79c
            next = 0xf6b6U;
            if (true) { next = 0xf79cU; transfer_kind = 1U; }
            break;
        }
        case 0xf6b6U: { // 4e75 rts 
            next = 0xf6b8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf6b6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf6b8U: { // 142c0000 move.b $0(a4), d2
            next = 0xf6bcU;
            const auto source_address = r.address[4] + 0x0U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6bcU: { // 5302 subq.b #$1, d2
            next = 0xf6beU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[2];
            const auto value = m.sub(destination_value, source_value, 8U);
            m.db(2U, value);
            break;
        }
        case 0xf6beU: { // 19420000 move.b d2, $0(a4)
            next = 0xf6c2U;
            const auto value = r.data[2];
            const auto destination_address = r.address[4] + 0x0U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6c2U: { // 7201 moveq #$1, d1
            next = 0xf6c4U;
            r.data[1] = 0x1U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xf6c4U: { // 102c0001 move.b $1(a4), d0
            next = 0xf6c8U;
            const auto source_address = r.address[4] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6c8U: { // 44fc0000 move.w #$0, ccr
            next = 0xf6ccU;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
        case 0xf6ccU: { // 8101 sbcd.b d1, d0
            next = 0xf6ceU;
            const auto source_value = r.data[1];
            const auto value = m.sbcd(r.data[0], source_value);
            m.db(0U, value);
            break;
        }
        case 0xf6ceU: { // 19400001 move.b d0, $1(a4)
            next = 0xf6d2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[4] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6d2U: { // 122d004a move.b $4a(a5), d1
            next = 0xf6d6U;
            const auto source_address = r.address[5] + 0x4aU;
            const auto value = m.byte(source_address);
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6d6U: { // 41f41002 lea.l $2(a4, d1.w), a0
            next = 0xf6daU;
            const auto source_address = r.address[4] + 0x2U + static_cast<std::int16_t>(r.data[1]);
            r.address[0] = source_address;
            break;
        }
        case 0xf6daU: { // 1b50004b move.b (a0), $4b(a5)
            next = 0xf6deU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x4bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6deU: { // 9401 sub.b d1, d2
            next = 0xf6e0U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[2];
            const auto value = m.sub(destination_value, source_value, 8U);
            m.db(2U, value);
            break;
        }
        case 0xf6e0U: { // 6708 beq.b $f6ea
            next = 0xf6e2U;
            if ((r.status & 4U) != 0U) { next = 0xf6eaU; transfer_kind = 1U; }
            break;
        }
        case 0xf6e2U: { // 10e80001 move.b $1(a0), (a0)+
            next = 0xf6e6U;
            const auto value = m.byte(r.address[0] + 1U);
            const auto destination = r.address[0];
            r.address[0] += 1U;
            m.byte(destination, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6e6U: { // 5302 subq.b #$1, d2
            next = 0xf6e8U;
            m.db(2U, m.sub(r.data[2], 1U, 8U));
            break;
        }
        case 0xf6e8U: { // 66f8 bne.b $f6e2
            next = 0xf6eaU;
            if ((r.status & 4U) == 0U) { next = 0xf6e2U; transfer_kind = 1U; }
            break;
        }
        case 0xf6eaU: { // 142c0001 move.b $1(a4), d2
            next = 0xf6eeU;
            const auto source_address = r.address[4] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf6eeU: { // e09a ror.l #$8, d2
            next = 0xf6f0U;
            m.rotate_right(2U, 8U, 32U);
            break;
        }
        case 0xf6f0U: { // 43fa0f58 lea.l $1064a(pc), a1
            next = 0xf6f4U;
            const auto source_address = 0x1064aU;
            r.address[1] = source_address;
            break;
        }
        case 0xf6f4U: { // 4eb9000160fc jsr $160fc.l
            next = 0xf6faU;
            const auto result = m.call(c, 295U, 0xf6f4U, 0x160fcU, 0xf6faU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf6faU: { // 7037 moveq #$37, d0
            next = 0xf6fcU;
            r.data[0] = 0x37U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf6fcU: { // 41f90020c040 lea.l $20c040.l, a0
            next = 0xf702U;
            r.address[0] = 0x20c040U;
            break;
        }
        case 0xf702U: { // d0ed0078 adda.w $78(a5), a0
            next = 0xf706U;
            const auto source_address = r.address[5] + 0x78U;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf706U: { // 30bcff80 move.w #$ff80, (a0)
            next = 0xf70aU;
            const auto value = 0xff80U;
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf70aU: { // 5048 addq.w #$8, a0
            next = 0xf70cU;
            const auto source_value = 0x8U;
            r.address[0] += source_value;
            break;
        }
        case 0xf70cU: { // 51c8fff8 dbra d0, $f706
            next = 0xf710U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xf706U;
            break;
        }
        case 0xf710U: { // 7200 moveq #$0, d1
            next = 0xf712U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0xf712U: { // 41f900200090 lea.l $200090.l, a0
            next = 0xf718U;
            r.address[0] = 0x200090U;
            break;
        }
        case 0xf718U: { // d0ed007a adda.w $7a(a5), a0
            next = 0xf71cU;
            const auto source_address = r.address[5] + 0x7aU;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf71cU: { // 7006 moveq #$6, d0
            next = 0xf71eU;
            r.data[0] = 0x6U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf71eU: { // 3081 move.w d1, (a0)
            next = 0xf720U;
            const auto value = r.data[1];
            const auto destination_address = r.address[0];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf720U: { // d0fc0080 adda.w #$80, a0
            next = 0xf724U;
            const auto source_value = 0x80U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf724U: { // 51c8fff8 dbra d0, $f71e
            next = 0xf728U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0xf71eU;
            break;
        }
        case 0xf728U: { // 3b7c00040044 move.w #$4, $44(a5)
            next = 0xf72eU;
            m.word(r.address[5] + 0x44U, 0x4U);
            m.logic(0x4U, 16U);
            break;
        }
        case 0xf72eU: { // 426d0046 clr.w $46(a5)
            next = 0xf732U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf732U: { // 7000 moveq #$0, d0
            next = 0xf734U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf734U: { // 102d004b move.b $4b(a5), d0
            next = 0xf738U;
            const auto source_address = r.address[5] + 0x4bU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf738U: { // e940 asl.w #$4, d0
            next = 0xf73aU;
            m.asl_word(0U, 4U);
            break;
        }
        case 0xf73aU: { // 41f900026f1c lea.l $26f1c.l, a0
            next = 0xf740U;
            r.address[0] = 0x26f1cU;
            break;
        }
        case 0xf740U: { // d0c0 adda.w d0, a0
            next = 0xf742U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf742U: { // 2b48004c move.l a0, $4c(a5)
            next = 0xf746U;
            const auto value = r.address[0];
            const auto destination_address = r.address[5] + 0x4cU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf746U: { // 3b6800060036 move.w $6(a0), $36(a5)
            next = 0xf74cU;
            const auto source_address = r.address[0] + 0x6U;
            const auto value = c.host->character_profile(r.address[5], 6U, m.word(source_address));
            const auto destination_address = r.address[5] + 0x36U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf74cU: { // 3b6800080038 move.w $8(a0), $38(a5)
            next = 0xf752U;
            const auto source_address = r.address[0] + 0x8U;
            const auto value = c.host->character_profile(r.address[5], 8U, m.word(source_address));
            const auto destination_address = r.address[5] + 0x38U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf752U: { // 3b68000a003a move.w $a(a0), $3a(a5)
            next = 0xf758U;
            const auto source_address = r.address[0] + 0xaU;
            const auto value = c.host->character_profile(r.address[5], 10U, m.word(source_address));
            const auto destination_address = r.address[5] + 0x3aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf758U: { // 42ad003c clr.l $3c(a5)
            next = 0xf75cU;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0xf75cU: { // 1b7c0000005a move.b #$0, $5a(a5)
            next = 0xf762U;
            m.byte(r.address[5] + 0x5aU, 0x0U);
            m.logic(0x0U, 8U);
            break;
        }
        case 0xf762U: { // 1b7c0000005b move.b #$0, $5b(a5)
            next = 0xf768U;
            m.byte(r.address[5] + 0x5bU, 0x0U);
            m.logic(0x0U, 8U);
            break;
        }
        case 0xf768U: { // 3b7c001e0048 move.w #$1e, $48(a5)
            next = 0xf76eU;
            m.word(r.address[5] + 0x48U, 0x1eU);
            m.logic(0x1eU, 16U);
            break;
        }
        case 0xf76eU: { // 3b6d00700012 move.w $70(a5), $12(a5)
            next = 0xf774U;
            const auto source_address = r.address[5] + 0x70U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf774U: { // 3b7c00080016 move.w #$8, $16(a5)
            next = 0xf77aU;
            m.word(r.address[5] + 0x16U, 0x8U);
            m.logic(0x8U, 16U);
            break;
        }
        case 0xf77aU: { // 3b7c01c5001a move.w #$1c5, $1a(a5)
            next = 0xf780U;
            m.word(r.address[5] + 0x1aU, 0x1c5U);
            m.logic(0x1c5U, 16U);
            break;
        }
        case 0xf780U: { // 3b7c0000001c move.w #$0, $1c(a5)
            next = 0xf786U;
            m.word(r.address[5] + 0x1cU, 0x0U);
            m.logic(0x0U, 16U);
            break;
        }
        case 0xf786U: { // 3b7c00060058 move.w #$6, $58(a5)
            next = 0xf78cU;
            m.word(r.address[5] + 0x58U, 0x6U);
            m.logic(0x6U, 16U);
            break;
        }
        case 0xf78cU: { // 3b7c00180052 move.w #$18, $52(a5)
            next = 0xf792U;
            m.word(r.address[5] + 0x52U, 0x18U);
            m.logic(0x18U, 16U);
            break;
        }
        case 0xf792U: { // 426d005c clr.w $5c(a5)
            next = 0xf796U;
            const auto destination_address = r.address[5] + 0x5cU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf796U: { // 610004fe bsr.w $fc96
            next = 0xf79aU;
            const auto result = m.call(c, 194U, 0xf796U, 0xfc96U, 0xf79aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf79aU: { // 4e75 rts 
            next = 0xf79cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf79aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf79cU: { // 3c2d0050 move.w $50(a5), d6
            next = 0xf7a0U;
            m.dw(6U, m.word(r.address[5] + 0x50U));
            m.logic(r.data[6], 16U);
            break;
        }
        case 0xf7a0U: { // dd6c0084 add.w d6, $84(a4)
            next = 0xf7a4U;
            const auto source_value = r.data[6];
            const auto destination_address = r.address[4] + 0x84U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xf7a4U: { // 026c01ff0084 andi.w #$1ff, $84(a4)
            next = 0xf7aaU;
            const auto source_value = 0x1ffU;
            const auto destination_address = r.address[4] + 0x84U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0xf7aaU: { // 526d0046 addq.w #$1, $46(a5)
            next = 0xf7aeU;
            const auto address = r.address[5] + 0x46U;
            const auto value = m.add(m.word(address), 0x1U, 16U);
            m.word(address, value);
            break;
        }
        case 0xf7aeU: { // 0c6d00100046 cmpi.w #$10, $46(a5)
            next = 0xf7b4U;
            const auto source_value = 0x10U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf7b4U: { // 6606 bne.b $f7bc
            next = 0xf7b6U;
            if ((r.status & 4U) == 0U) { next = 0xf7bcU; transfer_kind = 1U; }
            break;
        }
        case 0xf7b6U: { // 3b7c00020044 move.w #$2, $44(a5)
            next = 0xf7bcU;
            m.word(r.address[5] + 0x44U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf7bcU: { // 4e75 rts 
            next = 0xf7beU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf7bcU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf7beU: { // 41f90020c000 lea.l $20c000.l, a0
            next = 0xf7c4U;
            r.address[0] = 0x20c000U;
            break;
        }
        case 0xf7c4U: { // d0ed0078 adda.w $78(a5), a0
            next = 0xf7c8U;
            const auto source_address = r.address[5] + 0x78U;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf7c8U: { // 302d0046 move.w $46(a5), d0
            next = 0xf7ccU;
            m.dw(0U, m.word(r.address[5] + 0x46U));
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf7ccU: { // 42700000 clr.w (a0, d0.w)
            next = 0xf7d0U;
            const auto destination_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf7d0U: { // 42700008 clr.w $8(a0, d0.w)
            next = 0xf7d4U;
            const auto destination_address = r.address[0] + 0x8U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf7d4U: { // 42700010 clr.w $10(a0, d0.w)
            next = 0xf7d8U;
            const auto destination_address = r.address[0] + 0x10U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf7d8U: { // 42700018 clr.w $18(a0, d0.w)
            next = 0xf7dcU;
            const auto destination_address = r.address[0] + 0x18U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf7dcU: { // 06400020 addi.w #$20, d0
            next = 0xf7e0U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf7e0U: { // 0c400400 cmpi.w #$400, d0
            next = 0xf7e4U;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf7e4U: { // 6508 bcs.b $f7ee
            next = 0xf7e6U;
            if ((r.status & 1U) != 0U) { next = 0xf7eeU; transfer_kind = 1U; }
            break;
        }
        case 0xf7e6U: { // 3b7c00050044 move.w #$5, $44(a5)
            next = 0xf7ecU;
            m.word(r.address[5] + 0x44U, 0x5U);
            m.logic(0x5U, 16U);
            break;
        }
        case 0xf7ecU: { // 4240 clr.w d0
            next = 0xf7eeU;
            const auto value = r.data[0];
            (void)value;
            m.dw(0U, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf7eeU: { // 3b400046 move.w d0, $46(a5)
            next = 0xf7f2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf7f2U: { // 062d0020005a addi.b #$20, $5a(a5)
            next = 0xf7f8U;
            const auto source_value = 0x20U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0xf7f8U: { // 42ad001e clr.l $1e(a5)
            next = 0xf7fcU;
            const auto destination_address = r.address[5] + 0x1eU;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0xf7fcU: { // 2b7cfffe80000026 move.l #$fffe8000, $26(a5)
            next = 0xf804U;
            m.lng(r.address[5] + 0x26U, 0xfffe8000U);
            m.logic(0xfffe8000U, 32U);
            break;
        }
        case 0xf804U: { // 4eb90000fed6 jsr $fed6.l
            next = 0xf80aU;
            const auto result = m.call(c, 201U, 0xf804U, 0xfed6U, 0xf80aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf80aU: { // 4eb9000102aa jsr $102aa.l
            next = 0xf810U;
            const auto result = m.call(c, 204U, 0xf80aU, 0x102aaU, 0xf810U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf810U: { // 4eb90000fc68 jsr $fc68.l
            next = 0xf816U;
            const auto result = m.call(c, 193U, 0xf810U, 0xfc68U, 0xf816U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf816U: { // 61000496 bsr.w $fcae
            next = 0xf81aU;
            const auto result = m.call(c, 195U, 0xf816U, 0xfcaeU, 0xf81aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf81aU: { // 4e75 rts 
            next = 0xf81cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf81aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf81cU: { // 4a2d003f tst.b $3f(a5)
            next = 0xf820U;
            m.logic(m.byte(r.address[5] + 0x3fU), 8U);
            break;
        }
        case 0xf820U: { // 6634 bne.b $f856
            next = 0xf822U;
            if ((r.status & 4U) == 0U) { next = 0xf856U; transfer_kind = 1U; }
            break;
        }
        case 0xf822U: { // 2f0c move.l a4, -(a7)
            next = 0xf824U;
            const auto value = r.address[4];
            r.address[7] -= 4U;
            const auto destination_address = r.address[7];
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf824U: { // 303c004a move.w #$4a, d0
            next = 0xf828U;
            m.dw(0U, 0x4aU);
            m.logic(r.data[0], 16U);
            break;
        }
        case 0xf828U: { // 4eb900016ff8 jsr $16ff8.l
            next = 0xf82eU;
            const auto result = m.call(c, 308U, 0xf828U, 0x16ff8U, 0xf82eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf82eU: { // 285f movea.l (a7)+, a4
            next = 0xf830U;
            const auto source_address = r.address[7];
            const auto value = m.lng(source_address);
            r.address[7] += 4U;
            r.address[4] = value;
            break;
        }
        case 0xf830U: { // 296d00540092 move.l $54(a5), $92(a4)
            next = 0xf836U;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[4] + 0x92U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf836U: { // 422d003c clr.b $3c(a5)
            next = 0xf83aU;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0xf83aU: { // 4a6d005c tst.w $5c(a5)
            next = 0xf83eU;
            m.logic(m.word(r.address[5] + 0x5cU), 16U);
            break;
        }
        case 0xf83eU: { // 6716 beq.b $f856
            next = 0xf840U;
            if ((r.status & 4U) != 0U) { next = 0xf856U; transfer_kind = 1U; }
            break;
        }
        case 0xf840U: { // 3c6d005c movea.w $5c(a5), a6
            next = 0xf844U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0xf844U: { // 3d7c00020044 move.w #$2, $44(a6)
            next = 0xf84aU;
            m.word(r.address[6] + 0x44U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf84aU: { // 2d6d00120062 move.l $12(a5), $62(a6)
            next = 0xf850U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[6] + 0x62U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf850U: { // 2d6d001a0066 move.l $1a(a5), $66(a6)
            next = 0xf856U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[6] + 0x66U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0xf856U: { // 206d0054 movea.l $54(a5), a0
            next = 0xf85aU;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0xf85aU: { // 532d003c subq.b #$1, $3c(a5)
            next = 0xf85eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0xf85eU: { // 6e32 bgt.b $f892
            next = 0xf860U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0xf892U; transfer_kind = 1U; }
            break;
        }
        case 0xf860U: { // 522d003f addq.b #$1, $3f(a5)
            next = 0xf864U;
            const auto address = r.address[5] + 0x3fU;
            const auto value = m.add(m.byte(address), 0x1U, 8U);
            m.byte(address, value);
            break;
        }
        case 0xf864U: { // 7000 moveq #$0, d0
            next = 0xf866U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0xf866U: { // 102d003f move.b $3f(a5), d0
            next = 0xf86aU;
            const auto source_address = r.address[5] + 0x3fU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf86aU: { // 0c400011 cmpi.w #$11, d0
            next = 0xf86eU;
            const auto source_value = 0x11U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0xf86eU: { // 6a28 bpl.b $f898
            next = 0xf870U;
            if ((r.status & 8U) == 0U) { next = 0xf898U; transfer_kind = 1U; }
            break;
        }
        case 0xf870U: { // 5340 subq.w #$1, d0
            next = 0xf872U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf872U: { // d040 add.w d0, d0
            next = 0xf874U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf874U: { // 3200 move.w d0, d1
            next = 0xf876U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf876U: { // d040 add.w d0, d0
            next = 0xf878U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf878U: { // d041 add.w d1, d0
            next = 0xf87aU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0xf87aU: { // 43fa0eb4 lea.l $10730(pc), a1
            next = 0xf87eU;
            const auto source_address = 0x10730U;
            r.address[1] = source_address;
            break;
        }
        case 0xf87eU: { // d2c0 adda.w d0, a1
            next = 0xf880U;
            const auto source_value = r.data[0];
            r.address[1] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0xf880U: { // 1b59005b move.b (a1)+, $5b(a5)
            next = 0xf884U;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            r.address[1] += 1U;
            const auto destination_address = r.address[5] + 0x5bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf884U: { // 1b59003c move.b (a1)+, $3c(a5)
            next = 0xf888U;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            r.address[1] += 1U;
            const auto destination_address = r.address[5] + 0x3cU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf888U: { // 206c0092 movea.l $92(a4), a0
            next = 0xf88cU;
            const auto source_address = r.address[4] + 0x92U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0xf88cU: { // 4a91 tst.l (a1)
            next = 0xf88eU;
            const auto destination_address = r.address[1];
            const auto value = m.lng(destination_address);
            m.logic(value, 32U);
            break;
        }
        case 0xf88eU: { // 6702 beq.b $f892
            next = 0xf890U;
            if ((r.status & 4U) != 0U) { next = 0xf892U; transfer_kind = 1U; }
            break;
        }
        case 0xf890U: { // 2051 movea.l (a1), a0
            next = 0xf892U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0xf892U: { // 6100043e bsr.w $fcd2
            next = 0xf896U;
            const auto result = m.call(c, 196U, 0xf892U, 0xfcd2U, 0xf896U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xf896U: { // 4e75 rts 
            next = 0xf898U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf896U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf898U: { // 3b7cffff0052 move.w #$ffff, $52(a5)
            next = 0xf89eU;
            m.word(r.address[5] + 0x52U, 0xffffU);
            m.logic(0xffffU, 16U);
            break;
        }
        case 0xf89eU: { // 426d0044 clr.w $44(a5)
            next = 0xf8a2U;
            const auto destination_address = r.address[5] + 0x44U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf8a2U: { // 4df83200 lea.l $3200.w, a6
            next = 0xf8a6U;
            r.address[6] = 0x3200U;
            break;
        }
        case 0xf8a6U: { // 4aae0042 tst.l $42(a6)
            next = 0xf8aaU;
            const auto destination_address = r.address[6] + 0x42U;
            const auto value = m.lng(destination_address);
            m.logic(value, 32U);
            break;
        }
        case 0xf8aaU: { // 6648 bne.b $f8f4
            next = 0xf8acU;
            if ((r.status & 4U) == 0U) { next = 0xf8f4U; transfer_kind = 1U; }
            break;
        }
        case 0xf8acU: { // 2d7c0001375a0002 move.l #$1375a, $2(a6)
            next = 0xf8b4U;
            m.lng(r.address[6] + 0x2U, 0x1375aU);
            m.logic(0x1375aU, 32U);
            break;
        }
        case 0xf8b4U: { // 3d6d00120012 move.w $12(a5), $12(a6)
            next = 0xf8baU;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf8baU: { // 3d7c00080016 move.w #$8, $16(a6)
            next = 0xf8c0U;
            m.word(r.address[6] + 0x16U, 0x8U);
            m.logic(0x8U, 16U);
            break;
        }
        case 0xf8c0U: { // 3d6d001a001a move.w $1a(a5), $1a(a6)
            next = 0xf8c6U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0xf8c6U: { // 1d6d004b004b move.b $4b(a5), $4b(a6)
            next = 0xf8ccU;
            const auto source_address = r.address[5] + 0x4bU;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[6] + 0x4bU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf8ccU: { // 1d7c0001003d move.b #$1, $3d(a6)
            next = 0xf8d2U;
            m.byte(r.address[6] + 0x3dU, 0x1U);
            m.logic(0x1U, 8U);
            break;
        }
        case 0xf8d2U: { // 3d7c00020058 move.w #$2, $58(a6)
            next = 0xf8d8U;
            m.word(r.address[6] + 0x58U, 0x2U);
            m.logic(0x2U, 16U);
            break;
        }
        case 0xf8d8U: { // 3d7c00090052 move.w #$9, $52(a6)
            next = 0xf8deU;
            m.word(r.address[6] + 0x52U, 0x9U);
            m.logic(0x9U, 16U);
            break;
        }
        case 0xf8deU: { // 1cbc0080 move.b #$80, (a6)
            next = 0xf8e2U;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0xf8e2U: { // 1d7c00800080 move.b #$80, $80(a6)
            next = 0xf8e8U;
            m.byte(r.address[6] + 0x80U, 0x80U);
            m.logic(0x80U, 8U);
            break;
        }
        case 0xf8e8U: { // 1d7c00800100 move.b #$80, $100(a6)
            next = 0xf8eeU;
            m.byte(r.address[6] + 0x100U, 0x80U);
            m.logic(0x80U, 8U);
            break;
        }
        case 0xf8eeU: { // 1d7c00800180 move.b #$80, $180(a6)
            next = 0xf8f4U;
            m.byte(r.address[6] + 0x180U, 0x80U);
            m.logic(0x80U, 8U);
            break;
        }
        case 0xf8f4U: { // 4e75 rts 
            next = 0xf8f6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf8f4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0xf8f6U: { // 4af80d1a tas.b $d1a.w
            next = 0xf8faU;
            const auto destination_address = 0xd1aU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0xf8faU: { // 426d0044 clr.w $44(a5)
            next = 0xf8feU;
            const auto destination_address = r.address[5] + 0x44U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0xf8feU: { // 4e75 rts 
            next = 0xf900U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xf8feU, r.program_counter)) return *event;
            return result;
            break;
        }
        // Retained F178 disassembly: lifecycle modes 8, 9 and 10.
        case 0xf900U: case 0xf93cU: case 0xf9a4U: case 0xf9b4U: case 0xf9c2U: {
            const auto result = m.ret();
            if (auto event = m.interrupt(c, pc, r.program_counter)) return *event;
            return result;
        }
        case 0xf902U: // 0c7800020c16 cmpi.w #2,$c16.w
            next = 0xf908U; (void)m.sub(m.word(0xc16U), 2U, 16U, true); break;
        case 0xf908U: // 660000aa bne.w $f9b4
            next = (r.status & 4U) ? 0xf90cU : 0xf9b4U; transfer_kind = 1U; break;
        case 0xf90cU: // 4a380d0c tst.b $d0c.w
            next = 0xf910U; m.logic(m.byte(0xd0cU), 8U); break;
        case 0xf910U: // 672c beq.b $f93e
            next = (r.status & 4U) ? 0xf93eU : 0xf912U; transfer_kind = 1U; break;
        case 0xf912U: case 0xf920U: case 0xf92eU: {
            const auto offset = pc == 0xf912U ? 0x86U : pc == 0xf920U ? 0xa4U : 0xa8U;
            next = pc + 4U; r.data[2] = m.lng(r.address[4] + offset); m.logic(r.data[2], 32U); break;
        }
        case 0xf916U: case 0xf924U: case 0xf932U: case 0xf988U:
            next = pc + 4U;
            r.address[1] = pc == 0xf916U ? 0x107d8U : pc == 0xf924U ? 0x107dcU : 0x107e0U;
            break;
        case 0xf91aU: case 0xf928U: case 0xf936U: case 0xf98cU: {
            const auto result = m.call(c, 296U, pc, 0x1610eU, pc + 6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; break;
        }
        case 0xf93eU: // 222c00a8 move.l $a8(a4),d1
            next = 0xf942U; r.data[1] = m.lng(r.address[4] + 0xa8U); m.logic(r.data[1], 32U); break;
        case 0xf942U: // 6762 beq.b $f9a6
            next = (r.status & 4U) ? 0xf9a6U : 0xf944U; transfer_kind = 1U; break;
        case 0xf944U: case 0xf952U: case 0xf960U: case 0xf96aU:
            next = pc + 6U;
            r.data[0] = pc == 0xf944U ? 0x50000U : pc == 0xf952U ? 0x10000U
                : pc == 0xf960U ? 0x3000U : 0x200U;
            m.logic(r.data[0], 32U); break;
        case 0xf94aU: case 0xf958U:
            next = pc + 6U;
            (void)m.sub(r.data[1], pc == 0xf94aU ? 0x2000000U : 0x100000U, 32U, true); break;
        case 0xf966U: case 0xf970U:
            next = pc + 2U; (void)m.sub(r.data[1], r.data[0], 32U, true); break;
        case 0xf950U: case 0xf95eU: case 0xf968U: case 0xf972U:
            next = (r.status & 8U) ? pc + 2U : 0xf976U; transfer_kind = 1U; break;
        case 0xf974U:
            next = 0xf976U; r.data[0] = r.data[1]; m.logic(r.data[0], 32U); break;
        case 0xf976U: case 0xf97cU: {
            const auto result = m.call(c, pc == 0xf976U ? 286U : 285U, pc,
                pc == 0xf976U ? 0x15ebeU : 0x15ea0U, pc + 6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; break;
        }
        case 0xf982U:
            next = 0xf986U; m.lng(r.address[4] + 0xa8U, r.data[0]); m.logic(r.data[0], 32U); break;
        case 0xf986U:
            next = 0xf988U; r.data[2] = r.data[0]; m.logic(r.data[2], 32U); break;
        case 0xf992U: {
            const auto result = m.call(c, 187U, pc, 0xfb44U, 0xf996U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; break;
        }
        case 0xf996U:
            next = 0xf998U; r.address[7] -= 4U; m.lng(r.address[7], r.address[4]);
            m.logic(r.address[4], 32U); break;
        case 0xf998U:
            next = 0xf99cU; m.dw(0U, 0x40U); m.logic(0x40U, 16U); break;
        case 0xf99cU: {
            const auto result = m.call(c, 308U, pc, 0x16ff8U, 0xf9a2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; break;
        }
        case 0xf9a2U:
            next = 0xf9a4U; r.address[4] = m.lng(r.address[7]); r.address[7] += 4U; break;
        case 0xf9a6U:
            next = 0xf9acU; m.word(r.address[5] + 0x44U, 10U); m.logic(10U, 16U); break;
        case 0xf9acU:
            next = 0xf9b0U; m.db(0U, m.byte(r.address[5] + 0x6dU)); m.logic(r.data[0], 8U); break;
        case 0xf9b0U: {
            next = 0xf9b4U;
            const auto old = m.byte(0xc05U);
            const auto bit = static_cast<std::uint8_t>(1U << (r.data[0] & 7U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit) ? 0U : 4U));
            m.byte(0xc05U, old & ~bit); break;
        }
        case 0xf9b6U:
            next = 0xf9bcU; (void)m.sub(m.word(0xc16U), 5U, 16U, true); break;
        case 0xf9bcU:
            next = (r.status & 4U) ? 0xf9beU : 0xf9c2U; transfer_kind = 1U; break;
        case 0xf9beU: { // 6100fbb2 bsr.w $f572; source-defined interior entry.
            const auto result = m.call(c, 178U, pc, 0xf572U, 0xf9c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter; break;
        }
        case 0xfe18U: { // 61000022 bsr.w $fe3c
            next = 0xfe1cU;
            const auto result = m.call(c, 199U, 0xfe18U, 0xfe3cU, 0xfe1cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xfe1cU: { // 61000036 bsr.w $fe54
            next = 0xfe20U;
            const auto result = m.call(c, 200U, 0xfe1cU, 0xfe54U, 0xfe20U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xfe20U: { // 610000b4 bsr.w $fed6
            next = 0xfe24U;
            const auto result = m.call(c, 201U, 0xfe20U, 0xfed6U, 0xfe24U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xfe24U: { // 4eb900010a24 jsr $10a24.l
            next = 0xfe2aU;
            const auto result = m.call(c, 208U, 0xfe24U, 0x10a24U, 0xfe2aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xfe2aU: { // 6100047e bsr.w $102aa
            next = 0xfe2eU;
            const auto result = m.call(c, 204U, 0xfe2aU, 0x102aaU, 0xfe2eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xfe2eU: { // 61000544 bsr.w $10374
            next = 0xfe32U;
            const auto result = m.call(c, 205U, 0xfe2eU, 0x10374U, 0xfe32U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xfe32U: { // 610005b0 bsr.w $103e4
            next = 0xfe36U;
            const auto result = m.call(c, 206U, 0xfe32U, 0x103e4U, 0xfe36U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xfe36U: { // 6100fe76 bsr.w $fcae
            next = 0xfe3aU;
            const auto result = m.call(c, 195U, 0xfe36U, 0xfcaeU, 0xfe3aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0xfe3aU: { // 4e75 rts 
            next = 0xfe3cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0xfe3aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0xf3c0U:
        case 0xf3c4U:
        case 0xf3c6U:
        case 0xf3caU:
        case 0xf3ccU:
        case 0xf3d0U:
        case 0xf3d4U:
        case 0xf3d6U:
        case 0xf3daU:
        case 0xf3deU:
        case 0xf3e2U:
        case 0xf3e6U:
        case 0xf3eaU:
        case 0xf3eeU:
        case 0xf3f2U:
        case 0xf3f6U:
        case 0xf3faU: case 0xf3feU: case 0xf402U:
        case 0xf51aU:
        case 0xf51eU:
        case 0xf522U:
        case 0xf526U:
        case 0xf528U:
        case 0xf52cU:
        case 0xf52eU:
        case 0xf532U:
        case 0xf538U:
        case 0xf53cU:
        case 0xf53eU:
        case 0xf542U:
        case 0xf548U:
        case 0xf54cU:
        case 0xf552U:
        case 0xf556U:
        case 0xf558U:
        case 0xf55cU:
        case 0xf55eU: case 0xf564U: case 0xf56aU: case 0xf570U:
        case 0xf572U:
        case 0xf578U:
        case 0xf57eU:
        case 0xf584U:
        case 0xf588U:
        case 0xf58cU:
        case 0xf590U:
        case 0xf594U:
        case 0xf598U:
        case 0xf59cU:
        case 0xf5a0U:
        case 0xf5a4U:
        case 0xf5a8U:
        case 0xf5aaU:
        case 0xf5acU:
        case 0xf5aeU:
        case 0xf5b0U:
        case 0xf5b2U:
        case 0xf5b4U:
        case 0xf5b6U:
        case 0xf5baU:
        case 0xf5beU:
        case 0xf5c0U:
        case 0xf5c2U:
        case 0xf5c4U:
        case 0xf5c6U:
        case 0xf5c8U:
        case 0xf5ccU:
        case 0xf5d0U:
        case 0xf5d2U:
        case 0xf5d4U:
        case 0xf5d6U:
        case 0xf5d8U:
        case 0xf5daU:
        case 0xf5dcU:
        case 0xf5deU:
        case 0xf5e4U:
        case 0xf5e8U:
        case 0xf5eaU:
        case 0xf5ecU:
        case 0xf5f0U:
        case 0xf5f4U:
        case 0xf5f6U:
        case 0xf5fcU:
        case 0xf600U:
        case 0xf604U:
        case 0xf608U:
        case 0xf60cU:
        case 0xf610U:
        case 0xf614U:
        case 0xf618U:
        case 0xf61cU:
        case 0xf620U:
        case 0xf624U:
        case 0xf626U:
        case 0xf62cU:
        case 0xf630U:
        case 0xf634U:
        case 0xf636U:
        case 0xf638U:
        case 0xf63cU:
        case 0xf63eU:
        case 0xf642U:
        case 0xf646U:
        case 0xf648U:
        case 0xf64eU:
        case 0xf652U:
        case 0xf656U:
        case 0xf658U:
        case 0xf65cU:
        case 0xf662U:
        case 0xf664U:
        case 0xf668U:
        case 0xf66cU:
        case 0xf66eU:
        case 0xf670U:
        case 0xf676U:
        case 0xf678U:
        case 0xf67cU:
        case 0xf67eU:
        case 0xf682U:
        case 0xf684U: case 0xf68aU: case 0xf68eU:
        case 0xf690U:
        case 0xf696U:
        case 0xf69aU:
        case 0xf69eU:
        case 0xf6a4U:
        case 0xf6a6U:
        case 0xf6aaU:
        case 0xf6b0U:
        case 0xf6b2U:
        case 0xf6b6U:
        case 0xf6b8U:
        case 0xf6bcU:
        case 0xf6beU:
        case 0xf6c2U:
        case 0xf6c4U:
        case 0xf6c8U:
        case 0xf6ccU:
        case 0xf6ceU:
        case 0xf6d2U:
        case 0xf6d6U:
        case 0xf6daU:
        case 0xf6deU:
        case 0xf6e0U:
        case 0xf6e2U:
        case 0xf6e6U:
        case 0xf6e8U:
        case 0xf6eaU:
        case 0xf6eeU:
        case 0xf6f0U:
        case 0xf6f4U:
        case 0xf6faU:
        case 0xf6fcU:
        case 0xf702U:
        case 0xf706U:
        case 0xf70aU:
        case 0xf70cU:
        case 0xf710U:
        case 0xf712U:
        case 0xf718U:
        case 0xf71cU:
        case 0xf71eU:
        case 0xf720U:
        case 0xf724U:
        case 0xf728U:
        case 0xf72eU:
        case 0xf732U:
        case 0xf734U:
        case 0xf738U:
        case 0xf73aU:
        case 0xf740U:
        case 0xf742U:
        case 0xf746U:
        case 0xf74cU:
        case 0xf752U:
        case 0xf758U:
        case 0xf75cU:
        case 0xf762U:
        case 0xf768U:
        case 0xf76eU:
        case 0xf774U:
        case 0xf77aU:
        case 0xf780U:
        case 0xf786U:
        case 0xf78cU:
        case 0xf792U:
        case 0xf796U:
        case 0xf79aU:
        case 0xf79cU:
        case 0xf7a0U:
        case 0xf7a4U:
        case 0xf7aaU:
        case 0xf7aeU:
        case 0xf7b4U:
        case 0xf7b6U:
        case 0xf7bcU:
        case 0xf7beU:
        case 0xf7c4U:
        case 0xf7c8U:
        case 0xf7ccU:
        case 0xf7d0U:
        case 0xf7d4U:
        case 0xf7d8U:
        case 0xf7dcU:
        case 0xf7e0U:
        case 0xf7e4U:
        case 0xf7e6U:
        case 0xf7ecU:
        case 0xf7eeU:
        case 0xf7f2U:
        case 0xf7f8U:
        case 0xf7fcU:
        case 0xf804U:
        case 0xf80aU:
        case 0xf810U:
        case 0xf816U:
        case 0xf81aU:
        case 0xf81cU:
        case 0xf820U:
        case 0xf822U:
        case 0xf824U:
        case 0xf828U:
        case 0xf82eU:
        case 0xf830U:
        case 0xf836U:
        case 0xf83aU:
        case 0xf83eU:
        case 0xf840U:
        case 0xf844U:
        case 0xf84aU:
        case 0xf850U:
        case 0xf856U:
        case 0xf85aU:
        case 0xf85eU:
        case 0xf860U:
        case 0xf864U:
        case 0xf866U:
        case 0xf86aU:
        case 0xf86eU:
        case 0xf870U:
        case 0xf872U:
        case 0xf874U:
        case 0xf876U:
        case 0xf878U:
        case 0xf87aU:
        case 0xf87eU:
        case 0xf880U:
        case 0xf884U:
        case 0xf888U:
        case 0xf88cU:
        case 0xf88eU:
        case 0xf890U:
        case 0xf892U:
        case 0xf896U:
        case 0xf898U:
        case 0xf89eU:
        case 0xf8a2U:
        case 0xf8a6U:
        case 0xf8aaU:
        case 0xf8acU:
        case 0xf8b4U:
        case 0xf8baU:
        case 0xf8c0U:
        case 0xf8c6U:
        case 0xf8ccU:
        case 0xf8d2U:
        case 0xf8d8U:
        case 0xf8deU:
        case 0xf8e2U:
        case 0xf8e8U:
        case 0xf8eeU:
        case 0xf8f4U:
        case 0xf8f6U:
        case 0xf8faU:
        case 0xf8feU:
        case 0xfe18U:
        case 0xf900U: case 0xf902U: case 0xf908U: case 0xf90cU: case 0xf910U:
        case 0xf912U: case 0xf916U: case 0xf91aU: case 0xf920U: case 0xf924U:
        case 0xf928U: case 0xf92eU: case 0xf932U: case 0xf936U: case 0xf93cU:
        case 0xf93eU: case 0xf942U: case 0xf944U: case 0xf94aU: case 0xf950U:
        case 0xf952U: case 0xf958U: case 0xf95eU: case 0xf960U: case 0xf966U:
        case 0xf968U: case 0xf96aU: case 0xf970U: case 0xf972U: case 0xf974U:
        case 0xf976U: case 0xf97cU: case 0xf982U: case 0xf986U: case 0xf988U:
        case 0xf98cU: case 0xf992U: case 0xf996U: case 0xf998U: case 0xf99cU:
        case 0xf9a2U: case 0xf9a4U: case 0xf9a6U: case 0xf9acU: case 0xf9b0U:
        case 0xf9b4U: case 0xf9b6U: case 0xf9bcU: case 0xf9beU: case 0xf9c2U:
        case 0xfe1cU:
        case 0xfe20U:
        case 0xfe24U:
        case 0xfe2aU:
        case 0xfe2eU:
        case 0xfe32U:
        case 0xfe36U:
        case 0xfe3aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
}
