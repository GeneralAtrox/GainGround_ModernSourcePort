
FunctionResult cpu_b_stage_gap_021ba0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21ba0U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x21ba6U;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21ba6U: { // 671e beq.b $21bc6
            next = 0x21ba8U;
            if ((r.status & 4U) != 0U) { next = 0x21bc6U; transfer_kind = 1U; }
            break;
        }
        case 0x21ba8U: { // 6100b404 bsr.w $1cfae
            next = 0x21bacU;
            const auto result = m.call(c, 686U, 0x21ba8U, 0x1cfaeU, 0x21bacU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21bacU: { // 6100c016 bsr.w $1dbc4
            next = 0x21bb0U;
            const auto result = m.call(c, 352U, 0x21bacU, 0x1dbc4U, 0x21bb0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21bb0U: { // 6100c572 bsr.w $1e124
            next = 0x21bb4U;
            const auto result = m.call(c, 357U, 0x21bb0U, 0x1e124U, 0x21bb4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21bb4U: { // 6100cc44 bsr.w $1e7fa
            next = 0x21bb8U;
            const auto result = m.call(c, 687U, 0x21bb4U, 0x1e7faU, 0x21bb8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21bb8U: { // 6100d12a bsr.w $1ece4
            next = 0x21bbcU;
            const auto result = m.call(c, 365U, 0x21bb8U, 0x1ece4U, 0x21bbcU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21bbcU: { // 4a2d003f tst.b $3f(a5)
            next = 0x21bc0U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x21bc0U: { // 6602 bne.b $21bc4
            next = 0x21bc2U;
            if ((r.status & 4U) == 0U) { next = 0x21bc4U; transfer_kind = 1U; }
            break;
        }
        case 0x21bc2U: { // 4e75 rts 
            next = 0x21bc4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21bc2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21bc4U: { // 6050 bra.b $21c16
            next = 0x21bc6U;
            next = 0x21c16U; transfer_kind = 1U;
            break;
        }
        case 0x21bc6U: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x21bccU;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21bccU: { // 6724 beq.b $21bf2
            next = 0x21bceU;
            if ((r.status & 4U) != 0U) { next = 0x21bf2U; transfer_kind = 1U; }
            break;
        }
        case 0x21bceU: { // 6100a45e bsr.w $1c02e
            next = 0x21bd2U;
            const auto result = m.call(c, 688U, 0x21bceU, 0x1c02eU, 0x21bd2U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21bd2U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21bd6U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x21bd6U: { // 6608 bne.b $21be0
            next = 0x21bd8U;
            if ((r.status & 4U) == 0U) { next = 0x21be0U; transfer_kind = 1U; }
            break;
        }
        case 0x21bd8U: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x21bdeU;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21bdeU: { // 6604 bne.b $21be4
            next = 0x21be0U;
            if ((r.status & 4U) == 0U) { next = 0x21be4U; transfer_kind = 1U; }
            break;
        }
        case 0x21be0U: { // 61000034 bsr.w $21c16
            next = 0x21be4U;
            const auto result = m.call(c, 690U, 0x21be0U, 0x21c16U, 0x21be4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21be4U: { // 6100c00a bsr.w $1dbf0
            next = 0x21be8U;
            const auto result = m.call(c, 353U, 0x21be4U, 0x1dbf0U, 0x21be8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21be8U: { // 6100d612 bsr.w $1f1fc
            next = 0x21becU;
            const auto result = m.call(c, 371U, 0x21be8U, 0x1f1fcU, 0x21becU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21becU: { // 6100cc0c bsr.w $1e7fa
            next = 0x21bf0U;
            const auto result = m.call(c, 687U, 0x21becU, 0x1e7faU, 0x21bf0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21bf0U: { // 4e75 rts 
            next = 0x21bf2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21bf0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21bf2U: { // 08ad00030078 bclr.b #$3, $78(a5)
            next = 0x21bf8U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x78U), (v1 & ~(1U << 3U)) & 0xffU);
            break;
        }
        case 0x21bf8U: { // 61009e92 bsr.w $1ba8c
            next = 0x21bfcU;
            const auto result = m.call(c, 562U, 0x21bf8U, 0x1ba8cU, 0x21bfcU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21bfcU: { // 6100d3de bsr.w $1efdc
            next = 0x21c00U;
            const auto result = m.call(c, 368U, 0x21bfcU, 0x1efdcU, 0x21c00U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c00U: { // 6100bfc2 bsr.w $1dbc4
            next = 0x21c04U;
            const auto result = m.call(c, 352U, 0x21c00U, 0x1dbc4U, 0x21c04U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c04U: { // 6100bfea bsr.w $1dbf0
            next = 0x21c08U;
            const auto result = m.call(c, 353U, 0x21c04U, 0x1dbf0U, 0x21c08U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c08U: { // 6100c51a bsr.w $1e124
            next = 0x21c0cU;
            const auto result = m.call(c, 357U, 0x21c08U, 0x1e124U, 0x21c0cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c0cU: { // 6100c6f6 bsr.w $1e304
            next = 0x21c10U;
            const auto result = m.call(c, 360U, 0x21c0cU, 0x1e304U, 0x21c10U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c10U: { // 6100d0d2 bsr.w $1ece4
            next = 0x21c14U;
            const auto result = m.call(c, 365U, 0x21c10U, 0x1ece4U, 0x21c14U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c14U: { // 4e75 rts 
            next = 0x21c16U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21c14U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21c16U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21c1aU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x21c1aU: { // 522e0028 addq.b #$1, $28(a6)
            next = 0x21c1eU;
            const auto v1 = m.add(m.byte((r.address[6] + 0x28U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x28U), v1);
            break;
        }
        case 0x21c1eU: { // 4e75 rts 
            next = 0x21c20U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21c1eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21ba0U:
        case 0x21ba6U:
        case 0x21ba8U:
        case 0x21bacU:
        case 0x21bb0U:
        case 0x21bb4U:
        case 0x21bb8U:
        case 0x21bbcU:
        case 0x21bc0U:
        case 0x21bc2U:
        case 0x21bc4U:
        case 0x21bc6U:
        case 0x21bccU:
        case 0x21bceU:
        case 0x21bd2U:
        case 0x21bd6U:
        case 0x21bd8U:
        case 0x21bdeU:
        case 0x21be0U:
        case 0x21be4U:
        case 0x21be8U:
        case 0x21becU:
        case 0x21bf0U:
        case 0x21bf2U:
        case 0x21bf8U:
        case 0x21bfcU:
        case 0x21c00U:
        case 0x21c04U:
        case 0x21c08U:
        case 0x21c0cU:
        case 0x21c10U:
        case 0x21c14U:
        case 0x21c16U:
        case 0x21c1aU:
        case 0x21c1eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_021c16(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21c16U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21c1aU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x21c1aU: { // 522e0028 addq.b #$1, $28(a6)
            next = 0x21c1eU;
            const auto v1 = m.add(m.byte((r.address[6] + 0x28U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x28U), v1);
            break;
        }
        case 0x21c1eU: { // 4e75 rts 
            next = 0x21c20U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21c1eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21c16U:
        case 0x21c1aU:
        case 0x21c1eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
