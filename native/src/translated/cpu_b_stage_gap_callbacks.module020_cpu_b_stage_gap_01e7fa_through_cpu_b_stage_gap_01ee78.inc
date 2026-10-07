
FunctionResult cpu_b_stage_gap_01e7fa(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e220U: { // 6100ff36 bsr.w $1e158
            next = 0x1e224U;
            const auto result = m.call(c, 358U, 0x1e220U, 0x1e158U, 0x1e224U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e22aU: { // 4eb900015d24 jsr $15d24.l
            next = 0x1e230U;
            const auto result = m.indirect_call(c, 0x1e22aU, 0x15d24U, 0x1e230U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e230U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1e236U;
            const auto result = m.indirect_call(c, 0x1e230U, 0x15df2U, 0x1e236U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e236U: { // 4e75 rts 
            next = 0x1e238U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1e236U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1e7faU: { // 610003f4 bsr.w $1ebf0
            next = 0x1e7feU;
            const auto result = m.call(c, 363U, 0x1e7faU, 0x1ebf0U, 0x1e7feU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        default:
            if (!(cpu_b_stage_gap_01e7fa_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_gap_01e7fa_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_gap_01e7fa_dispatch_cases_002(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_gap_01e7fa_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

FunctionResult cpu_b_stage_gap_01ee78(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1ee78U: { // 206d0050 movea.l $50(a5), a0
            next = 0x1ee7cU;
            const auto v1 = m.lng((r.address[5] + 0x50U));
            r.address[0] = v1;
            break;
        }
        case 0x1ee7cU: { // 302d0054 move.w $54(a5), d0
            next = 0x1ee80U;
            const auto v1 = m.word((r.address[5] + 0x54U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ee80U: { // b018 cmp.b (a0)+, d0
            next = 0x1ee82U;
            const auto ea1 = r.address[0]; r.address[0] += 1U;
            (void)m.sub((r.data[0] & 0xffU), m.byte(ea1), 8U, true);
            break;
        }
        case 0x1ee82U: { // 6d06 blt.b $1ee8a
            next = 0x1ee84U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1ee8aU; transfer_kind = 1U; }
            break;
        }
        case 0x1ee84U: { // d0fc0005 adda.w #$5, a0
            next = 0x1ee88U;
            r.address[0] += 0x5U;
            break;
        }
        case 0x1ee88U: { // 60f6 bra.b $1ee80
            next = 0x1ee8aU;
            next = 0x1ee80U; transfer_kind = 1U;
            break;
        }
        case 0x1ee8aU: { // 303c0c00 move.w #$c00, d0
            next = 0x1ee8eU;
            const auto v1 = 0xc00U;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ee8eU: { // d018 add.b (a0)+, d0
            next = 0x1ee90U;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = m.add((r.data[0] & 0xffU), m.byte(ea2), 8U);
            m.db(0, v1);
            break;
        }
        case 0x1ee90U: { // 3b400008 move.w d0, $8(a5)
            next = 0x1ee94U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ee94U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1ee98U;
            const auto ea2 = r.address[0]; r.address[0] += 2U;
            const auto v1 = m.word(ea2);
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ee98U: { // 1218 move.b (a0)+, d1
            next = 0x1ee9aU;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = m.byte(ea2);
            m.db(1, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1ee9aU: { // 6714 beq.b $1eeb0
            next = 0x1ee9cU;
            if ((r.status & 4U) != 0U) { next = 0x1eeb0U; transfer_kind = 1U; }
            break;
        }
        case 0x1ee9cU: { // 08d50007 bset.b #$7, (a5)
            next = 0x1eea0U;
            const auto v1 = m.byte(r.address[5]);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 7U) & 1U) ? 0U : 4U));
            m.byte(r.address[5], (v1 | (1U << 7U)) & 0xffU);
            break;
        }
        case 0x1eea0U: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x1eea4U;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = m.byte(ea2);
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1eea4U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1eea8U;
            const auto v1 = m.add(m.word((r.address[5] + 0x54U)), 0x1U, 16U);
            m.word((r.address[5] + 0x54U), v1);
            break;
        }
        case 0x1eea8U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1eeaeU;
            const auto result = m.indirect_call(c, 0x1eea8U, 0x15df2U, 0x1eeaeU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1eeaeU: { // 4e75 rts 
            next = 0x1eeb0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1eeaeU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1eeb0U: { // 08950007 bclr.b #$7, (a5)
            next = 0x1eeb4U;
            const auto v1 = m.byte(r.address[5]);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 7U) & 1U) ? 0U : 4U));
            m.byte(r.address[5], (v1 & ~(1U << 7U)) & 0xffU);
            break;
        }
        case 0x1eeb4U: { // 4e75 rts 
            next = 0x1eeb6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1eeb4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1ee78U:
        case 0x1ee7cU:
        case 0x1ee80U:
        case 0x1ee82U:
        case 0x1ee84U:
        case 0x1ee88U:
        case 0x1ee8aU:
        case 0x1ee8eU:
        case 0x1ee90U:
        case 0x1ee94U:
        case 0x1ee98U:
        case 0x1ee9aU:
        case 0x1ee9cU:
        case 0x1eea0U:
        case 0x1eea4U:
        case 0x1eea8U:
        case 0x1eeaeU:
        case 0x1eeb0U:
        case 0x1eeb4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
