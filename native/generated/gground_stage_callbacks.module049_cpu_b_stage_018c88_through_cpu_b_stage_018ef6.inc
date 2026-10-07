
FunctionResult cpu_b_stage_018c88(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18d00U: { // 4eb90001631a jsr $1631a.l
            next = 0x18d06U;
            const auto result = m.call(c, 304U, 0x18d00U, 0x1631aU, 0x18d06U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18d06U: { // 4eb9000163ba jsr $163ba.l
            next = 0x18d0cU;
            const auto result = m.call(c, 867U, 0x18d06U, 0x163baU, 0x18d0cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18d48U: { // 4e75 rts 
            next = 0x18d4aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18d48U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18d5aU: { // 4e75 rts 
            next = 0x18d5cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18d5aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18d72U: { // 4e75 rts 
            next = 0x18d74U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18d72U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18dc0U: { // 4eb90001631a jsr $1631a.l
            next = 0x18dc6U;
            const auto result = m.call(c, 304U, 0x18dc0U, 0x1631aU, 0x18dc6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18dc6U: { // 4eb9000163ba jsr $163ba.l
            next = 0x18dccU;
            const auto result = m.call(c, 867U, 0x18dc6U, 0x163baU, 0x18dccU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18e18U: { // 4e75 rts 
            next = 0x18e1aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18e18U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18e48U: { // 4e75 rts 
            next = 0x18e4aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18e48U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18e7cU: { // 4e75 rts 
            next = 0x18e7eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18e7cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18ea4U: { // 4e75 rts 
            next = 0x18ea6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18ea4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18ec2U: { // 4e75 rts 
            next = 0x18ec4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18ec2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1911cU: { // 4e75 rts 
            next = 0x1911eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1911cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19164U: { // 4e75 rts 
            next = 0x19166U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19164U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_018c88_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_018c88_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_018c88_dispatch_cases_002(m, c, pc, next, transfer_kind) || cpu_b_stage_018c88_dispatch_cases_003(m, c, pc, next, transfer_kind) || cpu_b_stage_018c88_dispatch_cases_004(m, c, pc, next, transfer_kind) || cpu_b_stage_018c88_dispatch_cases_005(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_018c88_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

FunctionResult cpu_b_stage_018ef6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
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
        case 0x18ef6U: { // 6000ffd6 bra.w $18ece
            next = 0x18efaU;
            if (true) { next = 0x18eceU; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
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
        case 0x18ef6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
