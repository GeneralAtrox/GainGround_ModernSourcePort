
FunctionResult cpu_b_stage_021502(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21502U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x21508U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21508U: { // 6728 beq.b $21532
            next = 0x2150aU;
            if ((r.status & 4U) != 0U) { next = 0x21532U; transfer_kind = 1U; }
            break;
        }
        case 0x2150aU: { // 4a2d003f tst.b $3f(a5)
            next = 0x2150eU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2150eU: { // 6708 beq.b $21518
            next = 0x21510U;
            if ((r.status & 4U) != 0U) { next = 0x21518U; transfer_kind = 1U; }
            break;
        }
        case 0x21510U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21514U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21514U: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x21518U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21518U: { // 6100b9dc bsr.w $1cef6
            next = 0x2151cU;
            const auto result = m.call(c, 334U, 0x21518U, 0x1cef6U, 0x2151cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2151cU: { // 6100c6a6 bsr.w $1dbc4
            next = 0x21520U;
            const auto result = m.call(c, 352U, 0x2151cU, 0x1dbc4U, 0x21520U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21520U: { // 6100c6ce bsr.w $1dbf0
            next = 0x21524U;
            const auto result = m.call(c, 353U, 0x21520U, 0x1dbf0U, 0x21524U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21524U: { // 6100cbfe bsr.w $1e124
            next = 0x21528U;
            const auto result = m.call(c, 357U, 0x21524U, 0x1e124U, 0x21528U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21528U: { // 6100cdda bsr.w $1e304
            next = 0x2152cU;
            const auto result = m.call(c, 360U, 0x21528U, 0x1e304U, 0x2152cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2152cU: { // 6100d7b6 bsr.w $1ece4
            next = 0x21530U;
            const auto result = m.call(c, 365U, 0x2152cU, 0x1ece4U, 0x21530U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21530U: { // 4e75 rts 
            next = 0x21532U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21530U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21532U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21536U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21536U: { // 6708 beq.b $21540
            next = 0x21538U;
            if ((r.status & 4U) != 0U) { next = 0x21540U; transfer_kind = 1U; }
            break;
        }
        case 0x21538U: { // 4df83400 lea.l $3400.w, a6
            next = 0x2153cU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x2153cU: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x21540U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21540U: { // 1b6d005f005e move.b $5f(a5), $5e(a5)
            next = 0x21546U;
            const auto source_address = r.address[5] + 0x5fU;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x5eU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21546U: { // 6100a878 bsr.w $1bdc0
            next = 0x2154aU;
            const auto result = m.call(c, 327U, 0x21546U, 0x1bdc0U, 0x2154aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2154aU: { // 6100dcfe bsr.w $1f24a
            next = 0x2154eU;
            const auto result = m.call(c, 372U, 0x2154aU, 0x1f24aU, 0x2154eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2154eU: { // 6100ce56 bsr.w $1e3a6
            next = 0x21552U;
            const auto result = m.call(c, 361U, 0x2154eU, 0x1e3a6U, 0x21552U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21552U: { // 4e75 rts 
            next = 0x21554U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21552U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21502U:
        case 0x21508U:
        case 0x2150aU:
        case 0x2150eU:
        case 0x21510U:
        case 0x21514U:
        case 0x21518U:
        case 0x2151cU:
        case 0x21520U:
        case 0x21524U:
        case 0x21528U:
        case 0x2152cU:
        case 0x21530U:
        case 0x21532U:
        case 0x21536U:
        case 0x21538U:
        case 0x2153cU:
        case 0x21540U:
        case 0x21546U:
        case 0x2154aU:
        case 0x2154eU:
        case 0x21552U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021554(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21554U: { // 6100a86a bsr.w $1bdc0
            next = 0x21558U;
            const auto result = m.call(c, 327U, 0x21554U, 0x1bdc0U, 0x21558U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21558U: { // 6100c696 bsr.w $1dbf0
            next = 0x2155cU;
            const auto result = m.call(c, 353U, 0x21558U, 0x1dbf0U, 0x2155cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2155cU: { // 6100e0f6 bsr.w $1f654
            next = 0x21560U;
            const auto result = m.call(c, 686U, 0x2155cU, 0x1f654U, 0x21560U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21560U: { // 6100ce44 bsr.w $1e3a6
            next = 0x21564U;
            const auto result = m.call(c, 361U, 0x21560U, 0x1e3a6U, 0x21564U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21564U: { // 4e75 rts 
            next = 0x21566U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21564U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21554U:
        case 0x21558U:
        case 0x2155cU:
        case 0x21560U:
        case 0x21564U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
