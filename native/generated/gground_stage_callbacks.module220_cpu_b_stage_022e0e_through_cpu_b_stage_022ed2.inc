
FunctionResult cpu_b_stage_022e0e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22e0eU: { // 61000ab8 bsr.w $238c8
            next = 0x22e12U;
            const auto result = m.call(c, 416U, 0x22e0eU, 0x238c8U, 0x22e12U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22e12U: { // 2b7c00022e560002 move.l #$22e56, $2(a5)
            next = 0x22e1aU;
            const auto value = 0x22e56U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22e1aU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22e1eU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22e1eU: { // 41f90002429e lea.l $2429e.l, a0
            next = 0x22e24U;
            const auto source_address = 0x2429eU;
            r.address[0] = source_address;
            break;
        }
        case 0x22e24U: { // 7007 moveq #$7, d0
            next = 0x22e26U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22e26U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22e28U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22e28U: { // 51c8fffc dbra d0, $22e26
            next = 0x22e2cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22e26U;
            break;
        }
        case 0x22e2cU: { // 610009fe bsr.w $2382c
            next = 0x22e30U;
            const auto result = m.call(c, 413U, 0x22e2cU, 0x2382cU, 0x22e30U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22e30U: { // 4e75 rts 
            next = 0x22e32U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22e30U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22e0eU:
        case 0x22e12U:
        case 0x22e1aU:
        case 0x22e1eU:
        case 0x22e24U:
        case 0x22e26U:
        case 0x22e28U:
        case 0x22e2cU:
        case 0x22e30U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022e32(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22e32U: { // 61000a94 bsr.w $238c8
            next = 0x22e36U;
            const auto result = m.call(c, 416U, 0x22e32U, 0x238c8U, 0x22e36U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22e36U: { // 2b7c00022e560002 move.l #$22e56, $2(a5)
            next = 0x22e3eU;
            const auto value = 0x22e56U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22e3eU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22e42U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22e42U: { // 41f9000242ae lea.l $242ae.l, a0
            next = 0x22e48U;
            const auto source_address = 0x242aeU;
            r.address[0] = source_address;
            break;
        }
        case 0x22e48U: { // 7007 moveq #$7, d0
            next = 0x22e4aU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22e4aU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22e4cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22e4cU: { // 51c8fffc dbra d0, $22e4a
            next = 0x22e50U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22e4aU;
            break;
        }
        case 0x22e50U: { // 610009da bsr.w $2382c
            next = 0x22e54U;
            const auto result = m.call(c, 413U, 0x22e50U, 0x2382cU, 0x22e54U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22e54U: { // 4e75 rts 
            next = 0x22e56U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22e54U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22e32U:
        case 0x22e36U:
        case 0x22e3eU:
        case 0x22e42U:
        case 0x22e48U:
        case 0x22e4aU:
        case 0x22e4cU:
        case 0x22e50U:
        case 0x22e54U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022ed2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22ed2U: { // 6100090e bsr.w $237e2
            next = 0x22ed6U;
            const auto result = m.call(c, 412U, 0x22ed2U, 0x237e2U, 0x22ed6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22ed6U: { // 103c000e move.b #$e, d0
            next = 0x22edaU;
            const auto value = 0xeU;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22edaU: { // 902d0036 sub.b $36(a5), d0
            next = 0x22edeU;
            const auto source_address = r.address[5] + 0x36U;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x22edeU: { // 6a02 bpl.b $22ee2
            next = 0x22ee0U;
            if ((r.status & 8U) == 0U) { next = 0x22ee2U; transfer_kind = 1U; }
            break;
        }
        case 0x22ee0U: { // 7000 moveq #$0, d0
            next = 0x22ee2U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22ee2U: { // 1b400037 move.b d0, $37(a5)
            next = 0x22ee6U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x37U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22ee6U: { // 45f900024314 lea.l $24314.l, a2
            next = 0x22eecU;
            const auto source_address = 0x24314U;
            r.address[2] = source_address;
            break;
        }
        case 0x22eecU: { // 61000a3c bsr.w $2392a
            next = 0x22ef0U;
            const auto result = m.call(c, 419U, 0x22eecU, 0x2392aU, 0x22ef0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22ef0U: { // 4e75 rts 
            next = 0x22ef2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22ef0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22ed2U:
        case 0x22ed6U:
        case 0x22edaU:
        case 0x22edeU:
        case 0x22ee0U:
        case 0x22ee2U:
        case 0x22ee6U:
        case 0x22eecU:
        case 0x22ef0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
