
FunctionResult cpu_b_stage_0212ea(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x212eaU: { // 4a2d003f tst.b $3f(a5)
            next = 0x212eeU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x212eeU: { // 6708 beq.b $212f8
            next = 0x212f0U;
            if ((r.status & 4U) != 0U) { next = 0x212f8U; transfer_kind = 1U; }
            break;
        }
        case 0x212f0U: { // 4df83400 lea.l $3400.w, a6
            next = 0x212f4U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x212f4U: { // 522e002a addq.b #$1, $2a(a6)
            next = 0x212f8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x2aU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x212f8U: { // 3b7cfff00016 move.w #$fff0, $16(a5)
            next = 0x212feU;
            const auto value = 0xfff0U;
            const auto destination_address = r.address[5] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x212feU: { // 6100b21c bsr.w $1c51c
            next = 0x21302U;
            const auto result = m.call(c, 655U, 0x212feU, 0x1c51cU, 0x21302U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21302U: { // 6100df46 bsr.w $1f24a
            next = 0x21306U;
            const auto result = m.call(c, 372U, 0x21302U, 0x1f24aU, 0x21306U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21306U: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x2130cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x2130cU: { // 6702 beq.b $21310
            next = 0x2130eU;
            if ((r.status & 4U) != 0U) { next = 0x21310U; transfer_kind = 1U; }
            break;
        }
        case 0x2130eU: { // 4e75 rts 
            next = 0x21310U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2130eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21310U: { // 6100d202 bsr.w $1e514
            next = 0x21314U;
            const auto result = m.call(c, 670U, 0x21310U, 0x1e514U, 0x21314U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21314U: { // 4e75 rts 
            next = 0x21316U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21314U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x212eaU:
        case 0x212eeU:
        case 0x212f0U:
        case 0x212f4U:
        case 0x212f8U:
        case 0x212feU:
        case 0x21302U:
        case 0x21306U:
        case 0x2130cU:
        case 0x2130eU:
        case 0x21310U:
        case 0x21314U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021316(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21316U: { // 6100a9f4 bsr.w $1bd0c
            next = 0x2131aU;
            const auto result = m.call(c, 326U, 0x21316U, 0x1bd0cU, 0x2131aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2131aU: { // 6100c8a8 bsr.w $1dbc4
            next = 0x2131eU;
            const auto result = m.call(c, 352U, 0x2131aU, 0x1dbc4U, 0x2131eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2131eU: { // 6100c8d0 bsr.w $1dbf0
            next = 0x21322U;
            const auto result = m.call(c, 353U, 0x2131eU, 0x1dbf0U, 0x21322U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21322U: { // 6100cb0c bsr.w $1de30
            next = 0x21326U;
            const auto result = m.call(c, 355U, 0x21322U, 0x1de30U, 0x21326U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21326U: { // 6100cdc6 bsr.w $1e0ee
            next = 0x2132aU;
            const auto result = m.call(c, 356U, 0x21326U, 0x1e0eeU, 0x2132aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2132aU: { // 6100cdf8 bsr.w $1e124
            next = 0x2132eU;
            const auto result = m.call(c, 357U, 0x2132aU, 0x1e124U, 0x2132eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2132eU: { // 1b7c00000059 move.b #$0, $59(a5)
            next = 0x21334U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x59U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21334U: { // 6100ce4a bsr.w $1e180
            next = 0x21338U;
            const auto result = m.call(c, 359U, 0x21334U, 0x1e180U, 0x21338U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21338U: { // 4e75 rts 
            next = 0x2133aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21338U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21316U:
        case 0x2131aU:
        case 0x2131eU:
        case 0x21322U:
        case 0x21326U:
        case 0x2132aU:
        case 0x2132eU:
        case 0x21334U:
        case 0x21338U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02133a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2133aU: { // 4a2d003f tst.b $3f(a5)
            next = 0x2133eU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2133eU: { // 6708 beq.b $21348
            next = 0x21340U;
            if ((r.status & 4U) != 0U) { next = 0x21348U; transfer_kind = 1U; }
            break;
        }
        case 0x21340U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21344U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21344U: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x21348U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x22U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21348U: { // 6100b308 bsr.w $1c652
            next = 0x2134cU;
            const auto result = m.call(c, 657U, 0x21348U, 0x1c652U, 0x2134cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2134cU: { // 6100deae bsr.w $1f1fc
            next = 0x21350U;
            const auto result = m.call(c, 371U, 0x2134cU, 0x1f1fcU, 0x21350U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21350U: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x21356U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21356U: { // 6702 beq.b $2135a
            next = 0x21358U;
            if ((r.status & 4U) != 0U) { next = 0x2135aU; transfer_kind = 1U; }
            break;
        }
        case 0x21358U: { // 4e75 rts 
            next = 0x2135aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21358U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2135aU: { // 6100d23e bsr.w $1e59a
            next = 0x2135eU;
            const auto result = m.call(c, 671U, 0x2135aU, 0x1e59aU, 0x2135eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2135eU: { // 4e75 rts 
            next = 0x21360U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2135eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2133aU:
        case 0x2133eU:
        case 0x21340U:
        case 0x21344U:
        case 0x21348U:
        case 0x2134cU:
        case 0x21350U:
        case 0x21356U:
        case 0x21358U:
        case 0x2135aU:
        case 0x2135eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
