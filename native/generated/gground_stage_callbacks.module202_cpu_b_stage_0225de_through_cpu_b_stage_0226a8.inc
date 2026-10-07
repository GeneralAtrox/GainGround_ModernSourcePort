
FunctionResult cpu_b_stage_0225de(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x225deU: { // 610012e8 bsr.w $238c8
            next = 0x225e2U;
            const auto result = m.call(c, 416U, 0x225deU, 0x238c8U, 0x225e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x225e2U: { // 2b7c000226020002 move.l #$22602, $2(a5)
            next = 0x225eaU;
            const auto value = 0x22602U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x225eaU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x225eeU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x225eeU: { // 41f900023d86 lea.l $23d86.l, a0
            next = 0x225f4U;
            const auto source_address = 0x23d86U;
            r.address[0] = source_address;
            break;
        }
        case 0x225f4U: { // 7007 moveq #$7, d0
            next = 0x225f6U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x225f6U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x225f8U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x225f8U: { // 51c8fffc dbra d0, $225f6
            next = 0x225fcU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x225f6U;
            break;
        }
        case 0x225fcU: { // 6100122e bsr.w $2382c
            next = 0x22600U;
            const auto result = m.call(c, 413U, 0x225fcU, 0x2382cU, 0x22600U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22600U: { // 4e75 rts 
            next = 0x22602U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22600U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x225deU:
        case 0x225e2U:
        case 0x225eaU:
        case 0x225eeU:
        case 0x225f4U:
        case 0x225f6U:
        case 0x225f8U:
        case 0x225fcU:
        case 0x22600U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0226a8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x226a8U: { // 61001138 bsr.w $237e2
            next = 0x226acU;
            const auto result = m.call(c, 412U, 0x226a8U, 0x237e2U, 0x226acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x226acU: { // 302d001c move.w $1c(a5), d0
            next = 0x226b0U;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x226b0U: { // e540 asl.w #$2, d0
            next = 0x226b2U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x226b2U: { // 4efb0002 jmp $226b6(pc, d0.w)
            next = 0x226b6U;
            next = (0x226b6U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x226b6U: { // 60000008 bra.w $226c0
            next = 0x226baU;
            if (true) { next = 0x226c0U; transfer_kind = 1U; }
            break;
        }
        case 0x226baU: { // 6000001e bra.w $226da
            next = 0x226beU;
            if (true) { next = 0x226daU; transfer_kind = 1U; }
            break;
        }
        case 0x226beU: { // 4e75 rts 
            next = 0x226c0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x226beU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x226c0U: { // 202d000a move.l $a(a5), d0
            next = 0x226c4U;
            const auto source_address = r.address[5] + 0xaU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x226c4U: { // 028001010100 andi.l #$1010100, d0
            next = 0x226caU;
            r.data[0] &= 0x1010100U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x226caU: { // 6602 bne.b $226ce
            next = 0x226ccU;
            if ((r.status & 4U) == 0U) { next = 0x226ceU; transfer_kind = 1U; }
            break;
        }
        case 0x226ccU: { // 4e75 rts 
            next = 0x226ceU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x226ccU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x226ceU: { // 1b7c00020022 move.b #$2, $22(a5)
            next = 0x226d4U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x22U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x226d4U: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x226d8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x226d8U: { // 4e75 rts 
            next = 0x226daU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x226d8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x226daU: { // 45f900023df4 lea.l $23df4.l, a2
            next = 0x226e0U;
            const auto source_address = 0x23df4U;
            r.address[2] = source_address;
            break;
        }
        case 0x226e0U: { // 61001248 bsr.w $2392a
            next = 0x226e4U;
            const auto result = m.call(c, 419U, 0x226e0U, 0x2392aU, 0x226e4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x226e4U: { // 4e75 rts 
            next = 0x226e6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x226e4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x226a8U:
        case 0x226acU:
        case 0x226b0U:
        case 0x226b2U:
        case 0x226b6U:
        case 0x226baU:
        case 0x226beU:
        case 0x226c0U:
        case 0x226c4U:
        case 0x226caU:
        case 0x226ccU:
        case 0x226ceU:
        case 0x226d4U:
        case 0x226d8U:
        case 0x226daU:
        case 0x226e0U:
        case 0x226e4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
