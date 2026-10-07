
FunctionResult cpu_b_stage_018a84(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18a84U: { // 4a2d0051 tst.b $51(a5)
            next = 0x18a88U;
            const auto destination_address = r.address[5] + 0x51U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x18a88U: { // 6706 beq.b $18a90
            next = 0x18a8aU;
            if ((r.status & 4U) != 0U) { next = 0x18a90U; transfer_kind = 1U; }
            break;
        }
        case 0x18a8aU: { // 3b7c000a0068 move.w #$a, $68(a5)
            next = 0x18a90U;
            const auto value = 0xaU;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18a90U: { // 41fa2112 lea.l $1aba4(pc), a0
            next = 0x18a94U;
            const auto source_address = 0x1aba4U;
            r.address[0] = source_address;
            break;
        }
        case 0x18a94U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x18a98U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18a98U: { // 6f04 ble.b $18a9e
            next = 0x18a9aU;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x18a9eU; transfer_kind = 1U; }
            break;
        }
        case 0x18a9aU: { // 41fa211c lea.l $1abb8(pc), a0
            next = 0x18a9eU;
            const auto source_address = 0x1abb8U;
            r.address[0] = source_address;
            break;
        }
        case 0x18a9eU: { // 302d0064 move.w $64(a5), d0
            next = 0x18aa2U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18aa2U: { // 1b7000010001 move.b $1(a0, d0.w), $1(a5)
            next = 0x18aa8U;
            const auto source_address = r.address[0] + 0x1U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18aa8U: { // 3b7000020006 move.w $2(a0, d0.w), $6(a5)
            next = 0x18aaeU;
            const auto source_address = r.address[0] + 0x2U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18aaeU: { // 4e75 rts 
            next = 0x18ab0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18aaeU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18a84U:
        case 0x18a88U:
        case 0x18a8aU:
        case 0x18a90U:
        case 0x18a94U:
        case 0x18a98U:
        case 0x18a9aU:
        case 0x18a9eU:
        case 0x18aa2U:
        case 0x18aa8U:
        case 0x18aaeU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_018b36(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18b36U: { // 41f900200000 lea.l $200000.l, a0
            next = 0x18b3cU;
            const auto source_address = 0x200000U;
            r.address[0] = source_address;
            break;
        }
        case 0x18b3cU: { // d0ed0062 adda.w $62(a5), a0
            next = 0x18b40U;
            const auto source_address = r.address[5] + 0x62U;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x18b40U: { // 382d0060 move.w $60(a5), d4
            next = 0x18b44U;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            m.dw(4U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b44U: { // 6100fda8 bsr.w $188ee
            next = 0x18b48U;
            const auto result = m.call(c, 879U, 0x18b44U, 0x188eeU, 0x18b48U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18b48U: { // 4e75 rts 
            next = 0x18b4aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18b48U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18b36U:
        case 0x18b3cU:
        case 0x18b40U:
        case 0x18b44U:
        case 0x18b48U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
