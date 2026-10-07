
FunctionResult cpu_b_stage_019fcc(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19fe4U: { // 6100eb64 bsr.w $18b4a
            next = 0x19fe8U;
            const auto result = m.call(c, 889U, 0x19fe4U, 0x18b4aU, 0x19fe8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a084U: { // 4eb90001631a jsr $1631a.l
            next = 0x1a08aU;
            const auto result = m.call(c, 304U, 0x1a084U, 0x1631aU, 0x1a08aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a08aU: { // 4eb9000163ba jsr $163ba.l
            next = 0x1a090U;
            const auto result = m.call(c, 867U, 0x1a08aU, 0x163baU, 0x1a090U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a094U: { // 4eb900016372 jsr $16372.l
            next = 0x1a09aU;
            const auto result = m.call(c, 306U, 0x1a094U, 0x16372U, 0x1a09aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a0a4U: { // 87c2 divs.w d2, d3
            next = 0x1a0a6U;
            const auto divisor = r.data[2];
            if ((divisor & 0xffffU) == 0U) {
                const auto result = m.exception(c, 5U, 0x1a0a4U, 0x1a0a6U);
                if (result.status != TranslationStatus::complete || result.control != 2U) return result;
                next = r.program_counter;
            } else {
                m.divide(3U, divisor, true);
            }
            break;
        }
        case 0x1a0aeU: { // 84c4 divu.w d4, d2
            next = 0x1a0b0U;
            const auto divisor = r.data[4];
            if ((divisor & 0xffffU) == 0U) {
                const auto result = m.exception(c, 5U, 0x1a0aeU, 0x1a0b0U);
                if (result.status != TranslationStatus::complete || result.control != 2U) return result;
                next = r.program_counter;
            } else {
                m.divide(2U, divisor, false);
            }
            break;
        }
        case 0x1a0deU: { // 4e75 rts 
            next = 0x1a0e0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a0deU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_019fcc_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_019fcc_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_019fcc_dispatch_cases_002(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_019fcc_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

FunctionResult cpu_b_stage_01a16a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a16aU: { // 41f90003a698 lea.l $3a698.l, a0
            next = 0x1a170U;
            const auto source_address = 0x3a698U;
            r.address[0] = source_address;
            break;
        }
        case 0x1a170U: { // 302d0058 move.w $58(a5), d0
            next = 0x1a174U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a174U: { // 6b26 bmi.b $1a19c
            next = 0x1a176U;
            if ((r.status & 8U) != 0U) { next = 0x1a19cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a176U: { // e540 asl.w #$2, d0
            next = 0x1a178U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1a178U: { // 3200 move.w d0, d1
            next = 0x1a17aU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a17aU: { // e740 asl.w #$3, d0
            next = 0x1a17cU;
            m.asl_word(0U, 3U);
            break;
        }
        case 0x1a17cU: { // d041 add.w d1, d0
            next = 0x1a17eU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a17eU: { // d0c0 adda.w d0, a0
            next = 0x1a180U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1a180U: { // 2b48005c move.l a0, $5c(a5)
            next = 0x1a184U;
            const auto value = r.address[0];
            const auto destination_address = r.address[5] + 0x5cU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a184U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1a188U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a188U: { // 6b12 bmi.b $1a19c
            next = 0x1a18aU;
            if ((r.status & 8U) != 0U) { next = 0x1a19cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a18aU: { // 5648 addq.w #$3, a0
            next = 0x1a18cU;
            const auto source_value = 0x3U;
            r.address[0] += source_value;
            break;
        }
        case 0x1a18cU: { // 1b500001 move.b (a0), $1(a5)
            next = 0x1a190U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a190U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1a196U;
            const auto result = m.call(c, 280U, 0x1a190U, 0x15d24U, 0x1a196U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a196U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a19cU;
            const auto result = m.call(c, 282U, 0x1a196U, 0x15df2U, 0x1a19cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a19cU: { // 4e75 rts 
            next = 0x1a19eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a19cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a16aU:
        case 0x1a170U:
        case 0x1a174U:
        case 0x1a176U:
        case 0x1a178U:
        case 0x1a17aU:
        case 0x1a17cU:
        case 0x1a17eU:
        case 0x1a180U:
        case 0x1a184U:
        case 0x1a188U:
        case 0x1a18aU:
        case 0x1a18cU:
        case 0x1a190U:
        case 0x1a196U:
        case 0x1a19cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
