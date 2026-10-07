
FunctionResult cpu_b_stage_gap_0200e0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x200e0U: { // 4a2d003f tst.b $3f(a5)
            next = 0x200e4U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x200e4U: { // 6650 bne.b $20136
            next = 0x200e6U;
            if ((r.status & 4U) == 0U) { next = 0x20136U; transfer_kind = 1U; }
            break;
        }
        case 0x200e6U: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x200eaU;
            const auto v1 = m.sub(m.word((r.address[5] + 0x74U)), 0x1U, 16U);
            m.word((r.address[5] + 0x74U), v1);
            break;
        }
        case 0x200eaU: { // 6f4a ble.b $20136
            next = 0x200ecU;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x20136U; transfer_kind = 1U; }
            break;
        }
        case 0x200ecU: { // 61000636 bsr.w $20724
            next = 0x200f0U;
            const auto result = m.call(c, 387U, 0x200ecU, 0x20724U, 0x200f0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x200f0U: { // 6544 bcs.b $20136
            next = 0x200f2U;
            if ((r.status & 1U) != 0U) { next = 0x20136U; transfer_kind = 1U; }
            break;
        }
        case 0x200f2U: { // 61000518 bsr.w $2060c
            next = 0x200f6U;
            const auto result = m.call(c, 385U, 0x200f2U, 0x2060cU, 0x200f6U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x200f6U: { // 653e bcs.b $20136
            next = 0x200f8U;
            if ((r.status & 1U) != 0U) { next = 0x20136U; transfer_kind = 1U; }
            break;
        }
        case 0x200f8U: { // 202d001e move.l $1e(a5), d0
            next = 0x200fcU;
            const auto v1 = m.lng((r.address[5] + 0x1eU));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x200fcU: { // d1ad0012 add.l d0, $12(a5)
            next = 0x20100U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x12U)), r.data[0], 32U);
            m.lng((r.address[5] + 0x12U), v1);
            break;
        }
        case 0x20100U: { // 202d0026 move.l $26(a5), d0
            next = 0x20104U;
            const auto v1 = m.lng((r.address[5] + 0x26U));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x20104U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x20108U;
            const auto v1 = m.add(m.lng((r.address[5] + 0x1aU)), r.data[0], 32U);
            m.lng((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x20108U: { // 302d0056 move.w $56(a5), d0
            next = 0x2010cU;
            const auto v1 = m.word((r.address[5] + 0x56U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2010cU: { // 5240 addq.w #$1, d0
            next = 0x2010eU;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x1U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x2010eU: { // 0c400004 cmpi.w #$4, d0
            next = 0x20112U;
            (void)m.sub((r.data[0] & 0xffffU), 0x4U, 16U, true);
            break;
        }
        case 0x20112U: { // 6d02 blt.b $20116
            next = 0x20114U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x20116U; transfer_kind = 1U; }
            break;
        }
        case 0x20114U: { // 7000 moveq #$0, d0
            next = 0x20116U;
            r.data[0] = 0x0U; m.logic(r.data[0], 32U);
            break;
        }
        case 0x20116U: { // 3b400056 move.w d0, $56(a5)
            next = 0x2011aU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x56U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x2011aU: { // d06d005a add.w $5a(a5), d0
            next = 0x2011eU;
            const auto v1 = m.add((r.data[0] & 0xffffU), m.word((r.address[5] + 0x5aU)), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x2011eU: { // 3b400008 move.w d0, $8(a5)
            next = 0x20122U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x20122U: { // 4eb900015d24 jsr $15d24.l
            next = 0x20128U;
            const auto result = m.indirect_call(c, 0x20122U, 0x15d24U, 0x20128U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20128U: { // 4eb9000207c2 jsr $207c2.l
            next = 0x2012eU;
            const auto result = m.indirect_call(c, 0x20128U, 0x207c2U, 0x2012eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2012eU: { // 4eb900015df2 jsr $15df2.l
            next = 0x20134U;
            const auto result = m.indirect_call(c, 0x2012eU, 0x15df2U, 0x20134U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20134U: { // 4e75 rts 
            next = 0x20136U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20134U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20136U: { // 61000764 bsr.w $2089c
            next = 0x2013aU;
            const auto result = m.call(c, 389U, 0x20136U, 0x2089cU, 0x2013aU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2013aU: { // 4e75 rts 
            next = 0x2013cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2013aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x200e0U:
        case 0x200e4U:
        case 0x200e6U:
        case 0x200eaU:
        case 0x200ecU:
        case 0x200f0U:
        case 0x200f2U:
        case 0x200f6U:
        case 0x200f8U:
        case 0x200fcU:
        case 0x20100U:
        case 0x20104U:
        case 0x20108U:
        case 0x2010cU:
        case 0x2010eU:
        case 0x20112U:
        case 0x20114U:
        case 0x20116U:
        case 0x2011aU:
        case 0x2011eU:
        case 0x20122U:
        case 0x20128U:
        case 0x2012eU:
        case 0x20134U:
        case 0x20136U:
        case 0x2013aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
