
FunctionResult cpu_b_stage_022cb2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22cb2U: { // 61000b2e bsr.w $237e2
            next = 0x22cb6U;
            const auto result = m.call(c, 412U, 0x22cb2U, 0x237e2U, 0x22cb6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22cb6U: { // 302d001c move.w $1c(a5), d0
            next = 0x22cbaU;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x22cbaU: { // e540 asl.w #$2, d0
            next = 0x22cbcU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x22cbcU: { // 4efb0002 jmp $22cc0(pc, d0.w)
            next = 0x22cc0U;
            next = (0x22cc0U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x22cc0U: { // 60000008 bra.w $22cca
            next = 0x22cc4U;
            if (true) { next = 0x22ccaU; transfer_kind = 1U; }
            break;
        }
        case 0x22cc4U: { // 60000020 bra.w $22ce6
            next = 0x22cc8U;
            if (true) { next = 0x22ce6U; transfer_kind = 1U; }
            break;
        }
        case 0x22cc8U: { // 4e75 rts 
            next = 0x22ccaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22cc8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22ccaU: { // 4a2d0080 tst.b $80(a5)
            next = 0x22cceU;
            const auto destination_address = r.address[5] + 0x80U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x22cceU: { // 6a02 bpl.b $22cd2
            next = 0x22cd0U;
            if ((r.status & 8U) == 0U) { next = 0x22cd2U; transfer_kind = 1U; }
            break;
        }
        case 0x22cd0U: { // 4e75 rts 
            next = 0x22cd2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22cd0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22cd2U: { // 4a2d0200 tst.b $200(a5)
            next = 0x22cd6U;
            const auto destination_address = r.address[5] + 0x200U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x22cd6U: { // 6a02 bpl.b $22cda
            next = 0x22cd8U;
            if ((r.status & 8U) == 0U) { next = 0x22cdaU; transfer_kind = 1U; }
            break;
        }
        case 0x22cd8U: { // 4e75 rts 
            next = 0x22cdaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22cd8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22cdaU: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x22cdeU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x22cdeU: { // 1b7c00010022 move.b #$1, $22(a5)
            next = 0x22ce4U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x22U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22ce4U: { // 4e75 rts 
            next = 0x22ce6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22ce4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22ce6U: { // 45f9000241e6 lea.l $241e6.l, a2
            next = 0x22cecU;
            const auto source_address = 0x241e6U;
            r.address[2] = source_address;
            break;
        }
        case 0x22cecU: { // 61000c3c bsr.w $2392a
            next = 0x22cf0U;
            const auto result = m.call(c, 419U, 0x22cecU, 0x2392aU, 0x22cf0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22cf0U: { // 4e75 rts 
            next = 0x22cf2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22cf0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22cb2U:
        case 0x22cb6U:
        case 0x22cbaU:
        case 0x22cbcU:
        case 0x22cc0U:
        case 0x22cc4U:
        case 0x22cc8U:
        case 0x22ccaU:
        case 0x22cceU:
        case 0x22cd0U:
        case 0x22cd2U:
        case 0x22cd6U:
        case 0x22cd8U:
        case 0x22cdaU:
        case 0x22cdeU:
        case 0x22ce4U:
        case 0x22ce6U:
        case 0x22cecU:
        case 0x22cf0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022c46(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22c46U: { // 61000c80 bsr.w $238c8
            next = 0x22c4aU;
            const auto result = m.call(c, 416U, 0x22c46U, 0x238c8U, 0x22c4aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22c4aU: { // 2b7c00022cb20002 move.l #$22cb2, $2(a5)
            next = 0x22c52U;
            const auto value = 0x22cb2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22c52U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22c56U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22c56U: { // 41fa155e lea.l $241b6(pc), a0
            next = 0x22c5aU;
            const auto source_address = 0x241b6U;
            r.address[0] = source_address;
            break;
        }
        case 0x22c5aU: { // 4e71 nop 
            next = 0x22c5cU;
            break;
        }
        case 0x22c5cU: { // 7007 moveq #$7, d0
            next = 0x22c5eU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22c5eU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22c60U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22c60U: { // 51c8fffc dbra d0, $22c5e
            next = 0x22c64U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22c5eU;
            break;
        }
        case 0x22c64U: { // 61000bc6 bsr.w $2382c
            next = 0x22c68U;
            const auto result = m.call(c, 413U, 0x22c64U, 0x2382cU, 0x22c68U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22c68U: { // 4e75 rts 
            next = 0x22c6aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22c68U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22c46U:
        case 0x22c4aU:
        case 0x22c52U:
        case 0x22c56U:
        case 0x22c5aU:
        case 0x22c5cU:
        case 0x22c5eU:
        case 0x22c60U:
        case 0x22c64U:
        case 0x22c68U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
