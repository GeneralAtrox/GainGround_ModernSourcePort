
FunctionResult cpu_b_stage_0187b8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x187b8U: { // 4a6d0038 tst.w $38(a5)
            next = 0x187bcU;
            const auto destination_address = r.address[5] + 0x38U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x187bcU: { // 670a beq.b $187c8
            next = 0x187beU;
            if ((r.status & 4U) != 0U) { next = 0x187c8U; transfer_kind = 1U; }
            break;
        }
        case 0x187beU: { // 536d0038 subq.w #$1, $38(a5)
            next = 0x187c2U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x38U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x187c2U: { // 302d003a move.w $3a(a5), d0
            next = 0x187c6U;
            const auto source_address = r.address[5] + 0x3aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187c6U: { // 603a bra.b $18802
            next = 0x187c8U;
            if (true) { next = 0x18802U; transfer_kind = 1U; }
            break;
        }
        case 0x18802U: { // d06d005a add.w $5a(a5), d0
            next = 0x18806U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18806U: { // 024007ff andi.w #$7ff, d0
            next = 0x1880aU;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1880aU: { // 3b40005a move.w d0, $5a(a5)
            next = 0x1880eU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1880eU: { // 06400080 addi.w #$80, d0
            next = 0x18812U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18812U: { // 02400700 andi.w #$700, d0
            next = 0x18816U;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18816U: { // ec48 lsr.w #$6, d0
            next = 0x18818U;
            m.shift_word(0U, 6U, false, false);
            break;
        }
        case 0x18818U: { // 3b400064 move.w d0, $64(a5)
            next = 0x1881cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1881cU: { // 4e75 rts 
            next = 0x1881eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1881cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x187b8U:
        case 0x187bcU:
        case 0x187beU:
        case 0x187c2U:
        case 0x187c6U:
        case 0x18802U:
        case 0x18806U:
        case 0x1880aU:
        case 0x1880eU:
        case 0x18812U:
        case 0x18816U:
        case 0x18818U:
        case 0x1881cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_01881e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1881eU: { // 206d005c movea.l $5c(a5), a0
            next = 0x18822U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x18822U: { // 302d0064 move.w $64(a5), d0
            next = 0x18826U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18826U: { // 1b7000010001 move.b $1(a0, d0.w), $1(a5)
            next = 0x1882cU;
            const auto source_address = r.address[0] + 0x1U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1882cU: { // 3b7000020006 move.w $2(a0, d0.w), $6(a5)
            next = 0x18832U;
            const auto source_address = r.address[0] + 0x2U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18832U: { // 4e75 rts 
            next = 0x18834U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18832U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1881eU:
        case 0x18822U:
        case 0x18826U:
        case 0x1882cU:
        case 0x18832U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_018834(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18834U: { // 302d006a move.w $6a(a5), d0
            next = 0x18838U;
            const auto source_address = r.address[5] + 0x6aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18838U: { // b06d0008 cmp.w $8(a5), d0
            next = 0x1883cU;
            const auto source_address = r.address[5] + 0x8U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1883cU: { // 660e bne.b $1884c
            next = 0x1883eU;
            if ((r.status & 4U) == 0U) { next = 0x1884cU; transfer_kind = 1U; }
            break;
        }
        case 0x1883eU: { // 322d0046 move.w $46(a5), d1
            next = 0x18842U;
            const auto source_address = r.address[5] + 0x46U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18842U: { // b26d006c cmp.w $6c(a5), d1
            next = 0x18846U;
            const auto source_address = r.address[5] + 0x6cU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18846U: { // 670e beq.b $18856
            next = 0x18848U;
            if ((r.status & 4U) != 0U) { next = 0x18856U; transfer_kind = 1U; }
            break;
        }
        case 0x18848U: { // 303c0c6d move.w #$c6d, d0
            next = 0x1884cU;
            const auto value = 0xc6dU;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1884cU: { // 3b400008 move.w d0, $8(a5)
            next = 0x18850U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18850U: { // 3b6d0046006c move.w $46(a5), $6c(a5)
            next = 0x18856U;
            const auto source_address = r.address[5] + 0x46U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18856U: { // 4e75 rts 
            next = 0x18858U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18856U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18834U:
        case 0x18838U:
        case 0x1883cU:
        case 0x1883eU:
        case 0x18842U:
        case 0x18846U:
        case 0x18848U:
        case 0x1884cU:
        case 0x18850U:
        case 0x18856U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
