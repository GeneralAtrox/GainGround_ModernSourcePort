
FunctionResult cpu_b_stage_01a6b6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a6b6U: { // 47f86c00 lea.l $6c00.w, a3
            next = 0x1a6baU;
            const auto source_address = 0x6c00U;
            r.address[3] = source_address;
            break;
        }
        case 0x1a6baU: { // 4a5b tst.w (a3)+
            next = 0x1a6bcU;
            const auto destination_address = r.address[3];
            const auto value = m.word(destination_address);
            r.address[3] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0x1a6bcU: { // 6b04 bmi.b $1a6c2
            next = 0x1a6beU;
            if ((r.status & 8U) != 0U) { next = 0x1a6c2U; transfer_kind = 1U; }
            break;
        }
        case 0x1a6beU: { // 47f87002 lea.l $7002.w, a3
            next = 0x1a6c2U;
            const auto source_address = 0x7002U;
            r.address[3] = source_address;
            break;
        }
        case 0x1a6c2U: { // 302d001a move.w $1a(a5), d0
            next = 0x1a6c6U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a6c6U: { // 0c400200 cmpi.w #$200, d0
            next = 0x1a6caU;
            const auto source_value = 0x200U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a6caU: { // 64000068 bcc.w $1a734
            next = 0x1a6ceU;
            if ((r.status & 1U) == 0U) { next = 0x1a734U; transfer_kind = 1U; }
            break;
        }
        case 0x1a6ceU: { // 3e00 move.w d0, d7
            next = 0x1a6d0U;
            const auto value = r.data[0];
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a6d0U: { // 06470020 addi.w #$20, d7
            next = 0x1a6d4U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[7];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x1a6d4U: { // 04400020 subi.w #$20, d0
            next = 0x1a6d8U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a6d8U: { // 6b10 bmi.b $1a6ea
            next = 0x1a6daU;
            if ((r.status & 8U) != 0U) { next = 0x1a6eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1a6daU: { // 0c4701ff cmpi.w #$1ff, d7
            next = 0x1a6deU;
            const auto source_value = 0x1ffU;
            const auto destination_value = r.data[7];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a6deU: { // 6f04 ble.b $1a6e4
            next = 0x1a6e0U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1a6e4U; transfer_kind = 1U; }
            break;
        }
        case 0x1a6e0U: { // 3e3c01ff move.w #$1ff, d7
            next = 0x1a6e4U;
            const auto value = 0x1ffU;
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a6e4U: { // 9e40 sub.w d0, d7
            next = 0x1a6e6U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[7];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x1a6e6U: { // d040 add.w d0, d0
            next = 0x1a6e8U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a6e8U: { // d6c0 adda.w d0, a3
            next = 0x1a6eaU;
            const auto source_value = r.data[0];
            r.address[3] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1a6eaU: { // 301b move.w (a3)+, d0
            next = 0x1a6ecU;
            const auto source_address = r.address[3];
            const auto value = m.word(source_address);
            r.address[3] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a6ecU: { // 6742 beq.b $1a730
            next = 0x1a6eeU;
            if ((r.status & 4U) != 0U) { next = 0x1a730U; transfer_kind = 1U; }
            break;
        }
        case 0x1a6eeU: { // 3c40 movea.w d0, a6
            next = 0x1a6f0U;
            const auto value = r.data[0];
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x1a6f0U: { // 7000 moveq #$0, d0
            next = 0x1a6f2U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1a6f2U: { // 0c2e000a000b cmpi.b #$a, $b(a6)
            next = 0x1a6f8U;
            const auto source_value = 0xaU;
            const auto destination_address = r.address[6] + 0xbU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1a6f8U: { // 6636 bne.b $1a730
            next = 0x1a6faU;
            if ((r.status & 4U) == 0U) { next = 0x1a730U; transfer_kind = 1U; }
            break;
        }
        case 0x1a6faU: { // 4a2e003f tst.b $3f(a6)
            next = 0x1a6feU;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1a6feU: { // 6630 bne.b $1a730
            next = 0x1a700U;
            if ((r.status & 4U) == 0U) { next = 0x1a730U; transfer_kind = 1U; }
            break;
        }
        case 0x1a700U: { // 302d0012 move.w $12(a5), d0
            next = 0x1a704U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a704U: { // b06e002a cmp.w $2a(a6), d0
            next = 0x1a708U;
            const auto source_address = r.address[6] + 0x2aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a708U: { // 6b26 bmi.b $1a730
            next = 0x1a70aU;
            if ((r.status & 8U) != 0U) { next = 0x1a730U; transfer_kind = 1U; }
            break;
        }
        case 0x1a70aU: { // b06e002c cmp.w $2c(a6), d0
            next = 0x1a70eU;
            const auto source_address = r.address[6] + 0x2cU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a70eU: { // 6e20 bgt.b $1a730
            next = 0x1a710U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a730U; transfer_kind = 1U; }
            break;
        }
        case 0x1a710U: { // 302d001a move.w $1a(a5), d0
            next = 0x1a714U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a714U: { // b06e0032 cmp.w $32(a6), d0
            next = 0x1a718U;
            const auto source_address = r.address[6] + 0x32U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a718U: { // 6b16 bmi.b $1a730
            next = 0x1a71aU;
            if ((r.status & 8U) != 0U) { next = 0x1a730U; transfer_kind = 1U; }
            break;
        }
        case 0x1a71aU: { // b06e0034 cmp.w $34(a6), d0
            next = 0x1a71eU;
            const auto source_address = r.address[6] + 0x34U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a71eU: { // 6e10 bgt.b $1a730
            next = 0x1a720U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a730U; transfer_kind = 1U; }
            break;
        }
        case 0x1a720U: { // 556e0046 subq.w #$2, $46(a6)
            next = 0x1a724U;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[6] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a724U: { // 6e04 bgt.b $1a72a
            next = 0x1a726U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a72aU; transfer_kind = 1U; }
            break;
        }
        case 0x1a726U: { // 4aee003f tas.b $3f(a6)
            next = 0x1a72aU;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x1a72aU: { // 44fc0001 move.w #$1, ccr
            next = 0x1a72eU;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x1U & 0x1fU));
            break;
        }
        case 0x1a72eU: { // 4e75 rts 
            next = 0x1a730U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a72eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a730U: { // 51cfffb8 dbra d7, $1a6ea
            next = 0x1a734U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x1a6eaU;
            break;
        }
        case 0x1a734U: { // 44fc0000 move.w #$0, ccr
            next = 0x1a738U;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
        case 0x1a738U: { // 4e75 rts 
            next = 0x1a73aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a738U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a6b6U:
        case 0x1a6baU:
        case 0x1a6bcU:
        case 0x1a6beU:
        case 0x1a6c2U:
        case 0x1a6c6U:
        case 0x1a6caU:
        case 0x1a6ceU:
        case 0x1a6d0U:
        case 0x1a6d4U:
        case 0x1a6d8U:
        case 0x1a6daU:
        case 0x1a6deU:
        case 0x1a6e0U:
        case 0x1a6e4U:
        case 0x1a6e6U:
        case 0x1a6e8U:
        case 0x1a6eaU:
        case 0x1a6ecU:
        case 0x1a6eeU:
        case 0x1a6f0U:
        case 0x1a6f2U:
        case 0x1a6f8U:
        case 0x1a6faU:
        case 0x1a6feU:
        case 0x1a700U:
        case 0x1a704U:
        case 0x1a708U:
        case 0x1a70aU:
        case 0x1a70eU:
        case 0x1a710U:
        case 0x1a714U:
        case 0x1a718U:
        case 0x1a71aU:
        case 0x1a71eU:
        case 0x1a720U:
        case 0x1a724U:
        case 0x1a726U:
        case 0x1a72aU:
        case 0x1a72eU:
        case 0x1a730U:
        case 0x1a734U:
        case 0x1a738U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
