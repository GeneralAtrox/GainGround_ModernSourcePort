
FunctionResult cpu_b_stage_gap_020228(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20228U: { // 202d001e move.l $1e(a5), d0
            next = 0x2022cU;
            const auto v1 = m.lng((r.address[5] + 0x1eU));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x2022cU: { // d1ad0012 add.l d0, $12(a5)
            next = 0x20230U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x12U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x12U), v1);
            break;
        }
        case 0x20230U: { // 202d0026 move.l $26(a5), d0
            next = 0x20234U;
            const auto v1 = m.lng((r.address[5] + 0x26U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x20234U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x20238U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x1aU)), r.data[0], 32U);
            m.lng((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x20238U: { // 202d0022 move.l $22(a5), d0
            next = 0x2023cU;
            const auto v1 = m.lng((r.address[5] + 0x22U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x2023cU: { // 90ad0066 sub.l $66(a5), d0
            next = 0x20240U;
            const auto v1 = m.sub(r.data[0], m.lng((r.address[5] + 0x66U)), 32U);
            r.data[0] = v1;
            break;
        }
        case 0x20240U: { // 2b400022 move.l d0, $22(a5)
            next = 0x20244U;
            const auto v1 = r.data[0];
            m.lng((r.address[5] + 0x22U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x20244U: { // d1ad0016 add.l d0, $16(a5)
            next = 0x20248U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x16U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x16U), v1);
            break;
        }
        case 0x20248U: { // 0c6d00060016 cmpi.w #$6, $16(a5)
            next = 0x2024eU;
            (void)m.sub(m.word((r.address[5] + 0x16U)), 0x6U, 16U, true);
            break;
        }
        case 0x2024eU: { // 6f2c ble.b $2027c
            next = 0x20250U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x2027cU; transfer_kind = 1U; }
            break;
        }
        case 0x20250U: { // 302d0016 move.w $16(a5), d0
            next = 0x20254U;
            const auto v1 = m.word((r.address[5] + 0x16U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20254U: { // 04400010 subi.w #$10, d0
            next = 0x20258U;
            const auto v1 = m.sub((r.data[0] & 0xffffU), 0x10U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x20258U: { // 3200 move.w d0, d1
            next = 0x2025aU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.dw(1, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2025aU: { // 0241ffe0 andi.w #$ffe0, d1
            next = 0x2025eU;
            const auto v1 = ((r.data[1] & 0xffffU) & 0xffe0U) & 0xffffU;
            m.dw(1, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2025eU: { // 6602 bne.b $20262
            next = 0x20260U;
            if ((r.status & 4U) == 0U) { next = 0x20262U; transfer_kind = 1U; }
            break;
        }
        case 0x20260U: { // e240 asr.w #$1, d0
            next = 0x20262U;
            m.shift_word(0, 1U, false, true);
            break;
        }
        case 0x20262U: { // e240 asr.w #$1, d0
            next = 0x20264U;
            m.shift_word(0, 1U, false, true);
            break;
        }
        case 0x20264U: { // 0640003f addi.w #$3f, d0
            next = 0x20268U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x3fU, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x20268U: { // 1b400010 move.b d0, $10(a5)
            next = 0x2026cU;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x10U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x2026cU: { // 1b400011 move.b d0, $11(a5)
            next = 0x20270U;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x11U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x20270U: { // 08ed00020001 bset.b #$2, $1(a5)
            next = 0x20276U;
            const auto v1 = m.byte((r.address[5] + 0x1U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x1U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x20276U: { // 6100e4e2 bsr.w $1e75a
            next = 0x2027aU;
            const auto result = m.call(c, 683U, 0x20276U, 0x1e75aU, 0x2027aU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2027aU: { // 4e75 rts 
            next = 0x2027cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2027aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2027cU: { // 6100d972 bsr.w $1dbf0
            next = 0x20280U;
            const auto result = m.call(c, 353U, 0x2027cU, 0x1dbf0U, 0x20280U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20280U: { // 2b7c000800000016 move.l #$80000, $16(a5)
            next = 0x20288U;
            const auto v1 = 0x80000U;
            m.lng((r.address[5] + 0x16U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x20288U: { // 3b7c0200005c move.w #$200, $5c(a5)
            next = 0x2028eU;
            const auto v1 = 0x200U;
            m.word((r.address[5] + 0x5cU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2028eU: { // 08ad00010078 bclr.b #$1, $78(a5)
            next = 0x20294U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x78U), (v1 & ~(1U << 1U)) & 0xffU);
            break;
        }
        case 0x20294U: { // 3b7cffff005c move.w #$ffff, $5c(a5)
            next = 0x2029aU;
            const auto v1 = 0xffffU;
            m.word((r.address[5] + 0x5cU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2029aU: { // 4e75 rts 
            next = 0x2029cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2029aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20228U:
        case 0x2022cU:
        case 0x20230U:
        case 0x20234U:
        case 0x20238U:
        case 0x2023cU:
        case 0x20240U:
        case 0x20244U:
        case 0x20248U:
        case 0x2024eU:
        case 0x20250U:
        case 0x20254U:
        case 0x20258U:
        case 0x2025aU:
        case 0x2025eU:
        case 0x20260U:
        case 0x20262U:
        case 0x20264U:
        case 0x20268U:
        case 0x2026cU:
        case 0x20270U:
        case 0x20276U:
        case 0x2027aU:
        case 0x2027cU:
        case 0x20280U:
        case 0x20288U:
        case 0x2028eU:
        case 0x20294U:
        case 0x2029aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
