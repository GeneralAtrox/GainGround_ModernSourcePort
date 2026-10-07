
FunctionResult cpu_b_stage_022af0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22af0U: { // 61000dd6 bsr.w $238c8
            next = 0x22af4U;
            const auto result = m.call(c, 416U, 0x22af0U, 0x238c8U, 0x22af4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22af4U: { // 2b7c00022b5c0002 move.l #$22b5c, $2(a5)
            next = 0x22afcU;
            const auto value = 0x22b5cU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22afcU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22b00U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22b00U: { // 41f9000240e6 lea.l $240e6.l, a0
            next = 0x22b06U;
            const auto source_address = 0x240e6U;
            r.address[0] = source_address;
            break;
        }
        case 0x22b06U: { // 7007 moveq #$7, d0
            next = 0x22b08U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22b08U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22b0aU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22b0aU: { // 51c8fffc dbra d0, $22b08
            next = 0x22b0eU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22b08U;
            break;
        }
        case 0x22b0eU: { // 61000d1c bsr.w $2382c
            next = 0x22b12U;
            const auto result = m.call(c, 413U, 0x22b0eU, 0x2382cU, 0x22b12U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22b12U: { // 4e75 rts 
            next = 0x22b14U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22b12U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22af0U:
        case 0x22af4U:
        case 0x22afcU:
        case 0x22b00U:
        case 0x22b06U:
        case 0x22b08U:
        case 0x22b0aU:
        case 0x22b0eU:
        case 0x22b12U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022b14(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22b14U: { // 61000db2 bsr.w $238c8
            next = 0x22b18U;
            const auto result = m.call(c, 416U, 0x22b14U, 0x238c8U, 0x22b18U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22b18U: { // 2b7c00022b5c0002 move.l #$22b5c, $2(a5)
            next = 0x22b20U;
            const auto value = 0x22b5cU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22b20U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22b24U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22b24U: { // 41f9000240f6 lea.l $240f6.l, a0
            next = 0x22b2aU;
            const auto source_address = 0x240f6U;
            r.address[0] = source_address;
            break;
        }
        case 0x22b2aU: { // 7007 moveq #$7, d0
            next = 0x22b2cU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22b2cU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22b2eU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22b2eU: { // 51c8fffc dbra d0, $22b2c
            next = 0x22b32U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22b2cU;
            break;
        }
        case 0x22b32U: { // 61000cf8 bsr.w $2382c
            next = 0x22b36U;
            const auto result = m.call(c, 413U, 0x22b32U, 0x2382cU, 0x22b36U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22b36U: { // 4e75 rts 
            next = 0x22b38U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22b36U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22b14U:
        case 0x22b18U:
        case 0x22b20U:
        case 0x22b24U:
        case 0x22b2aU:
        case 0x22b2cU:
        case 0x22b2eU:
        case 0x22b32U:
        case 0x22b36U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022b38(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22b38U: { // 61000d8e bsr.w $238c8
            next = 0x22b3cU;
            const auto result = m.call(c, 416U, 0x22b38U, 0x238c8U, 0x22b3cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22b3cU: { // 2b7c00022b5c0002 move.l #$22b5c, $2(a5)
            next = 0x22b44U;
            const auto value = 0x22b5cU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22b44U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22b48U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22b48U: { // 41f900024106 lea.l $24106.l, a0
            next = 0x22b4eU;
            const auto source_address = 0x24106U;
            r.address[0] = source_address;
            break;
        }
        case 0x22b4eU: { // 7007 moveq #$7, d0
            next = 0x22b50U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22b50U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22b52U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22b52U: { // 51c8fffc dbra d0, $22b50
            next = 0x22b56U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22b50U;
            break;
        }
        case 0x22b56U: { // 61000cd4 bsr.w $2382c
            next = 0x22b5aU;
            const auto result = m.call(c, 413U, 0x22b56U, 0x2382cU, 0x22b5aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22b5aU: { // 4e75 rts 
            next = 0x22b5cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22b5aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22b38U:
        case 0x22b3cU:
        case 0x22b44U:
        case 0x22b48U:
        case 0x22b4eU:
        case 0x22b50U:
        case 0x22b52U:
        case 0x22b56U:
        case 0x22b5aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
