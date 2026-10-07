
FunctionResult cpu_b_stage_019720(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1971eU: { // 4e75 rts 
            next = 0x19720U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1971eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19720U: { // 6000fffc bra.w $1971e
            next = 0x19724U;
            if (true) { next = 0x1971eU; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1971eU:
        case 0x19720U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_019724(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1971eU: { // 4e75 rts 
            next = 0x19720U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1971eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19724U: { // 6000fff8 bra.w $1971e
            next = 0x19728U;
            if (true) { next = 0x1971eU; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1971eU:
        case 0x19724U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_019728(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19728U: { // 6000000e bra.w $19738
            next = 0x1972cU;
            if (true) { next = 0x19738U; transfer_kind = 1U; }
            break;
        }
        case 0x19738U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x1973cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1973cU: { // 6e18 bgt.b $19756
            next = 0x1973eU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19756U; transfer_kind = 1U; }
            break;
        }
        case 0x1973eU: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x19744U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19744U: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x19748U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19748U: { // 0c6d00080064 cmpi.w #$8, $64(a5)
            next = 0x1974eU;
            const auto source_value = 0x8U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1974eU: { // 6b06 bmi.b $19756
            next = 0x19750U;
            if ((r.status & 8U) != 0U) { next = 0x19756U; transfer_kind = 1U; }
            break;
        }
        case 0x19750U: { // 3b7c00070064 move.w #$7, $64(a5)
            next = 0x19756U;
            const auto value = 0x7U;
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19756U: { // 0c6d00040064 cmpi.w #$4, $64(a5)
            next = 0x1975cU;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1975cU: { // 6b40 bmi.b $1979e
            next = 0x1975eU;
            if ((r.status & 8U) != 0U) { next = 0x1979eU; transfer_kind = 1U; }
            break;
        }
        case 0x1975eU: { // 202d0026 move.l $26(a5), d0
            next = 0x19762U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x19762U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x19766U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x19766U: { // 202d0022 move.l $22(a5), d0
            next = 0x1976aU;
            const auto source_address = r.address[5] + 0x22U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1976aU: { // d0ad0016 add.l $16(a5), d0
            next = 0x1976eU;
            const auto source_address = r.address[5] + 0x16U;
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[0] = value;
            break;
        }
        case 0x1976eU: { // 04ad000040000022 subi.l #$4000, $22(a5)
            next = 0x19776U;
            const auto source_value = 0x4000U;
            const auto destination_address = r.address[5] + 0x22U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.sub(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x19776U: { // 6a22 bpl.b $1979a
            next = 0x19778U;
            if ((r.status & 8U) == 0U) { next = 0x1979aU; transfer_kind = 1U; }
            break;
        }
        case 0x19778U: { // 223c00180000 move.l #$180000, d1
            next = 0x1977eU;
            const auto value = 0x180000U;
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1977eU: { // 0c6d00020048 cmpi.w #$2, $48(a5)
            next = 0x19784U;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[5] + 0x48U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19784U: { // 6706 beq.b $1978c
            next = 0x19786U;
            if ((r.status & 4U) != 0U) { next = 0x1978cU; transfer_kind = 1U; }
            break;
        }
        case 0x19786U: { // 223c00200000 move.l #$200000, d1
            next = 0x1978cU;
            const auto value = 0x200000U;
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1978cU: { // b081 cmp.l d1, d0
            next = 0x1978eU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 32U, true);
            break;
        }
        case 0x1978eU: { // 6e0a bgt.b $1979a
            next = 0x19790U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1979aU; transfer_kind = 1U; }
            break;
        }
        case 0x19790U: { // 2b410016 move.l d1, $16(a5)
            next = 0x19794U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x16U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19794U: { // 426d0040 clr.w $40(a5)
            next = 0x19798U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19798U: { // 4e75 rts 
            next = 0x1979aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19798U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1979aU: { // 2b400016 move.l d0, $16(a5)
            next = 0x1979eU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x16U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1979eU: { // 4e75 rts 
            next = 0x197a0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1979eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19728U:
        case 0x19738U:
        case 0x1973cU:
        case 0x1973eU:
        case 0x19744U:
        case 0x19748U:
        case 0x1974eU:
        case 0x19750U:
        case 0x19756U:
        case 0x1975cU:
        case 0x1975eU:
        case 0x19762U:
        case 0x19766U:
        case 0x1976aU:
        case 0x1976eU:
        case 0x19776U:
        case 0x19778U:
        case 0x1977eU:
        case 0x19784U:
        case 0x19786U:
        case 0x1978cU:
        case 0x1978eU:
        case 0x19790U:
        case 0x19794U:
        case 0x19798U:
        case 0x1979aU:
        case 0x1979eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
