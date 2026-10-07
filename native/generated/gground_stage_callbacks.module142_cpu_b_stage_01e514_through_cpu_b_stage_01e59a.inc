
FunctionResult cpu_b_stage_01e514(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e51cU: { // 4e75 rts 
            next = 0x1e51eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1e51cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01e514_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01e514_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1e514U:
        case 0x1e51aU:
        case 0x1e51cU:
        case 0x1e51eU:
        case 0x1e520U:
        case 0x1e524U:
        case 0x1e528U:
        case 0x1e52aU:
        case 0x1e52eU:
        case 0x1e530U:
        case 0x1e532U:
        case 0x1e536U:
        case 0x1e53aU:
        case 0x1e53eU:
        case 0x1e542U:
        case 0x1e544U:
        case 0x1e546U:
        case 0x1e548U:
        case 0x1e54aU:
        case 0x1e54eU:
        case 0x1e552U:
        case 0x1e556U:
        case 0x1e55aU:
        case 0x1e55eU:
        case 0x1e562U:
        case 0x1e564U:
        case 0x1e566U:
        case 0x1e568U:
        case 0x1e56aU:
        case 0x1e56eU:
        case 0x1e572U:
        case 0x1e576U:
        case 0x1e57aU:
        case 0x1e57eU:
        case 0x1e582U:
        case 0x1e584U:
        case 0x1e586U:
        case 0x1e588U:
        case 0x1e58aU:
        case 0x1e58eU:
        case 0x1e592U:
        case 0x1e594U:
        case 0x1e596U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01e59a_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e59aU: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x1e5a0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e5a0U: { // 6702 beq.b $1e5a4
            next = 0x1e5a2U;
            if ((r.status & 4U) != 0U) { next = 0x1e5a4U; transfer_kind = 1U; }
            break;
        }
        case 0x1e5a8U: { // 7200 moveq #$0, d1
            next = 0x1e5aaU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e5aaU: { // 7000 moveq #$0, d0
            next = 0x1e5acU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1e5acU: { // 102d0058 move.b $58(a5), d0
            next = 0x1e5b0U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e5b0U: { // 0c400008 cmpi.w #$8, d0
            next = 0x1e5b4U;
            const auto source_value = 0x8U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e5b4U: { // 6e0c bgt.b $1e5c2
            next = 0x1e5b6U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e5c2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e5b6U: { // 9247 sub.w d7, d1
            next = 0x1e5b8U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e5b8U: { // d247 add.w d7, d1
            next = 0x1e5baU;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e5baU: { // 51c8fffc dbra d0, $1e5b8
            next = 0x1e5beU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e5b8U;
            break;
        }
        case 0x1e5beU: { // 6000fc32 bra.w $1e1f2
            next = 0x1e5c2U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e5c2U: { // 7200 moveq #$0, d1
            next = 0x1e5c4U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e5c4U: { // 0c6d00030074 cmpi.w #$3, $74(a5)
            next = 0x1e5caU;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e5caU: { // 6e02 bgt.b $1e5ce
            next = 0x1e5ccU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e5ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1e5ccU: { // d247 add.w d7, d1
            next = 0x1e5ceU;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e5ceU: { // 3007 move.w d7, d0
            next = 0x1e5d0U;
            const auto value = r.data[7];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e5d0U: { // e740 asl.w #$3, d0
            next = 0x1e5d2U;
            m.asl_word(0U, 3U);
            break;
        }
        case 0x1e5d2U: { // d240 add.w d0, d1
            next = 0x1e5d4U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e5d4U: { // 302d005c move.w $5c(a5), d0
            next = 0x1e5d8U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e5d8U: { // e158 rol.w #$8, d0
            next = 0x1e5daU;
            const auto old = r.data[0] & 0xffffU;
            const auto value = ((old << 8U) | (old >> 8U)) & 0xffffU;
            m.dw(0U, value);
            m.logic(value, 16U);
            r.status = static_cast<std::uint16_t>(r.status | (value & 1U));
            break;
        }
        case 0x1e5daU: { // 02400007 andi.w #$7, d0
            next = 0x1e5deU;
            const auto source_value = 0x7U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e5deU: { // 3407 move.w d7, d2
            next = 0x1e5e0U;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e5e0U: { // d442 add.w d2, d2
            next = 0x1e5e2U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e5e2U: { // 9242 sub.w d2, d1
            next = 0x1e5e4U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e5e4U: { // d242 add.w d2, d1
            next = 0x1e5e6U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e5e6U: { // 51c8fffc dbra d0, $1e5e4
            next = 0x1e5eaU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e5e4U;
            break;
        }
        case 0x1e5eaU: { // 6000fc06 bra.w $1e1f2
            next = 0x1e5eeU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01e59a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e5a2U: { // 4e75 rts 
            next = 0x1e5a4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1e5a2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1e5a4U: { // 6100064a bsr.w $1ebf0
            next = 0x1e5a8U;
            const auto result = m.call(c, 363U, 0x1e5a4U, 0x1ebf0U, 0x1e5a8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        default:
            if (!(cpu_b_stage_01e59a_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1e59aU:
        case 0x1e5a0U:
        case 0x1e5a2U:
        case 0x1e5a4U:
        case 0x1e5a8U:
        case 0x1e5aaU:
        case 0x1e5acU:
        case 0x1e5b0U:
        case 0x1e5b4U:
        case 0x1e5b6U:
        case 0x1e5b8U:
        case 0x1e5baU:
        case 0x1e5beU:
        case 0x1e5c2U:
        case 0x1e5c4U:
        case 0x1e5caU:
        case 0x1e5ccU:
        case 0x1e5ceU:
        case 0x1e5d0U:
        case 0x1e5d2U:
        case 0x1e5d4U:
        case 0x1e5d8U:
        case 0x1e5daU:
        case 0x1e5deU:
        case 0x1e5e0U:
        case 0x1e5e2U:
        case 0x1e5e4U:
        case 0x1e5e6U:
        case 0x1e5eaU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
