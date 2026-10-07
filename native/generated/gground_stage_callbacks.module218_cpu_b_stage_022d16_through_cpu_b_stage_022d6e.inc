
FunctionResult cpu_b_stage_022d16(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22d16U: { // 61000bb0 bsr.w $238c8
            next = 0x22d1aU;
            const auto result = m.call(c, 416U, 0x22d16U, 0x238c8U, 0x22d1aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22d1aU: { // 2b7c00022d5e0002 move.l #$22d5e, $2(a5)
            next = 0x22d22U;
            const auto value = 0x22d5eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22d22U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22d26U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22d26U: { // 41f900024206 lea.l $24206.l, a0
            next = 0x22d2cU;
            const auto source_address = 0x24206U;
            r.address[0] = source_address;
            break;
        }
        case 0x22d2cU: { // 7007 moveq #$7, d0
            next = 0x22d2eU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22d2eU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22d30U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22d30U: { // 51c8fffc dbra d0, $22d2e
            next = 0x22d34U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22d2eU;
            break;
        }
        case 0x22d34U: { // 61000af6 bsr.w $2382c
            next = 0x22d38U;
            const auto result = m.call(c, 413U, 0x22d34U, 0x2382cU, 0x22d38U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22d38U: { // 4e75 rts 
            next = 0x22d3aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22d38U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22d16U:
        case 0x22d1aU:
        case 0x22d22U:
        case 0x22d26U:
        case 0x22d2cU:
        case 0x22d2eU:
        case 0x22d30U:
        case 0x22d34U:
        case 0x22d38U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022d3a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22d3aU: { // 61000b8c bsr.w $238c8
            next = 0x22d3eU;
            const auto result = m.call(c, 416U, 0x22d3aU, 0x238c8U, 0x22d3eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22d3eU: { // 2b7c00022d5e0002 move.l #$22d5e, $2(a5)
            next = 0x22d46U;
            const auto value = 0x22d5eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22d46U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22d4aU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22d4aU: { // 41f900024216 lea.l $24216.l, a0
            next = 0x22d50U;
            const auto source_address = 0x24216U;
            r.address[0] = source_address;
            break;
        }
        case 0x22d50U: { // 7007 moveq #$7, d0
            next = 0x22d52U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22d52U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22d54U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22d54U: { // 51c8fffc dbra d0, $22d52
            next = 0x22d58U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22d52U;
            break;
        }
        case 0x22d58U: { // 61000ad2 bsr.w $2382c
            next = 0x22d5cU;
            const auto result = m.call(c, 413U, 0x22d58U, 0x2382cU, 0x22d5cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22d5cU: { // 4e75 rts 
            next = 0x22d5eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22d5cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22d3aU:
        case 0x22d3eU:
        case 0x22d46U:
        case 0x22d4aU:
        case 0x22d50U:
        case 0x22d52U:
        case 0x22d54U:
        case 0x22d58U:
        case 0x22d5cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022dda(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22ddaU: { // 61000a06 bsr.w $237e2
            next = 0x22ddeU;
            const auto result = m.call(c, 412U, 0x22ddaU, 0x237e2U, 0x22ddeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22ddeU: { // 45f900024276 lea.l $24276.l, a2
            next = 0x22de4U;
            const auto source_address = 0x24276U;
            r.address[2] = source_address;
            break;
        }
        case 0x22de4U: { // 61000b44 bsr.w $2392a
            next = 0x22de8U;
            const auto result = m.call(c, 419U, 0x22de4U, 0x2392aU, 0x22de8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22de8U: { // 4e75 rts 
            next = 0x22deaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22de8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22ddaU:
        case 0x22ddeU:
        case 0x22de4U:
        case 0x22de8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022d6e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22d6eU: { // 61000b58 bsr.w $238c8
            next = 0x22d72U;
            const auto result = m.call(c, 416U, 0x22d6eU, 0x238c8U, 0x22d72U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22d72U: { // 2b7c00022dda0002 move.l #$22dda, $2(a5)
            next = 0x22d7aU;
            const auto value = 0x22ddaU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22d7aU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22d7eU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22d7eU: { // 41f900024246 lea.l $24246.l, a0
            next = 0x22d84U;
            const auto source_address = 0x24246U;
            r.address[0] = source_address;
            break;
        }
        case 0x22d84U: { // 7007 moveq #$7, d0
            next = 0x22d86U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22d86U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22d88U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22d88U: { // 51c8fffc dbra d0, $22d86
            next = 0x22d8cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22d86U;
            break;
        }
        case 0x22d8cU: { // 61000a9e bsr.w $2382c
            next = 0x22d90U;
            const auto result = m.call(c, 413U, 0x22d8cU, 0x2382cU, 0x22d90U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22d90U: { // 4e75 rts 
            next = 0x22d92U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22d90U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22d6eU:
        case 0x22d72U:
        case 0x22d7aU:
        case 0x22d7eU:
        case 0x22d84U:
        case 0x22d86U:
        case 0x22d88U:
        case 0x22d8cU:
        case 0x22d90U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
