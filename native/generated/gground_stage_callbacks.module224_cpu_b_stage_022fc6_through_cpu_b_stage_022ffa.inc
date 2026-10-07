
FunctionResult cpu_b_stage_022fc6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22fc6U: { // 61000900 bsr.w $238c8
            next = 0x22fcaU;
            const auto result = m.call(c, 416U, 0x22fc6U, 0x238c8U, 0x22fcaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22fcaU: { // 2b7c00022fea0002 move.l #$22fea, $2(a5)
            next = 0x22fd2U;
            const auto value = 0x22feaU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22fd2U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22fd6U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22fd6U: { // 41f9000243ee lea.l $243ee.l, a0
            next = 0x22fdcU;
            const auto source_address = 0x243eeU;
            r.address[0] = source_address;
            break;
        }
        case 0x22fdcU: { // 7007 moveq #$7, d0
            next = 0x22fdeU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22fdeU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22fe0U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22fe0U: { // 51c8fffc dbra d0, $22fde
            next = 0x22fe4U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22fdeU;
            break;
        }
        case 0x22fe4U: { // 61000846 bsr.w $2382c
            next = 0x22fe8U;
            const auto result = m.call(c, 413U, 0x22fe4U, 0x2382cU, 0x22fe8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22fe8U: { // 4e75 rts 
            next = 0x22feaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22fe8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22fc6U:
        case 0x22fcaU:
        case 0x22fd2U:
        case 0x22fd6U:
        case 0x22fdcU:
        case 0x22fdeU:
        case 0x22fe0U:
        case 0x22fe4U:
        case 0x22fe8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_023066(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23066U: { // 6100077a bsr.w $237e2
            next = 0x2306aU;
            const auto result = m.call(c, 412U, 0x23066U, 0x237e2U, 0x2306aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2306aU: { // 103c000e move.b #$e, d0
            next = 0x2306eU;
            const auto value = 0xeU;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x2306eU: { // 902d0036 sub.b $36(a5), d0
            next = 0x23072U;
            const auto source_address = r.address[5] + 0x36U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x23072U: { // 6a02 bpl.b $23076
            next = 0x23074U;
            if ((r.status & 8U) == 0U) { next = 0x23076U; transfer_kind = 1U; }
            break;
        }
        case 0x23074U: { // 7000 moveq #$0, d0
            next = 0x23076U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x23076U: { // 1b400037 move.b d0, $37(a5)
            next = 0x2307aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x37U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x2307aU: { // 45f900024480 lea.l $24480.l, a2
            next = 0x23080U;
            const auto source_address = 0x24480U;
            r.address[2] = source_address;
            break;
        }
        case 0x23080U: { // 610008a8 bsr.w $2392a
            next = 0x23084U;
            const auto result = m.call(c, 419U, 0x23080U, 0x2392aU, 0x23084U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23084U: { // 4e75 rts 
            next = 0x23086U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23084U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23066U:
        case 0x2306aU:
        case 0x2306eU:
        case 0x23072U:
        case 0x23074U:
        case 0x23076U:
        case 0x2307aU:
        case 0x23080U:
        case 0x23084U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022ffa(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22ffaU: { // 610008cc bsr.w $238c8
            next = 0x22ffeU;
            const auto result = m.call(c, 416U, 0x22ffaU, 0x238c8U, 0x22ffeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22ffeU: { // 2b7c000230660002 move.l #$23066, $2(a5)
            next = 0x23006U;
            const auto value = 0x23066U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x23006U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2300aU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2300aU: { // 41f900024450 lea.l $24450.l, a0
            next = 0x23010U;
            const auto source_address = 0x24450U;
            r.address[0] = source_address;
            break;
        }
        case 0x23010U: { // 7007 moveq #$7, d0
            next = 0x23012U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x23012U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x23014U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x23014U: { // 51c8fffc dbra d0, $23012
            next = 0x23018U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x23012U;
            break;
        }
        case 0x23018U: { // 61000812 bsr.w $2382c
            next = 0x2301cU;
            const auto result = m.call(c, 413U, 0x23018U, 0x2382cU, 0x2301cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2301cU: { // 4e75 rts 
            next = 0x2301eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2301cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22ffaU:
        case 0x22ffeU:
        case 0x23006U:
        case 0x2300aU:
        case 0x23010U:
        case 0x23012U:
        case 0x23014U:
        case 0x23018U:
        case 0x2301cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
