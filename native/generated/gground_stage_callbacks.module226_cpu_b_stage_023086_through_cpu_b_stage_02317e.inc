
FunctionResult cpu_b_stage_023086(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23086U: { // 61000840 bsr.w $238c8
            next = 0x2308aU;
            const auto result = m.call(c, 416U, 0x23086U, 0x238c8U, 0x2308aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2308aU: { // 2b7c000230f20002 move.l #$230f2, $2(a5)
            next = 0x23092U;
            const auto value = 0x230f2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x23092U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x23096U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x23096U: { // 41f9000244c0 lea.l $244c0.l, a0
            next = 0x2309cU;
            const auto source_address = 0x244c0U;
            r.address[0] = source_address;
            break;
        }
        case 0x2309cU: { // 7007 moveq #$7, d0
            next = 0x2309eU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2309eU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x230a0U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x230a0U: { // 51c8fffc dbra d0, $2309e
            next = 0x230a4U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2309eU;
            break;
        }
        case 0x230a4U: { // 61000786 bsr.w $2382c
            next = 0x230a8U;
            const auto result = m.call(c, 413U, 0x230a4U, 0x2382cU, 0x230a8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x230a8U: { // 4e75 rts 
            next = 0x230aaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x230a8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23086U:
        case 0x2308aU:
        case 0x23092U:
        case 0x23096U:
        case 0x2309cU:
        case 0x2309eU:
        case 0x230a0U:
        case 0x230a4U:
        case 0x230a8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0230aa(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x230aaU: { // 6100081c bsr.w $238c8
            next = 0x230aeU;
            const auto result = m.call(c, 416U, 0x230aaU, 0x238c8U, 0x230aeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x230aeU: { // 2b7c000230f20002 move.l #$230f2, $2(a5)
            next = 0x230b6U;
            const auto value = 0x230f2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x230b6U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x230baU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x230baU: { // 41f9000244d2 lea.l $244d2.l, a0
            next = 0x230c0U;
            const auto source_address = 0x244d2U;
            r.address[0] = source_address;
            break;
        }
        case 0x230c0U: { // 7007 moveq #$7, d0
            next = 0x230c2U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x230c2U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x230c4U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x230c4U: { // 51c8fffc dbra d0, $230c2
            next = 0x230c8U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x230c2U;
            break;
        }
        case 0x230c8U: { // 61000762 bsr.w $2382c
            next = 0x230ccU;
            const auto result = m.call(c, 413U, 0x230c8U, 0x2382cU, 0x230ccU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x230ccU: { // 4e75 rts 
            next = 0x230ceU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x230ccU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x230aaU:
        case 0x230aeU:
        case 0x230b6U:
        case 0x230baU:
        case 0x230c0U:
        case 0x230c2U:
        case 0x230c4U:
        case 0x230c8U:
        case 0x230ccU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0230ce(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x230ceU: { // 610007f8 bsr.w $238c8
            next = 0x230d2U;
            const auto result = m.call(c, 416U, 0x230ceU, 0x238c8U, 0x230d2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x230d2U: { // 2b7c000230f20002 move.l #$230f2, $2(a5)
            next = 0x230daU;
            const auto value = 0x230f2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x230daU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x230deU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x230deU: { // 41f9000244e4 lea.l $244e4.l, a0
            next = 0x230e4U;
            const auto source_address = 0x244e4U;
            r.address[0] = source_address;
            break;
        }
        case 0x230e4U: { // 7007 moveq #$7, d0
            next = 0x230e6U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x230e6U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x230e8U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x230e8U: { // 51c8fffc dbra d0, $230e6
            next = 0x230ecU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x230e6U;
            break;
        }
        case 0x230ecU: { // 6100073e bsr.w $2382c
            next = 0x230f0U;
            const auto result = m.call(c, 413U, 0x230ecU, 0x2382cU, 0x230f0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x230f0U: { // 4e75 rts 
            next = 0x230f2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x230f0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x230ceU:
        case 0x230d2U:
        case 0x230daU:
        case 0x230deU:
        case 0x230e4U:
        case 0x230e6U:
        case 0x230e8U:
        case 0x230ecU:
        case 0x230f0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02317e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2317eU: { // 61000662 bsr.w $237e2
            next = 0x23182U;
            const auto result = m.call(c, 412U, 0x2317eU, 0x237e2U, 0x23182U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23182U: { // 45f90002457c lea.l $2457c.l, a2
            next = 0x23188U;
            const auto source_address = 0x2457cU;
            r.address[2] = source_address;
            break;
        }
        case 0x23188U: { // 610007a0 bsr.w $2392a
            next = 0x2318cU;
            const auto result = m.call(c, 419U, 0x23188U, 0x2392aU, 0x2318cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2318cU: { // 4e75 rts 
            next = 0x2318eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2318cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2317eU:
        case 0x23182U:
        case 0x23188U:
        case 0x2318cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
