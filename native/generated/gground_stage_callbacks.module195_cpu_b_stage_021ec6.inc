
FunctionResult cpu_b_stage_021ec6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21ec6U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21ecaU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21ecaU: { // 671e beq.b $21eea
            next = 0x21eccU;
            if ((r.status & 4U) != 0U) { next = 0x21eeaU; transfer_kind = 1U; }
            break;
        }
        case 0x21eccU: { // 4df83400 lea.l $3400.w, a6
            next = 0x21ed0U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21ed0U: { // 522e0026 addq.b #$1, $26(a6)
            next = 0x21ed4U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x26U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21ed4U: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x21edcU;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x21edcU: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x21ee2U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x21ee2U: { // 7200 moveq #$0, d1
            next = 0x21ee4U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x21ee4U: { // 6100c41e bsr.w $1e304
            next = 0x21ee8U;
            const auto result = m.call(c, 360U, 0x21ee4U, 0x1e304U, 0x21ee8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ee8U: { // 4e75 rts 
            next = 0x21eeaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21ee8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21eeaU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x21ef0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21ef0U: { // 671a beq.b $21f0c
            next = 0x21ef2U;
            if ((r.status & 4U) != 0U) { next = 0x21f0cU; transfer_kind = 1U; }
            break;
        }
        case 0x21ef2U: { // 6100b002 bsr.w $1cef6
            next = 0x21ef6U;
            const auto result = m.call(c, 334U, 0x21ef2U, 0x1cef6U, 0x21ef6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ef6U: { // 6100bccc bsr.w $1dbc4
            next = 0x21efaU;
            const auto result = m.call(c, 352U, 0x21ef6U, 0x1dbc4U, 0x21efaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21efaU: { // 6100bcf4 bsr.w $1dbf0
            next = 0x21efeU;
            const auto result = m.call(c, 353U, 0x21efaU, 0x1dbf0U, 0x21efeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21efeU: { // 6100c224 bsr.w $1e124
            next = 0x21f02U;
            const auto result = m.call(c, 357U, 0x21efeU, 0x1e124U, 0x21f02U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f02U: { // 6100c400 bsr.w $1e304
            next = 0x21f06U;
            const auto result = m.call(c, 360U, 0x21f02U, 0x1e304U, 0x21f06U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f06U: { // 6100cddc bsr.w $1ece4
            next = 0x21f0aU;
            const auto result = m.call(c, 365U, 0x21f06U, 0x1ece4U, 0x21f0aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f0aU: { // 4e75 rts 
            next = 0x21f0cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21f0aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21f0cU: { // 61009eb2 bsr.w $1bdc0
            next = 0x21f10U;
            const auto result = m.call(c, 327U, 0x21f0cU, 0x1bdc0U, 0x21f10U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f10U: { // 6100d2ea bsr.w $1f1fc
            next = 0x21f14U;
            const auto result = m.call(c, 371U, 0x21f10U, 0x1f1fcU, 0x21f14U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f14U: { // 6100c3ee bsr.w $1e304
            next = 0x21f18U;
            const auto result = m.call(c, 360U, 0x21f14U, 0x1e304U, 0x21f18U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f18U: { // 4e75 rts 
            next = 0x21f1aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21f18U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21ec6U:
        case 0x21ecaU:
        case 0x21eccU:
        case 0x21ed0U:
        case 0x21ed4U:
        case 0x21edcU:
        case 0x21ee2U:
        case 0x21ee4U:
        case 0x21ee8U:
        case 0x21eeaU:
        case 0x21ef0U:
        case 0x21ef2U:
        case 0x21ef6U:
        case 0x21efaU:
        case 0x21efeU:
        case 0x21f02U:
        case 0x21f06U:
        case 0x21f0aU:
        case 0x21f0cU:
        case 0x21f10U:
        case 0x21f14U:
        case 0x21f18U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
