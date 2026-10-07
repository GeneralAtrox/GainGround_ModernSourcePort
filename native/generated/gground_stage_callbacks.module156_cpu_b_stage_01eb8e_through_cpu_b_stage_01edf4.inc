
FunctionResult cpu_b_stage_01eb8e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1eb8eU: { // 61000060 bsr.w $1ebf0
            next = 0x1eb92U;
            const auto result = m.call(c, 363U, 0x1eb8eU, 0x1ebf0U, 0x1eb92U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1eb92U: { // 526d0056 addq.w #$1, $56(a5)
            next = 0x1eb96U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1eb96U: { // 302d0056 move.w $56(a5), d0
            next = 0x1eb9aU;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eb9aU: { // 0240001f andi.w #$1f, d0
            next = 0x1eb9eU;
            const auto source_value = 0x1fU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1eb9eU: { // 7200 moveq #$0, d1
            next = 0x1eba0U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1eba0U: { // 9247 sub.w d7, d1
            next = 0x1eba2U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1eba2U: { // d247 add.w d7, d1
            next = 0x1eba4U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1eba4U: { // 51c8fffc dbra d0, $1eba2
            next = 0x1eba8U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1eba2U;
            break;
        }
        case 0x1eba8U: { // 6000f648 bra.w $1e1f2
            next = 0x1ebacU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1eb8eU:
        case 0x1eb92U:
        case 0x1eb96U:
        case 0x1eb9aU:
        case 0x1eb9eU:
        case 0x1eba0U:
        case 0x1eba2U:
        case 0x1eba4U:
        case 0x1eba8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01edf4_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1edf4U: { // 206d0050 movea.l $50(a5), a0
            next = 0x1edf8U;
            const auto source_address = r.address[5] + 0x50U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1edf8U: { // 302d0054 move.w $54(a5), d0
            next = 0x1edfcU;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1edfcU: { // b018 cmp.b (a0)+, d0
            next = 0x1edfeU;
            const auto source_address = r.address[0];
            const auto source_value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1edfeU: { // 6f06 ble.b $1ee06
            next = 0x1ee00U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1ee06U; transfer_kind = 1U; }
            break;
        }
        case 0x1ee00U: { // d0fc0005 adda.w #$5, a0
            next = 0x1ee04U;
            const auto source_value = 0x5U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1ee04U: { // 60f6 bra.b $1edfc
            next = 0x1ee06U;
            if (true) { next = 0x1edfcU; transfer_kind = 1U; }
            break;
        }
        case 0x1ee06U: { // 303c0c00 move.w #$c00, d0
            next = 0x1ee0aU;
            const auto value = 0xc00U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ee0aU: { // d018 add.b (a0)+, d0
            next = 0x1ee0cU;
            const auto source_address = r.address[0];
            const auto source_value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x1ee0cU: { // 3b400008 move.w d0, $8(a5)
            next = 0x1ee10U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ee10U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1ee14U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ee14U: { // 1218 move.b (a0)+, d1
            next = 0x1ee16U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1ee16U: { // 6714 beq.b $1ee2c
            next = 0x1ee18U;
            if ((r.status & 4U) != 0U) { next = 0x1ee2cU; transfer_kind = 1U; }
            break;
        }
        case 0x1ee18U: { // 08d50007 bset.b #$7, (a5)
            next = 0x1ee1cU;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1ee1cU: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x1ee20U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1ee20U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1ee24U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1ee2cU: { // 08950007 bclr.b #$7, (a5)
            next = 0x1ee30U;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01edf4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1ee24U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1ee2aU;
            const auto result = m.call(c, 282U, 0x1ee24U, 0x15df2U, 0x1ee2aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ee2aU: { // 4e75 rts 
            next = 0x1ee2cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1ee2aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1ee30U: { // 4e75 rts 
            next = 0x1ee32U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1ee30U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01edf4_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1edf4U:
        case 0x1edf8U:
        case 0x1edfcU:
        case 0x1edfeU:
        case 0x1ee00U:
        case 0x1ee04U:
        case 0x1ee06U:
        case 0x1ee0aU:
        case 0x1ee0cU:
        case 0x1ee10U:
        case 0x1ee14U:
        case 0x1ee16U:
        case 0x1ee18U:
        case 0x1ee1cU:
        case 0x1ee20U:
        case 0x1ee24U:
        case 0x1ee2aU:
        case 0x1ee2cU:
        case 0x1ee30U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
