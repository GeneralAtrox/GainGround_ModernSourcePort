
FunctionResult cpu_b_stage_020f9c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20f9cU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x20fa2U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x20fa2U: { // 6728 beq.b $20fcc
            next = 0x20fa4U;
            if ((r.status & 4U) != 0U) { next = 0x20fccU; transfer_kind = 1U; }
            break;
        }
        case 0x20fa4U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20fa8U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20fa8U: { // 6708 beq.b $20fb2
            next = 0x20faaU;
            if ((r.status & 4U) != 0U) { next = 0x20fb2U; transfer_kind = 1U; }
            break;
        }
        case 0x20faaU: { // 4df83400 lea.l $3400.w, a6
            next = 0x20faeU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x20faeU: { // 522e0028 addq.b #$1, $28(a6)
            next = 0x20fb2U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x28U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x20fb2U: { // 6100bf42 bsr.w $1cef6
            next = 0x20fb6U;
            const auto result = m.call(c, 334U, 0x20fb2U, 0x1cef6U, 0x20fb6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20fb6U: { // 6100cc0c bsr.w $1dbc4
            next = 0x20fbaU;
            const auto result = m.call(c, 352U, 0x20fb6U, 0x1dbc4U, 0x20fbaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20fbaU: { // 6100cc34 bsr.w $1dbf0
            next = 0x20fbeU;
            const auto result = m.call(c, 353U, 0x20fbaU, 0x1dbf0U, 0x20fbeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20fbeU: { // 6100d164 bsr.w $1e124
            next = 0x20fc2U;
            const auto result = m.call(c, 357U, 0x20fbeU, 0x1e124U, 0x20fc2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20fc2U: { // 6100d340 bsr.w $1e304
            next = 0x20fc6U;
            const auto result = m.call(c, 360U, 0x20fc2U, 0x1e304U, 0x20fc6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20fc6U: { // 6100dd1c bsr.w $1ece4
            next = 0x20fcaU;
            const auto result = m.call(c, 365U, 0x20fc6U, 0x1ece4U, 0x20fcaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20fcaU: { // 4e75 rts 
            next = 0x20fccU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20fcaU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20fccU: { // 4a2d003f tst.b $3f(a5)
            next = 0x20fd0U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20fd0U: { // 6708 beq.b $20fda
            next = 0x20fd2U;
            if ((r.status & 4U) != 0U) { next = 0x20fdaU; transfer_kind = 1U; }
            break;
        }
        case 0x20fd2U: { // 4df83400 lea.l $3400.w, a6
            next = 0x20fd6U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x20fd6U: { // 522e0028 addq.b #$1, $28(a6)
            next = 0x20fdaU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x28U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x20fdaU: { // 6100ade4 bsr.w $1bdc0
            next = 0x20fdeU;
            const auto result = m.call(c, 327U, 0x20fdaU, 0x1bdc0U, 0x20fdeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20fdeU: { // 6100e21c bsr.w $1f1fc
            next = 0x20fe2U;
            const auto result = m.call(c, 371U, 0x20fdeU, 0x1f1fcU, 0x20fe2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20fe2U: { // 6100d3c2 bsr.w $1e3a6
            next = 0x20fe6U;
            const auto result = m.call(c, 361U, 0x20fe2U, 0x1e3a6U, 0x20fe6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20fe6U: { // 4e75 rts 
            next = 0x20fe8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20fe6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20f9cU:
        case 0x20fa2U:
        case 0x20fa4U:
        case 0x20fa8U:
        case 0x20faaU:
        case 0x20faeU:
        case 0x20fb2U:
        case 0x20fb6U:
        case 0x20fbaU:
        case 0x20fbeU:
        case 0x20fc2U:
        case 0x20fc6U:
        case 0x20fcaU:
        case 0x20fccU:
        case 0x20fd0U:
        case 0x20fd2U:
        case 0x20fd6U:
        case 0x20fdaU:
        case 0x20fdeU:
        case 0x20fe2U:
        case 0x20fe6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_020ff6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20ff6U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20ffaU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20ffaU: { // 6710 beq.b $2100c
            next = 0x20ffcU;
            if ((r.status & 4U) != 0U) { next = 0x2100cU; transfer_kind = 1U; }
            break;
        }
        case 0x20ffcU: { // 2b7c0001edf40002 move.l #$1edf4, $2(a5)
            next = 0x21004U;
            const auto value = 0x1edf4U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x21004U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x2100aU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x2100aU: { // 4e75 rts 
            next = 0x2100cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2100aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2100cU: { // 7200 moveq #$0, d1
            next = 0x2100eU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x2100eU: { // 4eb90001e1f2 jsr $1e1f2.l
            next = 0x21014U;
            const auto result = m.call(c, 554U, 0x2100eU, 0x1e1f2U, 0x21014U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21014U: { // 4e75 rts 
            next = 0x21016U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21014U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20ff6U:
        case 0x20ffaU:
        case 0x20ffcU:
        case 0x21004U:
        case 0x2100aU:
        case 0x2100cU:
        case 0x2100eU:
        case 0x21014U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_020fea(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20feaU: { // 6100dcf8 bsr.w $1ece4
            next = 0x20feeU;
            const auto result = m.call(c, 365U, 0x20feaU, 0x1ece4U, 0x20feeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20feeU: { // 2b7c00020ff60002 move.l #$20ff6, $2(a5)
            next = 0x20ff6U;
            const auto value = 0x20ff6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20feaU:
        case 0x20feeU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
