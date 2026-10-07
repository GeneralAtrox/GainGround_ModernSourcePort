
FunctionResult cpu_b_stage_gap_021854(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21854U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x2185aU;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x2185aU: { // 671e beq.b $2187a
            next = 0x2185cU;
            if ((r.status & 4U) != 0U) { next = 0x2187aU; transfer_kind = 1U; }
            break;
        }
        case 0x2185cU: { // 6100b750 bsr.w $1cfae
            next = 0x21860U;
            const auto result = m.call(c, 686U, 0x2185cU, 0x1cfaeU, 0x21860U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21860U: { // 6100c362 bsr.w $1dbc4
            next = 0x21864U;
            const auto result = m.call(c, 352U, 0x21860U, 0x1dbc4U, 0x21864U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21864U: { // 6100c8be bsr.w $1e124
            next = 0x21868U;
            const auto result = m.call(c, 357U, 0x21864U, 0x1e124U, 0x21868U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21868U: { // 6100cf90 bsr.w $1e7fa
            next = 0x2186cU;
            const auto result = m.call(c, 687U, 0x21868U, 0x1e7faU, 0x2186cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2186cU: { // 6100d476 bsr.w $1ece4
            next = 0x21870U;
            const auto result = m.call(c, 365U, 0x2186cU, 0x1ece4U, 0x21870U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21870U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21874U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x21874U: { // 6602 bne.b $21878
            next = 0x21876U;
            if ((r.status & 4U) == 0U) { next = 0x21878U; transfer_kind = 1U; }
            break;
        }
        case 0x21876U: { // 4e75 rts 
            next = 0x21878U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21876U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21878U: { // 6050 bra.b $218ca
            next = 0x2187aU;
            next = 0x218caU; transfer_kind = 1U;
            break;
        }
        case 0x2187aU: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x21880U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21880U: { // 6724 beq.b $218a6
            next = 0x21882U;
            if ((r.status & 4U) != 0U) { next = 0x218a6U; transfer_kind = 1U; }
            break;
        }
        case 0x21882U: { // 6100a7aa bsr.w $1c02e
            next = 0x21886U;
            const auto result = m.call(c, 688U, 0x21882U, 0x1c02eU, 0x21886U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21886U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2188aU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x2188aU: { // 6608 bne.b $21894
            next = 0x2188cU;
            if ((r.status & 4U) == 0U) { next = 0x21894U; transfer_kind = 1U; }
            break;
        }
        case 0x2188cU: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x21892U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21892U: { // 6604 bne.b $21898
            next = 0x21894U;
            if ((r.status & 4U) == 0U) { next = 0x21898U; transfer_kind = 1U; }
            break;
        }
        case 0x21894U: { // 61000034 bsr.w $218ca
            next = 0x21898U;
            const auto result = m.call(c, 691U, 0x21894U, 0x218caU, 0x21898U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21898U: { // 6100c356 bsr.w $1dbf0
            next = 0x2189cU;
            const auto result = m.call(c, 353U, 0x21898U, 0x1dbf0U, 0x2189cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2189cU: { // 6100d95e bsr.w $1f1fc
            next = 0x218a0U;
            const auto result = m.call(c, 371U, 0x2189cU, 0x1f1fcU, 0x218a0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218a0U: { // 6100cf58 bsr.w $1e7fa
            next = 0x218a4U;
            const auto result = m.call(c, 687U, 0x218a0U, 0x1e7faU, 0x218a4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218a4U: { // 4e75 rts 
            next = 0x218a6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x218a4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x218a6U: { // 08ad00030078 bclr.b #$3, $78(a5)
            next = 0x218acU;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x78U), (v1 & ~(1U << 3U)) & 0xffU);
            break;
        }
        case 0x218acU: { // 6100a45e bsr.w $1bd0c
            next = 0x218b0U;
            const auto result = m.call(c, 326U, 0x218acU, 0x1bd0cU, 0x218b0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218b0U: { // 6100d72a bsr.w $1efdc
            next = 0x218b4U;
            const auto result = m.call(c, 368U, 0x218b0U, 0x1efdcU, 0x218b4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218b4U: { // 6100c30e bsr.w $1dbc4
            next = 0x218b8U;
            const auto result = m.call(c, 352U, 0x218b4U, 0x1dbc4U, 0x218b8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218b8U: { // 6100c336 bsr.w $1dbf0
            next = 0x218bcU;
            const auto result = m.call(c, 353U, 0x218b8U, 0x1dbf0U, 0x218bcU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218bcU: { // 6100c866 bsr.w $1e124
            next = 0x218c0U;
            const auto result = m.call(c, 357U, 0x218bcU, 0x1e124U, 0x218c0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218c0U: { // 6100ca42 bsr.w $1e304
            next = 0x218c4U;
            const auto result = m.call(c, 360U, 0x218c0U, 0x1e304U, 0x218c4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218c4U: { // 6100d41e bsr.w $1ece4
            next = 0x218c8U;
            const auto result = m.call(c, 365U, 0x218c4U, 0x1ece4U, 0x218c8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218c8U: { // 4e75 rts 
            next = 0x218caU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x218c8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x218caU: { // 4df83400 lea.l $3400.w, a6
            next = 0x218ceU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x218ceU: { // 522e002c addq.b #$1, $2c(a6)
            next = 0x218d2U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x2cU)), 0x1U, 8U);
            m.byte((r.address[6] + 0x2cU), v1);
            break;
        }
        case 0x218d2U: { // 4e75 rts 
            next = 0x218d4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x218d2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21854U:
        case 0x2185aU:
        case 0x2185cU:
        case 0x21860U:
        case 0x21864U:
        case 0x21868U:
        case 0x2186cU:
        case 0x21870U:
        case 0x21874U:
        case 0x21876U:
        case 0x21878U:
        case 0x2187aU:
        case 0x21880U:
        case 0x21882U:
        case 0x21886U:
        case 0x2188aU:
        case 0x2188cU:
        case 0x21892U:
        case 0x21894U:
        case 0x21898U:
        case 0x2189cU:
        case 0x218a0U:
        case 0x218a4U:
        case 0x218a6U:
        case 0x218acU:
        case 0x218b0U:
        case 0x218b4U:
        case 0x218b8U:
        case 0x218bcU:
        case 0x218c0U:
        case 0x218c4U:
        case 0x218c8U:
        case 0x218caU:
        case 0x218ceU:
        case 0x218d2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_0218ca(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x218caU: { // 4df83400 lea.l $3400.w, a6
            next = 0x218ceU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x218ceU: { // 522e002c addq.b #$1, $2c(a6)
            next = 0x218d2U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x2cU)), 0x1U, 8U);
            m.byte((r.address[6] + 0x2cU), v1);
            break;
        }
        case 0x218d2U: { // 4e75 rts 
            next = 0x218d4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x218d2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x218caU:
        case 0x218ceU:
        case 0x218d2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
