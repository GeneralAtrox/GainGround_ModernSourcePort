
FunctionResult cpu_b_stage_02174e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2174eU: { // 6100b198 bsr.w $1c8e8
            next = 0x21752U;
            const auto result = m.call(c, 661U, 0x2174eU, 0x1c8e8U, 0x21752U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21752U: { // 6100e0ac bsr.w $1f800
            next = 0x21756U;
            const auto result = m.call(c, 687U, 0x21752U, 0x1f800U, 0x21756U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21756U: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x2175cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x2175cU: { // 6702 beq.b $21760
            next = 0x2175eU;
            if ((r.status & 4U) != 0U) { next = 0x21760U; transfer_kind = 1U; }
            break;
        }
        case 0x2175eU: { // 4e75 rts 
            next = 0x21760U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2175eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21760U: { // 6100cf6a bsr.w $1e6cc
            next = 0x21764U;
            const auto result = m.call(c, 674U, 0x21760U, 0x1e6ccU, 0x21764U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21764U: { // 102d0058 move.b $58(a5), d0
            next = 0x21768U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21768U: { // 0240003e andi.w #$3e, d0
            next = 0x2176cU;
            const auto source_value = 0x3eU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x2176cU: { // 0c400020 cmpi.w #$20, d0
            next = 0x21770U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x21770U: { // 6702 beq.b $21774
            next = 0x21772U;
            if ((r.status & 4U) != 0U) { next = 0x21774U; transfer_kind = 1U; }
            break;
        }
        case 0x21772U: { // 4e75 rts 
            next = 0x21774U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21772U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21774U: { // 6100d56e bsr.w $1ece4
            next = 0x21778U;
            const auto result = m.call(c, 365U, 0x21774U, 0x1ece4U, 0x21778U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21778U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2177cU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2177cU: { // 6708 beq.b $21786
            next = 0x2177eU;
            if ((r.status & 4U) != 0U) { next = 0x21786U; transfer_kind = 1U; }
            break;
        }
        case 0x2177eU: { // 4df83400 lea.l $3400.w, a6
            next = 0x21782U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21782U: { // 522e002e addq.b #$1, $2e(a6)
            next = 0x21786U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x2eU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21786U: { // 4e75 rts 
            next = 0x21788U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21786U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2174eU:
        case 0x21752U:
        case 0x21756U:
        case 0x2175cU:
        case 0x2175eU:
        case 0x21760U:
        case 0x21764U:
        case 0x21768U:
        case 0x2176cU:
        case 0x21770U:
        case 0x21772U:
        case 0x21774U:
        case 0x21778U:
        case 0x2177cU:
        case 0x2177eU:
        case 0x21782U:
        case 0x21786U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021788(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21788U: { // 6100b15e bsr.w $1c8e8
            next = 0x2178cU;
            const auto result = m.call(c, 661U, 0x21788U, 0x1c8e8U, 0x2178cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2178cU: { // 6100da6e bsr.w $1f1fc
            next = 0x21790U;
            const auto result = m.call(c, 371U, 0x2178cU, 0x1f1fcU, 0x21790U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21790U: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x21796U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21796U: { // 6702 beq.b $2179a
            next = 0x21798U;
            if ((r.status & 4U) != 0U) { next = 0x2179aU; transfer_kind = 1U; }
            break;
        }
        case 0x21798U: { // 4e75 rts 
            next = 0x2179aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21798U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2179aU: { // 6100cf30 bsr.w $1e6cc
            next = 0x2179eU;
            const auto result = m.call(c, 674U, 0x2179aU, 0x1e6ccU, 0x2179eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2179eU: { // 102d0058 move.b $58(a5), d0
            next = 0x217a2U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x217a2U: { // 0240003e andi.w #$3e, d0
            next = 0x217a6U;
            const auto source_value = 0x3eU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x217a6U: { // 0c400020 cmpi.w #$20, d0
            next = 0x217aaU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x217aaU: { // 6702 beq.b $217ae
            next = 0x217acU;
            if ((r.status & 4U) != 0U) { next = 0x217aeU; transfer_kind = 1U; }
            break;
        }
        case 0x217acU: { // 4e75 rts 
            next = 0x217aeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x217acU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x217aeU: { // 6100d534 bsr.w $1ece4
            next = 0x217b2U;
            const auto result = m.call(c, 365U, 0x217aeU, 0x1ece4U, 0x217b2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x217b2U: { // 4a2d003f tst.b $3f(a5)
            next = 0x217b6U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x217b6U: { // 6708 beq.b $217c0
            next = 0x217b8U;
            if ((r.status & 4U) != 0U) { next = 0x217c0U; transfer_kind = 1U; }
            break;
        }
        case 0x217b8U: { // 4df83400 lea.l $3400.w, a6
            next = 0x217bcU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x217bcU: { // 522e0030 addq.b #$1, $30(a6)
            next = 0x217c0U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x30U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x217c0U: { // 4e75 rts 
            next = 0x217c2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x217c0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21788U:
        case 0x2178cU:
        case 0x21790U:
        case 0x21796U:
        case 0x21798U:
        case 0x2179aU:
        case 0x2179eU:
        case 0x217a2U:
        case 0x217a6U:
        case 0x217aaU:
        case 0x217acU:
        case 0x217aeU:
        case 0x217b2U:
        case 0x217b6U:
        case 0x217b8U:
        case 0x217bcU:
        case 0x217c0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
