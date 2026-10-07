
FunctionResult cpu_b_stage_0227f8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x227f8U: { // 610010ce bsr.w $238c8
            next = 0x227fcU;
            const auto result = m.call(c, 416U, 0x227f8U, 0x238c8U, 0x227fcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x227fcU: { // 2b7c000228640002 move.l #$22864, $2(a5)
            next = 0x22804U;
            const auto value = 0x22864U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22804U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22808U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22808U: { // 41f900023f12 lea.l $23f12.l, a0
            next = 0x2280eU;
            const auto source_address = 0x23f12U;
            r.address[0] = source_address;
            break;
        }
        case 0x2280eU: { // 7007 moveq #$7, d0
            next = 0x22810U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22810U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22812U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22812U: { // 51c8fffc dbra d0, $22810
            next = 0x22816U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22810U;
            break;
        }
        case 0x22816U: { // 61001014 bsr.w $2382c
            next = 0x2281aU;
            const auto result = m.call(c, 413U, 0x22816U, 0x2382cU, 0x2281aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2281aU: { // 4e75 rts 
            next = 0x2281cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2281aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x227f8U:
        case 0x227fcU:
        case 0x22804U:
        case 0x22808U:
        case 0x2280eU:
        case 0x22810U:
        case 0x22812U:
        case 0x22816U:
        case 0x2281aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02281c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2281cU: { // 610010aa bsr.w $238c8
            next = 0x22820U;
            const auto result = m.call(c, 416U, 0x2281cU, 0x238c8U, 0x22820U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22820U: { // 2b7c000228640002 move.l #$22864, $2(a5)
            next = 0x22828U;
            const auto value = 0x22864U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22828U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2282cU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2282cU: { // 41f900023f22 lea.l $23f22.l, a0
            next = 0x22832U;
            const auto source_address = 0x23f22U;
            r.address[0] = source_address;
            break;
        }
        case 0x22832U: { // 7007 moveq #$7, d0
            next = 0x22834U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22834U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22836U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22836U: { // 51c8fffc dbra d0, $22834
            next = 0x2283aU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22834U;
            break;
        }
        case 0x2283aU: { // 61000ff0 bsr.w $2382c
            next = 0x2283eU;
            const auto result = m.call(c, 413U, 0x2283aU, 0x2382cU, 0x2283eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2283eU: { // 4e75 rts 
            next = 0x22840U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2283eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2281cU:
        case 0x22820U:
        case 0x22828U:
        case 0x2282cU:
        case 0x22832U:
        case 0x22834U:
        case 0x22836U:
        case 0x2283aU:
        case 0x2283eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022840(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22840U: { // 61001086 bsr.w $238c8
            next = 0x22844U;
            const auto result = m.call(c, 416U, 0x22840U, 0x238c8U, 0x22844U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22844U: { // 2b7c000228640002 move.l #$22864, $2(a5)
            next = 0x2284cU;
            const auto value = 0x22864U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2284cU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22850U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22850U: { // 41f900023f32 lea.l $23f32.l, a0
            next = 0x22856U;
            const auto source_address = 0x23f32U;
            r.address[0] = source_address;
            break;
        }
        case 0x22856U: { // 7007 moveq #$7, d0
            next = 0x22858U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22858U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2285aU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2285aU: { // 51c8fffc dbra d0, $22858
            next = 0x2285eU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22858U;
            break;
        }
        case 0x2285eU: { // 61000fcc bsr.w $2382c
            next = 0x22862U;
            const auto result = m.call(c, 413U, 0x2285eU, 0x2382cU, 0x22862U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22862U: { // 4e75 rts 
            next = 0x22864U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22862U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22840U:
        case 0x22844U:
        case 0x2284cU:
        case 0x22850U:
        case 0x22856U:
        case 0x22858U:
        case 0x2285aU:
        case 0x2285eU:
        case 0x22862U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
