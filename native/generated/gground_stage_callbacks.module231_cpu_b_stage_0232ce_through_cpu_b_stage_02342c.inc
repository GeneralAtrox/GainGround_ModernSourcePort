
FunctionResult cpu_b_stage_0232ce(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x232ceU: { // 610005f8 bsr.w $238c8
            next = 0x232d2U;
            const auto result = m.call(c, 416U, 0x232ceU, 0x238c8U, 0x232d2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x232d2U: { // 2b7c000232f20002 move.l #$232f2, $2(a5)
            next = 0x232daU;
            const auto value = 0x232f2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x232daU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x232deU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x232deU: { // 41f9000246b8 lea.l $246b8.l, a0
            next = 0x232e4U;
            const auto source_address = 0x246b8U;
            r.address[0] = source_address;
            break;
        }
        case 0x232e4U: { // 7007 moveq #$7, d0
            next = 0x232e6U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x232e6U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x232e8U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x232e8U: { // 51c8fffc dbra d0, $232e6
            next = 0x232ecU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x232e6U;
            break;
        }
        case 0x232ecU: { // 6100053e bsr.w $2382c
            next = 0x232f0U;
            const auto result = m.call(c, 413U, 0x232ecU, 0x2382cU, 0x232f0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x232f0U: { // 4e75 rts 
            next = 0x232f2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x232f0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x232ceU:
        case 0x232d2U:
        case 0x232daU:
        case 0x232deU:
        case 0x232e4U:
        case 0x232e6U:
        case 0x232e8U:
        case 0x232ecU:
        case 0x232f0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02333e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2333eU: { // 610004a2 bsr.w $237e2
            next = 0x23342U;
            const auto result = m.call(c, 412U, 0x2333eU, 0x237e2U, 0x23342U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23342U: { // 4e75 rts 
            next = 0x23344U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23342U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2333eU:
        case 0x23342U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_023330(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23330U: { // 61000596 bsr.w $238c8
            next = 0x23334U;
            const auto result = m.call(c, 416U, 0x23330U, 0x238c8U, 0x23334U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23334U: { // 2b7c0002333e0002 move.l #$2333e, $2(a5)
            next = 0x2333cU;
            const auto value = 0x2333eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2333cU: { // 4e75 rts 
            next = 0x2333eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2333cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23330U:
        case 0x23334U:
        case 0x2333cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_023344(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23344U: { // 61000582 bsr.w $238c8
            next = 0x23348U;
            const auto result = m.call(c, 416U, 0x23344U, 0x238c8U, 0x23348U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23348U: { // 4dfa0066 lea.l $233b0(pc), a6
            next = 0x2334cU;
            const auto source_address = 0x233b0U;
            r.address[6] = source_address;
            break;
        }
        case 0x2334cU: { // 2b4e0002 move.l a6, $2(a5)
            next = 0x23350U;
            const auto value = r.address[6];
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x23350U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x23354U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x23354U: { // 41f9000246e8 lea.l $246e8.l, a0
            next = 0x2335aU;
            const auto source_address = 0x246e8U;
            r.address[0] = source_address;
            break;
        }
        case 0x2335aU: { // 7007 moveq #$7, d0
            next = 0x2335cU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2335cU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2335eU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2335eU: { // 51c8fffc dbra d0, $2335c
            next = 0x23362U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2335cU;
            break;
        }
        case 0x23362U: { // 610004c8 bsr.w $2382c
            next = 0x23366U;
            const auto result = m.call(c, 413U, 0x23362U, 0x2382cU, 0x23366U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23366U: { // 4e75 rts 
            next = 0x23368U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23366U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23344U:
        case 0x23348U:
        case 0x2334cU:
        case 0x23350U:
        case 0x23354U:
        case 0x2335aU:
        case 0x2335cU:
        case 0x2335eU:
        case 0x23362U:
        case 0x23366U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02342c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2342cU: { // 610003b4 bsr.w $237e2
            next = 0x23430U;
            const auto result = m.call(c, 412U, 0x2342cU, 0x237e2U, 0x23430U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23430U: { // 45f900024718 lea.l $24718.l, a2
            next = 0x23436U;
            const auto source_address = 0x24718U;
            r.address[2] = source_address;
            break;
        }
        case 0x23436U: { // 610004f2 bsr.w $2392a
            next = 0x2343aU;
            const auto result = m.call(c, 419U, 0x23436U, 0x2392aU, 0x2343aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2343aU: { // 4e75 rts 
            next = 0x2343cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2343aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2342cU:
        case 0x23430U:
        case 0x23436U:
        case 0x2343aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
