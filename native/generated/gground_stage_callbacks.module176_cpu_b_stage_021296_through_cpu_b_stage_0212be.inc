
FunctionResult cpu_b_stage_021296(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21296U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2129aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2129aU: { // 6708 beq.b $212a4
            next = 0x2129cU;
            if ((r.status & 4U) != 0U) { next = 0x212a4U; transfer_kind = 1U; }
            break;
        }
        case 0x2129cU: { // 4df83400 lea.l $3400.w, a6
            next = 0x212a0U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x212a0U: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x212a4U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x212a4U: { // 6100b1ce bsr.w $1c474
            next = 0x212a8U;
            const auto result = m.call(c, 654U, 0x212a4U, 0x1c474U, 0x212a8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x212a8U: { // 6100c91a bsr.w $1dbc4
            next = 0x212acU;
            const auto result = m.call(c, 352U, 0x212a8U, 0x1dbc4U, 0x212acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x212acU: { // 6100dd2e bsr.w $1efdc
            next = 0x212b0U;
            const auto result = m.call(c, 368U, 0x212acU, 0x1efdcU, 0x212b0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x212b0U: { // 6100c93e bsr.w $1dbf0
            next = 0x212b4U;
            const auto result = m.call(c, 353U, 0x212b0U, 0x1dbf0U, 0x212b4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x212b4U: { // 6100ce6e bsr.w $1e124
            next = 0x212b8U;
            const auto result = m.call(c, 357U, 0x212b4U, 0x1e124U, 0x212b8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x212b8U: { // 6100d04a bsr.w $1e304
            next = 0x212bcU;
            const auto result = m.call(c, 360U, 0x212b8U, 0x1e304U, 0x212bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x212bcU: { // 4e75 rts 
            next = 0x212beU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x212bcU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21296U:
        case 0x2129aU:
        case 0x2129cU:
        case 0x212a0U:
        case 0x212a4U:
        case 0x212a8U:
        case 0x212acU:
        case 0x212b0U:
        case 0x212b4U:
        case 0x212b8U:
        case 0x212bcU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0212be(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x212beU: { // 4a2d003f tst.b $3f(a5)
            next = 0x212c2U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x212c2U: { // 6708 beq.b $212cc
            next = 0x212c4U;
            if ((r.status & 4U) != 0U) { next = 0x212ccU; transfer_kind = 1U; }
            break;
        }
        case 0x212c4U: { // 4df83400 lea.l $3400.w, a6
            next = 0x212c8U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x212c8U: { // 522e0028 addq.b #$1, $28(a6)
            next = 0x212ccU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x28U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x212ccU: { // 3b7cfff00016 move.w #$fff0, $16(a5)
            next = 0x212d2U;
            const auto value = 0xfff0U;
            const auto destination_address = r.address[5] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x212d2U: { // 6100b248 bsr.w $1c51c
            next = 0x212d6U;
            const auto result = m.call(c, 655U, 0x212d2U, 0x1c51cU, 0x212d6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x212d6U: { // 6100df72 bsr.w $1f24a
            next = 0x212daU;
            const auto result = m.call(c, 372U, 0x212d6U, 0x1f24aU, 0x212daU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x212daU: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x212e0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x212e0U: { // 6702 beq.b $212e4
            next = 0x212e2U;
            if ((r.status & 4U) != 0U) { next = 0x212e4U; transfer_kind = 1U; }
            break;
        }
        case 0x212e2U: { // 4e75 rts 
            next = 0x212e4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x212e2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x212e4U: { // 6100d22e bsr.w $1e514
            next = 0x212e8U;
            const auto result = m.call(c, 670U, 0x212e4U, 0x1e514U, 0x212e8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x212e8U: { // 4e75 rts 
            next = 0x212eaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x212e8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x212beU:
        case 0x212c2U:
        case 0x212c4U:
        case 0x212c8U:
        case 0x212ccU:
        case 0x212d2U:
        case 0x212d6U:
        case 0x212daU:
        case 0x212e0U:
        case 0x212e2U:
        case 0x212e4U:
        case 0x212e8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
