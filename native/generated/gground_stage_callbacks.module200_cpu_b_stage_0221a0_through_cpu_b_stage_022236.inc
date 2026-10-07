
FunctionResult cpu_b_stage_0221a0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x221a0U: { // 4a2d003f tst.b $3f(a5)
            next = 0x221a4U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x221a4U: { // 6718 beq.b $221be
            next = 0x221a6U;
            if ((r.status & 4U) != 0U) { next = 0x221beU; transfer_kind = 1U; }
            break;
        }
        case 0x221a6U: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x221aeU;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x221aeU: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x221b4U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x221b4U: { // 7200 moveq #$0, d1
            next = 0x221b6U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x221b6U: { // 6100bfc8 bsr.w $1e180
            next = 0x221baU;
            const auto result = m.call(c, 359U, 0x221b6U, 0x1e180U, 0x221baU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x221baU: { // 4e75 rts 
            next = 0x221bcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x221baU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x221beU: { // 6100a972 bsr.w $1cb32
            next = 0x221c2U;
            const auto result = m.call(c, 664U, 0x221beU, 0x1cb32U, 0x221c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x221c2U: { // 6100a96e bsr.w $1cb32
            next = 0x221c6U;
            const auto result = m.call(c, 664U, 0x221c2U, 0x1cb32U, 0x221c6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x221c6U: { // 61009964 bsr.w $1bb2c
            next = 0x221caU;
            const auto result = m.call(c, 564U, 0x221c6U, 0x1bb2cU, 0x221caU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x221caU: { // 6100b9f8 bsr.w $1dbc4
            next = 0x221ceU;
            const auto result = m.call(c, 352U, 0x221caU, 0x1dbc4U, 0x221ceU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x221ceU: { // 6100ba20 bsr.w $1dbf0
            next = 0x221d2U;
            const auto result = m.call(c, 353U, 0x221ceU, 0x1dbf0U, 0x221d2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x221d2U: { // 6100bc5c bsr.w $1de30
            next = 0x221d6U;
            const auto result = m.call(c, 355U, 0x221d2U, 0x1de30U, 0x221d6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x221d6U: { // 6100bf16 bsr.w $1e0ee
            next = 0x221daU;
            const auto result = m.call(c, 356U, 0x221d6U, 0x1e0eeU, 0x221daU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x221daU: { // 6100bf48 bsr.w $1e124
            next = 0x221deU;
            const auto result = m.call(c, 357U, 0x221daU, 0x1e124U, 0x221deU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x221deU: { // 6100bfa0 bsr.w $1e180
            next = 0x221e2U;
            const auto result = m.call(c, 359U, 0x221deU, 0x1e180U, 0x221e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x221e2U: { // 4e75 rts 
            next = 0x221e4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x221e2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x221a0U:
        case 0x221a4U:
        case 0x221a6U:
        case 0x221aeU:
        case 0x221b4U:
        case 0x221b6U:
        case 0x221baU:
        case 0x221beU:
        case 0x221c2U:
        case 0x221c6U:
        case 0x221caU:
        case 0x221ceU:
        case 0x221d2U:
        case 0x221d6U:
        case 0x221daU:
        case 0x221deU:
        case 0x221e2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022222(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22222U: { // 6100a91e bsr.w $1cb42
            next = 0x22226U;
            const auto result = m.call(c, 665U, 0x22222U, 0x1cb42U, 0x22226U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22226U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2222aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2222aU: { // 6604 bne.b $22230
            next = 0x2222cU;
            if ((r.status & 4U) == 0U) { next = 0x22230U; transfer_kind = 1U; }
            break;
        }
        case 0x2222cU: { // 6100d426 bsr.w $1f654
            next = 0x22230U;
            const auto result = m.call(c, 686U, 0x2222cU, 0x1f654U, 0x22230U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22230U: { // 6100c8be bsr.w $1eaf0
            next = 0x22234U;
            const auto result = m.call(c, 682U, 0x22230U, 0x1eaf0U, 0x22234U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22234U: { // 4e75 rts 
            next = 0x22236U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22234U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22222U:
        case 0x22226U:
        case 0x2222aU:
        case 0x2222cU:
        case 0x22230U:
        case 0x22234U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022236(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22236U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2223aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2223aU: { // 6714 beq.b $22250
            next = 0x2223cU;
            if ((r.status & 4U) != 0U) { next = 0x22250U; transfer_kind = 1U; }
            break;
        }
        case 0x2223cU: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x22244U;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22244U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x2224aU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x2224aU: { // 6100c942 bsr.w $1eb8e
            next = 0x2224eU;
            const auto result = m.call(c, 683U, 0x2224aU, 0x1eb8eU, 0x2224eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2224eU: { // 4e75 rts 
            next = 0x22250U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2224eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22250U: { // 61009b6e bsr.w $1bdc0
            next = 0x22254U;
            const auto result = m.call(c, 327U, 0x22250U, 0x1bdc0U, 0x22254U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22254U: { // 6100d830 bsr.w $1fa86
            next = 0x22258U;
            const auto result = m.call(c, 692U, 0x22254U, 0x1fa86U, 0x22258U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22258U: { // 6100c934 bsr.w $1eb8e
            next = 0x2225cU;
            const auto result = m.call(c, 683U, 0x22258U, 0x1eb8eU, 0x2225cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2225cU: { // 4e75 rts 
            next = 0x2225eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2225cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22236U:
        case 0x2223aU:
        case 0x2223cU:
        case 0x22244U:
        case 0x2224aU:
        case 0x2224eU:
        case 0x22250U:
        case 0x22254U:
        case 0x22258U:
        case 0x2225cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
