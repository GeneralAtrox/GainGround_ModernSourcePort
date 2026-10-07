
FunctionResult cpu_b_stage_023112(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23112U: { // 610007b4 bsr.w $238c8
            next = 0x23116U;
            const auto result = m.call(c, 416U, 0x23112U, 0x238c8U, 0x23116U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23116U: { // 2b7c0002317e0002 move.l #$2317e, $2(a5)
            next = 0x2311eU;
            const auto value = 0x2317eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2311eU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x23122U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x23122U: { // 41f900024546 lea.l $24546.l, a0
            next = 0x23128U;
            const auto source_address = 0x24546U;
            r.address[0] = source_address;
            break;
        }
        case 0x23128U: { // 7007 moveq #$7, d0
            next = 0x2312aU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2312aU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2312cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2312cU: { // 51c8fffc dbra d0, $2312a
            next = 0x23130U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2312aU;
            break;
        }
        case 0x23130U: { // 610006fa bsr.w $2382c
            next = 0x23134U;
            const auto result = m.call(c, 413U, 0x23130U, 0x2382cU, 0x23134U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23134U: { // 4e75 rts 
            next = 0x23136U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23134U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23112U:
        case 0x23116U:
        case 0x2311eU:
        case 0x23122U:
        case 0x23128U:
        case 0x2312aU:
        case 0x2312cU:
        case 0x23130U:
        case 0x23134U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_023136(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23136U: { // 61000790 bsr.w $238c8
            next = 0x2313aU;
            const auto result = m.call(c, 416U, 0x23136U, 0x238c8U, 0x2313aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2313aU: { // 2b7c0002317e0002 move.l #$2317e, $2(a5)
            next = 0x23142U;
            const auto value = 0x2317eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x23142U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x23146U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x23146U: { // 41f900024558 lea.l $24558.l, a0
            next = 0x2314cU;
            const auto source_address = 0x24558U;
            r.address[0] = source_address;
            break;
        }
        case 0x2314cU: { // 7007 moveq #$7, d0
            next = 0x2314eU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2314eU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x23150U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x23150U: { // 51c8fffc dbra d0, $2314e
            next = 0x23154U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2314eU;
            break;
        }
        case 0x23154U: { // 610006d6 bsr.w $2382c
            next = 0x23158U;
            const auto result = m.call(c, 413U, 0x23154U, 0x2382cU, 0x23158U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23158U: { // 4e75 rts 
            next = 0x2315aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23158U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23136U:
        case 0x2313aU:
        case 0x23142U:
        case 0x23146U:
        case 0x2314cU:
        case 0x2314eU:
        case 0x23150U:
        case 0x23154U:
        case 0x23158U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02315a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2315aU: { // 6100076c bsr.w $238c8
            next = 0x2315eU;
            const auto result = m.call(c, 416U, 0x2315aU, 0x238c8U, 0x2315eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2315eU: { // 2b7c0002317e0002 move.l #$2317e, $2(a5)
            next = 0x23166U;
            const auto value = 0x2317eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x23166U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2316aU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2316aU: { // 41f90002456a lea.l $2456a.l, a0
            next = 0x23170U;
            const auto source_address = 0x2456aU;
            r.address[0] = source_address;
            break;
        }
        case 0x23170U: { // 7007 moveq #$7, d0
            next = 0x23172U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x23172U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x23174U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x23174U: { // 51c8fffc dbra d0, $23172
            next = 0x23178U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x23172U;
            break;
        }
        case 0x23178U: { // 610006b2 bsr.w $2382c
            next = 0x2317cU;
            const auto result = m.call(c, 413U, 0x23178U, 0x2382cU, 0x2317cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2317cU: { // 4e75 rts 
            next = 0x2317eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2317cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2315aU:
        case 0x2315eU:
        case 0x23166U:
        case 0x2316aU:
        case 0x23170U:
        case 0x23172U:
        case 0x23174U:
        case 0x23178U:
        case 0x2317cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
