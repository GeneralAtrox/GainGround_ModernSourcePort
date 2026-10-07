
FunctionResult cpu_b_stage_gap_01ff88(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1ff88U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1ff8cU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x1ff8cU: { // 660000b2 bne.w $20040
            next = 0x1ff90U;
            if ((r.status & 4U) == 0U) { next = 0x20040U; transfer_kind = 1U; }
            break;
        }
        case 0x1ff90U: { // 61000792 bsr.w $20724
            next = 0x1ff94U;
            const auto result = m.call(c, 387U, 0x1ff90U, 0x20724U, 0x1ff94U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ff94U: { // 650000aa bcs.w $20040
            next = 0x1ff98U;
            if ((r.status & 1U) != 0U) { next = 0x20040U; transfer_kind = 1U; }
            break;
        }
        case 0x1ff98U: { // 61000672 bsr.w $2060c
            next = 0x1ff9cU;
            const auto result = m.call(c, 385U, 0x1ff98U, 0x2060cU, 0x1ff9cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ff9cU: { // 650000a2 bcs.w $20040
            next = 0x1ffa0U;
            if ((r.status & 1U) != 0U) { next = 0x20040U; transfer_kind = 1U; }
            break;
        }
        case 0x1ffa0U: { // 202d001e move.l $1e(a5), d0
            next = 0x1ffa4U;
            const auto v1 = m.lng((r.address[5] + 0x1eU));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x1ffa4U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x1ffa8U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x12U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x12U), v1);
            break;
        }
        case 0x1ffa8U: { // 202d0026 move.l $26(a5), d0
            next = 0x1ffacU;
            const auto v1 = m.lng((r.address[5] + 0x26U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x1ffacU: { // d1ad001a add.l d0, $1a(a5)
            next = 0x1ffb0U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x1aU)), r.data[0], 32U);
            m.lng((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x1ffb0U: { // 202d0022 move.l $22(a5), d0
            next = 0x1ffb4U;
            const auto v1 = m.lng((r.address[5] + 0x22U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x1ffb4U: { // 90ad0050 sub.l $50(a5), d0
            next = 0x1ffb8U;
            const auto v1 = m.sub(r.data[0], m.lng((r.address[5] + 0x50U)), 32U);
            r.data[0] = v1;
            break;
        }
        case 0x1ffb8U: { // 2b400022 move.l d0, $22(a5)
            next = 0x1ffbcU;
            const auto v1 = r.data[0];
            m.lng((r.address[5] + 0x22U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x1ffbcU: { // d1ad0016 add.l d0, $16(a5)
            next = 0x1ffc0U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x16U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x16U), v1);
            break;
        }
        case 0x1ffc0U: { // 302d0016 move.w $16(a5), d0
            next = 0x1ffc4U;
            const auto v1 = m.word((r.address[5] + 0x16U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ffc4U: { // 04400010 subi.w #$10, d0
            next = 0x1ffc8U;
            const auto v1 = m.sub((r.data[0] & 0xffffU), 0x10U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1ffc8U: { // 3200 move.w d0, d1
            next = 0x1ffcaU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.dw(1, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ffcaU: { // 0241ffe0 andi.w #$ffe0, d1
            next = 0x1ffceU;
            const auto v1 = ((r.data[1] & 0xffffU) & 0xffe0U) & 0xffffU;
            m.dw(1, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ffceU: { // 6602 bne.b $1ffd2
            next = 0x1ffd0U;
            if ((r.status & 4U) == 0U) { next = 0x1ffd2U; transfer_kind = 1U; }
            break;
        }
        case 0x1ffd0U: { // e240 asr.w #$1, d0
            next = 0x1ffd2U;
            m.shift_word(0, 1U, false, true);
            break;
        }
        case 0x1ffd2U: { // e240 asr.w #$1, d0
            next = 0x1ffd4U;
            m.shift_word(0, 1U, false, true);
            break;
        }
        case 0x1ffd4U: { // 0640003f addi.w #$3f, d0
            next = 0x1ffd8U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x3fU, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1ffd8U: { // 1b400010 move.b d0, $10(a5)
            next = 0x1ffdcU;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x10U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1ffdcU: { // 1b400011 move.b d0, $11(a5)
            next = 0x1ffe0U;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x11U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1ffe0U: { // 302d005c move.w $5c(a5), d0
            next = 0x1ffe4U;
            const auto v1 = m.word((r.address[5] + 0x5cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ffe4U: { // 064000c0 addi.w #$c0, d0
            next = 0x1ffe8U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0xc0U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1ffe8U: { // 3b40005c move.w d0, $5c(a5)
            next = 0x1ffecU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x5cU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ffecU: { // 02400700 andi.w #$700, d0
            next = 0x1fff0U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x700U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1fff0U: { // ec48 lsr.w #$6, d0
            next = 0x1fff2U;
            m.shift_word(0, 6U, false, false);
            break;
        }
        case 0x1fff2U: { // 206d0048 movea.l $48(a5), a0
            next = 0x1fff6U;
            const auto v1 = m.lng((r.address[5] + 0x48U));
            r.address[0] = v1;
            break;
        }
        case 0x1fff6U: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x1fffcU;
            const auto v1 = m.word((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1fffcU: { // 5640 addq.w #$3, d0
            next = 0x1fffeU;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x3U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1fffeU: { // 10300000 move.b (a0, d0.w), d0
            next = 0x20002U;
            const auto v1 = m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x20002U: { // 022d00fc0001 andi.b #$fc, $1(a5)
            next = 0x20008U;
            const auto v1 = (m.byte((r.address[5] + 0x1U)) & 0xfcU) & 0xffU;
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x20008U: { // 812d0001 or.b d0, $1(a5)
            next = 0x2000cU;
            const auto v1 = (m.byte((r.address[5] + 0x1U)) | (r.data[0] & 0xffU)) & 0xffU;
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x2000cU: { // 302d0056 move.w $56(a5), d0
            next = 0x20010U;
            const auto v1 = m.word((r.address[5] + 0x56U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20010U: { // 5240 addq.w #$1, d0
            next = 0x20012U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x1U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x20012U: { // 0c400003 cmpi.w #$3, d0
            next = 0x20016U;
            (void)m.sub((r.data[0] & 0xffffU), 0x3U, 16U, true);
            break;
        }
        case 0x20016U: { // 6d02 blt.b $2001a
            next = 0x20018U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x2001aU; transfer_kind = 1U; }
            break;
        }
        case 0x20018U: { // 7000 moveq #$0, d0
            next = 0x2001aU;
            r.data[0] = 0x0U; m.logic(r.data[0], 32U);
            break;
        }
        case 0x2001aU: { // 3b400056 move.w d0, $56(a5)
            next = 0x2001eU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x56U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2001eU: { // d06d005a add.w $5a(a5), d0
            next = 0x20022U;
            const auto v1 = m.add((r.data[0] & 0xffffU), m.word((r.address[5] + 0x5aU)), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x20022U: { // 3b400008 move.w d0, $8(a5)
            next = 0x20026U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20026U: { // 08ed00020001 bset.b #$2, $1(a5)
            next = 0x2002cU;
            const auto v1 = m.byte((r.address[5] + 0x1U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x1U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x2002cU: { // 4eb900015d24 jsr $15d24.l
            next = 0x20032U;
            const auto result = m.indirect_call(c, 0x2002cU, 0x15d24U, 0x20032U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20032U: { // 4eb9000207c2 jsr $207c2.l
            next = 0x20038U;
            const auto result = m.indirect_call(c, 0x20032U, 0x207c2U, 0x20038U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20038U: { // 4eb900015df2 jsr $15df2.l
            next = 0x2003eU;
            const auto result = m.indirect_call(c, 0x20038U, 0x15df2U, 0x2003eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2003eU: { // 4e75 rts 
            next = 0x20040U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2003eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20040U: { // 6100085a bsr.w $2089c
            next = 0x20044U;
            const auto result = m.call(c, 389U, 0x20040U, 0x2089cU, 0x20044U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20044U: { // 4e75 rts 
            next = 0x20046U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20044U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1ff88U:
        case 0x1ff8cU:
        case 0x1ff90U:
        case 0x1ff94U:
        case 0x1ff98U:
        case 0x1ff9cU:
        case 0x1ffa0U:
        case 0x1ffa4U:
        case 0x1ffa8U:
        case 0x1ffacU:
        case 0x1ffb0U:
        case 0x1ffb4U:
        case 0x1ffb8U:
        case 0x1ffbcU:
        case 0x1ffc0U:
        case 0x1ffc4U:
        case 0x1ffc8U:
        case 0x1ffcaU:
        case 0x1ffceU:
        case 0x1ffd0U:
        case 0x1ffd2U:
        case 0x1ffd4U:
        case 0x1ffd8U:
        case 0x1ffdcU:
        case 0x1ffe0U:
        case 0x1ffe4U:
        case 0x1ffe8U:
        case 0x1ffecU:
        case 0x1fff0U:
        case 0x1fff2U:
        case 0x1fff6U:
        case 0x1fffcU:
        case 0x1fffeU:
        case 0x20002U:
        case 0x20008U:
        case 0x2000cU:
        case 0x20010U:
        case 0x20012U:
        case 0x20016U:
        case 0x20018U:
        case 0x2001aU:
        case 0x2001eU:
        case 0x20022U:
        case 0x20026U:
        case 0x2002cU:
        case 0x20032U:
        case 0x20038U:
        case 0x2003eU:
        case 0x20040U:
        case 0x20044U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
