
FunctionResult cpu_b_stage_022602(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22602U: { // 610011de bsr.w $237e2
            next = 0x22606U;
            const auto result = m.call(c, 412U, 0x22602U, 0x237e2U, 0x22606U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22606U: { // 302d001c move.w $1c(a5), d0
            next = 0x2260aU;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x2260aU: { // e540 asl.w #$2, d0
            next = 0x2260cU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x2260cU: { // 4efb0002 jmp $22610(pc, d0.w)
            next = 0x22610U;
            next = (0x22610U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x22610U: { // 60000008 bra.w $2261a
            next = 0x22614U;
            if (true) { next = 0x2261aU; transfer_kind = 1U; }
            break;
        }
        case 0x22614U: { // 6000001a bra.w $22630
            next = 0x22618U;
            if (true) { next = 0x22630U; transfer_kind = 1U; }
            break;
        }
        case 0x22618U: { // 4e75 rts 
            next = 0x2261aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22618U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2261aU: { // 0c6d04380008 cmpi.w #$438, $8(a5)
            next = 0x22620U;
            const auto source_value = 0x438U;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x22620U: { // 6c02 bge.b $22624
            next = 0x22622U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x22624U; transfer_kind = 1U; }
            break;
        }
        case 0x22622U: { // 4e75 rts 
            next = 0x22624U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22622U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22624U: { // 1b7c00020022 move.b #$2, $22(a5)
            next = 0x2262aU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x22U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x2262aU: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x2262eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x2262eU: { // 4e75 rts 
            next = 0x22630U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2262eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22630U: { // 45f900023d96 lea.l $23d96.l, a2
            next = 0x22636U;
            const auto source_address = 0x23d96U;
            r.address[2] = source_address;
            break;
        }
        case 0x22636U: { // 610012f2 bsr.w $2392a
            next = 0x2263aU;
            const auto result = m.call(c, 419U, 0x22636U, 0x2392aU, 0x2263aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2263aU: { // 4e75 rts 
            next = 0x2263cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2263aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22602U:
        case 0x22606U:
        case 0x2260aU:
        case 0x2260cU:
        case 0x22610U:
        case 0x22614U:
        case 0x22618U:
        case 0x2261aU:
        case 0x22620U:
        case 0x22622U:
        case 0x22624U:
        case 0x2262aU:
        case 0x2262eU:
        case 0x22630U:
        case 0x22636U:
        case 0x2263aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022596(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22596U: { // 61001330 bsr.w $238c8
            next = 0x2259aU;
            const auto result = m.call(c, 416U, 0x22596U, 0x238c8U, 0x2259aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2259aU: { // 2b7c000226020002 move.l #$22602, $2(a5)
            next = 0x225a2U;
            const auto value = 0x22602U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x225a2U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x225a6U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x225a6U: { // 41f900023d66 lea.l $23d66.l, a0
            next = 0x225acU;
            const auto source_address = 0x23d66U;
            r.address[0] = source_address;
            break;
        }
        case 0x225acU: { // 7007 moveq #$7, d0
            next = 0x225aeU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x225aeU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x225b0U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x225b0U: { // 51c8fffc dbra d0, $225ae
            next = 0x225b4U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x225aeU;
            break;
        }
        case 0x225b4U: { // 61001276 bsr.w $2382c
            next = 0x225b8U;
            const auto result = m.call(c, 413U, 0x225b4U, 0x2382cU, 0x225b8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x225b8U: { // 4e75 rts 
            next = 0x225baU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x225b8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22596U:
        case 0x2259aU:
        case 0x225a2U:
        case 0x225a6U:
        case 0x225acU:
        case 0x225aeU:
        case 0x225b0U:
        case 0x225b4U:
        case 0x225b8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0225ba(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x225baU: { // 6100130c bsr.w $238c8
            next = 0x225beU;
            const auto result = m.call(c, 416U, 0x225baU, 0x238c8U, 0x225beU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x225beU: { // 2b7c000226020002 move.l #$22602, $2(a5)
            next = 0x225c6U;
            const auto value = 0x22602U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x225c6U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x225caU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x225caU: { // 41fa17aa lea.l $23d76(pc), a0
            next = 0x225ceU;
            const auto source_address = 0x23d76U;
            r.address[0] = source_address;
            break;
        }
        case 0x225ceU: { // 4e71 nop 
            next = 0x225d0U;
            break;
        }
        case 0x225d0U: { // 7007 moveq #$7, d0
            next = 0x225d2U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x225d2U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x225d4U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x225d4U: { // 51c8fffc dbra d0, $225d2
            next = 0x225d8U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x225d2U;
            break;
        }
        case 0x225d8U: { // 61001252 bsr.w $2382c
            next = 0x225dcU;
            const auto result = m.call(c, 413U, 0x225d8U, 0x2382cU, 0x225dcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x225dcU: { // 4e75 rts 
            next = 0x225deU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x225dcU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x225baU:
        case 0x225beU:
        case 0x225c6U:
        case 0x225caU:
        case 0x225ceU:
        case 0x225d0U:
        case 0x225d2U:
        case 0x225d4U:
        case 0x225d8U:
        case 0x225dcU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
