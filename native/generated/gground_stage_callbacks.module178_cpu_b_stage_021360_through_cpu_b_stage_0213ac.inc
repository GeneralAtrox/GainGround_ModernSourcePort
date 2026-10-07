
FunctionResult cpu_b_stage_021360(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21360U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21364U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21364U: { // 6708 beq.b $2136e
            next = 0x21366U;
            if ((r.status & 4U) != 0U) { next = 0x2136eU; transfer_kind = 1U; }
            break;
        }
        case 0x21366U: { // 4df83400 lea.l $3400.w, a6
            next = 0x2136aU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x2136aU: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x2136eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x2136eU: { // 6100b2e2 bsr.w $1c652
            next = 0x21372U;
            const auto result = m.call(c, 657U, 0x2136eU, 0x1c652U, 0x21372U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21372U: { // 6100de88 bsr.w $1f1fc
            next = 0x21376U;
            const auto result = m.call(c, 371U, 0x21372U, 0x1f1fcU, 0x21376U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21376U: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x2137cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x2137cU: { // 6702 beq.b $21380
            next = 0x2137eU;
            if ((r.status & 4U) != 0U) { next = 0x21380U; transfer_kind = 1U; }
            break;
        }
        case 0x2137eU: { // 4e75 rts 
            next = 0x21380U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2137eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21380U: { // 6100d218 bsr.w $1e59a
            next = 0x21384U;
            const auto result = m.call(c, 671U, 0x21380U, 0x1e59aU, 0x21384U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21384U: { // 4e75 rts 
            next = 0x21386U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21384U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21360U:
        case 0x21364U:
        case 0x21366U:
        case 0x2136aU:
        case 0x2136eU:
        case 0x21372U:
        case 0x21376U:
        case 0x2137cU:
        case 0x2137eU:
        case 0x21380U:
        case 0x21384U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021386(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21386U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2138aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2138aU: { // 6708 beq.b $21394
            next = 0x2138cU;
            if ((r.status & 4U) != 0U) { next = 0x21394U; transfer_kind = 1U; }
            break;
        }
        case 0x2138cU: { // 4df83400 lea.l $3400.w, a6
            next = 0x21390U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21390U: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x21394U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x22U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21394U: { // 6100b3d4 bsr.w $1c76a
            next = 0x21398U;
            const auto result = m.call(c, 659U, 0x21394U, 0x1c76aU, 0x21398U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21398U: { // 6100dc42 bsr.w $1efdc
            next = 0x2139cU;
            const auto result = m.call(c, 368U, 0x21398U, 0x1efdcU, 0x2139cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2139cU: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x213a2U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x213a2U: { // 6702 beq.b $213a6
            next = 0x213a4U;
            if ((r.status & 4U) != 0U) { next = 0x213a6U; transfer_kind = 1U; }
            break;
        }
        case 0x213a4U: { // 4e75 rts 
            next = 0x213a6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x213a4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x213a6U: { // 6100d1f2 bsr.w $1e59a
            next = 0x213aaU;
            const auto result = m.call(c, 671U, 0x213a6U, 0x1e59aU, 0x213aaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x213aaU: { // 4e75 rts 
            next = 0x213acU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x213aaU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21386U:
        case 0x2138aU:
        case 0x2138cU:
        case 0x21390U:
        case 0x21394U:
        case 0x21398U:
        case 0x2139cU:
        case 0x213a2U:
        case 0x213a4U:
        case 0x213a6U:
        case 0x213aaU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0213ac(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x213acU: { // 4a2d003f tst.b $3f(a5)
            next = 0x213b0U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x213b0U: { // 6708 beq.b $213ba
            next = 0x213b2U;
            if ((r.status & 4U) != 0U) { next = 0x213baU; transfer_kind = 1U; }
            break;
        }
        case 0x213b2U: { // 4df83400 lea.l $3400.w, a6
            next = 0x213b6U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x213b6U: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x213baU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x213baU: { // 6100b3ae bsr.w $1c76a
            next = 0x213beU;
            const auto result = m.call(c, 659U, 0x213baU, 0x1c76aU, 0x213beU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x213beU: { // 6100dc1c bsr.w $1efdc
            next = 0x213c2U;
            const auto result = m.call(c, 368U, 0x213beU, 0x1efdcU, 0x213c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x213c2U: { // 082d00070041 btst.b #$7, $41(a5)
            next = 0x213c8U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x213c8U: { // 6702 beq.b $213cc
            next = 0x213caU;
            if ((r.status & 4U) != 0U) { next = 0x213ccU; transfer_kind = 1U; }
            break;
        }
        case 0x213caU: { // 4e75 rts 
            next = 0x213ccU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x213caU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x213ccU: { // 6100d1cc bsr.w $1e59a
            next = 0x213d0U;
            const auto result = m.call(c, 671U, 0x213ccU, 0x1e59aU, 0x213d0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x213d0U: { // 4e75 rts 
            next = 0x213d2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x213d0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x213acU:
        case 0x213b0U:
        case 0x213b2U:
        case 0x213b6U:
        case 0x213baU:
        case 0x213beU:
        case 0x213c2U:
        case 0x213c8U:
        case 0x213caU:
        case 0x213ccU:
        case 0x213d0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
