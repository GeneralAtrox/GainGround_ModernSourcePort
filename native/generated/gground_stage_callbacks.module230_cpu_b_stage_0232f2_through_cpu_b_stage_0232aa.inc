
FunctionResult cpu_b_stage_0232f2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x232f2U: { // 610004ee bsr.w $237e2
            next = 0x232f6U;
            const auto result = m.call(c, 412U, 0x232f2U, 0x237e2U, 0x232f6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x232f6U: { // 302d001c move.w $1c(a5), d0
            next = 0x232faU;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x232faU: { // e540 asl.w #$2, d0
            next = 0x232fcU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x232fcU: { // 4efb0002 jmp $23300(pc, d0.w)
            next = 0x23300U;
            next = (0x23300U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x23300U: { // 60000006 bra.w $23308
            next = 0x23304U;
            if (true) { next = 0x23308U; transfer_kind = 1U; }
            break;
        }
        case 0x23304U: { // 6000001e bra.w $23324
            next = 0x23308U;
            if (true) { next = 0x23324U; transfer_kind = 1U; }
            break;
        }
        case 0x23308U: { // 0c6d03840008 cmpi.w #$384, $8(a5)
            next = 0x2330eU;
            const auto source_value = 0x384U;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x2330eU: { // 6c02 bge.b $23312
            next = 0x23310U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x23312U; transfer_kind = 1U; }
            break;
        }
        case 0x23310U: { // 4e75 rts 
            next = 0x23312U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23310U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x23312U: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x23316U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x23316U: { // 1b7c00010022 move.b #$1, $22(a5)
            next = 0x2331cU;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x22U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x2331cU: { // 1b7c00010024 move.b #$1, $24(a5)
            next = 0x23322U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x24U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x23322U: { // 4e75 rts 
            next = 0x23324U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23322U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x23324U: { // 45f9000246c8 lea.l $246c8.l, a2
            next = 0x2332aU;
            const auto source_address = 0x246c8U;
            r.address[2] = source_address;
            break;
        }
        case 0x2332aU: { // 610005fe bsr.w $2392a
            next = 0x2332eU;
            const auto result = m.call(c, 419U, 0x2332aU, 0x2392aU, 0x2332eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2332eU: { // 4e75 rts 
            next = 0x23330U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2332eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x232f2U:
        case 0x232f6U:
        case 0x232faU:
        case 0x232fcU:
        case 0x23300U:
        case 0x23304U:
        case 0x23308U:
        case 0x2330eU:
        case 0x23310U:
        case 0x23312U:
        case 0x23316U:
        case 0x2331cU:
        case 0x23322U:
        case 0x23324U:
        case 0x2332aU:
        case 0x2332eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_023286(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23286U: { // 61000640 bsr.w $238c8
            next = 0x2328aU;
            const auto result = m.call(c, 416U, 0x23286U, 0x238c8U, 0x2328aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2328aU: { // 2b7c000232f20002 move.l #$232f2, $2(a5)
            next = 0x23292U;
            const auto value = 0x232f2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x23292U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x23296U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x23296U: { // 41f900024698 lea.l $24698.l, a0
            next = 0x2329cU;
            const auto source_address = 0x24698U;
            r.address[0] = source_address;
            break;
        }
        case 0x2329cU: { // 7007 moveq #$7, d0
            next = 0x2329eU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2329eU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x232a0U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x232a0U: { // 51c8fffc dbra d0, $2329e
            next = 0x232a4U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2329eU;
            break;
        }
        case 0x232a4U: { // 61000586 bsr.w $2382c
            next = 0x232a8U;
            const auto result = m.call(c, 413U, 0x232a4U, 0x2382cU, 0x232a8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x232a8U: { // 4e75 rts 
            next = 0x232aaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x232a8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23286U:
        case 0x2328aU:
        case 0x23292U:
        case 0x23296U:
        case 0x2329cU:
        case 0x2329eU:
        case 0x232a0U:
        case 0x232a4U:
        case 0x232a8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0232aa(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x232aaU: { // 6100061c bsr.w $238c8
            next = 0x232aeU;
            const auto result = m.call(c, 416U, 0x232aaU, 0x238c8U, 0x232aeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x232aeU: { // 2b7c000232f20002 move.l #$232f2, $2(a5)
            next = 0x232b6U;
            const auto value = 0x232f2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x232b6U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x232baU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x232baU: { // 41f9000246a8 lea.l $246a8.l, a0
            next = 0x232c0U;
            const auto source_address = 0x246a8U;
            r.address[0] = source_address;
            break;
        }
        case 0x232c0U: { // 7007 moveq #$7, d0
            next = 0x232c2U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x232c2U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x232c4U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x232c4U: { // 51c8fffc dbra d0, $232c2
            next = 0x232c8U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x232c2U;
            break;
        }
        case 0x232c8U: { // 61000562 bsr.w $2382c
            next = 0x232ccU;
            const auto result = m.call(c, 413U, 0x232c8U, 0x2382cU, 0x232ccU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x232ccU: { // 4e75 rts 
            next = 0x232ceU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x232ccU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x232aaU:
        case 0x232aeU:
        case 0x232b6U:
        case 0x232baU:
        case 0x232c0U:
        case 0x232c2U:
        case 0x232c4U:
        case 0x232c8U:
        case 0x232ccU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
