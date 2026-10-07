
FunctionResult cpu_b_stage_gap_02013c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2013cU: { // 4a2d003f tst.b $3f(a5)
            next = 0x20140U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x20140U: { // 6672 bne.b $201b4
            next = 0x20142U;
            if ((r.status & 4U) == 0U) { next = 0x201b4U; transfer_kind = 1U; }
            break;
        }
        case 0x20142U: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x20146U;
            const auto v1 = m.sub(m.word((r.address[5] + 0x74U)), 0x1U, 16U);
            m.word((r.address[5] + 0x74U), v1);
            break;
        }
        case 0x20146U: { // 6f6c ble.b $201b4
            next = 0x20148U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x201b4U; transfer_kind = 1U; }
            break;
        }
        case 0x20148U: { // 6100055a bsr.w $206a4
            next = 0x2014cU;
            const auto result = m.call(c, 386U, 0x20148U, 0x206a4U, 0x2014cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2014cU: { // 6566 bcs.b $201b4
            next = 0x2014eU;
            if ((r.status & 1U) != 0U) { next = 0x201b4U; transfer_kind = 1U; }
            break;
        }
        case 0x2014eU: { // 610004bc bsr.w $2060c
            next = 0x20152U;
            const auto result = m.call(c, 385U, 0x2014eU, 0x2060cU, 0x20152U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20152U: { // 6560 bcs.b $201b4
            next = 0x20154U;
            if ((r.status & 1U) != 0U) { next = 0x201b4U; transfer_kind = 1U; }
            break;
        }
        case 0x20154U: { // 202d001e move.l $1e(a5), d0
            next = 0x20158U;
            const auto v1 = m.lng((r.address[5] + 0x1eU));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x20158U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x2015cU;
            const auto v1 = m.add(m.lng((r.address[5] + 0x12U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x12U), v1);
            break;
        }
        case 0x2015cU: { // 202d0026 move.l $26(a5), d0
            next = 0x20160U;
            const auto v1 = m.lng((r.address[5] + 0x26U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x20160U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x20164U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x1aU)), r.data[0], 32U);
            m.lng((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x20164U: { // 08ed00020001 bset.b #$2, $1(a5)
            next = 0x2016aU;
            const auto v1 = m.byte((r.address[5] + 0x1U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x1U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x2016aU: { // 3b7c3f3f0010 move.w #$3f3f, $10(a5)
            next = 0x20170U;
            const auto v1 = 0x3f3fU;
            m.word((r.address[5] + 0x10U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20170U: { // 302d005c move.w $5c(a5), d0
            next = 0x20174U;
            const auto v1 = m.word((r.address[5] + 0x5cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20174U: { // 06400280 addi.w #$280, d0
            next = 0x20178U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x280U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x20178U: { // 02400300 andi.w #$300, d0
            next = 0x2017cU;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x300U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2017cU: { // 0c400100 cmpi.w #$100, d0
            next = 0x20180U;
            (void)m.sub((r.data[0] & 0xffffU), 0x100U, 16U, true);
            break;
        }
        case 0x20180U: { // 6c06 bge.b $20188
            next = 0x20182U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x20188U; transfer_kind = 1U; }
            break;
        }
        case 0x20182U: { // 1b7c00fe0010 move.b #$fe, $10(a5)
            next = 0x20188U;
            const auto v1 = 0xfeU;
            m.byte((r.address[5] + 0x10U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x20188U: { // 302d005c move.w $5c(a5), d0
            next = 0x2018cU;
            const auto v1 = m.word((r.address[5] + 0x5cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2018cU: { // 06400080 addi.w #$80, d0
            next = 0x20190U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x80U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x20190U: { // 02400300 andi.w #$300, d0
            next = 0x20194U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x300U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20194U: { // 0c400100 cmpi.w #$100, d0
            next = 0x20198U;
            (void)m.sub((r.data[0] & 0xffffU), 0x100U, 16U, true);
            break;
        }
        case 0x20198U: { // 6c06 bge.b $201a0
            next = 0x2019aU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x201a0U; transfer_kind = 1U; }
            break;
        }
        case 0x2019aU: { // 1b7c00fe0011 move.b #$fe, $11(a5)
            next = 0x201a0U;
            const auto v1 = 0xfeU;
            m.byte((r.address[5] + 0x11U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x201a0U: { // 4eb900015d24 jsr $15d24.l
            next = 0x201a6U;
            const auto result = m.indirect_call(c, 0x201a0U, 0x15d24U, 0x201a6U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x201a6U: { // 4eb9000207c2 jsr $207c2.l
            next = 0x201acU;
            const auto result = m.indirect_call(c, 0x201a6U, 0x207c2U, 0x201acU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x201acU: { // 4eb900015df2 jsr $15df2.l
            next = 0x201b2U;
            const auto result = m.indirect_call(c, 0x201acU, 0x15df2U, 0x201b2U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x201b2U: { // 4e75 rts 
            next = 0x201b4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x201b2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x201b4U: { // 610006e6 bsr.w $2089c
            next = 0x201b8U;
            const auto result = m.call(c, 389U, 0x201b4U, 0x2089cU, 0x201b8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x201b8U: { // 4e75 rts 
            next = 0x201baU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x201b8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2013cU:
        case 0x20140U:
        case 0x20142U:
        case 0x20146U:
        case 0x20148U:
        case 0x2014cU:
        case 0x2014eU:
        case 0x20152U:
        case 0x20154U:
        case 0x20158U:
        case 0x2015cU:
        case 0x20160U:
        case 0x20164U:
        case 0x2016aU:
        case 0x20170U:
        case 0x20174U:
        case 0x20178U:
        case 0x2017cU:
        case 0x20180U:
        case 0x20182U:
        case 0x20188U:
        case 0x2018cU:
        case 0x20190U:
        case 0x20194U:
        case 0x20198U:
        case 0x2019aU:
        case 0x201a0U:
        case 0x201a6U:
        case 0x201acU:
        case 0x201b2U:
        case 0x201b4U:
        case 0x201b8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
