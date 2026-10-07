
FunctionResult cpu_b_stage_021dae(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21daeU: { // 4a2d003f tst.b $3f(a5)
            next = 0x21db2U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21db2U: { // 6716 beq.b $21dca
            next = 0x21db4U;
            if ((r.status & 4U) != 0U) { next = 0x21dcaU; transfer_kind = 1U; }
            break;
        }
        case 0x21db4U: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x21dbcU;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x21dbcU: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x21dc2U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x21dc2U: { // 7200 moveq #$0, d1
            next = 0x21dc4U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x21dc4U: { // 6100c42c bsr.w $1e1f2
            next = 0x21dc8U;
            const auto result = m.call(c, 554U, 0x21dc4U, 0x1e1f2U, 0x21dc8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21dc8U: { // 4e75 rts 
            next = 0x21dcaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21dc8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21dcaU: { // 6100ac7a bsr.w $1ca46
            next = 0x21dceU;
            const auto result = m.call(c, 663U, 0x21dcaU, 0x1ca46U, 0x21dceU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21dceU: { // 6100dcb6 bsr.w $1fa86
            next = 0x21dd2U;
            const auto result = m.call(c, 692U, 0x21dceU, 0x1fa86U, 0x21dd2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21dd2U: { // 6100bdf0 bsr.w $1dbc4
            next = 0x21dd6U;
            const auto result = m.call(c, 352U, 0x21dd2U, 0x1dbc4U, 0x21dd6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21dd6U: { // 6100c34c bsr.w $1e124
            next = 0x21ddaU;
            const auto result = m.call(c, 357U, 0x21dd6U, 0x1e124U, 0x21ddaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ddaU: { // 7200 moveq #$0, d1
            next = 0x21ddcU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x21ddcU: { // 6100c414 bsr.w $1e1f2
            next = 0x21de0U;
            const auto result = m.call(c, 554U, 0x21ddcU, 0x1e1f2U, 0x21de0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21de0U: { // 6100cf02 bsr.w $1ece4
            next = 0x21de4U;
            const auto result = m.call(c, 365U, 0x21de0U, 0x1ece4U, 0x21de4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21de4U: { // 4e75 rts 
            next = 0x21de6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21de4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21daeU:
        case 0x21db2U:
        case 0x21db4U:
        case 0x21dbcU:
        case 0x21dc2U:
        case 0x21dc4U:
        case 0x21dc8U:
        case 0x21dcaU:
        case 0x21dceU:
        case 0x21dd2U:
        case 0x21dd6U:
        case 0x21ddaU:
        case 0x21ddcU:
        case 0x21de0U:
        case 0x21de4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021de6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21de6U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21deaU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21deaU: { // 6716 beq.b $21e02
            next = 0x21decU;
            if ((r.status & 4U) != 0U) { next = 0x21e02U; transfer_kind = 1U; }
            break;
        }
        case 0x21decU: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x21df4U;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x21df4U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x21dfaU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x21dfaU: { // 7200 moveq #$0, d1
            next = 0x21dfcU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x21dfcU: { // 6100c3f4 bsr.w $1e1f2
            next = 0x21e00U;
            const auto result = m.call(c, 554U, 0x21dfcU, 0x1e1f2U, 0x21e00U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e00U: { // 4e75 rts 
            next = 0x21e02U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21e00U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21e02U: { // 6100ac42 bsr.w $1ca46
            next = 0x21e06U;
            const auto result = m.call(c, 663U, 0x21e02U, 0x1ca46U, 0x21e06U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e06U: { // 6100d1d4 bsr.w $1efdc
            next = 0x21e0aU;
            const auto result = m.call(c, 368U, 0x21e06U, 0x1efdcU, 0x21e0aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e0aU: { // 6100bdb8 bsr.w $1dbc4
            next = 0x21e0eU;
            const auto result = m.call(c, 352U, 0x21e0aU, 0x1dbc4U, 0x21e0eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e0eU: { // 6100c314 bsr.w $1e124
            next = 0x21e12U;
            const auto result = m.call(c, 357U, 0x21e0eU, 0x1e124U, 0x21e12U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e12U: { // 7200 moveq #$0, d1
            next = 0x21e14U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x21e14U: { // 6100c3dc bsr.w $1e1f2
            next = 0x21e18U;
            const auto result = m.call(c, 554U, 0x21e14U, 0x1e1f2U, 0x21e18U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e18U: { // 6100ceca bsr.w $1ece4
            next = 0x21e1cU;
            const auto result = m.call(c, 365U, 0x21e18U, 0x1ece4U, 0x21e1cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e1cU: { // 4e75 rts 
            next = 0x21e1eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21e1cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21de6U:
        case 0x21deaU:
        case 0x21decU:
        case 0x21df4U:
        case 0x21dfaU:
        case 0x21dfcU:
        case 0x21e00U:
        case 0x21e02U:
        case 0x21e06U:
        case 0x21e0aU:
        case 0x21e0eU:
        case 0x21e12U:
        case 0x21e14U:
        case 0x21e18U:
        case 0x21e1cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
