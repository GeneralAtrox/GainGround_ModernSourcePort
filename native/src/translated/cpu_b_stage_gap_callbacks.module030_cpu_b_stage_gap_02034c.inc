
FunctionResult cpu_b_stage_gap_02034c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2034cU: { // 4a2d003f tst.b $3f(a5)
            next = 0x20350U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x20350U: { // 666a bne.b $203bc
            next = 0x20352U;
            if ((r.status & 4U) == 0U) { next = 0x203bcU; transfer_kind = 1U; }
            break;
        }
        case 0x20352U: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x20356U;
            const auto v1 = m.sub(m.word((r.address[5] + 0x74U)), 0x1U, 16U);
            m.word((r.address[5] + 0x74U), v1);
            break;
        }
        case 0x20356U: { // 6f64 ble.b $203bc
            next = 0x20358U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x203bcU; transfer_kind = 1U; }
            break;
        }
        case 0x20358U: { // 610003ca bsr.w $20724
            next = 0x2035cU;
            const auto result = m.call(c, 387U, 0x20358U, 0x20724U, 0x2035cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2035cU: { // 655e bcs.b $203bc
            next = 0x2035eU;
            if ((r.status & 1U) != 0U) { next = 0x203bcU; transfer_kind = 1U; }
            break;
        }
        case 0x2035eU: { // 610002ac bsr.w $2060c
            next = 0x20362U;
            const auto result = m.call(c, 385U, 0x2035eU, 0x2060cU, 0x20362U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20362U: { // 6558 bcs.b $203bc
            next = 0x20364U;
            if ((r.status & 1U) != 0U) { next = 0x203bcU; transfer_kind = 1U; }
            break;
        }
        case 0x20364U: { // 0c6d00010056 cmpi.w #$1, $56(a5)
            next = 0x2036aU;
            (void)m.sub(m.word((r.address[5] + 0x56U)), 0x1U, 16U, true);
            break;
        }
        case 0x2036aU: { // 6d10 blt.b $2037c
            next = 0x2036cU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x2037cU; transfer_kind = 1U; }
            break;
        }
        case 0x2036cU: { // 202d001e move.l $1e(a5), d0
            next = 0x20370U;
            const auto v1 = m.lng((r.address[5] + 0x1eU));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x20370U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x20374U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x12U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x12U), v1);
            break;
        }
        case 0x20374U: { // 202d0026 move.l $26(a5), d0
            next = 0x20378U;
            const auto v1 = m.lng((r.address[5] + 0x26U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x20378U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x2037cU;
            const auto v1 = m.add(m.lng((r.address[5] + 0x1aU)), r.data[0], 32U);
            m.lng((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x2037cU: { // 302d0056 move.w $56(a5), d0
            next = 0x20380U;
            const auto v1 = m.word((r.address[5] + 0x56U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20380U: { // 5240 addq.w #$1, d0
            next = 0x20382U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x1U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x20382U: { // 3b400056 move.w d0, $56(a5)
            next = 0x20386U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x56U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20386U: { // 41f900032cae lea.l $32cae.l, a0
            next = 0x2038cU;
            r.address[0] = 0x32caeU;
            break;
        }
        case 0x2038cU: { // b058 cmp.w (a0)+, d0
            next = 0x2038eU;
            const auto ea1 = r.address[0]; r.address[0] += 2U;
            (void)m.sub((r.data[0] & 0xffffU), m.word(ea1), 16U, true);
            break;
        }
        case 0x2038eU: { // 6f06 ble.b $20396
            next = 0x20390U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x20396U; transfer_kind = 1U; }
            break;
        }
        case 0x20390U: { // d0fc0006 adda.w #$6, a0
            next = 0x20394U;
            r.address[0] += 0x6U;
            break;
        }
        case 0x20394U: { // 60f6 bra.b $2038c
            next = 0x20396U;
            next = 0x2038cU; transfer_kind = 1U;
            break;
        }
        case 0x20396U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x2039aU;
            const auto ea2 = r.address[0]; r.address[0] += 2U;
            const auto v1 = m.word(ea2);
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2039aU: { // 3b580008 move.w (a0)+, $8(a5)
            next = 0x2039eU;
            const auto ea2 = r.address[0]; r.address[0] += 2U;
            const auto v1 = m.word(ea2);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2039eU: { // 3b580010 move.w (a0)+, $10(a5)
            next = 0x203a2U;
            const auto ea2 = r.address[0]; r.address[0] += 2U;
            const auto v1 = m.word(ea2);
            m.word((r.address[5] + 0x10U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x203a2U: { // 08ed00020001 bset.b #$2, $1(a5)
            next = 0x203a8U;
            const auto v1 = m.byte((r.address[5] + 0x1U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x1U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x203a8U: { // 4eb900015d24 jsr $15d24.l
            next = 0x203aeU;
            const auto result = m.indirect_call(c, 0x203a8U, 0x15d24U, 0x203aeU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x203aeU: { // 4eb9000207c2 jsr $207c2.l
            next = 0x203b4U;
            const auto result = m.indirect_call(c, 0x203aeU, 0x207c2U, 0x203b4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x203b4U: { // 4eb900015df2 jsr $15df2.l
            next = 0x203baU;
            const auto result = m.indirect_call(c, 0x203b4U, 0x15df2U, 0x203baU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x203baU: { // 4e75 rts 
            next = 0x203bcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x203baU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x203bcU: { // 610004de bsr.w $2089c
            next = 0x203c0U;
            const auto result = m.call(c, 389U, 0x203bcU, 0x2089cU, 0x203c0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x203c0U: { // 4e75 rts 
            next = 0x203c2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x203c0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2034cU:
        case 0x20350U:
        case 0x20352U:
        case 0x20356U:
        case 0x20358U:
        case 0x2035cU:
        case 0x2035eU:
        case 0x20362U:
        case 0x20364U:
        case 0x2036aU:
        case 0x2036cU:
        case 0x20370U:
        case 0x20374U:
        case 0x20378U:
        case 0x2037cU:
        case 0x20380U:
        case 0x20382U:
        case 0x20386U:
        case 0x2038cU:
        case 0x2038eU:
        case 0x20390U:
        case 0x20394U:
        case 0x20396U:
        case 0x2039aU:
        case 0x2039eU:
        case 0x203a2U:
        case 0x203a8U:
        case 0x203aeU:
        case 0x203b4U:
        case 0x203baU:
        case 0x203bcU:
        case 0x203c0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
