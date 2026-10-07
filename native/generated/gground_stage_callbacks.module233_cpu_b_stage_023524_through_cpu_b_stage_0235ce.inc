
FunctionResult cpu_b_stage_023524(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23524U: { // 610002bc bsr.w $237e2
            next = 0x23528U;
            const auto result = m.call(c, 412U, 0x23524U, 0x237e2U, 0x23528U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23528U: { // 302d001c move.w $1c(a5), d0
            next = 0x2352cU;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x2352cU: { // e540 asl.w #$2, d0
            next = 0x2352eU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x2352eU: { // 4efb0002 jmp $23532(pc, d0.w)
            next = 0x23532U;
            next = (0x23532U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x23532U: { // 60000006 bra.w $2353a
            next = 0x23536U;
            if (true) { next = 0x2353aU; transfer_kind = 1U; }
            break;
        }
        case 0x23536U: { // 6000001e bra.w $23556
            next = 0x2353aU;
            if (true) { next = 0x23556U; transfer_kind = 1U; }
            break;
        }
        case 0x2353aU: { // 0c6d03840008 cmpi.w #$384, $8(a5)
            next = 0x23540U;
            const auto source_value = 0x384U;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x23540U: { // 6c02 bge.b $23544
            next = 0x23542U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x23544U; transfer_kind = 1U; }
            break;
        }
        case 0x23542U: { // 4e75 rts 
            next = 0x23544U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23542U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x23544U: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x23548U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x23548U: { // 1b7c00010022 move.b #$1, $22(a5)
            next = 0x2354eU;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x22U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x2354eU: { // 1b7c00010024 move.b #$1, $24(a5)
            next = 0x23554U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x24U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x23554U: { // 4e75 rts 
            next = 0x23556U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23554U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x23556U: { // 45f900024790 lea.l $24790.l, a2
            next = 0x2355cU;
            const auto source_address = 0x24790U;
            r.address[2] = source_address;
            break;
        }
        case 0x2355cU: { // 610003cc bsr.w $2392a
            next = 0x23560U;
            const auto result = m.call(c, 419U, 0x2355cU, 0x2392aU, 0x23560U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23560U: { // 4e75 rts 
            next = 0x23562U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23560U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23524U:
        case 0x23528U:
        case 0x2352cU:
        case 0x2352eU:
        case 0x23532U:
        case 0x23536U:
        case 0x2353aU:
        case 0x23540U:
        case 0x23542U:
        case 0x23544U:
        case 0x23548U:
        case 0x2354eU:
        case 0x23554U:
        case 0x23556U:
        case 0x2355cU:
        case 0x23560U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0234b8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x234b8U: { // 6100040e bsr.w $238c8
            next = 0x234bcU;
            const auto result = m.call(c, 416U, 0x234b8U, 0x238c8U, 0x234bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x234bcU: { // 2b7c000235240002 move.l #$23524, $2(a5)
            next = 0x234c4U;
            const auto value = 0x23524U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x234c4U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x234c8U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x234c8U: { // 41f900024760 lea.l $24760.l, a0
            next = 0x234ceU;
            const auto source_address = 0x24760U;
            r.address[0] = source_address;
            break;
        }
        case 0x234ceU: { // 7007 moveq #$7, d0
            next = 0x234d0U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x234d0U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x234d2U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x234d2U: { // 51c8fffc dbra d0, $234d0
            next = 0x234d6U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x234d0U;
            break;
        }
        case 0x234d6U: { // 61000354 bsr.w $2382c
            next = 0x234daU;
            const auto result = m.call(c, 413U, 0x234d6U, 0x2382cU, 0x234daU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x234daU: { // 4e75 rts 
            next = 0x234dcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x234daU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x234b8U:
        case 0x234bcU:
        case 0x234c4U:
        case 0x234c8U:
        case 0x234ceU:
        case 0x234d0U:
        case 0x234d2U:
        case 0x234d6U:
        case 0x234daU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0235ce(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x235ceU: { // 61000212 bsr.w $237e2
            next = 0x235d2U;
            const auto result = m.call(c, 412U, 0x235ceU, 0x237e2U, 0x235d2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x235d2U: { // 45f9000247e0 lea.l $247e0.l, a2
            next = 0x235d8U;
            const auto source_address = 0x247e0U;
            r.address[2] = source_address;
            break;
        }
        case 0x235d8U: { // 61000350 bsr.w $2392a
            next = 0x235dcU;
            const auto result = m.call(c, 419U, 0x235d8U, 0x2392aU, 0x235dcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x235dcU: { // 4e75 rts 
            next = 0x235deU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x235dcU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x235ceU:
        case 0x235d2U:
        case 0x235d8U:
        case 0x235dcU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
