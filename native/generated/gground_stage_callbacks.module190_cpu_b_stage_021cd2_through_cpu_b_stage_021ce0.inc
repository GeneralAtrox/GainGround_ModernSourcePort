
FunctionResult cpu_b_stage_021cd2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21cd2U: { // 6100a0ec bsr.w $1bdc0
            next = 0x21cd6U;
            const auto result = m.call(c, 327U, 0x21cd2U, 0x1bdc0U, 0x21cd6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cd6U: { // 6100d304 bsr.w $1efdc
            next = 0x21cdaU;
            const auto result = m.call(c, 368U, 0x21cd6U, 0x1efdcU, 0x21cdaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cdaU: { // 6100cd3e bsr.w $1ea1a
            next = 0x21cdeU;
            const auto result = m.call(c, 679U, 0x21cdaU, 0x1ea1aU, 0x21cdeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cdeU: { // 4e75 rts 
            next = 0x21ce0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21cdeU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21cd2U:
        case 0x21cd6U:
        case 0x21cdaU:
        case 0x21cdeU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021ce0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21ce0U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21ce4U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21ce4U: { // 6722 beq.b $21d08
            next = 0x21ce6U;
            if ((r.status & 4U) != 0U) { next = 0x21d08U; transfer_kind = 1U; }
            break;
        }
        case 0x21ce6U: { // 4a6d0012 tst.w $12(a5)
            next = 0x21ceaU;
            const auto destination_address = r.address[5] + 0x12U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x21ceaU: { // 6a08 bpl.b $21cf4
            next = 0x21cecU;
            if ((r.status & 8U) == 0U) { next = 0x21cf4U; transfer_kind = 1U; }
            break;
        }
        case 0x21cecU: { // 1b7c0000003f move.b #$0, $3f(a5)
            next = 0x21cf2U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x3fU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21cf2U: { // 6014 bra.b $21d08
            next = 0x21cf4U;
            if (true) { next = 0x21d08U; transfer_kind = 1U; }
            break;
        }
        case 0x21cf4U: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x21cfcU;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x21cfcU: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x21d02U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x21d02U: { // 6100c47c bsr.w $1e180
            next = 0x21d06U;
            const auto result = m.call(c, 359U, 0x21d02U, 0x1e180U, 0x21d06U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d06U: { // 4e75 rts 
            next = 0x21d08U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21d06U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21d08U: { // 1b7c0009000b move.b #$9, $b(a5)
            next = 0x21d0eU;
            const auto value = 0x9U;
            const auto destination_address = r.address[5] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21d0eU: { // 6100a20c bsr.w $1bf1c
            next = 0x21d12U;
            const auto result = m.call(c, 328U, 0x21d0eU, 0x1bf1cU, 0x21d12U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d12U: { // 6100d4e8 bsr.w $1f1fc
            next = 0x21d16U;
            const auto result = m.call(c, 371U, 0x21d12U, 0x1f1fcU, 0x21d16U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d16U: { // 6100beac bsr.w $1dbc4
            next = 0x21d1aU;
            const auto result = m.call(c, 352U, 0x21d16U, 0x1dbc4U, 0x21d1aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d1aU: { // 6100bed4 bsr.w $1dbf0
            next = 0x21d1eU;
            const auto result = m.call(c, 353U, 0x21d1aU, 0x1dbf0U, 0x21d1eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d1eU: { // 6100c110 bsr.w $1de30
            next = 0x21d22U;
            const auto result = m.call(c, 355U, 0x21d1eU, 0x1de30U, 0x21d22U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d22U: { // 6100c3ca bsr.w $1e0ee
            next = 0x21d26U;
            const auto result = m.call(c, 356U, 0x21d22U, 0x1e0eeU, 0x21d26U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d26U: { // 6100c3fc bsr.w $1e124
            next = 0x21d2aU;
            const auto result = m.call(c, 357U, 0x21d26U, 0x1e124U, 0x21d2aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d2aU: { // 6100c454 bsr.w $1e180
            next = 0x21d2eU;
            const auto result = m.call(c, 359U, 0x21d2aU, 0x1e180U, 0x21d2eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d2eU: { // 4e75 rts 
            next = 0x21d30U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21d2eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21ce0U:
        case 0x21ce4U:
        case 0x21ce6U:
        case 0x21ceaU:
        case 0x21cecU:
        case 0x21cf2U:
        case 0x21cf4U:
        case 0x21cfcU:
        case 0x21d02U:
        case 0x21d06U:
        case 0x21d08U:
        case 0x21d0eU:
        case 0x21d12U:
        case 0x21d16U:
        case 0x21d1aU:
        case 0x21d1eU:
        case 0x21d22U:
        case 0x21d26U:
        case 0x21d2aU:
        case 0x21d2eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
