
FunctionResult cpu_b_stage_0227f2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x227f2U: { // 61000fee bsr.w $237e2
            next = 0x227f6U;
            const auto result = m.call(c, 412U, 0x227f2U, 0x237e2U, 0x227f6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x227f6U: { // 4e75 rts 
            next = 0x227f8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x227f6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x227f2U:
        case 0x227f6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0227e0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x227e0U: { // 610010e6 bsr.w $238c8
            next = 0x227e4U;
            const auto result = m.call(c, 416U, 0x227e0U, 0x238c8U, 0x227e4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x227e4U: { // 2b7c000227f20002 move.l #$227f2, $2(a5)
            next = 0x227ecU;
            const auto value = 0x227f2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x227ecU: { // 6100103e bsr.w $2382c
            next = 0x227f0U;
            const auto result = m.call(c, 413U, 0x227ecU, 0x2382cU, 0x227f0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x227f0U: { // 4e75 rts 
            next = 0x227f2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x227f0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x227e0U:
        case 0x227e4U:
        case 0x227ecU:
        case 0x227f0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022864(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22864U: { // 61000f7c bsr.w $237e2
            next = 0x22868U;
            const auto result = m.call(c, 412U, 0x22864U, 0x237e2U, 0x22868U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22868U: { // 302d001c move.w $1c(a5), d0
            next = 0x2286cU;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x2286cU: { // e540 asl.w #$2, d0
            next = 0x2286eU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x2286eU: { // 4efb0002 jmp $22872(pc, d0.w)
            next = 0x22872U;
            next = (0x22872U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x22872U: { // 60000006 bra.w $2287a
            next = 0x22876U;
            if (true) { next = 0x2287aU; transfer_kind = 1U; }
            break;
        }
        case 0x22876U: { // 60000012 bra.w $2288a
            next = 0x2287aU;
            if (true) { next = 0x2288aU; transfer_kind = 1U; }
            break;
        }
        case 0x2287aU: { // 0c6d00f00008 cmpi.w #$f0, $8(a5)
            next = 0x22880U;
            const auto source_value = 0xf0U;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x22880U: { // 6e02 bgt.b $22884
            next = 0x22882U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x22884U; transfer_kind = 1U; }
            break;
        }
        case 0x22882U: { // 4e75 rts 
            next = 0x22884U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22882U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22884U: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x22888U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x22888U: { // 4e75 rts 
            next = 0x2288aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22888U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2288aU: { // 302d0008 move.w $8(a5), d0
            next = 0x2288eU;
            const auto source_address = r.address[5] + 0x8U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x2288eU: { // 0240003f andi.w #$3f, d0
            next = 0x22892U;
            const auto source_value = 0x3fU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x22892U: { // 6702 beq.b $22896
            next = 0x22894U;
            if ((r.status & 4U) != 0U) { next = 0x22896U; transfer_kind = 1U; }
            break;
        }
        case 0x22894U: { // 4e75 rts 
            next = 0x22896U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22894U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22896U: { // 45f900023f42 lea.l $23f42.l, a2
            next = 0x2289cU;
            const auto source_address = 0x23f42U;
            r.address[2] = source_address;
            break;
        }
        case 0x2289cU: { // 6100108c bsr.w $2392a
            next = 0x228a0U;
            const auto result = m.call(c, 419U, 0x2289cU, 0x2392aU, 0x228a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x228a0U: { // 4e75 rts 
            next = 0x228a2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x228a0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22864U:
        case 0x22868U:
        case 0x2286cU:
        case 0x2286eU:
        case 0x22872U:
        case 0x22876U:
        case 0x2287aU:
        case 0x22880U:
        case 0x22882U:
        case 0x22884U:
        case 0x22888U:
        case 0x2288aU:
        case 0x2288eU:
        case 0x22892U:
        case 0x22894U:
        case 0x22896U:
        case 0x2289cU:
        case 0x228a0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
