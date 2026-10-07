
FunctionResult cpu_b_stage_02270a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2270aU: { // 610010d6 bsr.w $237e2
            next = 0x2270eU;
            const auto result = m.call(c, 412U, 0x2270aU, 0x237e2U, 0x2270eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2270eU: { // 302d001c move.w $1c(a5), d0
            next = 0x22712U;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x22712U: { // e540 asl.w #$2, d0
            next = 0x22714U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x22714U: { // 4efb0002 jmp $22718(pc, d0.w)
            next = 0x22718U;
            next = (0x22718U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x22718U: { // 60000008 bra.w $22722
            next = 0x2271cU;
            if (true) { next = 0x22722U; transfer_kind = 1U; }
            break;
        }
        case 0x2271cU: { // 60000024 bra.w $22742
            next = 0x22720U;
            if (true) { next = 0x22742U; transfer_kind = 1U; }
            break;
        }
        case 0x22720U: { // 4e75 rts 
            next = 0x22722U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22720U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22722U: { // 202d000a move.l $a(a5), d0
            next = 0x22726U;
            const auto source_address = r.address[5] + 0xaU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x22726U: { // 028001010100 andi.l #$1010100, d0
            next = 0x2272cU;
            r.data[0] &= 0x1010100U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2272cU: { // 6602 bne.b $22730
            next = 0x2272eU;
            if ((r.status & 4U) == 0U) { next = 0x22730U; transfer_kind = 1U; }
            break;
        }
        case 0x2272eU: { // 4e75 rts 
            next = 0x22730U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2272eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22730U: { // 1b7c00020022 move.b #$2, $22(a5)
            next = 0x22736U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x22U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22736U: { // 1b7c00020028 move.b #$2, $28(a5)
            next = 0x2273cU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x28U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x2273cU: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x22740U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x22740U: { // 4e75 rts 
            next = 0x22742U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22740U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22742U: { // 45f900023e26 lea.l $23e26.l, a2
            next = 0x22748U;
            const auto source_address = 0x23e26U;
            r.address[2] = source_address;
            break;
        }
        case 0x22748U: { // 610011e0 bsr.w $2392a
            next = 0x2274cU;
            const auto result = m.call(c, 419U, 0x22748U, 0x2392aU, 0x2274cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2274cU: { // 4e75 rts 
            next = 0x2274eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2274cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2270aU:
        case 0x2270eU:
        case 0x22712U:
        case 0x22714U:
        case 0x22718U:
        case 0x2271cU:
        case 0x22720U:
        case 0x22722U:
        case 0x22726U:
        case 0x2272cU:
        case 0x2272eU:
        case 0x22730U:
        case 0x22736U:
        case 0x2273cU:
        case 0x22740U:
        case 0x22742U:
        case 0x22748U:
        case 0x2274cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0226e6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x226e6U: { // 610011e0 bsr.w $238c8
            next = 0x226eaU;
            const auto result = m.call(c, 416U, 0x226e6U, 0x238c8U, 0x226eaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x226eaU: { // 2b7c0002270a0002 move.l #$2270a, $2(a5)
            next = 0x226f2U;
            const auto value = 0x2270aU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x226f2U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x226f6U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x226f6U: { // 41f900023e16 lea.l $23e16.l, a0
            next = 0x226fcU;
            const auto source_address = 0x23e16U;
            r.address[0] = source_address;
            break;
        }
        case 0x226fcU: { // 7007 moveq #$7, d0
            next = 0x226feU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x226feU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22700U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22700U: { // 51c8fffc dbra d0, $226fe
            next = 0x22704U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x226feU;
            break;
        }
        case 0x22704U: { // 61001126 bsr.w $2382c
            next = 0x22708U;
            const auto result = m.call(c, 413U, 0x22704U, 0x2382cU, 0x22708U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22708U: { // 4e75 rts 
            next = 0x2270aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22708U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x226e6U:
        case 0x226eaU:
        case 0x226f2U:
        case 0x226f6U:
        case 0x226fcU:
        case 0x226feU:
        case 0x22700U:
        case 0x22704U:
        case 0x22708U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
