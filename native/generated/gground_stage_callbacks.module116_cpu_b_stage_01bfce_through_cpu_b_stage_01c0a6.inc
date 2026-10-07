
FunctionResult cpu_b_stage_01bfce(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c008U: { // 4e75 rts 
            next = 0x1c00aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c008U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c014U: { // 4e75 rts 
            next = 0x1c016U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c014U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c020U: { // 4e75 rts 
            next = 0x1c022U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c020U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c02cU: { // 4e75 rts 
            next = 0x1c02eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c02cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01bfce_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1bfceU:
        case 0x1bfd2U:
        case 0x1bfd4U:
        case 0x1bfd8U:
        case 0x1bfdcU:
        case 0x1bfdeU:
        case 0x1bfe2U:
        case 0x1bfe8U:
        case 0x1bfecU:
        case 0x1bff2U:
        case 0x1bff8U:
        case 0x1bffaU:
        case 0x1bffeU:
        case 0x1c002U:
        case 0x1c006U:
        case 0x1c008U:
        case 0x1c00aU:
        case 0x1c00eU:
        case 0x1c014U:
        case 0x1c016U:
        case 0x1c01aU:
        case 0x1c01eU:
        case 0x1c020U:
        case 0x1c022U:
        case 0x1c026U:
        case 0x1c02cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01c0a6_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c0a6U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1c0aaU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c0aaU: { // 6704 beq.b $1c0b0
            next = 0x1c0acU;
            if ((r.status & 4U) != 0U) { next = 0x1c0b0U; transfer_kind = 1U; }
            break;
        }
        case 0x1c0acU: { // 6000192c bra.w $1d9da
            next = 0x1c0b0U;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1c0b0U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1c0b4U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c0b4U: { // 6704 beq.b $1c0ba
            next = 0x1c0b6U;
            if ((r.status & 4U) != 0U) { next = 0x1c0baU; transfer_kind = 1U; }
            break;
        }
        case 0x1c0b6U: { // 60001932 bra.w $1d9ea
            next = 0x1c0baU;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1c0baU: { // 08ed00020078 bset.b #$2, $78(a5)
            next = 0x1c0c0U;
            const auto destination_address = r.address[5] + 0x78U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c0c0U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1c0c4U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c0c4U: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1c0caU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c0caU: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1c0d0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c0d0U: { // 6618 bne.b $1c0ea
            next = 0x1c0d2U;
            if ((r.status & 4U) == 0U) { next = 0x1c0eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1c0d2U: { // 302d0064 move.w $64(a5), d0
            next = 0x1c0d6U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c0d6U: { // b06d001a cmp.w $1a(a5), d0
            next = 0x1c0daU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1c0daU: { // 6602 bne.b $1c0de
            next = 0x1c0dcU;
            if ((r.status & 4U) == 0U) { next = 0x1c0deU; transfer_kind = 1U; }
            break;
        }
        case 0x1c0deU: { // 3b40001a move.w d0, $1a(a5)
            next = 0x1c0e2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c0e2U: { // 3b7c00040016 move.w #$4, $16(a5)
            next = 0x1c0e8U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c0eaU: { // 302d0064 move.w $64(a5), d0
            next = 0x1c0eeU;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c0eeU: { // 066d0010001a addi.w #$10, $1a(a5)
            next = 0x1c0f4U;
            const auto source_value = 0x10U;
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c0f4U: { // b06d001a cmp.w $1a(a5), d0
            next = 0x1c0f8U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1c0f8U: { // 6602 bne.b $1c0fc
            next = 0x1c0faU;
            if ((r.status & 4U) == 0U) { next = 0x1c0fcU; transfer_kind = 1U; }
            break;
        }
        case 0x1c0fcU: { // 3b40001a move.w d0, $1a(a5)
            next = 0x1c100U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c100U: { // 3b7c00040016 move.w #$4, $16(a5)
            next = 0x1c106U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01c0a6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c0dcU: { // 4e75 rts 
            next = 0x1c0deU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c0dcU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c0e8U: { // 4e75 rts 
            next = 0x1c0eaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c0e8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c0faU: { // 4e75 rts 
            next = 0x1c0fcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c0faU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c106U: { // 4e75 rts 
            next = 0x1c108U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c106U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01c0a6_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1c0a6U:
        case 0x1c0aaU:
        case 0x1c0acU:
        case 0x1c0b0U:
        case 0x1c0b4U:
        case 0x1c0b6U:
        case 0x1c0baU:
        case 0x1c0c0U:
        case 0x1c0c4U:
        case 0x1c0caU:
        case 0x1c0d0U:
        case 0x1c0d2U:
        case 0x1c0d6U:
        case 0x1c0daU:
        case 0x1c0dcU:
        case 0x1c0deU:
        case 0x1c0e2U:
        case 0x1c0e8U:
        case 0x1c0eaU:
        case 0x1c0eeU:
        case 0x1c0f4U:
        case 0x1c0f8U:
        case 0x1c0faU:
        case 0x1c0fcU:
        case 0x1c100U:
        case 0x1c106U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
