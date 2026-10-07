
FunctionResult cpu_b_stage_018a44(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18a44U: { // 0c38000a0d19 cmpi.b #$a, $d19.w
            next = 0x18a4aU;
            const auto source_value = 0xaU;
            const auto destination_address = 0xd19U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x18a4aU: { // 6e30 bgt.b $18a7c
            next = 0x18a4cU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18a7cU; transfer_kind = 1U; }
            break;
        }
        case 0x18a4cU: { // 49edff80 lea.l -$80(a5), a4
            next = 0x18a50U;
            const auto source_address = r.address[5] - 0x80U;
            r.address[4] = source_address;
            break;
        }
        case 0x18a50U: { // 3b6c00080008 move.w $8(a4), $8(a5)
            next = 0x18a56U;
            const auto source_address = r.address[4] + 0x8U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18a56U: { // 4a2c003f tst.b $3f(a4)
            next = 0x18a5aU;
            const auto destination_address = r.address[4] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x18a5aU: { // 660a bne.b $18a66
            next = 0x18a5cU;
            if ((r.status & 4U) == 0U) { next = 0x18a66U; transfer_kind = 1U; }
            break;
        }
        case 0x18a5cU: { // 6100fc06 bsr.w $18664
            next = 0x18a60U;
            const auto result = m.call(c, 872U, 0x18a5cU, 0x18664U, 0x18a60U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a60U: { // 6100fd66 bsr.w $187c8
            next = 0x18a64U;
            const auto result = m.call(c, 874U, 0x18a60U, 0x187c8U, 0x18a64U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a64U: { // 6012 bra.b $18a78
            next = 0x18a66U;
            if (true) { next = 0x18a78U; transfer_kind = 1U; }
            break;
        }
        case 0x18a66U: { // 3b6c00640064 move.w $64(a4), $64(a5)
            next = 0x18a6cU;
            const auto source_address = r.address[4] + 0x64U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18a6cU: { // 0c6c003c0070 cmpi.w #$3c, $70(a4)
            next = 0x18a72U;
            const auto source_value = 0x3cU;
            const auto destination_address = r.address[4] + 0x70U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18a72U: { // 6a04 bpl.b $18a78
            next = 0x18a74U;
            if ((r.status & 8U) == 0U) { next = 0x18a78U; transfer_kind = 1U; }
            break;
        }
        case 0x18a74U: { // 4255 clr.w (a5)
            next = 0x18a76U;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18a76U: { // 4e75 rts 
            next = 0x18a78U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18a76U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18a78U: { // 6100000a bsr.w $18a84
            next = 0x18a7cU;
            const auto result = m.call(c, 886U, 0x18a78U, 0x18a84U, 0x18a7cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a7cU: { // 4eb900015df2 jsr $15df2.l
            next = 0x18a82U;
            const auto result = m.call(c, 282U, 0x18a7cU, 0x15df2U, 0x18a82U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a82U: { // 4e75 rts 
            next = 0x18a84U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18a82U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18a44U:
        case 0x18a4aU:
        case 0x18a4cU:
        case 0x18a50U:
        case 0x18a56U:
        case 0x18a5aU:
        case 0x18a5cU:
        case 0x18a60U:
        case 0x18a64U:
        case 0x18a66U:
        case 0x18a6cU:
        case 0x18a72U:
        case 0x18a74U:
        case 0x18a76U:
        case 0x18a78U:
        case 0x18a7cU:
        case 0x18a82U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_019098(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19098U: { // 4a6d0060 tst.w $60(a5)
            next = 0x1909cU;
            const auto destination_address = r.address[5] + 0x60U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1909cU: { // 6714 beq.b $190b2
            next = 0x1909eU;
            if ((r.status & 4U) != 0U) { next = 0x190b2U; transfer_kind = 1U; }
            break;
        }
        case 0x1909eU: { // 536d0060 subq.w #$1, $60(a5)
            next = 0x190a2U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x60U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x190a2U: { // 082d00000061 btst.b #$0, $61(a5)
            next = 0x190a8U;
            const auto destination_address = r.address[5] + 0x61U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x190a8U: { // 672e beq.b $190d8
            next = 0x190aaU;
            if ((r.status & 4U) != 0U) { next = 0x190d8U; transfer_kind = 1U; }
            break;
        }
        case 0x190aaU: { // 3b7c0c180008 move.w #$c18, $8(a5)
            next = 0x190b0U;
            const auto value = 0xc18U;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x190b0U: { // 4e75 rts 
            next = 0x190b2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x190b0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x190b2U: { // 0c6d0c0e0008 cmpi.w #$c0e, $8(a5)
            next = 0x190b8U;
            const auto source_value = 0xc0eU;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x190b8U: { // 661e bne.b $190d8
            next = 0x190baU;
            if ((r.status & 4U) == 0U) { next = 0x190d8U; transfer_kind = 1U; }
            break;
        }
        case 0x190baU: { // 4a2d003f tst.b $3f(a5)
            next = 0x190beU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x190beU: { // 6610 bne.b $190d0
            next = 0x190c0U;
            if ((r.status & 4U) == 0U) { next = 0x190d0U; transfer_kind = 1U; }
            break;
        }
        case 0x190c0U: { // 302d006c move.w $6c(a5), d0
            next = 0x190c4U;
            const auto source_address = r.address[5] + 0x6cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x190c4U: { // b06d0046 cmp.w $46(a5), d0
            next = 0x190c8U;
            const auto source_address = r.address[5] + 0x46U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x190c8U: { // 6714 beq.b $190de
            next = 0x190caU;
            if ((r.status & 4U) != 0U) { next = 0x190deU; transfer_kind = 1U; }
            break;
        }
        case 0x190caU: { // 3b6d0046006c move.w $46(a5), $6c(a5)
            next = 0x190d0U;
            const auto source_address = r.address[5] + 0x46U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x190d0U: { // 3b7c0c170008 move.w #$c17, $8(a5)
            next = 0x190d6U;
            const auto value = 0xc17U;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x190d6U: { // 4e75 rts 
            next = 0x190d8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x190d6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x190d8U: { // 3b7c0c0e0008 move.w #$c0e, $8(a5)
            next = 0x190deU;
            const auto value = 0xc0eU;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x190deU: { // 4e75 rts 
            next = 0x190e0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x190deU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19098U:
        case 0x1909cU:
        case 0x1909eU:
        case 0x190a2U:
        case 0x190a8U:
        case 0x190aaU:
        case 0x190b0U:
        case 0x190b2U:
        case 0x190b8U:
        case 0x190baU:
        case 0x190beU:
        case 0x190c0U:
        case 0x190c4U:
        case 0x190c8U:
        case 0x190caU:
        case 0x190d0U:
        case 0x190d6U:
        case 0x190d8U:
        case 0x190deU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
