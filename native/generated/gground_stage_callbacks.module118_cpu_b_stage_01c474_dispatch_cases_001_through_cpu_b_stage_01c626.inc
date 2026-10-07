
static bool cpu_b_stage_01c474_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c4e8U: { // 02410040 andi.w #$40, d1
            next = 0x1c4ecU;
            const auto source_value = 0x40U;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c4ecU: { // 1b410039 move.b d1, $39(a5)
            next = 0x1c4f0U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x39U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c4f0U: { // e949 lsl.w #$4, d1
            next = 0x1c4f2U;
            m.shift_word(1U, 4U, true, false);
            break;
        }
        case 0x1c4f2U: { // 3b41005c move.w d1, $5c(a5)
            next = 0x1c4f6U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c4f6U: { // 3b41003a move.w d1, $3a(a5)
            next = 0x1c4faU;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x3aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c4faU: { // 082d00060040 btst.b #$6, $40(a5)
            next = 0x1c500U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x6U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c500U: { // 6606 bne.b $1c508
            next = 0x1c502U;
            if ((r.status & 4U) == 0U) { next = 0x1c508U; transfer_kind = 1U; }
            break;
        }
        case 0x1c502U: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1c508U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c508U: { // 08ad00000040 bclr.b #$0, $40(a5)
            next = 0x1c50eU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c50eU: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1c514U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c514U: { // 08ed00020040 bset.b #$2, $40(a5)
            next = 0x1c51aU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01c474(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c49aU: { // 4e75 rts 
            next = 0x1c49cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c49aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c51aU: { // 4e75 rts 
            next = 0x1c51cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c51aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01c474_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01c474_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01c474_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_01c626_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c62cU: { // e559 rol.w #$2, d1
            next = 0x1c62eU;
            const auto old = r.data[1] & 0xffffU;
            const auto value = ((old << 2U) | (old >> 14U)) & 0xffffU;
            m.dw(1U, value);
            m.logic(value, 16U);
            r.status = static_cast<std::uint16_t>(r.status | (value & 1U));
            break;
        }
        case 0x1c62eU: { // 02410040 andi.w #$40, d1
            next = 0x1c632U;
            const auto source_value = 0x40U;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c632U: { // 04410020 subi.w #$20, d1
            next = 0x1c636U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c636U: { // d26d0062 add.w $62(a5), d1
            next = 0x1c63aU;
            const auto source_address = r.address[5] + 0x62U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c63aU: { // 3b410012 move.w d1, $12(a5)
            next = 0x1c63eU;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c63eU: { // 4841 swap d1
            next = 0x1c640U;
            const auto value = (r.data[1] << 16U) | (r.data[1] >> 16U);
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1c640U: { // 02410030 andi.w #$30, d1
            next = 0x1c644U;
            const auto source_value = 0x30U;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c644U: { // 04410018 subi.w #$18, d1
            next = 0x1c648U;
            const auto source_value = 0x18U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c648U: { // d26d0064 add.w $64(a5), d1
            next = 0x1c64cU;
            const auto source_address = r.address[5] + 0x64U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c64cU: { // 3b41001a move.w d1, $1a(a5)
            next = 0x1c650U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01c626(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c626U: { // 4eb900015e28 jsr $15e28.l
            next = 0x1c62cU;
            const auto result = m.call(c, 283U, 0x1c626U, 0x15e28U, 0x1c62cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1c650U: { // 4e75 rts 
            next = 0x1c652U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c650U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01c626_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1c626U:
        case 0x1c62cU:
        case 0x1c62eU:
        case 0x1c632U:
        case 0x1c636U:
        case 0x1c63aU:
        case 0x1c63eU:
        case 0x1c640U:
        case 0x1c644U:
        case 0x1c648U:
        case 0x1c64cU:
        case 0x1c650U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
