
FunctionResult cpu_b_stage_gap_020046(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20046U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2004aU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x2004aU: { // 6600008e bne.w $200da
            next = 0x2004eU;
            if ((r.status & 4U) == 0U) { next = 0x200daU; transfer_kind = 1U; }
            break;
        }
        case 0x2004eU: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x20052U;
            const auto v1 = m.sub(m.word((r.address[5] + 0x74U)), 0x1U, 16U);
            m.word((r.address[5] + 0x74U), v1);
            break;
        }
        case 0x20052U: { // 6f000086 ble.w $200da
            next = 0x20056U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x200daU; transfer_kind = 1U; }
            break;
        }
        case 0x20056U: { // 6100064c bsr.w $206a4
            next = 0x2005aU;
            const auto result = m.call(c, 386U, 0x20056U, 0x206a4U, 0x2005aU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2005aU: { // 6500007e bcs.w $200da
            next = 0x2005eU;
            if ((r.status & 1U) != 0U) { next = 0x200daU; transfer_kind = 1U; }
            break;
        }
        case 0x2005eU: { // 610005ac bsr.w $2060c
            next = 0x20062U;
            const auto result = m.call(c, 385U, 0x2005eU, 0x2060cU, 0x20062U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20062U: { // 65000076 bcs.w $200da
            next = 0x20066U;
            if ((r.status & 1U) != 0U) { next = 0x200daU; transfer_kind = 1U; }
            break;
        }
        case 0x20066U: { // 202d001e move.l $1e(a5), d0
            next = 0x2006aU;
            const auto v1 = m.lng((r.address[5] + 0x1eU));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x2006aU: { // d1ad0012 add.l d0, $12(a5)
            next = 0x2006eU;
            const auto v1 = m.add(m.lng((r.address[5] + 0x12U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x12U), v1);
            break;
        }
        case 0x2006eU: { // 202d0026 move.l $26(a5), d0
            next = 0x20072U;
            const auto v1 = m.lng((r.address[5] + 0x26U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x20072U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x20076U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x1aU)), r.data[0], 32U);
            m.lng((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x20076U: { // 302d005c move.w $5c(a5), d0
            next = 0x2007aU;
            const auto v1 = m.word((r.address[5] + 0x5cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2007aU: { // 064000c0 addi.w #$c0, d0
            next = 0x2007eU;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0xc0U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x2007eU: { // 3b40005c move.w d0, $5c(a5)
            next = 0x20082U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x5cU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20082U: { // 02400700 andi.w #$700, d0
            next = 0x20086U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x700U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20086U: { // ec48 lsr.w #$6, d0
            next = 0x20088U;
            m.shift_word(0, 6U, false, false);
            break;
        }
        case 0x20088U: { // 206d0048 movea.l $48(a5), a0
            next = 0x2008cU;
            const auto v1 = m.lng((r.address[5] + 0x48U));
            r.address[0] = v1;
            break;
        }
        case 0x2008cU: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x20092U;
            const auto v1 = m.word((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20092U: { // 5640 addq.w #$3, d0
            next = 0x20094U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x3U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x20094U: { // 10300000 move.b (a0, d0.w), d0
            next = 0x20098U;
            const auto v1 = m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x20098U: { // 022d00fc0001 andi.b #$fc, $1(a5)
            next = 0x2009eU;
            const auto v1 = (m.byte((r.address[5] + 0x1U)) & 0xfcU) & 0xffU;
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x2009eU: { // 812d0001 or.b d0, $1(a5)
            next = 0x200a2U;
            const auto v1 = (m.byte((r.address[5] + 0x1U)) | (r.data[0] & 0xffU)) & 0xffU;
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x200a2U: { // 566d0046 addq.w #$3, $46(a5)
            next = 0x200a6U;
            const auto v1 = m.add(m.word((r.address[5] + 0x46U)), 0x3U, 16U);
            m.word((r.address[5] + 0x46U), v1);
            break;
        }
        case 0x200a6U: { // 302d0046 move.w $46(a5), d0
            next = 0x200aaU;
            const auto v1 = m.word((r.address[5] + 0x46U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x200aaU: { // 0640000f addi.w #$f, d0
            next = 0x200aeU;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0xfU, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x200aeU: { // 0c40003f cmpi.w #$3f, d0
            next = 0x200b2U;
            (void)m.sub((r.data[0] & 0xffffU), 0x3fU, 16U, true);
            break;
        }
        case 0x200b2U: { // 6d04 blt.b $200b8
            next = 0x200b4U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x200b8U; transfer_kind = 1U; }
            break;
        }
        case 0x200b4U: { // 303c003f move.w #$3f, d0
            next = 0x200b8U;
            const auto v1 = 0x3fU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x200b8U: { // 1b400010 move.b d0, $10(a5)
            next = 0x200bcU;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x10U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x200bcU: { // 1b400011 move.b d0, $11(a5)
            next = 0x200c0U;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x11U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x200c0U: { // 08ed00020001 bset.b #$2, $1(a5)
            next = 0x200c6U;
            const auto v1 = m.byte((r.address[5] + 0x1U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x1U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x200c6U: { // 4eb900015d24 jsr $15d24.l
            next = 0x200ccU;
            const auto result = m.indirect_call(c, 0x200c6U, 0x15d24U, 0x200ccU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x200ccU: { // 4eb9000207c2 jsr $207c2.l
            next = 0x200d2U;
            const auto result = m.indirect_call(c, 0x200ccU, 0x207c2U, 0x200d2U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x200d2U: { // 4eb900015df2 jsr $15df2.l
            next = 0x200d8U;
            const auto result = m.indirect_call(c, 0x200d2U, 0x15df2U, 0x200d8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x200d8U: { // 4e75 rts 
            next = 0x200daU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x200d8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x200daU: { // 610007c0 bsr.w $2089c
            next = 0x200deU;
            const auto result = m.call(c, 389U, 0x200daU, 0x2089cU, 0x200deU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x200deU: { // 4e75 rts 
            next = 0x200e0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x200deU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20046U:
        case 0x2004aU:
        case 0x2004eU:
        case 0x20052U:
        case 0x20056U:
        case 0x2005aU:
        case 0x2005eU:
        case 0x20062U:
        case 0x20066U:
        case 0x2006aU:
        case 0x2006eU:
        case 0x20072U:
        case 0x20076U:
        case 0x2007aU:
        case 0x2007eU:
        case 0x20082U:
        case 0x20086U:
        case 0x20088U:
        case 0x2008cU:
        case 0x20092U:
        case 0x20094U:
        case 0x20098U:
        case 0x2009eU:
        case 0x200a2U:
        case 0x200a6U:
        case 0x200aaU:
        case 0x200aeU:
        case 0x200b2U:
        case 0x200b4U:
        case 0x200b8U:
        case 0x200bcU:
        case 0x200c0U:
        case 0x200c6U:
        case 0x200ccU:
        case 0x200d2U:
        case 0x200d8U:
        case 0x200daU:
        case 0x200deU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
