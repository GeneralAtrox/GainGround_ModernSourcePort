
FunctionResult cpu_b_stage_022c6a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22c6aU: { // 61000c5c bsr.w $238c8
            next = 0x22c6eU;
            const auto result = m.call(c, 416U, 0x22c6aU, 0x238c8U, 0x22c6eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22c6eU: { // 2b7c00022cb20002 move.l #$22cb2, $2(a5)
            next = 0x22c76U;
            const auto value = 0x22cb2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22c76U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22c7aU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22c7aU: { // 41f9000241c6 lea.l $241c6.l, a0
            next = 0x22c80U;
            const auto source_address = 0x241c6U;
            r.address[0] = source_address;
            break;
        }
        case 0x22c80U: { // 7007 moveq #$7, d0
            next = 0x22c82U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22c82U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22c84U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22c84U: { // 51c8fffc dbra d0, $22c82
            next = 0x22c88U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22c82U;
            break;
        }
        case 0x22c88U: { // 61000ba2 bsr.w $2382c
            next = 0x22c8cU;
            const auto result = m.call(c, 413U, 0x22c88U, 0x2382cU, 0x22c8cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22c8cU: { // 4e75 rts 
            next = 0x22c8eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22c8cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22c6aU:
        case 0x22c6eU:
        case 0x22c76U:
        case 0x22c7aU:
        case 0x22c80U:
        case 0x22c82U:
        case 0x22c84U:
        case 0x22c88U:
        case 0x22c8cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022c8e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22c8eU: { // 61000c38 bsr.w $238c8
            next = 0x22c92U;
            const auto result = m.call(c, 416U, 0x22c8eU, 0x238c8U, 0x22c92U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22c92U: { // 2b7c00022cb20002 move.l #$22cb2, $2(a5)
            next = 0x22c9aU;
            const auto value = 0x22cb2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22c9aU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22c9eU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22c9eU: { // 41f9000241d6 lea.l $241d6.l, a0
            next = 0x22ca4U;
            const auto source_address = 0x241d6U;
            r.address[0] = source_address;
            break;
        }
        case 0x22ca4U: { // 7007 moveq #$7, d0
            next = 0x22ca6U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22ca6U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22ca8U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22ca8U: { // 51c8fffc dbra d0, $22ca6
            next = 0x22cacU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22ca6U;
            break;
        }
        case 0x22cacU: { // 61000b7e bsr.w $2382c
            next = 0x22cb0U;
            const auto result = m.call(c, 413U, 0x22cacU, 0x2382cU, 0x22cb0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22cb0U: { // 4e75 rts 
            next = 0x22cb2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22cb0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22c8eU:
        case 0x22c92U:
        case 0x22c9aU:
        case 0x22c9eU:
        case 0x22ca4U:
        case 0x22ca6U:
        case 0x22ca8U:
        case 0x22cacU:
        case 0x22cb0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022d5e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22d5eU: { // 61000a82 bsr.w $237e2
            next = 0x22d62U;
            const auto result = m.call(c, 412U, 0x22d5eU, 0x237e2U, 0x22d62U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22d62U: { // 45f900024226 lea.l $24226.l, a2
            next = 0x22d68U;
            const auto source_address = 0x24226U;
            r.address[2] = source_address;
            break;
        }
        case 0x22d68U: { // 61000bc0 bsr.w $2392a
            next = 0x22d6cU;
            const auto result = m.call(c, 419U, 0x22d68U, 0x2392aU, 0x22d6cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22d6cU: { // 4e75 rts 
            next = 0x22d6eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22d6cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22d5eU:
        case 0x22d62U:
        case 0x22d68U:
        case 0x22d6cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022cf2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22cf2U: { // 61000bd4 bsr.w $238c8
            next = 0x22cf6U;
            const auto result = m.call(c, 416U, 0x22cf2U, 0x238c8U, 0x22cf6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22cf6U: { // 2b7c00022d5e0002 move.l #$22d5e, $2(a5)
            next = 0x22cfeU;
            const auto value = 0x22d5eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22cfeU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22d02U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22d02U: { // 41f9000241f6 lea.l $241f6.l, a0
            next = 0x22d08U;
            const auto source_address = 0x241f6U;
            r.address[0] = source_address;
            break;
        }
        case 0x22d08U: { // 7007 moveq #$7, d0
            next = 0x22d0aU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22d0aU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22d0cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22d0cU: { // 51c8fffc dbra d0, $22d0a
            next = 0x22d10U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22d0aU;
            break;
        }
        case 0x22d10U: { // 61000b1a bsr.w $2382c
            next = 0x22d14U;
            const auto result = m.call(c, 413U, 0x22d10U, 0x2382cU, 0x22d14U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22d14U: { // 4e75 rts 
            next = 0x22d16U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22d14U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22cf2U:
        case 0x22cf6U:
        case 0x22cfeU:
        case 0x22d02U:
        case 0x22d08U:
        case 0x22d0aU:
        case 0x22d0cU:
        case 0x22d10U:
        case 0x22d14U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
