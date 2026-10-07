
FunctionResult cpu_b_stage_02263c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2263cU: { // 6100128a bsr.w $238c8
            next = 0x22640U;
            const auto result = m.call(c, 416U, 0x2263cU, 0x238c8U, 0x22640U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22640U: { // 2b7c000226a80002 move.l #$226a8, $2(a5)
            next = 0x22648U;
            const auto value = 0x226a8U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22648U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2264cU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2264cU: { // 41f900023dc4 lea.l $23dc4.l, a0
            next = 0x22652U;
            const auto source_address = 0x23dc4U;
            r.address[0] = source_address;
            break;
        }
        case 0x22652U: { // 7007 moveq #$7, d0
            next = 0x22654U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22654U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22656U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22656U: { // 51c8fffc dbra d0, $22654
            next = 0x2265aU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22654U;
            break;
        }
        case 0x2265aU: { // 610011d0 bsr.w $2382c
            next = 0x2265eU;
            const auto result = m.call(c, 413U, 0x2265aU, 0x2382cU, 0x2265eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2265eU: { // 4e75 rts 
            next = 0x22660U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2265eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2263cU:
        case 0x22640U:
        case 0x22648U:
        case 0x2264cU:
        case 0x22652U:
        case 0x22654U:
        case 0x22656U:
        case 0x2265aU:
        case 0x2265eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022660(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22660U: { // 61001266 bsr.w $238c8
            next = 0x22664U;
            const auto result = m.call(c, 416U, 0x22660U, 0x238c8U, 0x22664U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22664U: { // 2b7c000226a80002 move.l #$226a8, $2(a5)
            next = 0x2266cU;
            const auto value = 0x226a8U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2266cU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22670U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22670U: { // 41f900023dd4 lea.l $23dd4.l, a0
            next = 0x22676U;
            const auto source_address = 0x23dd4U;
            r.address[0] = source_address;
            break;
        }
        case 0x22676U: { // 7007 moveq #$7, d0
            next = 0x22678U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22678U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2267aU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2267aU: { // 51c8fffc dbra d0, $22678
            next = 0x2267eU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22678U;
            break;
        }
        case 0x2267eU: { // 610011ac bsr.w $2382c
            next = 0x22682U;
            const auto result = m.call(c, 413U, 0x2267eU, 0x2382cU, 0x22682U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22682U: { // 4e75 rts 
            next = 0x22684U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22682U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22660U:
        case 0x22664U:
        case 0x2266cU:
        case 0x22670U:
        case 0x22676U:
        case 0x22678U:
        case 0x2267aU:
        case 0x2267eU:
        case 0x22682U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022684(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22684U: { // 61001242 bsr.w $238c8
            next = 0x22688U;
            const auto result = m.call(c, 416U, 0x22684U, 0x238c8U, 0x22688U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22688U: { // 2b7c000226a80002 move.l #$226a8, $2(a5)
            next = 0x22690U;
            const auto value = 0x226a8U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22690U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22694U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22694U: { // 41f900023de4 lea.l $23de4.l, a0
            next = 0x2269aU;
            const auto source_address = 0x23de4U;
            r.address[0] = source_address;
            break;
        }
        case 0x2269aU: { // 7007 moveq #$7, d0
            next = 0x2269cU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2269cU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2269eU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2269eU: { // 51c8fffc dbra d0, $2269c
            next = 0x226a2U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2269cU;
            break;
        }
        case 0x226a2U: { // 61001188 bsr.w $2382c
            next = 0x226a6U;
            const auto result = m.call(c, 413U, 0x226a2U, 0x2382cU, 0x226a6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x226a6U: { // 4e75 rts 
            next = 0x226a8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x226a6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22684U:
        case 0x22688U:
        case 0x22690U:
        case 0x22694U:
        case 0x2269aU:
        case 0x2269cU:
        case 0x2269eU:
        case 0x226a2U:
        case 0x226a6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
