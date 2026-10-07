
FunctionResult cpu_b_stage_gap_014016(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x14016U: { // 7202 moveq #$2, d1
            next = 0x14018U;
            r.data[1] = 0x2U; m.logic(r.data[1], 32U);
            break;
        }
        case 0x14018U: { // 4df81480 lea.l $1480.w, a6
            next = 0x1401cU;
            r.address[6] = 0x1480U;
            break;
        }
        case 0x1401cU: { // 0c6e00050044 cmpi.w #$5, $44(a6)
            next = 0x14022U;
            (void)m.sub(m.word((r.address[6] + 0x44U)), 0x5U, 16U, true);
            break;
        }
        case 0x14022U: { // 661e bne.b $14042
            next = 0x14024U;
            if ((r.status & 4U) == 0U) { next = 0x14042U; transfer_kind = 1U; }
            break;
        }
        case 0x14024U: { // 302e0012 move.w $12(a6), d0
            next = 0x14028U;
            const auto v1 = m.word((r.address[6] + 0x12U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14028U: { // 906d002c sub.w $2c(a5), d0
            next = 0x1402cU;
            const auto v1 = m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x2cU)), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1402cU: { // 0c400010 cmpi.w #$10, d0
            next = 0x14030U;
            (void)m.sub((r.data[0] & 0xffffU), 0x10U, 16U, true);
            break;
        }
        case 0x14030U: { // 6e10 bgt.b $14042
            next = 0x14032U;
            if (((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U)) { next = 0x14042U; transfer_kind = 1U; }
            break;
        }
        case 0x14032U: { // 302e001a move.w $1a(a6), d0
            next = 0x14036U;
            const auto v1 = m.word((r.address[6] + 0x1aU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14036U: { // b06d0032 cmp.w $32(a5), d0
            next = 0x1403aU;
            (void)m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x32U)), 16U, true);
            break;
        }
        case 0x1403aU: { // 6b06 bmi.b $14042
            next = 0x1403cU;
            if ((r.status & 8U) != 0U) { next = 0x14042U; transfer_kind = 1U; }
            break;
        }
        case 0x1403cU: { // b06d0034 cmp.w $34(a5), d0
            next = 0x14040U;
            (void)m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x34U)), 16U, true);
            break;
        }
        case 0x14040U: { // 6f10 ble.b $14052
            next = 0x14042U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x14052U; transfer_kind = 1U; }
            break;
        }
        case 0x14042U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x14046U;
            r.address[6] = (r.address[6] + 0x80U);
            break;
        }
        case 0x14046U: { // 51c9ffd4 dbra d1, $1401c
            next = 0x1404aU;
            const auto v1 = static_cast<std::uint16_t>(r.data[1] - 1U); m.dw(1, v1);
            if (v1 != 0xffffU) { next = 0x1401cU; transfer_kind = 1U; }
            break;
        }
        case 0x1404aU: { // 4eb900015df2 jsr $15df2.l
            next = 0x14050U;
            const auto result = m.indirect_call(c, 0x1404aU, 0x15df2U, 0x14050U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x14050U: { // 4e75 rts 
            next = 0x14052U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x14050U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x14052U: { // 2b7c0001405a0002 move.l #$1405a, $2(a5)
            next = 0x1405aU;
            const auto v1 = 0x1405aU;
            m.lng((r.address[5] + 0x2U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x1405aU: { // 516d000e subq.w #$8, $e(a5)
            next = 0x1405eU;
            const auto v1 = m.sub(m.word((r.address[5] + 0xeU)), 0x8U, 16U);
            m.word((r.address[5] + 0xeU), v1);
            break;
        }
        case 0x1405eU: { // 0c6dffc0000e cmpi.w #$ffc0, $e(a5)
            next = 0x14064U;
            (void)m.sub(m.word((r.address[5] + 0xeU)), 0xffc0U, 16U, true);
            break;
        }
        case 0x14064U: { // 6f0c ble.b $14072
            next = 0x14066U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x14072U; transfer_kind = 1U; }
            break;
        }
        case 0x14066U: { // 516d002c subq.w #$8, $2c(a5)
            next = 0x1406aU;
            const auto v1 = m.sub(m.word((r.address[5] + 0x2cU)), 0x8U, 16U);
            m.word((r.address[5] + 0x2cU), v1);
            break;
        }
        case 0x1406aU: { // 4eb900015df2 jsr $15df2.l
            next = 0x14070U;
            const auto result = m.indirect_call(c, 0x1406aU, 0x15df2U, 0x14070U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x14070U: { // 4e75 rts 
            next = 0x14072U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x14070U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x14072U: { // 4255 clr.w (a5)
            next = 0x14074U;
            (void)m.word(r.address[5]);
            m.word(r.address[5], 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x14074U: { // 302d0020 move.w $20(a5), d0
            next = 0x14078U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14078U: { // 41f80d0f lea.l $d0f.w, a0
            next = 0x1407cU;
            r.address[0] = 0xd0fU;
            break;
        }
        case 0x1407cU: { // 4af00000 tas.b (a0, d0.w)
            next = 0x14080U;
            const auto v1 = m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])))); m.logic(v1, 8U); m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))), v1 | 0x80U);
            break;
        }
        case 0x14080U: { // 4e75 rts 
            next = 0x14082U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x14080U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x14016U:
        case 0x14018U:
        case 0x1401cU:
        case 0x14022U:
        case 0x14024U:
        case 0x14028U:
        case 0x1402cU:
        case 0x14030U:
        case 0x14032U:
        case 0x14036U:
        case 0x1403aU:
        case 0x1403cU:
        case 0x14040U:
        case 0x14042U:
        case 0x14046U:
        case 0x1404aU:
        case 0x14050U:
        case 0x14052U:
        case 0x1405aU:
        case 0x1405eU:
        case 0x14064U:
        case 0x14066U:
        case 0x1406aU:
        case 0x14070U:
        case 0x14072U:
        case 0x14074U:
        case 0x14078U:
        case 0x1407cU:
        case 0x14080U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_01405a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1405aU: { // 516d000e subq.w #$8, $e(a5)
            next = 0x1405eU;
            const auto v1 = m.sub(m.word((r.address[5] + 0xeU)), 0x8U, 16U);
            m.word((r.address[5] + 0xeU), v1);
            break;
        }
        case 0x1405eU: { // 0c6dffc0000e cmpi.w #$ffc0, $e(a5)
            next = 0x14064U;
            (void)m.sub(m.word((r.address[5] + 0xeU)), 0xffc0U, 16U, true);
            break;
        }
        case 0x14064U: { // 6f0c ble.b $14072
            next = 0x14066U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x14072U; transfer_kind = 1U; }
            break;
        }
        case 0x14066U: { // 516d002c subq.w #$8, $2c(a5)
            next = 0x1406aU;
            const auto v1 = m.sub(m.word((r.address[5] + 0x2cU)), 0x8U, 16U);
            m.word((r.address[5] + 0x2cU), v1);
            break;
        }
        case 0x1406aU: { // 4eb900015df2 jsr $15df2.l
            next = 0x14070U;
            const auto result = m.indirect_call(c, 0x1406aU, 0x15df2U, 0x14070U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x14070U: { // 4e75 rts 
            next = 0x14072U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x14070U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x14072U: { // 4255 clr.w (a5)
            next = 0x14074U;
            (void)m.word(r.address[5]);
            m.word(r.address[5], 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x14074U: { // 302d0020 move.w $20(a5), d0
            next = 0x14078U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14078U: { // 41f80d0f lea.l $d0f.w, a0
            next = 0x1407cU;
            r.address[0] = 0xd0fU;
            break;
        }
        case 0x1407cU: { // 4af00000 tas.b (a0, d0.w)
            next = 0x14080U;
            const auto v1 = m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])))); m.logic(v1, 8U); m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))), v1 | 0x80U);
            break;
        }
        case 0x14080U: { // 4e75 rts 
            next = 0x14082U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x14080U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1405aU:
        case 0x1405eU:
        case 0x14064U:
        case 0x14066U:
        case 0x1406aU:
        case 0x14070U:
        case 0x14072U:
        case 0x14074U:
        case 0x14078U:
        case 0x1407cU:
        case 0x14080U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
