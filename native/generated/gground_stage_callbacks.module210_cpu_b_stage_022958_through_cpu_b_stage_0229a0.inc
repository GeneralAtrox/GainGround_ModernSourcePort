
FunctionResult cpu_b_stage_022958(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22958U: { // 61000f6e bsr.w $238c8
            next = 0x2295cU;
            const auto result = m.call(c, 416U, 0x22958U, 0x238c8U, 0x2295cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2295cU: { // 2b7c000229c40002 move.l #$229c4, $2(a5)
            next = 0x22964U;
            const auto value = 0x229c4U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22964U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22968U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22968U: { // 41f900023fea lea.l $23fea.l, a0
            next = 0x2296eU;
            const auto source_address = 0x23feaU;
            r.address[0] = source_address;
            break;
        }
        case 0x2296eU: { // 7007 moveq #$7, d0
            next = 0x22970U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22970U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22972U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22972U: { // 51c8fffc dbra d0, $22970
            next = 0x22976U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22970U;
            break;
        }
        case 0x22976U: { // 61000eb4 bsr.w $2382c
            next = 0x2297aU;
            const auto result = m.call(c, 413U, 0x22976U, 0x2382cU, 0x2297aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2297aU: { // 4e75 rts 
            next = 0x2297cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2297aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22958U:
        case 0x2295cU:
        case 0x22964U:
        case 0x22968U:
        case 0x2296eU:
        case 0x22970U:
        case 0x22972U:
        case 0x22976U:
        case 0x2297aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02297c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2297cU: { // 61000f4a bsr.w $238c8
            next = 0x22980U;
            const auto result = m.call(c, 416U, 0x2297cU, 0x238c8U, 0x22980U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22980U: { // 2b7c000229c40002 move.l #$229c4, $2(a5)
            next = 0x22988U;
            const auto value = 0x229c4U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22988U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2298cU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2298cU: { // 41f900023ffa lea.l $23ffa.l, a0
            next = 0x22992U;
            const auto source_address = 0x23ffaU;
            r.address[0] = source_address;
            break;
        }
        case 0x22992U: { // 7007 moveq #$7, d0
            next = 0x22994U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22994U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22996U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22996U: { // 51c8fffc dbra d0, $22994
            next = 0x2299aU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22994U;
            break;
        }
        case 0x2299aU: { // 61000e90 bsr.w $2382c
            next = 0x2299eU;
            const auto result = m.call(c, 413U, 0x2299aU, 0x2382cU, 0x2299eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2299eU: { // 4e75 rts 
            next = 0x229a0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2299eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2297cU:
        case 0x22980U:
        case 0x22988U:
        case 0x2298cU:
        case 0x22992U:
        case 0x22994U:
        case 0x22996U:
        case 0x2299aU:
        case 0x2299eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0229a0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x229a0U: { // 61000f26 bsr.w $238c8
            next = 0x229a4U;
            const auto result = m.call(c, 416U, 0x229a0U, 0x238c8U, 0x229a4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x229a4U: { // 2b7c000229c40002 move.l #$229c4, $2(a5)
            next = 0x229acU;
            const auto value = 0x229c4U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x229acU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x229b0U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x229b0U: { // 41f90002400a lea.l $2400a.l, a0
            next = 0x229b6U;
            const auto source_address = 0x2400aU;
            r.address[0] = source_address;
            break;
        }
        case 0x229b6U: { // 7007 moveq #$7, d0
            next = 0x229b8U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x229b8U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x229baU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x229baU: { // 51c8fffc dbra d0, $229b8
            next = 0x229beU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x229b8U;
            break;
        }
        case 0x229beU: { // 61000e6c bsr.w $2382c
            next = 0x229c2U;
            const auto result = m.call(c, 413U, 0x229beU, 0x2382cU, 0x229c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x229c2U: { // 4e75 rts 
            next = 0x229c4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x229c2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x229a0U:
        case 0x229a4U:
        case 0x229acU:
        case 0x229b0U:
        case 0x229b6U:
        case 0x229b8U:
        case 0x229baU:
        case 0x229beU:
        case 0x229c2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
