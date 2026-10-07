
FunctionResult cpu_b_stage_022c06(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22c06U: { // 61000bda bsr.w $237e2
            next = 0x22c0aU;
            const auto result = m.call(c, 412U, 0x22c06U, 0x237e2U, 0x22c0aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22c0aU: { // 302d001c move.w $1c(a5), d0
            next = 0x22c0eU;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x22c0eU: { // e540 asl.w #$2, d0
            next = 0x22c10U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x22c10U: { // 4efb0002 jmp $22c14(pc, d0.w)
            next = 0x22c14U;
            next = (0x22c14U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x22c14U: { // 60000008 bra.w $22c1e
            next = 0x22c18U;
            if (true) { next = 0x22c1eU; transfer_kind = 1U; }
            break;
        }
        case 0x22c18U: { // 60000020 bra.w $22c3a
            next = 0x22c1cU;
            if (true) { next = 0x22c3aU; transfer_kind = 1U; }
            break;
        }
        case 0x22c1cU: { // 4e75 rts 
            next = 0x22c1eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22c1cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22c1eU: { // 0c6d00f00008 cmpi.w #$f0, $8(a5)
            next = 0x22c24U;
            const auto source_value = 0xf0U;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x22c24U: { // 6e02 bgt.b $22c28
            next = 0x22c26U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x22c28U; transfer_kind = 1U; }
            break;
        }
        case 0x22c26U: { // 4e75 rts 
            next = 0x22c28U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22c26U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22c28U: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x22c2cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x22c2cU: { // 1b7c00010022 move.b #$1, $22(a5)
            next = 0x22c32U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x22U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22c32U: { // 1b7c00010024 move.b #$1, $24(a5)
            next = 0x22c38U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x24U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22c38U: { // 4e75 rts 
            next = 0x22c3aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22c38U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22c3aU: { // 45f900024196 lea.l $24196.l, a2
            next = 0x22c40U;
            const auto source_address = 0x24196U;
            r.address[2] = source_address;
            break;
        }
        case 0x22c40U: { // 61000ce8 bsr.w $2392a
            next = 0x22c44U;
            const auto result = m.call(c, 419U, 0x22c40U, 0x2392aU, 0x22c44U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22c44U: { // 4e75 rts 
            next = 0x22c46U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22c44U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22c06U:
        case 0x22c0aU:
        case 0x22c0eU:
        case 0x22c10U:
        case 0x22c14U:
        case 0x22c18U:
        case 0x22c1cU:
        case 0x22c1eU:
        case 0x22c24U:
        case 0x22c26U:
        case 0x22c28U:
        case 0x22c2cU:
        case 0x22c32U:
        case 0x22c38U:
        case 0x22c3aU:
        case 0x22c40U:
        case 0x22c44U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022b9a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22b9aU: { // 61000d2c bsr.w $238c8
            next = 0x22b9eU;
            const auto result = m.call(c, 416U, 0x22b9aU, 0x238c8U, 0x22b9eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22b9eU: { // 2b7c00022c060002 move.l #$22c06, $2(a5)
            next = 0x22ba6U;
            const auto value = 0x22c06U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22ba6U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22baaU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22baaU: { // 41f900024166 lea.l $24166.l, a0
            next = 0x22bb0U;
            const auto source_address = 0x24166U;
            r.address[0] = source_address;
            break;
        }
        case 0x22bb0U: { // 7007 moveq #$7, d0
            next = 0x22bb2U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22bb2U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22bb4U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22bb4U: { // 51c8fffc dbra d0, $22bb2
            next = 0x22bb8U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22bb2U;
            break;
        }
        case 0x22bb8U: { // 61000c72 bsr.w $2382c
            next = 0x22bbcU;
            const auto result = m.call(c, 413U, 0x22bb8U, 0x2382cU, 0x22bbcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22bbcU: { // 4e75 rts 
            next = 0x22bbeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22bbcU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22b9aU:
        case 0x22b9eU:
        case 0x22ba6U:
        case 0x22baaU:
        case 0x22bb0U:
        case 0x22bb2U:
        case 0x22bb4U:
        case 0x22bb8U:
        case 0x22bbcU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
