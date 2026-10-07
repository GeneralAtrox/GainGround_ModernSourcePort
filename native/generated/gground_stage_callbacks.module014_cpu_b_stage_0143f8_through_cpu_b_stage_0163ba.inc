
FunctionResult cpu_b_stage_0143f8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x143f8U: { // 4a2d003f tst.b $3f(a5)
            next = 0x143fcU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x143fcU: { // 6728 beq.b $14426
            next = 0x143feU;
            if ((r.status & 4U) != 0U) { next = 0x14426U; transfer_kind = 1U; }
            break;
        }
        case 0x143feU: { // 536d0022 subq.w #$1, $22(a5)
            next = 0x14402U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x22U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x14402U: { // 6e22 bgt.b $14426
            next = 0x14404U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x14426U; transfer_kind = 1U; }
            break;
        }
        case 0x14404U: { // 546d0020 addq.w #$2, $20(a5)
            next = 0x14408U;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[5] + 0x20U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x14408U: { // 302d0020 move.w $20(a5), d0
            next = 0x1440cU;
            const auto source_address = r.address[5] + 0x20U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1440cU: { // 0c40000a cmpi.w #$a, d0
            next = 0x14410U;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x14410U: { // 6b04 bmi.b $14416
            next = 0x14412U;
            if ((r.status & 8U) != 0U) { next = 0x14416U; transfer_kind = 1U; }
            break;
        }
        case 0x14412U: { // 4255 clr.w (a5)
            next = 0x14414U;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x14414U: { // 4e75 rts 
            next = 0x14416U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x14414U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x14416U: { // 3b7c00030022 move.w #$3, $22(a5)
            next = 0x1441cU;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x22U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1441cU: { // 41fa18f8 lea.l $15d16(pc), a0
            next = 0x14420U;
            const auto source_address = 0x15d16U;
            r.address[0] = source_address;
            break;
        }
        case 0x14420U: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x14426U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x14426U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1442cU;
            const auto result = m.call(c, 282U, 0x14426U, 0x15df2U, 0x1442cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1442cU: { // 4e75 rts 
            next = 0x1442eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1442cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x143f8U:
        case 0x143fcU:
        case 0x143feU:
        case 0x14402U:
        case 0x14404U:
        case 0x14408U:
        case 0x1440cU:
        case 0x14410U:
        case 0x14412U:
        case 0x14414U:
        case 0x14416U:
        case 0x1441cU:
        case 0x14420U:
        case 0x14426U:
        case 0x1442cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0163ba(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x163baU: { // 41fa0856 lea.l $16c12(pc), a0
            next = 0x163beU;
            const auto source_address = 0x16c12U;
            r.address[0] = source_address;
            break;
        }
        case 0x163beU: { // 7000 moveq #$0, d0
            next = 0x163c0U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x163c0U: { // 1001 move.b d1, d0
            next = 0x163c2U;
            const auto value = r.data[1];
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x163c2U: { // 02410700 andi.w #$700, d1
            next = 0x163c6U;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x163c6U: { // 08010008 btst.b #$8, d1
            next = 0x163caU;
            const auto old = r.data[1];
            const auto bit_mask = 1U << (0x8U & 31U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x163caU: { // 6608 bne.b $163d4
            next = 0x163ccU;
            if ((r.status & 4U) == 0U) { next = 0x163d4U; transfer_kind = 1U; }
            break;
        }
        case 0x163ccU: { // 10300000 move.b (a0, d0.w), d0
            next = 0x163d0U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x163d0U: { // d240 add.w d0, d1
            next = 0x163d2U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x163d2U: { // 4e75 rts 
            next = 0x163d4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x163d2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x163d4U: { // 4a00 tst.b d0
            next = 0x163d6U;
            const auto value = r.data[0];
            m.logic(value, 8U);
            break;
        }
        case 0x163d6U: { // 670c beq.b $163e4
            next = 0x163d8U;
            if ((r.status & 4U) != 0U) { next = 0x163e4U; transfer_kind = 1U; }
            break;
        }
        case 0x163d8U: { // d27c0100 add.w #$100, d1
            next = 0x163dcU;
            const auto source_value = 0x100U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x163dcU: { // 4400 neg.b d0
            next = 0x163deU;
            const auto old = r.data[0];
            const auto value = m.sub(0U, old, 8U);
            m.db(0U, value);
            break;
        }
        case 0x163deU: { // 10300000 move.b (a0, d0.w), d0
            next = 0x163e2U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x163e2U: { // 9240 sub.w d0, d1
            next = 0x163e4U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x163e4U: { // 4e75 rts 
            next = 0x163e6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x163e4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x163baU:
        case 0x163beU:
        case 0x163c0U:
        case 0x163c2U:
        case 0x163c6U:
        case 0x163caU:
        case 0x163ccU:
        case 0x163d0U:
        case 0x163d2U:
        case 0x163d4U:
        case 0x163d6U:
        case 0x163d8U:
        case 0x163dcU:
        case 0x163deU:
        case 0x163e2U:
        case 0x163e4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
