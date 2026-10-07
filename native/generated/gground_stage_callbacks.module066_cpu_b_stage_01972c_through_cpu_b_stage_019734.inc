
FunctionResult cpu_b_stage_01972c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1972cU: { // 60000072 bra.w $197a0
            next = 0x19730U;
            if (true) { next = 0x197a0U; transfer_kind = 1U; }
            break;
        }
        case 0x197a0U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x197a4U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x197a4U: { // 6e1a bgt.b $197c0
            next = 0x197a6U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x197c0U; transfer_kind = 1U; }
            break;
        }
        case 0x197a6U: { // 3b7c00030068 move.w #$3, $68(a5)
            next = 0x197acU;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x197acU: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x197b0U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x197b0U: { // 0c6d00080064 cmpi.w #$8, $64(a5)
            next = 0x197b6U;
            const auto source_value = 0x8U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x197b6U: { // 6b08 bmi.b $197c0
            next = 0x197b8U;
            if ((r.status & 8U) != 0U) { next = 0x197c0U; transfer_kind = 1U; }
            break;
        }
        case 0x197b8U: { // 426d0040 clr.w $40(a5)
            next = 0x197bcU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x197bcU: { // 426d0064 clr.w $64(a5)
            next = 0x197c0U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x197c0U: { // 4e75 rts 
            next = 0x197c2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x197c0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1972cU:
        case 0x197a0U:
        case 0x197a4U:
        case 0x197a6U:
        case 0x197acU:
        case 0x197b0U:
        case 0x197b6U:
        case 0x197b8U:
        case 0x197bcU:
        case 0x197c0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_019730(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19730U: { // 60000090 bra.w $197c2
            next = 0x19734U;
            if (true) { next = 0x197c2U; transfer_kind = 1U; }
            break;
        }
        case 0x197c2U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x197c6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x197c6U: { // 6e1a bgt.b $197e2
            next = 0x197c8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x197e2U; transfer_kind = 1U; }
            break;
        }
        case 0x197c8U: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x197ceU;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x197ceU: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x197d2U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x197d2U: { // 0c6d00060064 cmpi.w #$6, $64(a5)
            next = 0x197d8U;
            const auto source_value = 0x6U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x197d8U: { // 6b08 bmi.b $197e2
            next = 0x197daU;
            if ((r.status & 8U) != 0U) { next = 0x197e2U; transfer_kind = 1U; }
            break;
        }
        case 0x197daU: { // 426d0040 clr.w $40(a5)
            next = 0x197deU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x197deU: { // 426d0064 clr.w $64(a5)
            next = 0x197e2U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x197e2U: { // 4e75 rts 
            next = 0x197e4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x197e2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19730U:
        case 0x197c2U:
        case 0x197c6U:
        case 0x197c8U:
        case 0x197ceU:
        case 0x197d2U:
        case 0x197d8U:
        case 0x197daU:
        case 0x197deU:
        case 0x197e2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_019734(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19734U: { // 600000ae bra.w $197e4
            next = 0x19738U;
            if (true) { next = 0x197e4U; transfer_kind = 1U; }
            break;
        }
        case 0x197e4U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x197e8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x197e8U: { // 6e1a bgt.b $19804
            next = 0x197eaU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19804U; transfer_kind = 1U; }
            break;
        }
        case 0x197eaU: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x197f0U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x197f0U: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x197f4U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x197f4U: { // 0c6d00120064 cmpi.w #$12, $64(a5)
            next = 0x197faU;
            const auto source_value = 0x12U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x197faU: { // 6b08 bmi.b $19804
            next = 0x197fcU;
            if ((r.status & 8U) != 0U) { next = 0x19804U; transfer_kind = 1U; }
            break;
        }
        case 0x197fcU: { // 426d0040 clr.w $40(a5)
            next = 0x19800U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19800U: { // 426d0064 clr.w $64(a5)
            next = 0x19804U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19804U: { // 4e75 rts 
            next = 0x19806U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19804U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19734U:
        case 0x197e4U:
        case 0x197e8U:
        case 0x197eaU:
        case 0x197f0U:
        case 0x197f4U:
        case 0x197faU:
        case 0x197fcU:
        case 0x19800U:
        case 0x19804U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
