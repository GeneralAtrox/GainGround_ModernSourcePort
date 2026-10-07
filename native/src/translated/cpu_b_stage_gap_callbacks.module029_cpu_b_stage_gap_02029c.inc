
FunctionResult cpu_b_stage_gap_02029c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2029cU: { // 4a2d003f tst.b $3f(a5)
            next = 0x202a0U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x202a0U: { // 66000082 bne.w $20324
            next = 0x202a4U;
            if ((r.status & 4U) == 0U) { next = 0x20324U; transfer_kind = 1U; }
            break;
        }
        case 0x202a4U: { // 6100047e bsr.w $20724
            next = 0x202a8U;
            const auto result = m.call(c, 387U, 0x202a4U, 0x20724U, 0x202a8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x202a8U: { // 657a bcs.b $20324
            next = 0x202aaU;
            if ((r.status & 1U) != 0U) { next = 0x20324U; transfer_kind = 1U; }
            break;
        }
        case 0x202aaU: { // 61000360 bsr.w $2060c
            next = 0x202aeU;
            const auto result = m.call(c, 385U, 0x202aaU, 0x2060cU, 0x202aeU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x202aeU: { // 6574 bcs.b $20324
            next = 0x202b0U;
            if ((r.status & 1U) != 0U) { next = 0x20324U; transfer_kind = 1U; }
            break;
        }
        case 0x202b0U: { // 202d001e move.l $1e(a5), d0
            next = 0x202b4U;
            const auto v1 = m.lng((r.address[5] + 0x1eU));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x202b4U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x202b8U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x12U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x12U), v1);
            break;
        }
        case 0x202b8U: { // 202d0026 move.l $26(a5), d0
            next = 0x202bcU;
            const auto v1 = m.lng((r.address[5] + 0x26U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x202bcU: { // d1ad001a add.l d0, $1a(a5)
            next = 0x202c0U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x1aU)), r.data[0], 32U);
            m.lng((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x202c0U: { // 202d0022 move.l $22(a5), d0
            next = 0x202c4U;
            const auto v1 = m.lng((r.address[5] + 0x22U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x202c4U: { // 90ad0050 sub.l $50(a5), d0
            next = 0x202c8U;
            const auto v1 = m.sub(r.data[0], m.lng((r.address[5] + 0x50U)), 32U);
            r.data[0] = v1;
            break;
        }
        case 0x202c8U: { // 2b400022 move.l d0, $22(a5)
            next = 0x202ccU;
            const auto v1 = r.data[0];
            m.lng((r.address[5] + 0x22U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x202ccU: { // d1ad0016 add.l d0, $16(a5)
            next = 0x202d0U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x16U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x16U), v1);
            break;
        }
        case 0x202d0U: { // 302d0016 move.w $16(a5), d0
            next = 0x202d4U;
            const auto v1 = m.word((r.address[5] + 0x16U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x202d4U: { // 04400010 subi.w #$10, d0
            next = 0x202d8U;
            const auto v1 = m.sub((r.data[0] & 0xffffU), 0x10U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x202d8U: { // 3200 move.w d0, d1
            next = 0x202daU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.dw(1, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x202daU: { // 0241ffe0 andi.w #$ffe0, d1
            next = 0x202deU;
            const auto v1 = ((r.data[1] & 0xffffU) & 0xffe0U) & 0xffffU;
            m.dw(1, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x202deU: { // 6602 bne.b $202e2
            next = 0x202e0U;
            if ((r.status & 4U) == 0U) { next = 0x202e2U; transfer_kind = 1U; }
            break;
        }
        case 0x202e0U: { // e240 asr.w #$1, d0
            next = 0x202e2U;
            m.shift_word(0, 1U, false, true);
            break;
        }
        case 0x202e2U: { // e240 asr.w #$1, d0
            next = 0x202e4U;
            m.shift_word(0, 1U, false, true);
            break;
        }
        case 0x202e4U: { // 0640003f addi.w #$3f, d0
            next = 0x202e8U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x3fU, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x202e8U: { // 1b400010 move.b d0, $10(a5)
            next = 0x202ecU;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x10U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x202ecU: { // 1b400011 move.b d0, $11(a5)
            next = 0x202f0U;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x11U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x202f0U: { // 08ed00020001 bset.b #$2, $1(a5)
            next = 0x202f6U;
            const auto v1 = m.byte((r.address[5] + 0x1U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x1U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x202f6U: { // 302d0056 move.w $56(a5), d0
            next = 0x202faU;
            const auto v1 = m.word((r.address[5] + 0x56U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x202faU: { // 5240 addq.w #$1, d0
            next = 0x202fcU;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x1U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x202fcU: { // 0c400003 cmpi.w #$3, d0
            next = 0x20300U;
            (void)m.sub((r.data[0] & 0xffffU), 0x3U, 16U, true);
            break;
        }
        case 0x20300U: { // 6d02 blt.b $20304
            next = 0x20302U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x20304U; transfer_kind = 1U; }
            break;
        }
        case 0x20302U: { // 7000 moveq #$0, d0
            next = 0x20304U;
            r.data[0] = 0x0U; m.logic(r.data[0], 32U);
            break;
        }
        case 0x20304U: { // 3b400056 move.w d0, $56(a5)
            next = 0x20308U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x56U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20308U: { // d06d005a add.w $5a(a5), d0
            next = 0x2030cU;
            const auto v1 = m.add((r.data[0] & 0xffffU), m.word((r.address[5] + 0x5aU)), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x2030cU: { // 3b400008 move.w d0, $8(a5)
            next = 0x20310U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20310U: { // 4eb900015d24 jsr $15d24.l
            next = 0x20316U;
            const auto result = m.indirect_call(c, 0x20310U, 0x15d24U, 0x20316U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20316U: { // 4eb9000207c2 jsr $207c2.l
            next = 0x2031cU;
            const auto result = m.indirect_call(c, 0x20316U, 0x207c2U, 0x2031cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2031cU: { // 4eb900015df2 jsr $15df2.l
            next = 0x20322U;
            const auto result = m.indirect_call(c, 0x2031cU, 0x15df2U, 0x20322U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20322U: { // 4e75 rts 
            next = 0x20324U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20322U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20324U: { // 2b7c0001ef120002 move.l #$1ef12, $2(a5)
            next = 0x2032cU;
            const auto v1 = 0x1ef12U;
            m.lng((r.address[5] + 0x2U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x2032cU: { // 1b7c0009000b move.b #$9, $b(a5)
            next = 0x20332U;
            const auto v1 = 0x9U;
            m.byte((r.address[5] + 0xbU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x20332U: { // 3b7c00000042 move.w #$0, $42(a5)
            next = 0x20338U;
            const auto v1 = 0x0U;
            m.word((r.address[5] + 0x42U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20338U: { // 3b7c00000044 move.w #$0, $44(a5)
            next = 0x2033eU;
            const auto v1 = 0x0U;
            m.word((r.address[5] + 0x44U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2033eU: { // 1b7c0004003c move.b #$4, $3c(a5)
            next = 0x20344U;
            const auto v1 = 0x4U;
            m.byte((r.address[5] + 0x3cU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x20344U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x2034aU;
            const auto v1 = 0x0U;
            m.word((r.address[5] + 0x54U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2034aU: { // 4e75 rts 
            next = 0x2034cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2034aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2029cU:
        case 0x202a0U:
        case 0x202a4U:
        case 0x202a8U:
        case 0x202aaU:
        case 0x202aeU:
        case 0x202b0U:
        case 0x202b4U:
        case 0x202b8U:
        case 0x202bcU:
        case 0x202c0U:
        case 0x202c4U:
        case 0x202c8U:
        case 0x202ccU:
        case 0x202d0U:
        case 0x202d4U:
        case 0x202d8U:
        case 0x202daU:
        case 0x202deU:
        case 0x202e0U:
        case 0x202e2U:
        case 0x202e4U:
        case 0x202e8U:
        case 0x202ecU:
        case 0x202f0U:
        case 0x202f6U:
        case 0x202faU:
        case 0x202fcU:
        case 0x20300U:
        case 0x20302U:
        case 0x20304U:
        case 0x20308U:
        case 0x2030cU:
        case 0x20310U:
        case 0x20316U:
        case 0x2031cU:
        case 0x20322U:
        case 0x20324U:
        case 0x2032cU:
        case 0x20332U:
        case 0x20338U:
        case 0x2033eU:
        case 0x20344U:
        case 0x2034aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
