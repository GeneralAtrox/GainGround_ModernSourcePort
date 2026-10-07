
FunctionResult cpu_b_stage_0233c0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x233c0U: { // 61000506 bsr.w $238c8
            next = 0x233c4U;
            const auto result = m.call(c, 416U, 0x233c0U, 0x238c8U, 0x233c4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x233c4U: { // 2b7c0002342c0002 move.l #$2342c, $2(a5)
            next = 0x233ccU;
            const auto value = 0x2342cU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x233ccU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x233d0U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x233d0U: { // 41f9000246e8 lea.l $246e8.l, a0
            next = 0x233d6U;
            const auto source_address = 0x246e8U;
            r.address[0] = source_address;
            break;
        }
        case 0x233d6U: { // 7007 moveq #$7, d0
            next = 0x233d8U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x233d8U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x233daU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x233daU: { // 51c8fffc dbra d0, $233d8
            next = 0x233deU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x233d8U;
            break;
        }
        case 0x233deU: { // 6100044c bsr.w $2382c
            next = 0x233e2U;
            const auto result = m.call(c, 413U, 0x233deU, 0x2382cU, 0x233e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x233e2U: { // 4e75 rts 
            next = 0x233e4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x233e2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x233c0U:
        case 0x233c4U:
        case 0x233ccU:
        case 0x233d0U:
        case 0x233d6U:
        case 0x233d8U:
        case 0x233daU:
        case 0x233deU:
        case 0x233e2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0234a8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x234a8U: { // 61000338 bsr.w $237e2
            next = 0x234acU;
            const auto result = m.call(c, 412U, 0x234a8U, 0x237e2U, 0x234acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x234acU: { // 45f900024718 lea.l $24718.l, a2
            next = 0x234b2U;
            const auto source_address = 0x24718U;
            r.address[2] = source_address;
            break;
        }
        case 0x234b2U: { // 61000476 bsr.w $2392a
            next = 0x234b6U;
            const auto result = m.call(c, 419U, 0x234b2U, 0x2392aU, 0x234b6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x234b6U: { // 4e75 rts 
            next = 0x234b8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x234b6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x234a8U:
        case 0x234acU:
        case 0x234b2U:
        case 0x234b6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_02343c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2343cU: { // 6100048a bsr.w $238c8
            next = 0x23440U;
            const auto result = m.call(c, 416U, 0x2343cU, 0x238c8U, 0x23440U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23440U: { // 2b7c000234a80002 move.l #$234a8, $2(a5)
            next = 0x23448U;
            const auto value = 0x234a8U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x23448U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2344cU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2344cU: { // 41f9000246e8 lea.l $246e8.l, a0
            next = 0x23452U;
            const auto source_address = 0x246e8U;
            r.address[0] = source_address;
            break;
        }
        case 0x23452U: { // 7007 moveq #$7, d0
            next = 0x23454U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x23454U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x23456U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x23456U: { // 51c8fffc dbra d0, $23454
            next = 0x2345aU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x23454U;
            break;
        }
        case 0x2345aU: { // 610003d0 bsr.w $2382c
            next = 0x2345eU;
            const auto result = m.call(c, 413U, 0x2345aU, 0x2382cU, 0x2345eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2345eU: { // 4e75 rts 
            next = 0x23460U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2345eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2343cU:
        case 0x23440U:
        case 0x23448U:
        case 0x2344cU:
        case 0x23452U:
        case 0x23454U:
        case 0x23456U:
        case 0x2345aU:
        case 0x2345eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
