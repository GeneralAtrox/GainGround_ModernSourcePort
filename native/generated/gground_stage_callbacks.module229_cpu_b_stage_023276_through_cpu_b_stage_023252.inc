
FunctionResult cpu_b_stage_023276(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23276U: { // 6100056a bsr.w $237e2
            next = 0x2327aU;
            const auto result = m.call(c, 412U, 0x23276U, 0x237e2U, 0x2327aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2327aU: { // 45f900024678 lea.l $24678.l, a2
            next = 0x23280U;
            const auto source_address = 0x24678U;
            r.address[2] = source_address;
            break;
        }
        case 0x23280U: { // 610006a8 bsr.w $2392a
            next = 0x23284U;
            const auto result = m.call(c, 419U, 0x23280U, 0x2392aU, 0x23284U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23284U: { // 4e75 rts 
            next = 0x23286U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23284U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23276U:
        case 0x2327aU:
        case 0x23280U:
        case 0x23284U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02320a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2320aU: { // 610006bc bsr.w $238c8
            next = 0x2320eU;
            const auto result = m.call(c, 416U, 0x2320aU, 0x238c8U, 0x2320eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2320eU: { // 2b7c000232760002 move.l #$23276, $2(a5)
            next = 0x23216U;
            const auto value = 0x23276U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x23216U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2321aU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2321aU: { // 41f900024648 lea.l $24648.l, a0
            next = 0x23220U;
            const auto source_address = 0x24648U;
            r.address[0] = source_address;
            break;
        }
        case 0x23220U: { // 7007 moveq #$7, d0
            next = 0x23222U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x23222U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x23224U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x23224U: { // 51c8fffc dbra d0, $23222
            next = 0x23228U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x23222U;
            break;
        }
        case 0x23228U: { // 61000602 bsr.w $2382c
            next = 0x2322cU;
            const auto result = m.call(c, 413U, 0x23228U, 0x2382cU, 0x2322cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2322cU: { // 4e75 rts 
            next = 0x2322eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2322cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2320aU:
        case 0x2320eU:
        case 0x23216U:
        case 0x2321aU:
        case 0x23220U:
        case 0x23222U:
        case 0x23224U:
        case 0x23228U:
        case 0x2322cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02322e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2322eU: { // 61000698 bsr.w $238c8
            next = 0x23232U;
            const auto result = m.call(c, 416U, 0x2322eU, 0x238c8U, 0x23232U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23232U: { // 2b7c000232760002 move.l #$23276, $2(a5)
            next = 0x2323aU;
            const auto value = 0x23276U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2323aU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2323eU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2323eU: { // 41f900024658 lea.l $24658.l, a0
            next = 0x23244U;
            const auto source_address = 0x24658U;
            r.address[0] = source_address;
            break;
        }
        case 0x23244U: { // 7007 moveq #$7, d0
            next = 0x23246U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x23246U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x23248U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x23248U: { // 51c8fffc dbra d0, $23246
            next = 0x2324cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x23246U;
            break;
        }
        case 0x2324cU: { // 610005de bsr.w $2382c
            next = 0x23250U;
            const auto result = m.call(c, 413U, 0x2324cU, 0x2382cU, 0x23250U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23250U: { // 4e75 rts 
            next = 0x23252U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23250U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2322eU:
        case 0x23232U:
        case 0x2323aU:
        case 0x2323eU:
        case 0x23244U:
        case 0x23246U:
        case 0x23248U:
        case 0x2324cU:
        case 0x23250U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_023252(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23252U: { // 61000674 bsr.w $238c8
            next = 0x23256U;
            const auto result = m.call(c, 416U, 0x23252U, 0x238c8U, 0x23256U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23256U: { // 2b7c000232760002 move.l #$23276, $2(a5)
            next = 0x2325eU;
            const auto value = 0x23276U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2325eU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x23262U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x23262U: { // 41f900024668 lea.l $24668.l, a0
            next = 0x23268U;
            const auto source_address = 0x24668U;
            r.address[0] = source_address;
            break;
        }
        case 0x23268U: { // 7007 moveq #$7, d0
            next = 0x2326aU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2326aU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2326cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2326cU: { // 51c8fffc dbra d0, $2326a
            next = 0x23270U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2326aU;
            break;
        }
        case 0x23270U: { // 610005ba bsr.w $2382c
            next = 0x23274U;
            const auto result = m.call(c, 413U, 0x23270U, 0x2382cU, 0x23274U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23274U: { // 4e75 rts 
            next = 0x23276U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23274U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23252U:
        case 0x23256U:
        case 0x2325eU:
        case 0x23262U:
        case 0x23268U:
        case 0x2326aU:
        case 0x2326cU:
        case 0x23270U:
        case 0x23274U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
