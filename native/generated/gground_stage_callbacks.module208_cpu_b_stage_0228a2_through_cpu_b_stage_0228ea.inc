
FunctionResult cpu_b_stage_0228a2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x228a2U: { // 61001024 bsr.w $238c8
            next = 0x228a6U;
            const auto result = m.call(c, 416U, 0x228a2U, 0x238c8U, 0x228a6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x228a6U: { // 4dfa0066 lea.l $2290e(pc), a6
            next = 0x228aaU;
            const auto source_address = 0x2290eU;
            r.address[6] = source_address;
            break;
        }
        case 0x228aaU: { // 2b4e0002 move.l a6, $2(a5)
            next = 0x228aeU;
            const auto value = r.address[6];
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x228aeU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x228b2U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x228b2U: { // 41f900023f6e lea.l $23f6e.l, a0
            next = 0x228b8U;
            const auto source_address = 0x23f6eU;
            r.address[0] = source_address;
            break;
        }
        case 0x228b8U: { // 7007 moveq #$7, d0
            next = 0x228baU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x228baU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x228bcU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x228bcU: { // 51c8fffc dbra d0, $228ba
            next = 0x228c0U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x228baU;
            break;
        }
        case 0x228c0U: { // 61000f6a bsr.w $2382c
            next = 0x228c4U;
            const auto result = m.call(c, 413U, 0x228c0U, 0x2382cU, 0x228c4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x228c4U: { // 4e75 rts 
            next = 0x228c6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x228c4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x228a2U:
        case 0x228a6U:
        case 0x228aaU:
        case 0x228aeU:
        case 0x228b2U:
        case 0x228b8U:
        case 0x228baU:
        case 0x228bcU:
        case 0x228c0U:
        case 0x228c4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0228c6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x228c6U: { // 61001000 bsr.w $238c8
            next = 0x228caU;
            const auto result = m.call(c, 416U, 0x228c6U, 0x238c8U, 0x228caU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x228caU: { // 4dfa0042 lea.l $2290e(pc), a6
            next = 0x228ceU;
            const auto source_address = 0x2290eU;
            r.address[6] = source_address;
            break;
        }
        case 0x228ceU: { // 2b4e0002 move.l a6, $2(a5)
            next = 0x228d2U;
            const auto value = r.address[6];
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x228d2U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x228d6U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x228d6U: { // 41f900023f7e lea.l $23f7e.l, a0
            next = 0x228dcU;
            const auto source_address = 0x23f7eU;
            r.address[0] = source_address;
            break;
        }
        case 0x228dcU: { // 7007 moveq #$7, d0
            next = 0x228deU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x228deU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x228e0U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x228e0U: { // 51c8fffc dbra d0, $228de
            next = 0x228e4U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x228deU;
            break;
        }
        case 0x228e4U: { // 61000f46 bsr.w $2382c
            next = 0x228e8U;
            const auto result = m.call(c, 413U, 0x228e4U, 0x2382cU, 0x228e8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x228e8U: { // 4e75 rts 
            next = 0x228eaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x228e8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x228c6U:
        case 0x228caU:
        case 0x228ceU:
        case 0x228d2U:
        case 0x228d6U:
        case 0x228dcU:
        case 0x228deU:
        case 0x228e0U:
        case 0x228e4U:
        case 0x228e8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0228ea(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x228eaU: { // 61000fdc bsr.w $238c8
            next = 0x228eeU;
            const auto result = m.call(c, 416U, 0x228eaU, 0x238c8U, 0x228eeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x228eeU: { // 4dfa001e lea.l $2290e(pc), a6
            next = 0x228f2U;
            const auto source_address = 0x2290eU;
            r.address[6] = source_address;
            break;
        }
        case 0x228f2U: { // 2b4e0002 move.l a6, $2(a5)
            next = 0x228f6U;
            const auto value = r.address[6];
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x228f6U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x228faU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x228faU: { // 41f900023f8e lea.l $23f8e.l, a0
            next = 0x22900U;
            const auto source_address = 0x23f8eU;
            r.address[0] = source_address;
            break;
        }
        case 0x22900U: { // 7007 moveq #$7, d0
            next = 0x22902U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22902U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22904U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22904U: { // 51c8fffc dbra d0, $22902
            next = 0x22908U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22902U;
            break;
        }
        case 0x22908U: { // 61000f22 bsr.w $2382c
            next = 0x2290cU;
            const auto result = m.call(c, 413U, 0x22908U, 0x2382cU, 0x2290cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2290cU: { // 4e75 rts 
            next = 0x2290eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2290cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x228eaU:
        case 0x228eeU:
        case 0x228f2U:
        case 0x228f6U:
        case 0x228faU:
        case 0x22900U:
        case 0x22902U:
        case 0x22904U:
        case 0x22908U:
        case 0x2290cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
