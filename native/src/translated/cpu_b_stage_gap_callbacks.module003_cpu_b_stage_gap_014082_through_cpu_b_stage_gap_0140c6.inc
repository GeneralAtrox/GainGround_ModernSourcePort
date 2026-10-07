
FunctionResult cpu_b_stage_gap_014082(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x14082U: { // 7202 moveq #$2, d1
            next = 0x14084U;
            r.data[1] = 0x2U; m.logic(r.data[1], 32U);
            break;
        }
        case 0x14084U: { // 4df81480 lea.l $1480.w, a6
            next = 0x14088U;
            r.address[6] = 0x1480U;
            break;
        }
        case 0x14088U: { // 0c6e00050044 cmpi.w #$5, $44(a6)
            next = 0x1408eU;
            (void)m.sub(m.word((r.address[6] + 0x44U)), 0x5U, 16U, true);
            break;
        }
        case 0x1408eU: { // 661e bne.b $140ae
            next = 0x14090U;
            if ((r.status & 4U) == 0U) { next = 0x140aeU; transfer_kind = 1U; }
            break;
        }
        case 0x14090U: { // 302e0012 move.w $12(a6), d0
            next = 0x14094U;
            const auto v1 = m.word((r.address[6] + 0x12U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14094U: { // 906d002a sub.w $2a(a5), d0
            next = 0x14098U;
            const auto v1 = m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x2aU)), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x14098U: { // 0c40fff0 cmpi.w #$fff0, d0
            next = 0x1409cU;
            (void)m.sub((r.data[0] & 0xffffU), 0xfff0U, 16U, true);
            break;
        }
        case 0x1409cU: { // 6b10 bmi.b $140ae
            next = 0x1409eU;
            if ((r.status & 8U) != 0U) { next = 0x140aeU; transfer_kind = 1U; }
            break;
        }
        case 0x1409eU: { // 302e001a move.w $1a(a6), d0
            next = 0x140a2U;
            const auto v1 = m.word((r.address[6] + 0x1aU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x140a2U: { // b06d0032 cmp.w $32(a5), d0
            next = 0x140a6U;
            (void)m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x32U)), 16U, true);
            break;
        }
        case 0x140a6U: { // 6b06 bmi.b $140ae
            next = 0x140a8U;
            if ((r.status & 8U) != 0U) { next = 0x140aeU; transfer_kind = 1U; }
            break;
        }
        case 0x140a8U: { // b06d0034 cmp.w $34(a5), d0
            next = 0x140acU;
            (void)m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x34U)), 16U, true);
            break;
        }
        case 0x140acU: { // 6f10 ble.b $140be
            next = 0x140aeU;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x140beU; transfer_kind = 1U; }
            break;
        }
        case 0x140aeU: { // 4dee0080 lea.l $80(a6), a6
            next = 0x140b2U;
            r.address[6] = (r.address[6] + 0x80U);
            break;
        }
        case 0x140b2U: { // 51c9ffd4 dbra d1, $14088
            next = 0x140b6U;
            const auto v1 = static_cast<std::uint16_t>(r.data[1] - 1U); m.dw(1, v1);
            if (v1 != 0xffffU) { next = 0x14088U; transfer_kind = 1U; }
            break;
        }
        case 0x140b6U: { // 4eb900015df2 jsr $15df2.l
            next = 0x140bcU;
            const auto result = m.indirect_call(c, 0x140b6U, 0x15df2U, 0x140bcU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x140bcU: { // 4e75 rts 
            next = 0x140beU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x140bcU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x140beU: { // 2b7c000140c60002 move.l #$140c6, $2(a5)
            next = 0x140c6U;
            const auto v1 = 0x140c6U;
            m.lng((r.address[5] + 0x2U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x140c6U: { // 506d000e addq.w #$8, $e(a5)
            next = 0x140caU;
            const auto v1 = m.add(m.word((r.address[5] + 0xeU)), 0x8U, 16U);
            m.word((r.address[5] + 0xeU), v1);
            break;
        }
        case 0x140caU: { // 0c6d01c0000e cmpi.w #$1c0, $e(a5)
            next = 0x140d0U;
            (void)m.sub(m.word((r.address[5] + 0xeU)), 0x1c0U, 16U, true);
            break;
        }
        case 0x140d0U: { // 6a0c bpl.b $140de
            next = 0x140d2U;
            if ((r.status & 8U) == 0U) { next = 0x140deU; transfer_kind = 1U; }
            break;
        }
        case 0x140d2U: { // 506d002a addq.w #$8, $2a(a5)
            next = 0x140d6U;
            const auto v1 = m.add(m.word((r.address[5] + 0x2aU)), 0x8U, 16U);
            m.word((r.address[5] + 0x2aU), v1);
            break;
        }
        case 0x140d6U: { // 4eb900015df2 jsr $15df2.l
            next = 0x140dcU;
            const auto result = m.indirect_call(c, 0x140d6U, 0x15df2U, 0x140dcU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x140dcU: { // 4e75 rts 
            next = 0x140deU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x140dcU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x140deU: { // 4255 clr.w (a5)
            next = 0x140e0U;
            (void)m.word(r.address[5]);
            m.word(r.address[5], 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x140e0U: { // 302d0020 move.w $20(a5), d0
            next = 0x140e4U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x140e4U: { // 41f80d0f lea.l $d0f.w, a0
            next = 0x140e8U;
            r.address[0] = 0xd0fU;
            break;
        }
        case 0x140e8U: { // 4af00000 tas.b (a0, d0.w)
            next = 0x140ecU;
            const auto v1 = m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])))); m.logic(v1, 8U); m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))), v1 | 0x80U);
            break;
        }
        case 0x140ecU: { // 4e75 rts 
            next = 0x140eeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x140ecU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x14082U:
        case 0x14084U:
        case 0x14088U:
        case 0x1408eU:
        case 0x14090U:
        case 0x14094U:
        case 0x14098U:
        case 0x1409cU:
        case 0x1409eU:
        case 0x140a2U:
        case 0x140a6U:
        case 0x140a8U:
        case 0x140acU:
        case 0x140aeU:
        case 0x140b2U:
        case 0x140b6U:
        case 0x140bcU:
        case 0x140beU:
        case 0x140c6U:
        case 0x140caU:
        case 0x140d0U:
        case 0x140d2U:
        case 0x140d6U:
        case 0x140dcU:
        case 0x140deU:
        case 0x140e0U:
        case 0x140e4U:
        case 0x140e8U:
        case 0x140ecU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_0140c6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x140c6U: { // 506d000e addq.w #$8, $e(a5)
            next = 0x140caU;
            const auto v1 = m.add(m.word((r.address[5] + 0xeU)), 0x8U, 16U);
            m.word((r.address[5] + 0xeU), v1);
            break;
        }
        case 0x140caU: { // 0c6d01c0000e cmpi.w #$1c0, $e(a5)
            next = 0x140d0U;
            (void)m.sub(m.word((r.address[5] + 0xeU)), 0x1c0U, 16U, true);
            break;
        }
        case 0x140d0U: { // 6a0c bpl.b $140de
            next = 0x140d2U;
            if ((r.status & 8U) == 0U) { next = 0x140deU; transfer_kind = 1U; }
            break;
        }
        case 0x140d2U: { // 506d002a addq.w #$8, $2a(a5)
            next = 0x140d6U;
            const auto v1 = m.add(m.word((r.address[5] + 0x2aU)), 0x8U, 16U);
            m.word((r.address[5] + 0x2aU), v1);
            break;
        }
        case 0x140d6U: { // 4eb900015df2 jsr $15df2.l
            next = 0x140dcU;
            const auto result = m.indirect_call(c, 0x140d6U, 0x15df2U, 0x140dcU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x140dcU: { // 4e75 rts 
            next = 0x140deU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x140dcU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x140deU: { // 4255 clr.w (a5)
            next = 0x140e0U;
            (void)m.word(r.address[5]);
            m.word(r.address[5], 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x140e0U: { // 302d0020 move.w $20(a5), d0
            next = 0x140e4U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x140e4U: { // 41f80d0f lea.l $d0f.w, a0
            next = 0x140e8U;
            r.address[0] = 0xd0fU;
            break;
        }
        case 0x140e8U: { // 4af00000 tas.b (a0, d0.w)
            next = 0x140ecU;
            const auto v1 = m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])))); m.logic(v1, 8U); m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))), v1 | 0x80U);
            break;
        }
        case 0x140ecU: { // 4e75 rts 
            next = 0x140eeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x140ecU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x140c6U:
        case 0x140caU:
        case 0x140d0U:
        case 0x140d2U:
        case 0x140d6U:
        case 0x140dcU:
        case 0x140deU:
        case 0x140e0U:
        case 0x140e4U:
        case 0x140e8U:
        case 0x140ecU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
