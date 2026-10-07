
FunctionResult cpu_b_stage_gap_021954(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21954U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x2195aU;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x2195aU: { // 671e beq.b $2197a
            next = 0x2195cU;
            if ((r.status & 4U) != 0U) { next = 0x2197aU; transfer_kind = 1U; }
            break;
        }
        case 0x2195cU: { // 6100b650 bsr.w $1cfae
            next = 0x21960U;
            const auto result = m.call(c, 686U, 0x2195cU, 0x1cfaeU, 0x21960U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21960U: { // 6100c262 bsr.w $1dbc4
            next = 0x21964U;
            const auto result = m.call(c, 352U, 0x21960U, 0x1dbc4U, 0x21964U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21964U: { // 6100c7be bsr.w $1e124
            next = 0x21968U;
            const auto result = m.call(c, 357U, 0x21964U, 0x1e124U, 0x21968U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21968U: { // 6100ce90 bsr.w $1e7fa
            next = 0x2196cU;
            const auto result = m.call(c, 687U, 0x21968U, 0x1e7faU, 0x2196cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2196cU: { // 6100d376 bsr.w $1ece4
            next = 0x21970U;
            const auto result = m.call(c, 365U, 0x2196cU, 0x1ece4U, 0x21970U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21970U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21974U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x21974U: { // 6602 bne.b $21978
            next = 0x21976U;
            if ((r.status & 4U) == 0U) { next = 0x21978U; transfer_kind = 1U; }
            break;
        }
        case 0x21976U: { // 4e75 rts 
            next = 0x21978U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21976U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21978U: { // 6050 bra.b $219ca
            next = 0x2197aU;
            next = 0x219caU; transfer_kind = 1U;
            break;
        }
        case 0x2197aU: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x21980U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21980U: { // 6724 beq.b $219a6
            next = 0x21982U;
            if ((r.status & 4U) != 0U) { next = 0x219a6U; transfer_kind = 1U; }
            break;
        }
        case 0x21982U: { // 6100a6aa bsr.w $1c02e
            next = 0x21986U;
            const auto result = m.call(c, 688U, 0x21982U, 0x1c02eU, 0x21986U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21986U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2198aU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x2198aU: { // 6608 bne.b $21994
            next = 0x2198cU;
            if ((r.status & 4U) == 0U) { next = 0x21994U; transfer_kind = 1U; }
            break;
        }
        case 0x2198cU: { // 082d00020078 btst.b #$2, $78(a5)
            next = 0x21992U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x21992U: { // 6604 bne.b $21998
            next = 0x21994U;
            if ((r.status & 4U) == 0U) { next = 0x21998U; transfer_kind = 1U; }
            break;
        }
        case 0x21994U: { // 61000034 bsr.w $219ca
            next = 0x21998U;
            const auto result = m.call(c, 694U, 0x21994U, 0x219caU, 0x21998U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21998U: { // 6100c256 bsr.w $1dbf0
            next = 0x2199cU;
            const auto result = m.call(c, 353U, 0x21998U, 0x1dbf0U, 0x2199cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2199cU: { // 6100d85e bsr.w $1f1fc
            next = 0x219a0U;
            const auto result = m.call(c, 371U, 0x2199cU, 0x1f1fcU, 0x219a0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219a0U: { // 6100ce58 bsr.w $1e7fa
            next = 0x219a4U;
            const auto result = m.call(c, 687U, 0x219a0U, 0x1e7faU, 0x219a4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219a4U: { // 4e75 rts 
            next = 0x219a6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x219a4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x219a6U: { // 08ad00030078 bclr.b #$3, $78(a5)
            next = 0x219acU;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x78U), (v1 & ~(1U << 3U)) & 0xffU);
            break;
        }
        case 0x219acU: { // 6100a0de bsr.w $1ba8c
            next = 0x219b0U;
            const auto result = m.call(c, 562U, 0x219acU, 0x1ba8cU, 0x219b0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219b0U: { // 6100d62a bsr.w $1efdc
            next = 0x219b4U;
            const auto result = m.call(c, 368U, 0x219b0U, 0x1efdcU, 0x219b4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219b4U: { // 6100c20e bsr.w $1dbc4
            next = 0x219b8U;
            const auto result = m.call(c, 352U, 0x219b4U, 0x1dbc4U, 0x219b8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219b8U: { // 6100c236 bsr.w $1dbf0
            next = 0x219bcU;
            const auto result = m.call(c, 353U, 0x219b8U, 0x1dbf0U, 0x219bcU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219bcU: { // 6100c766 bsr.w $1e124
            next = 0x219c0U;
            const auto result = m.call(c, 357U, 0x219bcU, 0x1e124U, 0x219c0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219c0U: { // 6100c942 bsr.w $1e304
            next = 0x219c4U;
            const auto result = m.call(c, 360U, 0x219c0U, 0x1e304U, 0x219c4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219c4U: { // 6100d31e bsr.w $1ece4
            next = 0x219c8U;
            const auto result = m.call(c, 365U, 0x219c4U, 0x1ece4U, 0x219c8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219c8U: { // 4e75 rts 
            next = 0x219caU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x219c8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x219caU: { // 4df83400 lea.l $3400.w, a6
            next = 0x219ceU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x219ceU: { // 522e0030 addq.b #$1, $30(a6)
            next = 0x219d2U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x30U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x30U), v1);
            break;
        }
        case 0x219d2U: { // 4e75 rts 
            next = 0x219d4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x219d2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21954U:
        case 0x2195aU:
        case 0x2195cU:
        case 0x21960U:
        case 0x21964U:
        case 0x21968U:
        case 0x2196cU:
        case 0x21970U:
        case 0x21974U:
        case 0x21976U:
        case 0x21978U:
        case 0x2197aU:
        case 0x21980U:
        case 0x21982U:
        case 0x21986U:
        case 0x2198aU:
        case 0x2198cU:
        case 0x21992U:
        case 0x21994U:
        case 0x21998U:
        case 0x2199cU:
        case 0x219a0U:
        case 0x219a4U:
        case 0x219a6U:
        case 0x219acU:
        case 0x219b0U:
        case 0x219b4U:
        case 0x219b8U:
        case 0x219bcU:
        case 0x219c0U:
        case 0x219c4U:
        case 0x219c8U:
        case 0x219caU:
        case 0x219ceU:
        case 0x219d2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_0219ca(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x219caU: { // 4df83400 lea.l $3400.w, a6
            next = 0x219ceU;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x219ceU: { // 522e0030 addq.b #$1, $30(a6)
            next = 0x219d2U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x30U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x30U), v1);
            break;
        }
        case 0x219d2U: { // 4e75 rts 
            next = 0x219d4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x219d2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x219caU:
        case 0x219ceU:
        case 0x219d2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
