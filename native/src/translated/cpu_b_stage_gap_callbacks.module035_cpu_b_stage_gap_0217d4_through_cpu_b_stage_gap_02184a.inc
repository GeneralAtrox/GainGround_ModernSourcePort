
FunctionResult cpu_b_stage_gap_0217d4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x217d4U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x217daU;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x217daU: { // 671e beq.b $217fa
            next = 0x217dcU;
            if ((r.status & 4U) != 0U) { next = 0x217faU; transfer_kind = 1U; }
            break;
        }
        case 0x217dcU: { // 6100b7d0 bsr.w $1cfae
            next = 0x217e0U;
            const auto result = m.call(c, 686U, 0x217dcU, 0x1cfaeU, 0x217e0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x217e0U: { // 6100c3e2 bsr.w $1dbc4
            next = 0x217e4U;
            const auto result = m.call(c, 352U, 0x217e0U, 0x1dbc4U, 0x217e4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x217e4U: { // 6100c93e bsr.w $1e124
            next = 0x217e8U;
            const auto result = m.call(c, 357U, 0x217e4U, 0x1e124U, 0x217e8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x217e8U: { // 6100d010 bsr.w $1e7fa
            next = 0x217ecU;
            const auto result = m.call(c, 687U, 0x217e8U, 0x1e7faU, 0x217ecU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x217ecU: { // 6100d4f6 bsr.w $1ece4
            next = 0x217f0U;
            const auto result = m.call(c, 365U, 0x217ecU, 0x1ece4U, 0x217f0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x217f0U: { // 4a2d003f tst.b $3f(a5)
            next = 0x217f4U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x217f4U: { // 6602 bne.b $217f8
            next = 0x217f6U;
            if ((r.status & 4U) == 0U) { next = 0x217f8U; transfer_kind = 1U; }
            break;
        }
        case 0x217f6U: { // 4e75 rts 
            next = 0x217f8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x217f6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x217f8U: { // 6050 bra.b $2184a
            next = 0x217faU;
            next = 0x2184aU; transfer_kind = 1U;
            break;
        }
        case 0x217faU: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x21800U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21800U: { // 6724 beq.b $21826
            next = 0x21802U;
            if ((r.status & 4U) != 0U) { next = 0x21826U; transfer_kind = 1U; }
            break;
        }
        case 0x21802U: { // 6100a82a bsr.w $1c02e
            next = 0x21806U;
            const auto result = m.call(c, 688U, 0x21802U, 0x1c02eU, 0x21806U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21806U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2180aU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x2180aU: { // 6608 bne.b $21814
            next = 0x2180cU;
            if ((r.status & 4U) == 0U) { next = 0x21814U; transfer_kind = 1U; }
            break;
        }
        case 0x2180cU: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x21812U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21812U: { // 6604 bne.b $21818
            next = 0x21814U;
            if ((r.status & 4U) == 0U) { next = 0x21818U; transfer_kind = 1U; }
            break;
        }
        case 0x21814U: { // 61000034 bsr.w $2184a
            next = 0x21818U;
            const auto result = m.call(c, 689U, 0x21814U, 0x2184aU, 0x21818U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21818U: { // 6100c3d6 bsr.w $1dbf0
            next = 0x2181cU;
            const auto result = m.call(c, 353U, 0x21818U, 0x1dbf0U, 0x2181cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2181cU: { // 6100d9de bsr.w $1f1fc
            next = 0x21820U;
            const auto result = m.call(c, 371U, 0x2181cU, 0x1f1fcU, 0x21820U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21820U: { // 6100cfd8 bsr.w $1e7fa
            next = 0x21824U;
            const auto result = m.call(c, 687U, 0x21820U, 0x1e7faU, 0x21824U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21824U: { // 4e75 rts 
            next = 0x21826U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21824U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21826U: { // 08ad00030078 bclr.b #$3, $78(a5)
            next = 0x2182cU;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x78U), (v1 & ~(1U << 3U)) & 0xffU);
            break;
        }
        case 0x2182cU: { // 6100a4de bsr.w $1bd0c
            next = 0x21830U;
            const auto result = m.call(c, 326U, 0x2182cU, 0x1bd0cU, 0x21830U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21830U: { // 6100d7aa bsr.w $1efdc
            next = 0x21834U;
            const auto result = m.call(c, 368U, 0x21830U, 0x1efdcU, 0x21834U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21834U: { // 6100c38e bsr.w $1dbc4
            next = 0x21838U;
            const auto result = m.call(c, 352U, 0x21834U, 0x1dbc4U, 0x21838U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21838U: { // 6100c3b6 bsr.w $1dbf0
            next = 0x2183cU;
            const auto result = m.call(c, 353U, 0x21838U, 0x1dbf0U, 0x2183cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2183cU: { // 6100c8e6 bsr.w $1e124
            next = 0x21840U;
            const auto result = m.call(c, 357U, 0x2183cU, 0x1e124U, 0x21840U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21840U: { // 6100cac2 bsr.w $1e304
            next = 0x21844U;
            const auto result = m.call(c, 360U, 0x21840U, 0x1e304U, 0x21844U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21844U: { // 6100d49e bsr.w $1ece4
            next = 0x21848U;
            const auto result = m.call(c, 365U, 0x21844U, 0x1ece4U, 0x21848U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21848U: { // 4e75 rts 
            next = 0x2184aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21848U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2184aU: { // 4df83400 lea.l $3400.w, a6
            next = 0x2184eU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x2184eU: { // 522e002a addq.b #$1, $2a(a6)
            next = 0x21852U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x2aU)), 0x1U, 8U);
            m.byte((r.address[6] + 0x2aU), v1);
            break;
        }
        case 0x21852U: { // 4e75 rts 
            next = 0x21854U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21852U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x217d4U:
        case 0x217daU:
        case 0x217dcU:
        case 0x217e0U:
        case 0x217e4U:
        case 0x217e8U:
        case 0x217ecU:
        case 0x217f0U:
        case 0x217f4U:
        case 0x217f6U:
        case 0x217f8U:
        case 0x217faU:
        case 0x21800U:
        case 0x21802U:
        case 0x21806U:
        case 0x2180aU:
        case 0x2180cU:
        case 0x21812U:
        case 0x21814U:
        case 0x21818U:
        case 0x2181cU:
        case 0x21820U:
        case 0x21824U:
        case 0x21826U:
        case 0x2182cU:
        case 0x21830U:
        case 0x21834U:
        case 0x21838U:
        case 0x2183cU:
        case 0x21840U:
        case 0x21844U:
        case 0x21848U:
        case 0x2184aU:
        case 0x2184eU:
        case 0x21852U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_02184a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2184aU: { // 4df83400 lea.l $3400.w, a6
            next = 0x2184eU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x2184eU: { // 522e002a addq.b #$1, $2a(a6)
            next = 0x21852U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x2aU)), 0x1U, 8U);
            m.byte((r.address[6] + 0x2aU), v1);
            break;
        }
        case 0x21852U: { // 4e75 rts 
            next = 0x21854U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21852U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2184aU:
        case 0x2184eU:
        case 0x21852U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
