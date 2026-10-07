
FunctionResult cpu_b_stage_022bbe(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22bbeU: { // 61000d08 bsr.w $238c8
            next = 0x22bc2U;
            const auto result = m.call(c, 416U, 0x22bbeU, 0x238c8U, 0x22bc2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22bc2U: { // 2b7c00022c060002 move.l #$22c06, $2(a5)
            next = 0x22bcaU;
            const auto value = 0x22c06U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22bcaU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22bceU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22bceU: { // 41f900024176 lea.l $24176.l, a0
            next = 0x22bd4U;
            const auto source_address = 0x24176U;
            r.address[0] = source_address;
            break;
        }
        case 0x22bd4U: { // 7007 moveq #$7, d0
            next = 0x22bd6U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22bd6U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22bd8U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22bd8U: { // 51c8fffc dbra d0, $22bd6
            next = 0x22bdcU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22bd6U;
            break;
        }
        case 0x22bdcU: { // 61000c4e bsr.w $2382c
            next = 0x22be0U;
            const auto result = m.call(c, 413U, 0x22bdcU, 0x2382cU, 0x22be0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22be0U: { // 4e75 rts 
            next = 0x22be2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22be0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22bbeU:
        case 0x22bc2U:
        case 0x22bcaU:
        case 0x22bceU:
        case 0x22bd4U:
        case 0x22bd6U:
        case 0x22bd8U:
        case 0x22bdcU:
        case 0x22be0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022be2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22be2U: { // 61000ce4 bsr.w $238c8
            next = 0x22be6U;
            const auto result = m.call(c, 416U, 0x22be2U, 0x238c8U, 0x22be6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22be6U: { // 2b7c00022c060002 move.l #$22c06, $2(a5)
            next = 0x22beeU;
            const auto value = 0x22c06U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22beeU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22bf2U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22bf2U: { // 41f900024186 lea.l $24186.l, a0
            next = 0x22bf8U;
            const auto source_address = 0x24186U;
            r.address[0] = source_address;
            break;
        }
        case 0x22bf8U: { // 7007 moveq #$7, d0
            next = 0x22bfaU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22bfaU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22bfcU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22bfcU: { // 51c8fffc dbra d0, $22bfa
            next = 0x22c00U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22bfaU;
            break;
        }
        case 0x22c00U: { // 61000c2a bsr.w $2382c
            next = 0x22c04U;
            const auto result = m.call(c, 413U, 0x22c00U, 0x2382cU, 0x22c04U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22c04U: { // 4e75 rts 
            next = 0x22c06U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22c04U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22be2U:
        case 0x22be6U:
        case 0x22beeU:
        case 0x22bf2U:
        case 0x22bf8U:
        case 0x22bfaU:
        case 0x22bfcU:
        case 0x22c00U:
        case 0x22c04U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
