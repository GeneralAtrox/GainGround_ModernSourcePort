
FunctionResult cpu_b_stage_018858(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18858U: { // 2b7c000185420002 move.l #$18542, $2(a5)
            next = 0x18860U;
            const auto value = 0x18542U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x18860U: { // 3b6d006a0008 move.w $6a(a5), $8(a5)
            next = 0x18866U;
            const auto source_address = r.address[5] + 0x6aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18866U: { // 3b7c005a0070 move.w #$5a, $70(a5)
            next = 0x1886cU;
            const auto value = 0x5aU;
            const auto destination_address = r.address[5] + 0x70U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1886cU: { // 4aed0066 tas.b $66(a5)
            next = 0x18870U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x18870U: { // 426d0068 clr.w $68(a5)
            next = 0x18874U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18874U: { // 5b380d19 subq.b #$5, $d19.w
            next = 0x18878U;
            const auto source_value = 0x5U;
            const auto destination_address = 0xd19U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x18878U: { // 41fa2406 lea.l $1ac80(pc), a0
            next = 0x1887cU;
            const auto source_address = 0x1ac80U;
            r.address[0] = source_address;
            break;
        }
        case 0x1887cU: { // 7e04 moveq #$4, d7
            next = 0x1887eU;
            r.data[7] = 0x4U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x1887eU: { // 4df85400 lea.l $5400.w, a6
            next = 0x18882U;
            const auto source_address = 0x5400U;
            r.address[6] = source_address;
            break;
        }
        case 0x18882U: { // 3c3c002f move.w #$2f, d6
            next = 0x18886U;
            const auto value = 0x2fU;
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18886U: { // 4a16 tst.b (a6)
            next = 0x18888U;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x18888U: { // 6a0a bpl.b $18894
            next = 0x1888aU;
            if ((r.status & 8U) == 0U) { next = 0x18894U; transfer_kind = 1U; }
            break;
        }
        case 0x1888aU: { // 4dee0080 lea.l $80(a6), a6
            next = 0x1888eU;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x1888eU: { // 51cefff6 dbra d6, $18886
            next = 0x18892U;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x18886U;
            break;
        }
        case 0x18892U: { // 4e75 rts 
            next = 0x18894U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18892U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18894U: { // 1cbc0080 move.b #$80, (a6)
            next = 0x18898U;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18898U: { // 2d7c0001891c0002 move.l #$1891c, $2(a6)
            next = 0x188a0U;
            const auto value = 0x1891cU;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x188a0U: { // 1d7c0000000b move.b #$0, $b(a6)
            next = 0x188a6U;
            const auto value = 0x0U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x188a6U: { // 302d000c move.w $c(a5), d0
            next = 0x188aaU;
            const auto source_address = r.address[5] + 0xcU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188aaU: { // d058 add.w (a0)+, d0
            next = 0x188acU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x188acU: { // 3d40000c move.w d0, $c(a6)
            next = 0x188b0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188b0U: { // 302d000e move.w $e(a5), d0
            next = 0x188b4U;
            const auto source_address = r.address[5] + 0xeU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188b4U: { // d058 add.w (a0)+, d0
            next = 0x188b6U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x188b6U: { // 3d40000e move.w d0, $e(a6)
            next = 0x188baU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188baU: { // 3d6d001a001a move.w $1a(a5), $1a(a6)
            next = 0x188c0U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188c0U: { // 426e0064 clr.w $64(a6)
            next = 0x188c4U;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x188c4U: { // 426e0068 clr.w $68(a6)
            next = 0x188c8U;
            const auto destination_address = r.address[6] + 0x68U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x188c8U: { // 3d58006e move.w (a0)+, $6e(a6)
            next = 0x188ccU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[6] + 0x6eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188ccU: { // 3d7c004b0070 move.w #$4b, $70(a6)
            next = 0x188d2U;
            const auto value = 0x4bU;
            const auto destination_address = r.address[6] + 0x70U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188d2U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x188d6U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x188d6U: { // 51cfffae dbra d7, $18886
            next = 0x188daU;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x18886U;
            break;
        }
        case 0x188daU: { // 4e75 rts 
            next = 0x188dcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x188daU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18858U:
        case 0x18860U:
        case 0x18866U:
        case 0x1886cU:
        case 0x18870U:
        case 0x18874U:
        case 0x18878U:
        case 0x1887cU:
        case 0x1887eU:
        case 0x18882U:
        case 0x18886U:
        case 0x18888U:
        case 0x1888aU:
        case 0x1888eU:
        case 0x18892U:
        case 0x18894U:
        case 0x18898U:
        case 0x188a0U:
        case 0x188a6U:
        case 0x188aaU:
        case 0x188acU:
        case 0x188b0U:
        case 0x188b4U:
        case 0x188b6U:
        case 0x188baU:
        case 0x188c0U:
        case 0x188c4U:
        case 0x188c8U:
        case 0x188ccU:
        case 0x188d2U:
        case 0x188d6U:
        case 0x188daU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
