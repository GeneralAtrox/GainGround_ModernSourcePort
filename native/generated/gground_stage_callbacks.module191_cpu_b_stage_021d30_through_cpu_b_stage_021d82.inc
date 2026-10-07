
FunctionResult cpu_b_stage_021d30(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21d30U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21d34U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21d34U: { // 6724 beq.b $21d5a
            next = 0x21d36U;
            if ((r.status & 4U) != 0U) { next = 0x21d5aU; transfer_kind = 1U; }
            break;
        }
        case 0x21d36U: { // 0c6d0180001a cmpi.w #$180, $1a(a5)
            next = 0x21d3cU;
            const auto source_value = 0x180U;
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x21d3cU: { // 6d08 blt.b $21d46
            next = 0x21d3eU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x21d46U; transfer_kind = 1U; }
            break;
        }
        case 0x21d3eU: { // 1b7c0000003f move.b #$0, $3f(a5)
            next = 0x21d44U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x3fU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21d44U: { // 6014 bra.b $21d5a
            next = 0x21d46U;
            if (true) { next = 0x21d5aU; transfer_kind = 1U; }
            break;
        }
        case 0x21d46U: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x21d4eU;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x21d4eU: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x21d54U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x21d54U: { // 6100c42a bsr.w $1e180
            next = 0x21d58U;
            const auto result = m.call(c, 359U, 0x21d54U, 0x1e180U, 0x21d58U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d58U: { // 4e75 rts 
            next = 0x21d5aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21d58U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21d5aU: { // 1b7c0009000b move.b #$9, $b(a5)
            next = 0x21d60U;
            const auto value = 0x9U;
            const auto destination_address = r.address[5] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21d60U: { // 6100a1ba bsr.w $1bf1c
            next = 0x21d64U;
            const auto result = m.call(c, 328U, 0x21d60U, 0x1bf1cU, 0x21d64U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d64U: { // 6100d496 bsr.w $1f1fc
            next = 0x21d68U;
            const auto result = m.call(c, 371U, 0x21d64U, 0x1f1fcU, 0x21d68U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d68U: { // 6100be5a bsr.w $1dbc4
            next = 0x21d6cU;
            const auto result = m.call(c, 352U, 0x21d68U, 0x1dbc4U, 0x21d6cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d6cU: { // 6100be82 bsr.w $1dbf0
            next = 0x21d70U;
            const auto result = m.call(c, 353U, 0x21d6cU, 0x1dbf0U, 0x21d70U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d70U: { // 6100c0be bsr.w $1de30
            next = 0x21d74U;
            const auto result = m.call(c, 355U, 0x21d70U, 0x1de30U, 0x21d74U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d74U: { // 6100c378 bsr.w $1e0ee
            next = 0x21d78U;
            const auto result = m.call(c, 356U, 0x21d74U, 0x1e0eeU, 0x21d78U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d78U: { // 6100c3aa bsr.w $1e124
            next = 0x21d7cU;
            const auto result = m.call(c, 357U, 0x21d78U, 0x1e124U, 0x21d7cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d7cU: { // 6100c402 bsr.w $1e180
            next = 0x21d80U;
            const auto result = m.call(c, 359U, 0x21d7cU, 0x1e180U, 0x21d80U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d80U: { // 4e75 rts 
            next = 0x21d82U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21d80U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21d30U:
        case 0x21d34U:
        case 0x21d36U:
        case 0x21d3cU:
        case 0x21d3eU:
        case 0x21d44U:
        case 0x21d46U:
        case 0x21d4eU:
        case 0x21d54U:
        case 0x21d58U:
        case 0x21d5aU:
        case 0x21d60U:
        case 0x21d64U:
        case 0x21d68U:
        case 0x21d6cU:
        case 0x21d70U:
        case 0x21d74U:
        case 0x21d78U:
        case 0x21d7cU:
        case 0x21d80U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021d82(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21d82U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21d86U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21d86U: { // 6714 beq.b $21d9c
            next = 0x21d88U;
            if ((r.status & 4U) != 0U) { next = 0x21d9cU; transfer_kind = 1U; }
            break;
        }
        case 0x21d88U: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x21d90U;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x21d90U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x21d96U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x21d96U: { // 6100c60e bsr.w $1e3a6
            next = 0x21d9aU;
            const auto result = m.call(c, 361U, 0x21d96U, 0x1e3a6U, 0x21d9aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21d9aU: { // 4e75 rts 
            next = 0x21d9cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21d9aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21d9cU: { // 6100a022 bsr.w $1bdc0
            next = 0x21da0U;
            const auto result = m.call(c, 327U, 0x21d9cU, 0x1bdc0U, 0x21da0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21da0U: { // 6100be4e bsr.w $1dbf0
            next = 0x21da4U;
            const auto result = m.call(c, 353U, 0x21da0U, 0x1dbf0U, 0x21da4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21da4U: { // 6100d456 bsr.w $1f1fc
            next = 0x21da8U;
            const auto result = m.call(c, 371U, 0x21da4U, 0x1f1fcU, 0x21da8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21da8U: { // 6100c55a bsr.w $1e304
            next = 0x21dacU;
            const auto result = m.call(c, 360U, 0x21da8U, 0x1e304U, 0x21dacU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21dacU: { // 4e75 rts 
            next = 0x21daeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21dacU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21d82U:
        case 0x21d86U:
        case 0x21d88U:
        case 0x21d90U:
        case 0x21d96U:
        case 0x21d9aU:
        case 0x21d9cU:
        case 0x21da0U:
        case 0x21da4U:
        case 0x21da8U:
        case 0x21dacU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
