
FunctionResult cpu_b_stage_018494(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18494U: { // 1cbc0080 move.b #$80, (a6)
            next = 0x18498U;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18498U: { // 422e0001 clr.b $1(a6)
            next = 0x1849cU;
            const auto destination_address = r.address[6] + 0x1U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1849cU: { // 2d590002 move.l (a1)+, $2(a6)
            next = 0x184a0U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[1] += 4U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x184a0U: { // 3d590006 move.w (a1)+, $6(a6)
            next = 0x184a4U;
            const auto source_address = r.address[1];
            const auto value = m.word(source_address);
            r.address[1] += 2U;
            const auto destination_address = r.address[6] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x184a4U: { // 3d590008 move.w (a1)+, $8(a6)
            next = 0x184a8U;
            const auto source_address = r.address[1];
            const auto value = m.word(source_address);
            r.address[1] += 2U;
            const auto destination_address = r.address[6] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x184a8U: { // 5249 addq.w #$1, a1
            next = 0x184aaU;
            const auto source_value = 0x1U;
            r.address[1] += source_value;
            break;
        }
        case 0x184aaU: { // 1d59000b move.b (a1)+, $b(a6)
            next = 0x184aeU;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            r.address[1] += 1U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x184aeU: { // 3d59000c move.w (a1)+, $c(a6)
            next = 0x184b2U;
            const auto source_address = r.address[1];
            const auto value = m.word(source_address);
            r.address[1] += 2U;
            const auto destination_address = r.address[6] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x184b2U: { // 3d59000e move.w (a1)+, $e(a6)
            next = 0x184b6U;
            const auto source_address = r.address[1];
            const auto value = m.word(source_address);
            r.address[1] += 2U;
            const auto destination_address = r.address[6] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x184b6U: { // 41ee0012 lea.l $12(a6), a0
            next = 0x184baU;
            const auto source_address = r.address[6] + 0x12U;
            r.address[0] = source_address;
            break;
        }
        case 0x184baU: { // 303c0019 move.w #$19, d0
            next = 0x184beU;
            const auto value = 0x19U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x184beU: { // 20d9 move.l (a1)+, (a0)+
            next = 0x184c0U;
            const auto source_address = r.address[1];
            const auto value = m.lng(source_address);
            r.address[1] += 4U;
            const auto destination_address = r.address[0];
            m.lng(destination_address, value);
            r.address[0] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x184c0U: { // 51c8fffc dbra d0, $184be
            next = 0x184c4U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x184beU;
            break;
        }
        case 0x184c4U: { // 4e75 rts 
            next = 0x184c6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x184c4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18494U:
        case 0x18498U:
        case 0x1849cU:
        case 0x184a0U:
        case 0x184a4U:
        case 0x184a8U:
        case 0x184aaU:
        case 0x184aeU:
        case 0x184b2U:
        case 0x184b6U:
        case 0x184baU:
        case 0x184beU:
        case 0x184c0U:
        case 0x184c4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0184c6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x184c6U: { // 10380403 move.b $403.w, d0
            next = 0x184caU;
            const auto source_address = 0x403U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x184caU: { // 0240001c andi.w #$1c, d0
            next = 0x184ceU;
            const auto source_value = 0x1cU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x184ceU: { // e248 lsr.w #$1, d0
            next = 0x184d0U;
            m.shift_word(0U, 1U, false, false);
            break;
        }
        case 0x184d0U: { // 41fa2384 lea.l $1a856(pc), a0
            next = 0x184d4U;
            const auto source_address = 0x1a856U;
            r.address[0] = source_address;
            break;
        }
        case 0x184d4U: { // 30300000 move.w (a0, d0.w), d0
            next = 0x184d8U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x184d8U: { // 322e0046 move.w $46(a6), d1
            next = 0x184dcU;
            const auto source_address = r.address[6] + 0x46U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x184dcU: { // c0c1 mulu.w d1, d0
            next = 0x184deU;
            const auto value = (r.data[1] & 0xffffU) * (r.data[0] & 0xffffU);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x184deU: { // e448 lsr.w #$2, d0
            next = 0x184e0U;
            m.shift_word(0U, 2U, false, false);
            break;
        }
        case 0x184e0U: { // 3d400046 move.w d0, $46(a6)
            next = 0x184e4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x184e4U: { // 3d40006c move.w d0, $6c(a6)
            next = 0x184e8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x6cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x184e8U: { // 4e75 rts 
            next = 0x184eaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x184e8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x184c6U:
        case 0x184caU:
        case 0x184ceU:
        case 0x184d0U:
        case 0x184d4U:
        case 0x184d8U:
        case 0x184dcU:
        case 0x184deU:
        case 0x184e0U:
        case 0x184e4U:
        case 0x184e8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0183b4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x183b4U: { // 4dd5 lea.l (a5), a6
            next = 0x183b6U;
            const auto source_address = r.address[5];
            r.address[6] = source_address;
            break;
        }
        case 0x183b6U: { // 43fa24ae lea.l $1a866(pc), a1
            next = 0x183baU;
            const auto source_address = 0x1a866U;
            r.address[1] = source_address;
            break;
        }
        case 0x183baU: { // 610000d8 bsr.w $18494
            next = 0x183beU;
            const auto result = m.call(c, 649U, 0x183baU, 0x18494U, 0x183beU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x183beU: { // 61000106 bsr.w $184c6
            next = 0x183c2U;
            const auto result = m.call(c, 650U, 0x183beU, 0x184c6U, 0x183c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x183c2U: { // 11fc00050d19 move.b #$5, $d19.w
            next = 0x183c8U;
            const auto value = 0x5U;
            const auto destination_address = 0xd19U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x183c8U: { // 4e75 rts 
            next = 0x183caU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x183c8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x183b4U:
        case 0x183b6U:
        case 0x183baU:
        case 0x183beU:
        case 0x183c2U:
        case 0x183c8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
