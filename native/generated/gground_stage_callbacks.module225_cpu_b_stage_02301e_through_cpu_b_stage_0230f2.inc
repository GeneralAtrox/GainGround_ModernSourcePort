
FunctionResult cpu_b_stage_02301e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2301eU: { // 610008a8 bsr.w $238c8
            next = 0x23022U;
            const auto result = m.call(c, 416U, 0x2301eU, 0x238c8U, 0x23022U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23022U: { // 2b7c000230660002 move.l #$23066, $2(a5)
            next = 0x2302aU;
            const auto value = 0x23066U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2302aU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2302eU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2302eU: { // 41f900024460 lea.l $24460.l, a0
            next = 0x23034U;
            const auto source_address = 0x24460U;
            r.address[0] = source_address;
            break;
        }
        case 0x23034U: { // 7007 moveq #$7, d0
            next = 0x23036U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x23036U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x23038U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x23038U: { // 51c8fffc dbra d0, $23036
            next = 0x2303cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x23036U;
            break;
        }
        case 0x2303cU: { // 610007ee bsr.w $2382c
            next = 0x23040U;
            const auto result = m.call(c, 413U, 0x2303cU, 0x2382cU, 0x23040U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23040U: { // 4e75 rts 
            next = 0x23042U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23040U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2301eU:
        case 0x23022U:
        case 0x2302aU:
        case 0x2302eU:
        case 0x23034U:
        case 0x23036U:
        case 0x23038U:
        case 0x2303cU:
        case 0x23040U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_023042(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23042U: { // 61000884 bsr.w $238c8
            next = 0x23046U;
            const auto result = m.call(c, 416U, 0x23042U, 0x238c8U, 0x23046U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23046U: { // 2b7c000230660002 move.l #$23066, $2(a5)
            next = 0x2304eU;
            const auto value = 0x23066U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2304eU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x23052U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x23052U: { // 41f900024470 lea.l $24470.l, a0
            next = 0x23058U;
            const auto source_address = 0x24470U;
            r.address[0] = source_address;
            break;
        }
        case 0x23058U: { // 7007 moveq #$7, d0
            next = 0x2305aU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2305aU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2305cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2305cU: { // 51c8fffc dbra d0, $2305a
            next = 0x23060U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2305aU;
            break;
        }
        case 0x23060U: { // 610007ca bsr.w $2382c
            next = 0x23064U;
            const auto result = m.call(c, 413U, 0x23060U, 0x2382cU, 0x23064U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23064U: { // 4e75 rts 
            next = 0x23066U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23064U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23042U:
        case 0x23046U:
        case 0x2304eU:
        case 0x23052U:
        case 0x23058U:
        case 0x2305aU:
        case 0x2305cU:
        case 0x23060U:
        case 0x23064U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0230f2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x230f2U: { // 610006ee bsr.w $237e2
            next = 0x230f6U;
            const auto result = m.call(c, 412U, 0x230f2U, 0x237e2U, 0x230f6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x230f6U: { // 103c000e move.b #$e, d0
            next = 0x230faU;
            const auto value = 0xeU;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x230faU: { // 902d0036 sub.b $36(a5), d0
            next = 0x230feU;
            const auto source_address = r.address[5] + 0x36U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x230feU: { // 6a02 bpl.b $23102
            next = 0x23100U;
            if ((r.status & 8U) == 0U) { next = 0x23102U; transfer_kind = 1U; }
            break;
        }
        case 0x23100U: { // 7000 moveq #$0, d0
            next = 0x23102U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x23102U: { // 1b400037 move.b d0, $37(a5)
            next = 0x23106U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x37U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x23106U: { // 45fa13ee lea.l $244f6(pc), a2
            next = 0x2310aU;
            const auto source_address = 0x244f6U;
            r.address[2] = source_address;
            break;
        }
        case 0x2310aU: { // 4e71 nop 
            next = 0x2310cU;
            break;
        }
        case 0x2310cU: { // 6100081c bsr.w $2392a
            next = 0x23110U;
            const auto result = m.call(c, 419U, 0x2310cU, 0x2392aU, 0x23110U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23110U: { // 4e75 rts 
            next = 0x23112U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23110U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x230f2U:
        case 0x230f6U:
        case 0x230faU:
        case 0x230feU:
        case 0x23100U:
        case 0x23102U:
        case 0x23106U:
        case 0x2310aU:
        case 0x2310cU:
        case 0x23110U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
