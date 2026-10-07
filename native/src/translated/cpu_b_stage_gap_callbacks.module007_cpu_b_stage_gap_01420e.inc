
FunctionResult cpu_b_stage_gap_01420e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1420eU: { // 49f900000d1c lea.l $d1c.l, a4
            next = 0x14214U;
            r.address[4] = 0xd1cU;
            break;
        }
        case 0x14214U: { // 302d0020 move.w $20(a5), d0
            next = 0x14218U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14218U: { // d8c0 adda.w d0, a4
            next = 0x1421aU;
            r.address[4] += static_cast<std::uint32_t>(static_cast<std::int16_t>((r.data[0] & 0xffffU)));
            break;
        }
        case 0x1421aU: { // 3014 move.w (a4), d0
            next = 0x1421cU;
            const auto v1 = m.word(r.address[4]);
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1421cU: { // e540 asl.w #$2, d0
            next = 0x1421eU;
            m.asl_word(0, 2U);
            break;
        }
        case 0x1421eU: { // 4efb0002 jmp $14222(pc, d0.w)
            next = 0x14222U;
            next = 0x14222U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])); transfer_kind = 1U;
            break;
        }
        case 0x14222U: { // 6000000e bra.w $14232
            next = 0x14226U;
            next = 0x14232U; transfer_kind = 1U;
            break;
        }
        case 0x14226U: { // 60000024 bra.w $1424c
            next = 0x1422aU;
            next = 0x1424cU; transfer_kind = 1U;
            break;
        }
        case 0x1422aU: { // 60000044 bra.w $14270
            next = 0x1422eU;
            next = 0x14270U; transfer_kind = 1U;
            break;
        }
        case 0x1422eU: { // 60000060 bra.w $14290
            next = 0x14232U;
            next = 0x14290U; transfer_kind = 1U;
            break;
        }
        case 0x14232U: { // 4a2d003f tst.b $3f(a5)
            next = 0x14236U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x14236U: { // 67000086 beq.w $142be
            next = 0x1423aU;
            if ((r.status & 4U) != 0U) { next = 0x142beU; transfer_kind = 1U; }
            break;
        }
        case 0x1423aU: { // 38bc0001 move.w #$1, (a4)
            next = 0x1423eU;
            const auto v1 = 0x1U;
            m.word(r.address[4], v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1423eU: { // 426c0002 clr.w $2(a4)
            next = 0x14242U;
            (void)m.word((r.address[4] + 0x2U));
            m.word((r.address[4] + 0x2U), 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x14242U: { // 1b7c0004000b move.b #$4, $b(a5)
            next = 0x14248U;
            const auto v1 = 0x4U;
            m.byte((r.address[5] + 0xbU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x14248U: { // 7000 moveq #$0, d0
            next = 0x1424aU;
            r.data[0] = 0x0U; m.logic(r.data[0], 32U);
            break;
        }
        case 0x1424aU: { // 6068 bra.b $142b4
            next = 0x1424cU;
            next = 0x142b4U; transfer_kind = 1U;
            break;
        }
        case 0x1424cU: { // 526c0002 addq.w #$1, $2(a4)
            next = 0x14250U;
            const auto v1 = m.add(m.word((r.address[4] + 0x2U)), 0x1U, 16U);
            m.word((r.address[4] + 0x2U), v1);
            break;
        }
        case 0x14250U: { // 302c0002 move.w $2(a4), d0
            next = 0x14254U;
            const auto v1 = m.word((r.address[4] + 0x2U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14254U: { // 0c400008 cmpi.w #$8, d0
            next = 0x14258U;
            (void)m.sub((r.data[0] & 0xffffU), 0x8U, 16U, true);
            break;
        }
        case 0x14258U: { // 6b5a bmi.b $142b4
            next = 0x1425aU;
            if ((r.status & 8U) != 0U) { next = 0x142b4U; transfer_kind = 1U; }
            break;
        }
        case 0x1425aU: { // 38bc0002 move.w #$2, (a4)
            next = 0x1425eU;
            const auto v1 = 0x2U;
            m.word(r.address[4], v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1425eU: { // 397c00780002 move.w #$78, $2(a4)
            next = 0x14264U;
            const auto v1 = 0x78U;
            m.word((r.address[4] + 0x2U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14264U: { // 61000074 bsr.w $142da
            next = 0x14268U;
            const auto result = m.call(c, 682U, 0x14264U, 0x142daU, 0x14268U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x14268U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1426eU;
            const auto result = m.indirect_call(c, 0x14268U, 0x15df2U, 0x1426eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1426eU: { // 4e75 rts 
            next = 0x14270U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1426eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x14270U: { // 536c0002 subq.w #$1, $2(a4)
            next = 0x14274U;
            const auto v1 = m.sub(m.word((r.address[4] + 0x2U)), 0x1U, 16U);
            m.word((r.address[4] + 0x2U), v1);
            break;
        }
        case 0x14274U: { // 6e4c bgt.b $142c2
            next = 0x14276U;
            if (((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U)) { next = 0x142c2U; transfer_kind = 1U; }
            break;
        }
        case 0x14276U: { // 38bc0003 move.w #$3, (a4)
            next = 0x1427aU;
            const auto v1 = 0x3U;
            m.word(r.address[4], v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1427aU: { // 426c0002 clr.w $2(a4)
            next = 0x1427eU;
            (void)m.word((r.address[4] + 0x2U));
            m.word((r.address[4] + 0x2U), 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1427eU: { // 422d003f clr.b $3f(a5)
            next = 0x14282U;
            (void)m.byte((r.address[5] + 0x3fU));
            m.byte((r.address[5] + 0x3fU), 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x14282U: { // 1b7c000a000b move.b #$a, $b(a5)
            next = 0x14288U;
            const auto v1 = 0xaU;
            m.byte((r.address[5] + 0xbU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x14288U: { // 3b7c00140046 move.w #$14, $46(a5)
            next = 0x1428eU;
            const auto v1 = 0x14U;
            m.word((r.address[5] + 0x46U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1428eU: { // 601e bra.b $142ae
            next = 0x14290U;
            next = 0x142aeU; transfer_kind = 1U;
            break;
        }
        case 0x14290U: { // 526c0002 addq.w #$1, $2(a4)
            next = 0x14294U;
            const auto v1 = m.add(m.word((r.address[4] + 0x2U)), 0x1U, 16U);
            m.word((r.address[4] + 0x2U), v1);
            break;
        }
        case 0x14294U: { // 4a2d003f tst.b $3f(a5)
            next = 0x14298U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x14298U: { // 66a0 bne.b $1423a
            next = 0x1429aU;
            if ((r.status & 4U) == 0U) { next = 0x1423aU; transfer_kind = 1U; }
            break;
        }
        case 0x1429aU: { // 302c0002 move.w $2(a4), d0
            next = 0x1429eU;
            const auto v1 = m.word((r.address[4] + 0x2U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1429eU: { // 0c400008 cmpi.w #$8, d0
            next = 0x142a2U;
            (void)m.sub((r.data[0] & 0xffffU), 0x8U, 16U, true);
            break;
        }
        case 0x142a2U: { // 6b0a bmi.b $142ae
            next = 0x142a4U;
            if ((r.status & 8U) != 0U) { next = 0x142aeU; transfer_kind = 1U; }
            break;
        }
        case 0x142a4U: { // 4294 clr.l (a4)
            next = 0x142a6U;
            (void)m.lng(r.address[4]);
            m.lng(r.address[4], 0U);
            m.logic(0U, 32U);
            break;
        }
        case 0x142a6U: { // 3b7c0c060008 move.w #$c06, $8(a5)
            next = 0x142acU;
            const auto v1 = 0xc06U;
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x142acU: { // 6010 bra.b $142be
            next = 0x142aeU;
            next = 0x142beU; transfer_kind = 1U;
            break;
        }
        case 0x142aeU: { // 4640 not.w d0
            next = 0x142b0U;
            const auto v1 = (~(r.data[0] & 0xffffU)) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x142b0U: { // 02400007 andi.w #$7, d0
            next = 0x142b4U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x7U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x142b4U: { // e240 asr.w #$1, d0
            next = 0x142b6U;
            m.shift_word(0, 1U, false, true);
            break;
        }
        case 0x142b6U: { // 06400c07 addi.w #$c07, d0
            next = 0x142baU;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0xc07U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x142baU: { // 3b400008 move.w d0, $8(a5)
            next = 0x142beU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x142beU: { // 6100000a bsr.w $142ca
            next = 0x142c2U;
            const auto result = m.call(c, 681U, 0x142beU, 0x142caU, 0x142c2U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x142c2U: { // 4eb900015df2 jsr $15df2.l
            next = 0x142c8U;
            const auto result = m.indirect_call(c, 0x142c2U, 0x15df2U, 0x142c8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x142c8U: { // 4e75 rts 
            next = 0x142caU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x142c8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1420eU:
        case 0x14214U:
        case 0x14218U:
        case 0x1421aU:
        case 0x1421cU:
        case 0x1421eU:
        case 0x14222U:
        case 0x14226U:
        case 0x1422aU:
        case 0x1422eU:
        case 0x14232U:
        case 0x14236U:
        case 0x1423aU:
        case 0x1423eU:
        case 0x14242U:
        case 0x14248U:
        case 0x1424aU:
        case 0x1424cU:
        case 0x14250U:
        case 0x14254U:
        case 0x14258U:
        case 0x1425aU:
        case 0x1425eU:
        case 0x14264U:
        case 0x14268U:
        case 0x1426eU:
        case 0x14270U:
        case 0x14274U:
        case 0x14276U:
        case 0x1427aU:
        case 0x1427eU:
        case 0x14282U:
        case 0x14288U:
        case 0x1428eU:
        case 0x14290U:
        case 0x14294U:
        case 0x14298U:
        case 0x1429aU:
        case 0x1429eU:
        case 0x142a2U:
        case 0x142a4U:
        case 0x142a6U:
        case 0x142acU:
        case 0x142aeU:
        case 0x142b0U:
        case 0x142b4U:
        case 0x142b6U:
        case 0x142baU:
        case 0x142beU:
        case 0x142c2U:
        case 0x142c8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
