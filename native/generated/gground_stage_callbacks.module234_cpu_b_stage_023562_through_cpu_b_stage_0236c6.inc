
FunctionResult cpu_b_stage_023562(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23562U: { // 61000364 bsr.w $238c8
            next = 0x23566U;
            const auto result = m.call(c, 416U, 0x23562U, 0x238c8U, 0x23566U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23566U: { // 2b7c000235ce0002 move.l #$235ce, $2(a5)
            next = 0x2356eU;
            const auto value = 0x235ceU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2356eU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x23572U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x23572U: { // 41f9000247b0 lea.l $247b0.l, a0
            next = 0x23578U;
            const auto source_address = 0x247b0U;
            r.address[0] = source_address;
            break;
        }
        case 0x23578U: { // 7007 moveq #$7, d0
            next = 0x2357aU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2357aU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2357cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2357cU: { // 51c8fffc dbra d0, $2357a
            next = 0x23580U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2357aU;
            break;
        }
        case 0x23580U: { // 610002aa bsr.w $2382c
            next = 0x23584U;
            const auto result = m.call(c, 413U, 0x23580U, 0x2382cU, 0x23584U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23584U: { // 4e75 rts 
            next = 0x23586U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23584U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23562U:
        case 0x23566U:
        case 0x2356eU:
        case 0x23572U:
        case 0x23578U:
        case 0x2357aU:
        case 0x2357cU:
        case 0x23580U:
        case 0x23584U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02364a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2364aU: { // 61000196 bsr.w $237e2
            next = 0x2364eU;
            const auto result = m.call(c, 412U, 0x2364aU, 0x237e2U, 0x2364eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2364eU: { // 45f900024718 lea.l $24718.l, a2
            next = 0x23654U;
            const auto source_address = 0x24718U;
            r.address[2] = source_address;
            break;
        }
        case 0x23654U: { // 610002d4 bsr.w $2392a
            next = 0x23658U;
            const auto result = m.call(c, 419U, 0x23654U, 0x2392aU, 0x23658U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23658U: { // 4e75 rts 
            next = 0x2365aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23658U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2364aU:
        case 0x2364eU:
        case 0x23654U:
        case 0x23658U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0235de(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x235deU: { // 610002e8 bsr.w $238c8
            next = 0x235e2U;
            const auto result = m.call(c, 416U, 0x235deU, 0x238c8U, 0x235e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x235e2U: { // 2b7c0002364a0002 move.l #$2364a, $2(a5)
            next = 0x235eaU;
            const auto value = 0x2364aU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x235eaU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x235eeU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x235eeU: { // 41f9000246e8 lea.l $246e8.l, a0
            next = 0x235f4U;
            const auto source_address = 0x246e8U;
            r.address[0] = source_address;
            break;
        }
        case 0x235f4U: { // 7007 moveq #$7, d0
            next = 0x235f6U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x235f6U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x235f8U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x235f8U: { // 51c8fffc dbra d0, $235f6
            next = 0x235fcU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x235f6U;
            break;
        }
        case 0x235fcU: { // 6100022e bsr.w $2382c
            next = 0x23600U;
            const auto result = m.call(c, 413U, 0x235fcU, 0x2382cU, 0x23600U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23600U: { // 4e75 rts 
            next = 0x23602U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23600U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x235deU:
        case 0x235e2U:
        case 0x235eaU:
        case 0x235eeU:
        case 0x235f4U:
        case 0x235f6U:
        case 0x235f8U:
        case 0x235fcU:
        case 0x23600U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0236c6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x236c6U: { // 6100011a bsr.w $237e2
            next = 0x236caU;
            const auto result = m.call(c, 412U, 0x236c6U, 0x237e2U, 0x236caU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x236caU: { // 45f900024718 lea.l $24718.l, a2
            next = 0x236d0U;
            const auto source_address = 0x24718U;
            r.address[2] = source_address;
            break;
        }
        case 0x236d0U: { // 61000258 bsr.w $2392a
            next = 0x236d4U;
            const auto result = m.call(c, 419U, 0x236d0U, 0x2392aU, 0x236d4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x236d4U: { // 4e75 rts 
            next = 0x236d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x236d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x236c6U:
        case 0x236caU:
        case 0x236d0U:
        case 0x236d4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
