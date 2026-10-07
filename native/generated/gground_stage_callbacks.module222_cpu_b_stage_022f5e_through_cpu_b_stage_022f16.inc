
FunctionResult cpu_b_stage_022f5e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22f5eU: { // 61000882 bsr.w $237e2
            next = 0x22f62U;
            const auto result = m.call(c, 412U, 0x22f5eU, 0x237e2U, 0x22f62U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22f62U: { // 103c000e move.b #$e, d0
            next = 0x22f66U;
            const auto value = 0xeU;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22f66U: { // 902d0036 sub.b $36(a5), d0
            next = 0x22f6aU;
            const auto source_address = r.address[5] + 0x36U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x22f6aU: { // 6a02 bpl.b $22f6e
            next = 0x22f6cU;
            if ((r.status & 8U) == 0U) { next = 0x22f6eU; transfer_kind = 1U; }
            break;
        }
        case 0x22f6cU: { // 7000 moveq #$0, d0
            next = 0x22f6eU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22f6eU: { // 1b400037 move.b d0, $37(a5)
            next = 0x22f72U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x37U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22f72U: { // 45fa1416 lea.l $2438a(pc), a2
            next = 0x22f76U;
            const auto source_address = 0x2438aU;
            r.address[2] = source_address;
            break;
        }
        case 0x22f76U: { // 4e71 nop 
            next = 0x22f78U;
            break;
        }
        case 0x22f78U: { // 610009b0 bsr.w $2392a
            next = 0x22f7cU;
            const auto result = m.call(c, 419U, 0x22f78U, 0x2392aU, 0x22f7cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22f7cU: { // 4e75 rts 
            next = 0x22f7eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22f7cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22f5eU:
        case 0x22f62U:
        case 0x22f66U:
        case 0x22f6aU:
        case 0x22f6cU:
        case 0x22f6eU:
        case 0x22f72U:
        case 0x22f76U:
        case 0x22f78U:
        case 0x22f7cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022ef2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22ef2U: { // 610009d4 bsr.w $238c8
            next = 0x22ef6U;
            const auto result = m.call(c, 416U, 0x22ef2U, 0x238c8U, 0x22ef6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22ef6U: { // 2b7c00022f5e0002 move.l #$22f5e, $2(a5)
            next = 0x22efeU;
            const auto value = 0x22f5eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22efeU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22f02U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22f02U: { // 41f900024354 lea.l $24354.l, a0
            next = 0x22f08U;
            const auto source_address = 0x24354U;
            r.address[0] = source_address;
            break;
        }
        case 0x22f08U: { // 7007 moveq #$7, d0
            next = 0x22f0aU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22f0aU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22f0cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22f0cU: { // 51c8fffc dbra d0, $22f0a
            next = 0x22f10U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22f0aU;
            break;
        }
        case 0x22f10U: { // 6100091a bsr.w $2382c
            next = 0x22f14U;
            const auto result = m.call(c, 413U, 0x22f10U, 0x2382cU, 0x22f14U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22f14U: { // 4e75 rts 
            next = 0x22f16U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22f14U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22ef2U:
        case 0x22ef6U:
        case 0x22efeU:
        case 0x22f02U:
        case 0x22f08U:
        case 0x22f0aU:
        case 0x22f0cU:
        case 0x22f10U:
        case 0x22f14U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022f16(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22f16U: { // 610009b0 bsr.w $238c8
            next = 0x22f1aU;
            const auto result = m.call(c, 416U, 0x22f16U, 0x238c8U, 0x22f1aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22f1aU: { // 2b7c00022f5e0002 move.l #$22f5e, $2(a5)
            next = 0x22f22U;
            const auto value = 0x22f5eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22f22U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22f26U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22f26U: { // 41f900024366 lea.l $24366.l, a0
            next = 0x22f2cU;
            const auto source_address = 0x24366U;
            r.address[0] = source_address;
            break;
        }
        case 0x22f2cU: { // 7007 moveq #$7, d0
            next = 0x22f2eU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22f2eU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22f30U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22f30U: { // 51c8fffc dbra d0, $22f2e
            next = 0x22f34U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22f2eU;
            break;
        }
        case 0x22f34U: { // 610008f6 bsr.w $2382c
            next = 0x22f38U;
            const auto result = m.call(c, 413U, 0x22f34U, 0x2382cU, 0x22f38U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22f38U: { // 4e75 rts 
            next = 0x22f3aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22f38U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22f16U:
        case 0x22f1aU:
        case 0x22f22U:
        case 0x22f26U:
        case 0x22f2cU:
        case 0x22f2eU:
        case 0x22f30U:
        case 0x22f34U:
        case 0x22f38U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
