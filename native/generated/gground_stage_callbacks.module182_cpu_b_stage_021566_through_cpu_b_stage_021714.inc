
FunctionResult cpu_b_stage_021566(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21566U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2156aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2156aU: { // 6708 beq.b $21574
            next = 0x2156cU;
            if ((r.status & 4U) != 0U) { next = 0x21574U; transfer_kind = 1U; }
            break;
        }
        case 0x2156cU: { // 4df83400 lea.l $3400.w, a6
            next = 0x21570U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21570U: { // 522e0026 addq.b #$1, $26(a6)
            next = 0x21574U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x26U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21574U: { // 6100b1f4 bsr.w $1c76a
            next = 0x21578U;
            const auto result = m.call(c, 659U, 0x21574U, 0x1c76aU, 0x21578U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21578U: { // 6100da62 bsr.w $1efdc
            next = 0x2157cU;
            const auto result = m.call(c, 368U, 0x21578U, 0x1efdcU, 0x2157cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2157cU: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x21582U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21582U: { // 6702 beq.b $21586
            next = 0x21584U;
            if ((r.status & 4U) != 0U) { next = 0x21586U; transfer_kind = 1U; }
            break;
        }
        case 0x21584U: { // 4e75 rts 
            next = 0x21586U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21584U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21586U: { // 6100d012 bsr.w $1e59a
            next = 0x2158aU;
            const auto result = m.call(c, 671U, 0x21586U, 0x1e59aU, 0x2158aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2158aU: { // 4e75 rts 
            next = 0x2158cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2158aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21566U:
        case 0x2156aU:
        case 0x2156cU:
        case 0x21570U:
        case 0x21574U:
        case 0x21578U:
        case 0x2157cU:
        case 0x21582U:
        case 0x21584U:
        case 0x21586U:
        case 0x2158aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02158c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2158cU: { // 6100a832 bsr.w $1bdc0
            next = 0x21590U;
            const auto result = m.call(c, 327U, 0x2158cU, 0x1bdc0U, 0x21590U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21590U: { // 6100c65e bsr.w $1dbf0
            next = 0x21594U;
            const auto result = m.call(c, 353U, 0x21590U, 0x1dbf0U, 0x21594U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21594U: { // 6100dc66 bsr.w $1f1fc
            next = 0x21598U;
            const auto result = m.call(c, 371U, 0x21594U, 0x1f1fcU, 0x21598U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21598U: { // 6100d0c6 bsr.w $1e660
            next = 0x2159cU;
            const auto result = m.call(c, 673U, 0x21598U, 0x1e660U, 0x2159cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2159cU: { // 4e75 rts 
            next = 0x2159eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2159cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2158cU:
        case 0x21590U:
        case 0x21594U:
        case 0x21598U:
        case 0x2159cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021714(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21714U: { // 6100b1d2 bsr.w $1c8e8
            next = 0x21718U;
            const auto result = m.call(c, 661U, 0x21714U, 0x1c8e8U, 0x21718U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21718U: { // 6100e0e6 bsr.w $1f800
            next = 0x2171cU;
            const auto result = m.call(c, 687U, 0x21718U, 0x1f800U, 0x2171cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2171cU: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x21722U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21722U: { // 6702 beq.b $21726
            next = 0x21724U;
            if ((r.status & 4U) != 0U) { next = 0x21726U; transfer_kind = 1U; }
            break;
        }
        case 0x21724U: { // 4e75 rts 
            next = 0x21726U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21724U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21726U: { // 6100cfa4 bsr.w $1e6cc
            next = 0x2172aU;
            const auto result = m.call(c, 674U, 0x21726U, 0x1e6ccU, 0x2172aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2172aU: { // 102d0058 move.b $58(a5), d0
            next = 0x2172eU;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x2172eU: { // 0240003e andi.w #$3e, d0
            next = 0x21732U;
            const auto source_value = 0x3eU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x21732U: { // 0c400020 cmpi.w #$20, d0
            next = 0x21736U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x21736U: { // 6702 beq.b $2173a
            next = 0x21738U;
            if ((r.status & 4U) != 0U) { next = 0x2173aU; transfer_kind = 1U; }
            break;
        }
        case 0x21738U: { // 4e75 rts 
            next = 0x2173aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21738U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2173aU: { // 6100d5a8 bsr.w $1ece4
            next = 0x2173eU;
            const auto result = m.call(c, 365U, 0x2173aU, 0x1ece4U, 0x2173eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2173eU: { // 4a2d003f tst.b $3f(a5)
            next = 0x21742U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21742U: { // 6708 beq.b $2174c
            next = 0x21744U;
            if ((r.status & 4U) != 0U) { next = 0x2174cU; transfer_kind = 1U; }
            break;
        }
        case 0x21744U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21748U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21748U: { // 522e002c addq.b #$1, $2c(a6)
            next = 0x2174cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x2cU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x2174cU: { // 4e75 rts 
            next = 0x2174eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2174cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21714U:
        case 0x21718U:
        case 0x2171cU:
        case 0x21722U:
        case 0x21724U:
        case 0x21726U:
        case 0x2172aU:
        case 0x2172eU:
        case 0x21732U:
        case 0x21736U:
        case 0x21738U:
        case 0x2173aU:
        case 0x2173eU:
        case 0x21742U:
        case 0x21744U:
        case 0x21748U:
        case 0x2174cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
