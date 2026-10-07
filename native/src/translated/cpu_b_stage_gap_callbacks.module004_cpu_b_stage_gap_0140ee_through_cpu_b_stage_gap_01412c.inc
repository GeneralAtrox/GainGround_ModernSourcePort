
FunctionResult cpu_b_stage_gap_0140ee(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x140eeU: { // 4a380d2c tst.b $d2c.w
            next = 0x140f2U;
            m.logic(m.byte(0xd2cU), 8U);
            break;
        }
        case 0x140f2U: { // 6624 bne.b $14118
            next = 0x140f4U;
            if ((r.status & 4U) == 0U) { next = 0x14118U; transfer_kind = 1U; }
            break;
        }
        case 0x140f4U: { // 536d0022 subq.w #$1, $22(a5)
            next = 0x140f8U;
            const auto v1 = m.sub(m.word((r.address[5] + 0x22U)), 0x1U, 16U);
            m.word((r.address[5] + 0x22U), v1);
            break;
        }
        case 0x140f8U: { // 6e62 bgt.b $1415c
            next = 0x140faU;
            if (((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U)) { next = 0x1415cU; transfer_kind = 1U; }
            break;
        }
        case 0x140faU: { // 3b7c00020022 move.w #$2, $22(a5)
            next = 0x14100U;
            const auto v1 = 0x2U;
            m.word((r.address[5] + 0x22U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14100U: { // 526d0020 addq.w #$1, $20(a5)
            next = 0x14104U;
            const auto v1 = m.add(m.word((r.address[5] + 0x20U)), 0x1U, 16U);
            m.word((r.address[5] + 0x20U), v1);
            break;
        }
        case 0x14104U: { // 302d0020 move.w $20(a5), d0
            next = 0x14108U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14108U: { // 0240000f andi.w #$f, d0
            next = 0x1410cU;
            const auto v1 = ((r.data[0] & 0xffffU) & 0xfU) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1410cU: { // 41fa1bd4 lea.l $15ce2(pc), a0
            next = 0x14110U;
            r.address[0] = 0x15ce2U;
            break;
        }
        case 0x14110U: { // 1b7000000009 move.b (a0, d0.w), $9(a5)
            next = 0x14116U;
            const auto v1 = m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.byte((r.address[5] + 0x9U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x14116U: { // 6044 bra.b $1415c
            next = 0x14118U;
            next = 0x1415cU; transfer_kind = 1U;
            break;
        }
        case 0x14118U: { // 2b7c0001412c0002 move.l #$1412c, $2(a5)
            next = 0x14120U;
            const auto v1 = 0x1412cU;
            m.lng((r.address[5] + 0x2U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x14120U: { // 426d0020 clr.w $20(a5)
            next = 0x14124U;
            (void)m.word((r.address[5] + 0x20U));
            m.word((r.address[5] + 0x20U), 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x14124U: { // 3b7c00040022 move.w #$4, $22(a5)
            next = 0x1412aU;
            const auto v1 = 0x4U;
            m.word((r.address[5] + 0x22U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1412aU: { // 601c bra.b $14148
            next = 0x1412cU;
            next = 0x14148U; transfer_kind = 1U;
            break;
        }
        case 0x14148U: { // 41fa1ba8 lea.l $15cf2(pc), a0
            next = 0x1414cU;
            r.address[0] = 0x15cf2U;
            break;
        }
        case 0x1414cU: { // 302d0020 move.w $20(a5), d0
            next = 0x14150U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14150U: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x14156U;
            const auto v1 = m.word((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14156U: { // 3b7000020008 move.w $2(a0, d0.w), $8(a5)
            next = 0x1415cU;
            const auto v1 = m.word((r.address[0] + 0x2U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1415cU: { // 4eb900015df2 jsr $15df2.l
            next = 0x14162U;
            const auto result = m.indirect_call(c, 0x1415cU, 0x15df2U, 0x14162U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x14162U: { // 4e75 rts 
            next = 0x14164U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x14162U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x140eeU:
        case 0x140f2U:
        case 0x140f4U:
        case 0x140f8U:
        case 0x140faU:
        case 0x14100U:
        case 0x14104U:
        case 0x14108U:
        case 0x1410cU:
        case 0x14110U:
        case 0x14116U:
        case 0x14118U:
        case 0x14120U:
        case 0x14124U:
        case 0x1412aU:
        case 0x14148U:
        case 0x1414cU:
        case 0x14150U:
        case 0x14156U:
        case 0x1415cU:
        case 0x14162U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_01412c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1412cU: { // 536d0022 subq.w #$1, $22(a5)
            next = 0x14130U;
            const auto v1 = m.sub(m.word((r.address[5] + 0x22U)), 0x1U, 16U);
            m.word((r.address[5] + 0x22U), v1);
            break;
        }
        case 0x14130U: { // 6e16 bgt.b $14148
            next = 0x14132U;
            if (((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U)) { next = 0x14148U; transfer_kind = 1U; }
            break;
        }
        case 0x14132U: { // 3b7c00040022 move.w #$4, $22(a5)
            next = 0x14138U;
            const auto v1 = 0x4U;
            m.word((r.address[5] + 0x22U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14138U: { // 586d0020 addq.w #$4, $20(a5)
            next = 0x1413cU;
            const auto v1 = m.add(m.word((r.address[5] + 0x20U)), 0x4U, 16U);
            m.word((r.address[5] + 0x20U), v1);
            break;
        }
        case 0x1413cU: { // 0c6d00100020 cmpi.w #$10, $20(a5)
            next = 0x14142U;
            (void)m.sub(m.word((r.address[5] + 0x20U)), 0x10U, 16U, true);
            break;
        }
        case 0x14142U: { // 6b04 bmi.b $14148
            next = 0x14144U;
            if ((r.status & 8U) != 0U) { next = 0x14148U; transfer_kind = 1U; }
            break;
        }
        case 0x14144U: { // 4255 clr.w (a5)
            next = 0x14146U;
            (void)m.word(r.address[5]);
            m.word(r.address[5], 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x14146U: { // 4e75 rts 
            next = 0x14148U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x14146U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x14148U: { // 41fa1ba8 lea.l $15cf2(pc), a0
            next = 0x1414cU;
            r.address[0] = 0x15cf2U;
            break;
        }
        case 0x1414cU: { // 302d0020 move.w $20(a5), d0
            next = 0x14150U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14150U: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x14156U;
            const auto v1 = m.word((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14156U: { // 3b7000020008 move.w $2(a0, d0.w), $8(a5)
            next = 0x1415cU;
            const auto v1 = m.word((r.address[0] + 0x2U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1415cU: { // 4eb900015df2 jsr $15df2.l
            next = 0x14162U;
            const auto result = m.indirect_call(c, 0x1415cU, 0x15df2U, 0x14162U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x14162U: { // 4e75 rts 
            next = 0x14164U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x14162U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1412cU:
        case 0x14130U:
        case 0x14132U:
        case 0x14138U:
        case 0x1413cU:
        case 0x14142U:
        case 0x14144U:
        case 0x14146U:
        case 0x14148U:
        case 0x1414cU:
        case 0x14150U:
        case 0x14156U:
        case 0x1415cU:
        case 0x14162U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
