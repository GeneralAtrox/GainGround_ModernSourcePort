
FunctionResult cpu_b_stage_gap_0201ba(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x201baU: { // 4a2d003f tst.b $3f(a5)
            next = 0x201beU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x201beU: { // 6662 bne.b $20222
            next = 0x201c0U;
            if ((r.status & 4U) == 0U) { next = 0x20222U; transfer_kind = 1U; }
            break;
        }
        case 0x201c0U: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x201c4U;
            const auto v1 = m.sub(m.word((r.address[5] + 0x74U)), 0x1U, 16U);
            m.word((r.address[5] + 0x74U), v1);
            break;
        }
        case 0x201c4U: { // 6f5c ble.b $20222
            next = 0x201c6U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x20222U; transfer_kind = 1U; }
            break;
        }
        case 0x201c6U: { // 610004dc bsr.w $206a4
            next = 0x201caU;
            const auto result = m.call(c, 386U, 0x201c6U, 0x206a4U, 0x201caU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x201caU: { // 6556 bcs.b $20222
            next = 0x201ccU;
            if ((r.status & 1U) != 0U) { next = 0x20222U; transfer_kind = 1U; }
            break;
        }
        case 0x201ccU: { // 6100043e bsr.w $2060c
            next = 0x201d0U;
            const auto result = m.call(c, 385U, 0x201ccU, 0x2060cU, 0x201d0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x201d0U: { // 6550 bcs.b $20222
            next = 0x201d2U;
            if ((r.status & 1U) != 0U) { next = 0x20222U; transfer_kind = 1U; }
            break;
        }
        case 0x201d2U: { // 202d001e move.l $1e(a5), d0
            next = 0x201d6U;
            const auto v1 = m.lng((r.address[5] + 0x1eU));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x201d6U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x201daU;
            const auto v1 = m.add(m.lng((r.address[5] + 0x12U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x12U), v1);
            break;
        }
        case 0x201daU: { // 202d0026 move.l $26(a5), d0
            next = 0x201deU;
            const auto v1 = m.lng((r.address[5] + 0x26U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x201deU: { // d1ad001a add.l d0, $1a(a5)
            next = 0x201e2U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x1aU)), r.data[0], 32U);
            m.lng((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x201e2U: { // 302d0056 move.w $56(a5), d0
            next = 0x201e6U;
            const auto v1 = m.word((r.address[5] + 0x56U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x201e6U: { // 5240 addq.w #$1, d0
            next = 0x201e8U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x1U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x201e8U: { // 3b400056 move.w d0, $56(a5)
            next = 0x201ecU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x56U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x201ecU: { // 0c400008 cmpi.w #$8, d0
            next = 0x201f0U;
            (void)m.sub((r.data[0] & 0xffffU), 0x8U, 16U, true);
            break;
        }
        case 0x201f0U: { // 6c0c bge.b $201fe
            next = 0x201f2U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x201feU; transfer_kind = 1U; }
            break;
        }
        case 0x201f2U: { // 02400004 andi.w #$4, d0
            next = 0x201f6U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x4U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x201f6U: { // e448 lsr.w #$2, d0
            next = 0x201f8U;
            m.shift_word(0, 2U, false, false);
            break;
        }
        case 0x201f8U: { // d06d005a add.w $5a(a5), d0
            next = 0x201fcU;
            const auto v1 = m.add((r.data[0] & 0xffffU), m.word((r.address[5] + 0x5aU)), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x201fcU: { // 600c bra.b $2020a
            next = 0x201feU;
            next = 0x2020aU; transfer_kind = 1U;
            break;
        }
        case 0x201feU: { // 02400002 andi.w #$2, d0
            next = 0x20202U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x2U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20202U: { // e248 lsr.w #$1, d0
            next = 0x20204U;
            m.shift_word(0, 1U, false, false);
            break;
        }
        case 0x20204U: { // 5440 addq.w #$2, d0
            next = 0x20206U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x2U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x20206U: { // d06d005a add.w $5a(a5), d0
            next = 0x2020aU;
            const auto v1 = m.add((r.data[0] & 0xffffU), m.word((r.address[5] + 0x5aU)), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x2020aU: { // 3b400008 move.w d0, $8(a5)
            next = 0x2020eU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2020eU: { // 4eb900015d24 jsr $15d24.l
            next = 0x20214U;
            const auto result = m.indirect_call(c, 0x2020eU, 0x15d24U, 0x20214U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20214U: { // 4eb9000207c2 jsr $207c2.l
            next = 0x2021aU;
            const auto result = m.indirect_call(c, 0x20214U, 0x207c2U, 0x2021aU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2021aU: { // 4eb900015df2 jsr $15df2.l
            next = 0x20220U;
            const auto result = m.indirect_call(c, 0x2021aU, 0x15df2U, 0x20220U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20220U: { // 4e75 rts 
            next = 0x20222U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20220U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20222U: { // 61000678 bsr.w $2089c
            next = 0x20226U;
            const auto result = m.call(c, 389U, 0x20222U, 0x2089cU, 0x20226U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20226U: { // 4e75 rts 
            next = 0x20228U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20226U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x201baU:
        case 0x201beU:
        case 0x201c0U:
        case 0x201c4U:
        case 0x201c6U:
        case 0x201caU:
        case 0x201ccU:
        case 0x201d0U:
        case 0x201d2U:
        case 0x201d6U:
        case 0x201daU:
        case 0x201deU:
        case 0x201e2U:
        case 0x201e6U:
        case 0x201e8U:
        case 0x201ecU:
        case 0x201f0U:
        case 0x201f2U:
        case 0x201f6U:
        case 0x201f8U:
        case 0x201fcU:
        case 0x201feU:
        case 0x20202U:
        case 0x20204U:
        case 0x20206U:
        case 0x2020aU:
        case 0x2020eU:
        case 0x20214U:
        case 0x2021aU:
        case 0x20220U:
        case 0x20222U:
        case 0x20226U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
