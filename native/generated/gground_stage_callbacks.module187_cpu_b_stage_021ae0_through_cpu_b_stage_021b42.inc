
FunctionResult cpu_b_stage_021ae0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21ae0U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21ae4U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21ae4U: { // 6708 beq.b $21aee
            next = 0x21ae6U;
            if ((r.status & 4U) != 0U) { next = 0x21aeeU; transfer_kind = 1U; }
            break;
        }
        case 0x21ae6U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21aeaU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21aeaU: { // 522e0028 addq.b #$1, $28(a6)
            next = 0x21aeeU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x28U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21aeeU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x21af4U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21af4U: { // 671c beq.b $21b12
            next = 0x21af6U;
            if ((r.status & 4U) != 0U) { next = 0x21b12U; transfer_kind = 1U; }
            break;
        }
        case 0x21af6U: { // 6100b3fe bsr.w $1cef6
            next = 0x21afaU;
            const auto result = m.call(c, 334U, 0x21af6U, 0x1cef6U, 0x21afaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21afaU: { // 6100c0c8 bsr.w $1dbc4
            next = 0x21afeU;
            const auto result = m.call(c, 352U, 0x21afaU, 0x1dbc4U, 0x21afeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21afeU: { // 6100c0f0 bsr.w $1dbf0
            next = 0x21b02U;
            const auto result = m.call(c, 353U, 0x21afeU, 0x1dbf0U, 0x21b02U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b02U: { // 6100c620 bsr.w $1e124
            next = 0x21b06U;
            const auto result = m.call(c, 357U, 0x21b02U, 0x1e124U, 0x21b06U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b06U: { // 6100cdd2 bsr.w $1e8da
            next = 0x21b0aU;
            const auto result = m.call(c, 676U, 0x21b06U, 0x1e8daU, 0x21b0aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b0aU: { // 6100d1d8 bsr.w $1ece4
            next = 0x21b0eU;
            const auto result = m.call(c, 365U, 0x21b0aU, 0x1ece4U, 0x21b0eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b0eU: { // 4e75 rts 
            next = 0x21b10U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21b0eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21b12U: { // 6100a018 bsr.w $1bb2c
            next = 0x21b16U;
            const auto result = m.call(c, 564U, 0x21b12U, 0x1bb2cU, 0x21b16U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b16U: { // 6100c5d6 bsr.w $1e0ee
            next = 0x21b1aU;
            const auto result = m.call(c, 356U, 0x21b16U, 0x1e0eeU, 0x21b1aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b1aU: { // 6100d4c0 bsr.w $1efdc
            next = 0x21b1eU;
            const auto result = m.call(c, 368U, 0x21b1aU, 0x1efdcU, 0x21b1eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b1eU: { // 6100c0a4 bsr.w $1dbc4
            next = 0x21b22U;
            const auto result = m.call(c, 352U, 0x21b1eU, 0x1dbc4U, 0x21b22U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b22U: { // 6100c0cc bsr.w $1dbf0
            next = 0x21b26U;
            const auto result = m.call(c, 353U, 0x21b22U, 0x1dbf0U, 0x21b26U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b26U: { // 6100c308 bsr.w $1de30
            next = 0x21b2aU;
            const auto result = m.call(c, 355U, 0x21b26U, 0x1de30U, 0x21b2aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b2aU: { // 6100c5f8 bsr.w $1e124
            next = 0x21b2eU;
            const auto result = m.call(c, 357U, 0x21b2aU, 0x1e124U, 0x21b2eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b2eU: { // 6100cdaa bsr.w $1e8da
            next = 0x21b32U;
            const auto result = m.call(c, 676U, 0x21b2eU, 0x1e8daU, 0x21b32U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b32U: { // 4e75 rts 
            next = 0x21b34U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21b32U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21ae0U:
        case 0x21ae4U:
        case 0x21ae6U:
        case 0x21aeaU:
        case 0x21aeeU:
        case 0x21af4U:
        case 0x21af6U:
        case 0x21afaU:
        case 0x21afeU:
        case 0x21b02U:
        case 0x21b06U:
        case 0x21b0aU:
        case 0x21b0eU:
        case 0x21b12U:
        case 0x21b16U:
        case 0x21b1aU:
        case 0x21b1eU:
        case 0x21b22U:
        case 0x21b26U:
        case 0x21b2aU:
        case 0x21b2eU:
        case 0x21b32U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021b34(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21b34U: { // 6100a28a bsr.w $1bdc0
            next = 0x21b38U;
            const auto result = m.call(c, 327U, 0x21b34U, 0x1bdc0U, 0x21b38U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b38U: { // 6100d710 bsr.w $1f24a
            next = 0x21b3cU;
            const auto result = m.call(c, 372U, 0x21b38U, 0x1f24aU, 0x21b3cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b3cU: { // 6100ce6e bsr.w $1e9ac
            next = 0x21b40U;
            const auto result = m.call(c, 678U, 0x21b3cU, 0x1e9acU, 0x21b40U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b40U: { // 4e75 rts 
            next = 0x21b42U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21b40U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21b34U:
        case 0x21b38U:
        case 0x21b3cU:
        case 0x21b40U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021b42(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21b42U: { // 6100a27c bsr.w $1bdc0
            next = 0x21b46U;
            const auto result = m.call(c, 327U, 0x21b42U, 0x1bdc0U, 0x21b46U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b46U: { // 6100d860 bsr.w $1f3a8
            next = 0x21b4aU;
            const auto result = m.call(c, 373U, 0x21b46U, 0x1f3a8U, 0x21b4aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b4aU: { // 6100ce60 bsr.w $1e9ac
            next = 0x21b4eU;
            const auto result = m.call(c, 678U, 0x21b4aU, 0x1e9acU, 0x21b4eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b4eU: { // 4e75 rts 
            next = 0x21b50U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21b4eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21b42U:
        case 0x21b46U:
        case 0x21b4aU:
        case 0x21b4eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
