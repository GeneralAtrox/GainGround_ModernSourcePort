
FunctionResult cpu_b_stage_018ab0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18ab0U: { // 2b7c000189d20002 move.l #$189d2, $2(a5)
            next = 0x18ab8U;
            const auto value = 0x189d2U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x18ab8U: { // 3b6d006a0008 move.w $6a(a5), $8(a5)
            next = 0x18abeU;
            const auto source_address = r.address[5] + 0x6aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18abeU: { // 3b7c00b40070 move.w #$b4, $70(a5)
            next = 0x18ac4U;
            const auto value = 0xb4U;
            const auto destination_address = r.address[5] + 0x70U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ac4U: { // 3b7c00080066 move.w #$8, $66(a5)
            next = 0x18acaU;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18acaU: { // 426d0068 clr.w $68(a5)
            next = 0x18aceU;
            const auto destination_address = r.address[5] + 0x68U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18aceU: { // 4af80d1b tas.b $d1b.w
            next = 0x18ad2U;
            const auto destination_address = 0xd1bU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x18ad2U: { // 41fa21ca lea.l $1ac9e(pc), a0
            next = 0x18ad6U;
            const auto source_address = 0x1ac9eU;
            r.address[0] = source_address;
            break;
        }
        case 0x18ad6U: { // 7e05 moveq #$5, d7
            next = 0x18ad8U;
            r.data[7] = 0x5U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x18ad8U: { // 4df85400 lea.l $5400.w, a6
            next = 0x18adcU;
            const auto source_address = 0x5400U;
            r.address[6] = source_address;
            break;
        }
        case 0x18adcU: { // 3c3c002f move.w #$2f, d6
            next = 0x18ae0U;
            const auto value = 0x2fU;
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ae0U: { // 4a16 tst.b (a6)
            next = 0x18ae2U;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x18ae2U: { // 6a0a bpl.b $18aee
            next = 0x18ae4U;
            if ((r.status & 8U) == 0U) { next = 0x18aeeU; transfer_kind = 1U; }
            break;
        }
        case 0x18ae4U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x18ae8U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x18ae8U: { // 51cefff6 dbra d6, $18ae0
            next = 0x18aecU;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x18ae0U;
            break;
        }
        case 0x18aecU: { // 4e75 rts 
            next = 0x18aeeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18aecU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18aeeU: { // 1cbc0080 move.b #$80, (a6)
            next = 0x18af2U;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18af2U: { // 2d7c0001891c0002 move.l #$1891c, $2(a6)
            next = 0x18afaU;
            const auto value = 0x1891cU;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x18afaU: { // 1d7c0000000b move.b #$0, $b(a6)
            next = 0x18b00U;
            const auto value = 0x0U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18b00U: { // 302d000c move.w $c(a5), d0
            next = 0x18b04U;
            const auto source_address = r.address[5] + 0xcU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b04U: { // d058 add.w (a0)+, d0
            next = 0x18b06U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18b06U: { // 3d40000c move.w d0, $c(a6)
            next = 0x18b0aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b0aU: { // 302d000e move.w $e(a5), d0
            next = 0x18b0eU;
            const auto source_address = r.address[5] + 0xeU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b0eU: { // d058 add.w (a0)+, d0
            next = 0x18b10U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18b10U: { // 3d40000e move.w d0, $e(a6)
            next = 0x18b14U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b14U: { // 3d6d001a001a move.w $1a(a5), $1a(a6)
            next = 0x18b1aU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b1aU: { // 426e0064 clr.w $64(a6)
            next = 0x18b1eU;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18b1eU: { // 426e0068 clr.w $68(a6)
            next = 0x18b22U;
            const auto destination_address = r.address[6] + 0x68U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18b22U: { // 3d58006e move.w (a0)+, $6e(a6)
            next = 0x18b26U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[6] + 0x6eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b26U: { // 3d7c00960070 move.w #$96, $70(a6)
            next = 0x18b2cU;
            const auto value = 0x96U;
            const auto destination_address = r.address[6] + 0x70U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b2cU: { // 4dee0080 lea.l $80(a6), a6
            next = 0x18b30U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x18b30U: { // 51cfffae dbra d7, $18ae0
            next = 0x18b34U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x18ae0U;
            break;
        }
        case 0x18b34U: { // 4e75 rts 
            next = 0x18b36U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18b34U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18ab0U:
        case 0x18ab8U:
        case 0x18abeU:
        case 0x18ac4U:
        case 0x18acaU:
        case 0x18aceU:
        case 0x18ad2U:
        case 0x18ad6U:
        case 0x18ad8U:
        case 0x18adcU:
        case 0x18ae0U:
        case 0x18ae2U:
        case 0x18ae4U:
        case 0x18ae8U:
        case 0x18aecU:
        case 0x18aeeU:
        case 0x18af2U:
        case 0x18afaU:
        case 0x18b00U:
        case 0x18b04U:
        case 0x18b06U:
        case 0x18b0aU:
        case 0x18b0eU:
        case 0x18b10U:
        case 0x18b14U:
        case 0x18b1aU:
        case 0x18b1eU:
        case 0x18b22U:
        case 0x18b26U:
        case 0x18b2cU:
        case 0x18b30U:
        case 0x18b34U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
