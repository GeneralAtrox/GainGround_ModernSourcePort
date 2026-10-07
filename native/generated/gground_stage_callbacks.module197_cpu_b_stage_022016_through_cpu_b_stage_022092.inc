
FunctionResult cpu_b_stage_022016(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22016U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2201aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2201aU: { // 6716 beq.b $22032
            next = 0x2201cU;
            if ((r.status & 4U) != 0U) { next = 0x22032U; transfer_kind = 1U; }
            break;
        }
        case 0x2201cU: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x22024U;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22024U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x2202aU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x2202aU: { // 7200 moveq #$0, d1
            next = 0x2202cU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x2202cU: { // 6100ca3e bsr.w $1ea6c
            next = 0x22030U;
            const auto result = m.call(c, 680U, 0x2202cU, 0x1ea6cU, 0x22030U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22030U: { // 4e75 rts 
            next = 0x22032U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22030U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22032U: { // 6100aafe bsr.w $1cb32
            next = 0x22036U;
            const auto result = m.call(c, 664U, 0x22032U, 0x1cb32U, 0x22036U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22036U: { // 61009cd4 bsr.w $1bd0c
            next = 0x2203aU;
            const auto result = m.call(c, 326U, 0x22036U, 0x1bd0cU, 0x2203aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2203aU: { // 6100da4a bsr.w $1fa86
            next = 0x2203eU;
            const auto result = m.call(c, 692U, 0x2203aU, 0x1fa86U, 0x2203eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2203eU: { // 6100bb84 bsr.w $1dbc4
            next = 0x22042U;
            const auto result = m.call(c, 352U, 0x2203eU, 0x1dbc4U, 0x22042U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22042U: { // 6100bbac bsr.w $1dbf0
            next = 0x22046U;
            const auto result = m.call(c, 353U, 0x22042U, 0x1dbf0U, 0x22046U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22046U: { // 6100bde8 bsr.w $1de30
            next = 0x2204aU;
            const auto result = m.call(c, 355U, 0x22046U, 0x1de30U, 0x2204aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2204aU: { // 6100c0d8 bsr.w $1e124
            next = 0x2204eU;
            const auto result = m.call(c, 357U, 0x2204aU, 0x1e124U, 0x2204eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2204eU: { // 6100ca1c bsr.w $1ea6c
            next = 0x22052U;
            const auto result = m.call(c, 680U, 0x2204eU, 0x1ea6cU, 0x22052U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22052U: { // 4e75 rts 
            next = 0x22054U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22052U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22016U:
        case 0x2201aU:
        case 0x2201cU:
        case 0x22024U:
        case 0x2202aU:
        case 0x2202cU:
        case 0x22030U:
        case 0x22032U:
        case 0x22036U:
        case 0x2203aU:
        case 0x2203eU:
        case 0x22042U:
        case 0x22046U:
        case 0x2204aU:
        case 0x2204eU:
        case 0x22052U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022092(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22092U: { // 4a2d003f tst.b $3f(a5)
            next = 0x22096U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x22096U: { // 6716 beq.b $220ae
            next = 0x22098U;
            if ((r.status & 4U) != 0U) { next = 0x220aeU; transfer_kind = 1U; }
            break;
        }
        case 0x22098U: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x220a0U;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x220a0U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x220a6U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x220a6U: { // 7200 moveq #$0, d1
            next = 0x220a8U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x220a8U: { // 6100c9f6 bsr.w $1eaa0
            next = 0x220acU;
            const auto result = m.call(c, 681U, 0x220a8U, 0x1eaa0U, 0x220acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x220acU: { // 4e75 rts 
            next = 0x220aeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x220acU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x220aeU: { // 6100aa82 bsr.w $1cb32
            next = 0x220b2U;
            const auto result = m.call(c, 664U, 0x220aeU, 0x1cb32U, 0x220b2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x220b2U: { // 6100a2ca bsr.w $1c37e
            next = 0x220b6U;
            const auto result = m.call(c, 331U, 0x220b2U, 0x1c37eU, 0x220b6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x220b6U: { // 6100cf24 bsr.w $1efdc
            next = 0x220baU;
            const auto result = m.call(c, 368U, 0x220b6U, 0x1efdcU, 0x220baU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x220baU: { // 6100bb08 bsr.w $1dbc4
            next = 0x220beU;
            const auto result = m.call(c, 352U, 0x220baU, 0x1dbc4U, 0x220beU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x220beU: { // 6100bb30 bsr.w $1dbf0
            next = 0x220c2U;
            const auto result = m.call(c, 353U, 0x220beU, 0x1dbf0U, 0x220c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x220c2U: { // 6100bd6c bsr.w $1de30
            next = 0x220c6U;
            const auto result = m.call(c, 355U, 0x220c2U, 0x1de30U, 0x220c6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x220c6U: { // 6100c05c bsr.w $1e124
            next = 0x220caU;
            const auto result = m.call(c, 357U, 0x220c6U, 0x1e124U, 0x220caU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x220caU: { // 6100c9d4 bsr.w $1eaa0
            next = 0x220ceU;
            const auto result = m.call(c, 681U, 0x220caU, 0x1eaa0U, 0x220ceU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x220ceU: { // 4e75 rts 
            next = 0x220d0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x220ceU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22092U:
        case 0x22096U:
        case 0x22098U:
        case 0x220a0U:
        case 0x220a6U:
        case 0x220a8U:
        case 0x220acU:
        case 0x220aeU:
        case 0x220b2U:
        case 0x220b6U:
        case 0x220baU:
        case 0x220beU:
        case 0x220c2U:
        case 0x220c6U:
        case 0x220caU:
        case 0x220ceU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
