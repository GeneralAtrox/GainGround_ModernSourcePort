
FunctionResult cpu_b_stage_gap_0218d4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x218d4U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x218daU;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x218daU: { // 671e beq.b $218fa
            next = 0x218dcU;
            if ((r.status & 4U) != 0U) { next = 0x218faU; transfer_kind = 1U; }
            break;
        }
        case 0x218dcU: { // 6100b6d0 bsr.w $1cfae
            next = 0x218e0U;
            const auto result = m.call(c, 686U, 0x218dcU, 0x1cfaeU, 0x218e0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218e0U: { // 6100c2e2 bsr.w $1dbc4
            next = 0x218e4U;
            const auto result = m.call(c, 352U, 0x218e0U, 0x1dbc4U, 0x218e4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218e4U: { // 6100c83e bsr.w $1e124
            next = 0x218e8U;
            const auto result = m.call(c, 357U, 0x218e4U, 0x1e124U, 0x218e8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218e8U: { // 6100cf10 bsr.w $1e7fa
            next = 0x218ecU;
            const auto result = m.call(c, 687U, 0x218e8U, 0x1e7faU, 0x218ecU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218ecU: { // 6100d3f6 bsr.w $1ece4
            next = 0x218f0U;
            const auto result = m.call(c, 365U, 0x218ecU, 0x1ece4U, 0x218f0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x218f0U: { // 4a2d003f tst.b $3f(a5)
            next = 0x218f4U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x218f4U: { // 6602 bne.b $218f8
            next = 0x218f6U;
            if ((r.status & 4U) == 0U) { next = 0x218f8U; transfer_kind = 1U; }
            break;
        }
        case 0x218f6U: { // 4e75 rts 
            next = 0x218f8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x218f6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x218f8U: { // 6050 bra.b $2194a
            next = 0x218faU;
            next = 0x2194aU; transfer_kind = 1U;
            break;
        }
        case 0x218faU: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x21900U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21900U: { // 6724 beq.b $21926
            next = 0x21902U;
            if ((r.status & 4U) != 0U) { next = 0x21926U; transfer_kind = 1U; }
            break;
        }
        case 0x21902U: { // 6100a72a bsr.w $1c02e
            next = 0x21906U;
            const auto result = m.call(c, 688U, 0x21902U, 0x1c02eU, 0x21906U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21906U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2190aU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x2190aU: { // 6608 bne.b $21914
            next = 0x2190cU;
            if ((r.status & 4U) == 0U) { next = 0x21914U; transfer_kind = 1U; }
            break;
        }
        case 0x2190cU: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x21912U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21912U: { // 6604 bne.b $21918
            next = 0x21914U;
            if ((r.status & 4U) == 0U) { next = 0x21918U; transfer_kind = 1U; }
            break;
        }
        case 0x21914U: { // 61000034 bsr.w $2194a
            next = 0x21918U;
            const auto result = m.call(c, 693U, 0x21914U, 0x2194aU, 0x21918U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21918U: { // 6100c2d6 bsr.w $1dbf0
            next = 0x2191cU;
            const auto result = m.call(c, 353U, 0x21918U, 0x1dbf0U, 0x2191cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2191cU: { // 6100d8de bsr.w $1f1fc
            next = 0x21920U;
            const auto result = m.call(c, 371U, 0x2191cU, 0x1f1fcU, 0x21920U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21920U: { // 6100ced8 bsr.w $1e7fa
            next = 0x21924U;
            const auto result = m.call(c, 687U, 0x21920U, 0x1e7faU, 0x21924U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21924U: { // 4e75 rts 
            next = 0x21926U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21924U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21926U: { // 08ad00030078 bclr.b #$3, $78(a5)
            next = 0x2192cU;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x78U), (v1 & ~(1U << 3U)) & 0xffU);
            break;
        }
        case 0x2192cU: { // 6100a3de bsr.w $1bd0c
            next = 0x21930U;
            const auto result = m.call(c, 326U, 0x2192cU, 0x1bd0cU, 0x21930U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21930U: { // 6100d6aa bsr.w $1efdc
            next = 0x21934U;
            const auto result = m.call(c, 368U, 0x21930U, 0x1efdcU, 0x21934U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21934U: { // 6100c28e bsr.w $1dbc4
            next = 0x21938U;
            const auto result = m.call(c, 352U, 0x21934U, 0x1dbc4U, 0x21938U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21938U: { // 6100c2b6 bsr.w $1dbf0
            next = 0x2193cU;
            const auto result = m.call(c, 353U, 0x21938U, 0x1dbf0U, 0x2193cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2193cU: { // 6100c7e6 bsr.w $1e124
            next = 0x21940U;
            const auto result = m.call(c, 357U, 0x2193cU, 0x1e124U, 0x21940U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21940U: { // 6100c9c2 bsr.w $1e304
            next = 0x21944U;
            const auto result = m.call(c, 360U, 0x21940U, 0x1e304U, 0x21944U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21944U: { // 6100d39e bsr.w $1ece4
            next = 0x21948U;
            const auto result = m.call(c, 365U, 0x21944U, 0x1ece4U, 0x21948U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21948U: { // 4e75 rts 
            next = 0x2194aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21948U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2194aU: { // 4df83400 lea.l $3400.w, a6
            next = 0x2194eU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x2194eU: { // 522e002e addq.b #$1, $2e(a6)
            next = 0x21952U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x2eU)), 0x1U, 8U);
            m.byte((r.address[6] + 0x2eU), v1);
            break;
        }
        case 0x21952U: { // 4e75 rts 
            next = 0x21954U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21952U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x218d4U:
        case 0x218daU:
        case 0x218dcU:
        case 0x218e0U:
        case 0x218e4U:
        case 0x218e8U:
        case 0x218ecU:
        case 0x218f0U:
        case 0x218f4U:
        case 0x218f6U:
        case 0x218f8U:
        case 0x218faU:
        case 0x21900U:
        case 0x21902U:
        case 0x21906U:
        case 0x2190aU:
        case 0x2190cU:
        case 0x21912U:
        case 0x21914U:
        case 0x21918U:
        case 0x2191cU:
        case 0x21920U:
        case 0x21924U:
        case 0x21926U:
        case 0x2192cU:
        case 0x21930U:
        case 0x21934U:
        case 0x21938U:
        case 0x2193cU:
        case 0x21940U:
        case 0x21944U:
        case 0x21948U:
        case 0x2194aU:
        case 0x2194eU:
        case 0x21952U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_02194a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2194aU: { // 4df83400 lea.l $3400.w, a6
            next = 0x2194eU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x2194eU: { // 522e002e addq.b #$1, $2e(a6)
            next = 0x21952U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x2eU)), 0x1U, 8U);
            m.byte((r.address[6] + 0x2eU), v1);
            break;
        }
        case 0x21952U: { // 4e75 rts 
            next = 0x21954U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21952U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2194aU:
        case 0x2194eU:
        case 0x21952U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
