
FunctionResult cpu_b_stage_gap_0227ba(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x227baU: { // 61001026 bsr.w $237e2
            next = 0x227beU;
            const auto result = m.call(c, 412U, 0x227baU, 0x237e2U, 0x227beU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x227beU: { // 302d001c move.w $1c(a5), d0
            next = 0x227c2U;
            const auto v1 = m.word((r.address[5] + 0x1cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x227c2U: { // e540 asl.w #$2, d0
            next = 0x227c4U;
            m.asl_word(0, 2U);
            break;
        }
        case 0x227c4U: { // 4efb0002 jmp $227c8(pc, d0.w)
            next = 0x227c8U;
            next = 0x227c8U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])); transfer_kind = 1U;
            break;
        }
        case 0x227c8U: { // 60000008 bra.w $227d2
            next = 0x227ccU;
            next = 0x227d2U; transfer_kind = 1U;
            break;
        }
        case 0x227ccU: { // 60000010 bra.w $227de
            next = 0x227d0U;
            next = 0x227deU; transfer_kind = 1U;
            break;
        }
        case 0x227d2U: { // 45f900023eac lea.l $23eac.l, a2
            next = 0x227d8U;
            r.address[2] = 0x23eacU;
            break;
        }
        case 0x227d8U: { // 61001150 bsr.w $2392a
            next = 0x227dcU;
            const auto result = m.call(c, 419U, 0x227d8U, 0x2392aU, 0x227dcU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x227dcU: { // 4e75 rts 
            next = 0x227deU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x227dcU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x227deU: { // 4e75 rts 
            next = 0x227e0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x227deU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x227baU:
        case 0x227beU:
        case 0x227c2U:
        case 0x227c4U:
        case 0x227c8U:
        case 0x227ccU:
        case 0x227d2U:
        case 0x227d8U:
        case 0x227dcU:
        case 0x227deU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_02290e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2290eU: { // 61000ed2 bsr.w $237e2
            next = 0x22912U;
            const auto result = m.call(c, 412U, 0x2290eU, 0x237e2U, 0x22912U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22912U: { // 302d001c move.w $1c(a5), d0
            next = 0x22916U;
            const auto v1 = m.word((r.address[5] + 0x1cU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x22916U: { // e540 asl.w #$2, d0
            next = 0x22918U;
            m.asl_word(0, 2U);
            break;
        }
        case 0x22918U: { // 4efb0002 jmp $2291c(pc, d0.w)
            next = 0x2291cU;
            next = 0x2291cU + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])); transfer_kind = 1U;
            break;
        }
        case 0x2291cU: { // 60000008 bra.w $22926
            next = 0x22920U;
            next = 0x22926U; transfer_kind = 1U;
            break;
        }
        case 0x22920U: { // 6000002a bra.w $2294c
            next = 0x22924U;
            next = 0x2294cU; transfer_kind = 1U;
            break;
        }
        case 0x22926U: { // 45f900023f9e lea.l $23f9e.l, a2
            next = 0x2292cU;
            r.address[2] = 0x23f9eU;
            break;
        }
        case 0x2292cU: { // 61000ffc bsr.w $2392a
            next = 0x22930U;
            const auto result = m.call(c, 419U, 0x2292cU, 0x2392aU, 0x22930U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22930U: { // 0c6d04380008 cmpi.w #$438, $8(a5)
            next = 0x22936U;
            (void)m.sub(m.word((r.address[5] + 0x8U)), 0x438U, 16U, true);
            break;
        }
        case 0x22936U: { // 6c02 bge.b $2293a
            next = 0x22938U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x2293aU; transfer_kind = 1U; }
            break;
        }
        case 0x22938U: { // 4e75 rts 
            next = 0x2293aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22938U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2293aU: { // 1b7c00010026 move.b #$1, $26(a5)
            next = 0x22940U;
            const auto v1 = 0x1U;
            m.byte((r.address[5] + 0x26U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x22940U: { // 1b7c00010028 move.b #$1, $28(a5)
            next = 0x22946U;
            const auto v1 = 0x1U;
            m.byte((r.address[5] + 0x28U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x22946U: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x2294aU;
            const auto v1 = m.add(m.word((r.address[5] + 0x1cU)), 0x1U, 16U);
            m.word((r.address[5] + 0x1cU), v1);
            break;
        }
        case 0x2294aU: { // 4e75 rts 
            next = 0x2294cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2294aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2294cU: { // 45f900023f9e lea.l $23f9e.l, a2
            next = 0x22952U;
            r.address[2] = 0x23f9eU;
            break;
        }
        case 0x22952U: { // 61000fd6 bsr.w $2392a
            next = 0x22956U;
            const auto result = m.call(c, 419U, 0x22952U, 0x2392aU, 0x22956U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22956U: { // 4e75 rts 
            next = 0x22958U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22956U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2290eU:
        case 0x22912U:
        case 0x22916U:
        case 0x22918U:
        case 0x2291cU:
        case 0x22920U:
        case 0x22926U:
        case 0x2292cU:
        case 0x22930U:
        case 0x22936U:
        case 0x22938U:
        case 0x2293aU:
        case 0x22940U:
        case 0x22946U:
        case 0x2294aU:
        case 0x2294cU:
        case 0x22952U:
        case 0x22956U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_0231fa(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x231faU: { // 610005e6 bsr.w $237e2
            next = 0x231feU;
            const auto result = m.call(c, 412U, 0x231faU, 0x237e2U, 0x231feU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x231feU: { // 45f9000245fa lea.l $245fa.l, a2
            next = 0x23204U;
            r.address[2] = 0x245faU;
            break;
        }
        case 0x23204U: { // 61000724 bsr.w $2392a
            next = 0x23208U;
            const auto result = m.call(c, 419U, 0x23204U, 0x2392aU, 0x23208U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23208U: { // 4e75 rts 
            next = 0x2320aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23208U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x231faU:
        case 0x231feU:
        case 0x23204U:
        case 0x23208U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
