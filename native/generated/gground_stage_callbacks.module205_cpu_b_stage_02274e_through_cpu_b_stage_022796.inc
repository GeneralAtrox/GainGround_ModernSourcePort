
FunctionResult cpu_b_stage_02274e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2274eU: { // 61001178 bsr.w $238c8
            next = 0x22752U;
            const auto result = m.call(c, 416U, 0x2274eU, 0x238c8U, 0x22752U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22752U: { // 4dfa0066 lea.l $227ba(pc), a6
            next = 0x22756U;
            const auto source_address = 0x227baU;
            r.address[6] = source_address;
            break;
        }
        case 0x22756U: { // 2b4e0002 move.l a6, $2(a5)
            next = 0x2275aU;
            const auto value = r.address[6];
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2275aU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2275eU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2275eU: { // 41f900023e7c lea.l $23e7c.l, a0
            next = 0x22764U;
            const auto source_address = 0x23e7cU;
            r.address[0] = source_address;
            break;
        }
        case 0x22764U: { // 7007 moveq #$7, d0
            next = 0x22766U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22766U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22768U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22768U: { // 51c8fffc dbra d0, $22766
            next = 0x2276cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22766U;
            break;
        }
        case 0x2276cU: { // 610010be bsr.w $2382c
            next = 0x22770U;
            const auto result = m.call(c, 413U, 0x2276cU, 0x2382cU, 0x22770U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22770U: { // 4e75 rts 
            next = 0x22772U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22770U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2274eU:
        case 0x22752U:
        case 0x22756U:
        case 0x2275aU:
        case 0x2275eU:
        case 0x22764U:
        case 0x22766U:
        case 0x22768U:
        case 0x2276cU:
        case 0x22770U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022772(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22772U: { // 61001154 bsr.w $238c8
            next = 0x22776U;
            const auto result = m.call(c, 416U, 0x22772U, 0x238c8U, 0x22776U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22776U: { // 4dfa0042 lea.l $227ba(pc), a6
            next = 0x2277aU;
            const auto source_address = 0x227baU;
            r.address[6] = source_address;
            break;
        }
        case 0x2277aU: { // 2b4e0002 move.l a6, $2(a5)
            next = 0x2277eU;
            const auto value = r.address[6];
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2277eU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22782U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22782U: { // 41f900023e8c lea.l $23e8c.l, a0
            next = 0x22788U;
            const auto source_address = 0x23e8cU;
            r.address[0] = source_address;
            break;
        }
        case 0x22788U: { // 7007 moveq #$7, d0
            next = 0x2278aU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2278aU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2278cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2278cU: { // 51c8fffc dbra d0, $2278a
            next = 0x22790U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2278aU;
            break;
        }
        case 0x22790U: { // 6100109a bsr.w $2382c
            next = 0x22794U;
            const auto result = m.call(c, 413U, 0x22790U, 0x2382cU, 0x22794U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22794U: { // 4e75 rts 
            next = 0x22796U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22794U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22772U:
        case 0x22776U:
        case 0x2277aU:
        case 0x2277eU:
        case 0x22782U:
        case 0x22788U:
        case 0x2278aU:
        case 0x2278cU:
        case 0x22790U:
        case 0x22794U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022796(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22796U: { // 61001130 bsr.w $238c8
            next = 0x2279aU;
            const auto result = m.call(c, 416U, 0x22796U, 0x238c8U, 0x2279aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2279aU: { // 4dfa001e lea.l $227ba(pc), a6
            next = 0x2279eU;
            const auto source_address = 0x227baU;
            r.address[6] = source_address;
            break;
        }
        case 0x2279eU: { // 2b4e0002 move.l a6, $2(a5)
            next = 0x227a2U;
            const auto value = r.address[6];
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x227a2U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x227a6U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x227a6U: { // 41f900023e9c lea.l $23e9c.l, a0
            next = 0x227acU;
            const auto source_address = 0x23e9cU;
            r.address[0] = source_address;
            break;
        }
        case 0x227acU: { // 7007 moveq #$7, d0
            next = 0x227aeU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x227aeU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x227b0U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x227b0U: { // 51c8fffc dbra d0, $227ae
            next = 0x227b4U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x227aeU;
            break;
        }
        case 0x227b4U: { // 61001076 bsr.w $2382c
            next = 0x227b8U;
            const auto result = m.call(c, 413U, 0x227b4U, 0x2382cU, 0x227b8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x227b8U: { // 4e75 rts 
            next = 0x227baU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x227b8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22796U:
        case 0x2279aU:
        case 0x2279eU:
        case 0x227a2U:
        case 0x227a6U:
        case 0x227acU:
        case 0x227aeU:
        case 0x227b0U:
        case 0x227b4U:
        case 0x227b8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
