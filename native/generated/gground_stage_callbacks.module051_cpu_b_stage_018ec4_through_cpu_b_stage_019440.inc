
FunctionResult cpu_b_stage_018ec4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18ec4U: { // 302d0040 move.w $40(a5), d0
            next = 0x18ec8U;
            const auto source_address = r.address[5] + 0x40U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ec8U: { // e540 asl.w #$2, d0
            next = 0x18ecaU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x18ecaU: { // 4ebb002a jsr $18ef6(pc, d0.w)
            next = 0x18eceU;
            const auto target = (0x18ef6U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            const auto result = m.indirect_call(c, 0x18ecaU, target, 0x18eceU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18eceU: { // 41fa2128 lea.l $1aff8(pc), a0
            next = 0x18ed2U;
            const auto source_address = 0x1aff8U;
            r.address[0] = source_address;
            break;
        }
        case 0x18ed2U: { // 302d0040 move.w $40(a5), d0
            next = 0x18ed6U;
            const auto source_address = r.address[5] + 0x40U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ed6U: { // e740 asl.w #$3, d0
            next = 0x18ed8U;
            m.asl_word(0U, 3U);
            break;
        }
        case 0x18ed8U: { // 3200 move.w d0, d1
            next = 0x18edaU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18edaU: { // d040 add.w d0, d0
            next = 0x18edcU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18edcU: { // d041 add.w d1, d0
            next = 0x18edeU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18edeU: { // 322d005a move.w $5a(a5), d1
            next = 0x18ee2U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ee2U: { // 5341 subq.w #$1, d1
            next = 0x18ee4U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18ee4U: { // e741 asl.w #$3, d1
            next = 0x18ee6U;
            m.asl_word(1U, 3U);
            break;
        }
        case 0x18ee6U: { // d041 add.w d1, d0
            next = 0x18ee8U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18ee8U: { // d0c0 adda.w d0, a0
            next = 0x18eeaU;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x18eeaU: { // 302d0066 move.w $66(a5), d0
            next = 0x18eeeU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18eeeU: { // 1b7000000059 move.b (a0, d0.w), $59(a5)
            next = 0x18ef4U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x59U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18ef4U: { // 4e75 rts 
            next = 0x18ef6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18ef4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18ec4U:
        case 0x18ec8U:
        case 0x18ecaU:
        case 0x18eceU:
        case 0x18ed2U:
        case 0x18ed6U:
        case 0x18ed8U:
        case 0x18edaU:
        case 0x18edcU:
        case 0x18edeU:
        case 0x18ee2U:
        case 0x18ee4U:
        case 0x18ee6U:
        case 0x18ee8U:
        case 0x18eeaU:
        case 0x18eeeU:
        case 0x18ef4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_019440_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19440U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x19444U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19444U: { // 6e22 bgt.b $19468
            next = 0x19446U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19468U; transfer_kind = 1U; }
            break;
        }
        case 0x19446U: { // 3b7c00020068 move.w #$2, $68(a5)
            next = 0x1944cU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1944cU: { // 536d0066 subq.w #$1, $66(a5)
            next = 0x19450U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19450U: { // 6e04 bgt.b $19456
            next = 0x19452U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19456U; transfer_kind = 1U; }
            break;
        }
        case 0x19452U: { // 4255 clr.w (a5)
            next = 0x19454U;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19456U: { // 526d0008 addq.w #$1, $8(a5)
            next = 0x1945aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1945aU: { // 0c6d0c160008 cmpi.w #$c16, $8(a5)
            next = 0x19460U;
            const auto source_value = 0xc16U;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19460U: { // 6f06 ble.b $19468
            next = 0x19462U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x19468U; transfer_kind = 1U; }
            break;
        }
        case 0x19462U: { // 3b7c0c120008 move.w #$c12, $8(a5)
            next = 0x19468U;
            const auto value = 0xc12U;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19468U: { // 422d003f clr.b $3f(a5)
            next = 0x1946cU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_019440(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19454U: { // 4e75 rts 
            next = 0x19456U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19454U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1946cU: { // 4eb900015df2 jsr $15df2.l
            next = 0x19472U;
            const auto result = m.call(c, 282U, 0x1946cU, 0x15df2U, 0x19472U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19472U: { // 4e75 rts 
            next = 0x19474U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19472U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_019440_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19440U:
        case 0x19444U:
        case 0x19446U:
        case 0x1944cU:
        case 0x19450U:
        case 0x19452U:
        case 0x19454U:
        case 0x19456U:
        case 0x1945aU:
        case 0x19460U:
        case 0x19462U:
        case 0x19468U:
        case 0x1946cU:
        case 0x19472U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
