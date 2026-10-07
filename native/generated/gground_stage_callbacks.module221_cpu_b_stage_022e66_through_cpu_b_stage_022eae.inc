
FunctionResult cpu_b_stage_022e66(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22e66U: { // 61000a60 bsr.w $238c8
            next = 0x22e6aU;
            const auto result = m.call(c, 416U, 0x22e66U, 0x238c8U, 0x22e6aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22e6aU: { // 2b7c00022ed20002 move.l #$22ed2, $2(a5)
            next = 0x22e72U;
            const auto value = 0x22ed2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22e72U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22e76U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22e76U: { // 41f9000242de lea.l $242de.l, a0
            next = 0x22e7cU;
            const auto source_address = 0x242deU;
            r.address[0] = source_address;
            break;
        }
        case 0x22e7cU: { // 7007 moveq #$7, d0
            next = 0x22e7eU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22e7eU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22e80U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22e80U: { // 51c8fffc dbra d0, $22e7e
            next = 0x22e84U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22e7eU;
            break;
        }
        case 0x22e84U: { // 610009a6 bsr.w $2382c
            next = 0x22e88U;
            const auto result = m.call(c, 413U, 0x22e84U, 0x2382cU, 0x22e88U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22e88U: { // 4e75 rts 
            next = 0x22e8aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22e88U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22e66U:
        case 0x22e6aU:
        case 0x22e72U:
        case 0x22e76U:
        case 0x22e7cU:
        case 0x22e7eU:
        case 0x22e80U:
        case 0x22e84U:
        case 0x22e88U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022e8a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22e8aU: { // 61000a3c bsr.w $238c8
            next = 0x22e8eU;
            const auto result = m.call(c, 416U, 0x22e8aU, 0x238c8U, 0x22e8eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22e8eU: { // 2b7c00022ed20002 move.l #$22ed2, $2(a5)
            next = 0x22e96U;
            const auto value = 0x22ed2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22e96U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22e9aU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22e9aU: { // 41f9000242f0 lea.l $242f0.l, a0
            next = 0x22ea0U;
            const auto source_address = 0x242f0U;
            r.address[0] = source_address;
            break;
        }
        case 0x22ea0U: { // 7007 moveq #$7, d0
            next = 0x22ea2U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22ea2U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22ea4U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22ea4U: { // 51c8fffc dbra d0, $22ea2
            next = 0x22ea8U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22ea2U;
            break;
        }
        case 0x22ea8U: { // 61000982 bsr.w $2382c
            next = 0x22eacU;
            const auto result = m.call(c, 413U, 0x22ea8U, 0x2382cU, 0x22eacU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22eacU: { // 4e75 rts 
            next = 0x22eaeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22eacU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22e8aU:
        case 0x22e8eU:
        case 0x22e96U:
        case 0x22e9aU:
        case 0x22ea0U:
        case 0x22ea2U:
        case 0x22ea4U:
        case 0x22ea8U:
        case 0x22eacU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022eae(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22eaeU: { // 61000a18 bsr.w $238c8
            next = 0x22eb2U;
            const auto result = m.call(c, 416U, 0x22eaeU, 0x238c8U, 0x22eb2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22eb2U: { // 2b7c00022ed20002 move.l #$22ed2, $2(a5)
            next = 0x22ebaU;
            const auto value = 0x22ed2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22ebaU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22ebeU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22ebeU: { // 41f900024302 lea.l $24302.l, a0
            next = 0x22ec4U;
            const auto source_address = 0x24302U;
            r.address[0] = source_address;
            break;
        }
        case 0x22ec4U: { // 7007 moveq #$7, d0
            next = 0x22ec6U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22ec6U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22ec8U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22ec8U: { // 51c8fffc dbra d0, $22ec6
            next = 0x22eccU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22ec6U;
            break;
        }
        case 0x22eccU: { // 6100095e bsr.w $2382c
            next = 0x22ed0U;
            const auto result = m.call(c, 413U, 0x22eccU, 0x2382cU, 0x22ed0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22ed0U: { // 4e75 rts 
            next = 0x22ed2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22ed0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22eaeU:
        case 0x22eb2U:
        case 0x22ebaU:
        case 0x22ebeU:
        case 0x22ec4U:
        case 0x22ec6U:
        case 0x22ec8U:
        case 0x22eccU:
        case 0x22ed0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
