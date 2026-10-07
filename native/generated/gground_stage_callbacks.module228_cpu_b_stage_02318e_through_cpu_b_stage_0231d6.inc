
FunctionResult cpu_b_stage_02318e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2318eU: { // 61000738 bsr.w $238c8
            next = 0x23192U;
            const auto result = m.call(c, 416U, 0x2318eU, 0x238c8U, 0x23192U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23192U: { // 4dfa0066 lea.l $231fa(pc), a6
            next = 0x23196U;
            const auto source_address = 0x231faU;
            r.address[6] = source_address;
            break;
        }
        case 0x23196U: { // 2b4e0002 move.l a6, $2(a5)
            next = 0x2319aU;
            const auto value = r.address[6];
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2319aU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2319eU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2319eU: { // 41f9000245c4 lea.l $245c4.l, a0
            next = 0x231a4U;
            const auto source_address = 0x245c4U;
            r.address[0] = source_address;
            break;
        }
        case 0x231a4U: { // 7007 moveq #$7, d0
            next = 0x231a6U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x231a6U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x231a8U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x231a8U: { // 51c8fffc dbra d0, $231a6
            next = 0x231acU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x231a6U;
            break;
        }
        case 0x231acU: { // 6100067e bsr.w $2382c
            next = 0x231b0U;
            const auto result = m.call(c, 413U, 0x231acU, 0x2382cU, 0x231b0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x231b0U: { // 4e75 rts 
            next = 0x231b2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x231b0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2318eU:
        case 0x23192U:
        case 0x23196U:
        case 0x2319aU:
        case 0x2319eU:
        case 0x231a4U:
        case 0x231a6U:
        case 0x231a8U:
        case 0x231acU:
        case 0x231b0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0231b2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x231b2U: { // 61000714 bsr.w $238c8
            next = 0x231b6U;
            const auto result = m.call(c, 416U, 0x231b2U, 0x238c8U, 0x231b6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x231b6U: { // 4dfa0042 lea.l $231fa(pc), a6
            next = 0x231baU;
            const auto source_address = 0x231faU;
            r.address[6] = source_address;
            break;
        }
        case 0x231baU: { // 2b4e0002 move.l a6, $2(a5)
            next = 0x231beU;
            const auto value = r.address[6];
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x231beU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x231c2U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x231c2U: { // 41f9000245d6 lea.l $245d6.l, a0
            next = 0x231c8U;
            const auto source_address = 0x245d6U;
            r.address[0] = source_address;
            break;
        }
        case 0x231c8U: { // 7007 moveq #$7, d0
            next = 0x231caU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x231caU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x231ccU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x231ccU: { // 51c8fffc dbra d0, $231ca
            next = 0x231d0U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x231caU;
            break;
        }
        case 0x231d0U: { // 6100065a bsr.w $2382c
            next = 0x231d4U;
            const auto result = m.call(c, 413U, 0x231d0U, 0x2382cU, 0x231d4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x231d4U: { // 4e75 rts 
            next = 0x231d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x231d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x231b2U:
        case 0x231b6U:
        case 0x231baU:
        case 0x231beU:
        case 0x231c2U:
        case 0x231c8U:
        case 0x231caU:
        case 0x231ccU:
        case 0x231d0U:
        case 0x231d4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0231d6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x231d6U: { // 610006f0 bsr.w $238c8
            next = 0x231daU;
            const auto result = m.call(c, 416U, 0x231d6U, 0x238c8U, 0x231daU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x231daU: { // 4dfa001e lea.l $231fa(pc), a6
            next = 0x231deU;
            const auto source_address = 0x231faU;
            r.address[6] = source_address;
            break;
        }
        case 0x231deU: { // 2b4e0002 move.l a6, $2(a5)
            next = 0x231e2U;
            const auto value = r.address[6];
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x231e2U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x231e6U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x231e6U: { // 41f9000245e8 lea.l $245e8.l, a0
            next = 0x231ecU;
            const auto source_address = 0x245e8U;
            r.address[0] = source_address;
            break;
        }
        case 0x231ecU: { // 7007 moveq #$7, d0
            next = 0x231eeU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x231eeU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x231f0U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x231f0U: { // 51c8fffc dbra d0, $231ee
            next = 0x231f4U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x231eeU;
            break;
        }
        case 0x231f4U: { // 61000636 bsr.w $2382c
            next = 0x231f8U;
            const auto result = m.call(c, 413U, 0x231f4U, 0x2382cU, 0x231f8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x231f8U: { // 4e75 rts 
            next = 0x231faU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x231f8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x231d6U:
        case 0x231daU:
        case 0x231deU:
        case 0x231e2U:
        case 0x231e6U:
        case 0x231ecU:
        case 0x231eeU:
        case 0x231f0U:
        case 0x231f4U:
        case 0x231f8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
