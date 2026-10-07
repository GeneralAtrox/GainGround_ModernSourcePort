
FunctionResult cpu_b_stage_01a0e0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a0e0U: { // 2b7c00019c460002 move.l #$19c46, $2(a5)
            next = 0x1a0e8U;
            const auto value = 0x19c46U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a0e8U: { // 3b7c012c0070 move.w #$12c, $70(a5)
            next = 0x1a0eeU;
            const auto value = 0x12cU;
            const auto destination_address = r.address[5] + 0x70U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a0eeU: { // 3b7c00170058 move.w #$17, $58(a5)
            next = 0x1a0f4U;
            const auto value = 0x17U;
            const auto destination_address = r.address[5] + 0x58U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a0f4U: { // 3b7c00050068 move.w #$5, $68(a5)
            next = 0x1a0faU;
            const auto value = 0x5U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a0faU: { // 4af80d1b tas.b $d1b.w
            next = 0x1a0feU;
            const auto destination_address = 0xd1bU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x1a0feU: { // 41fa16c2 lea.l $1b7c2(pc), a0
            next = 0x1a102U;
            const auto source_address = 0x1b7c2U;
            r.address[0] = source_address;
            break;
        }
        case 0x1a102U: { // 7e0a moveq #$a, d7
            next = 0x1a104U;
            r.data[7] = 0xaU;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x1a104U: { // 4df85400 lea.l $5400.w, a6
            next = 0x1a108U;
            const auto source_address = 0x5400U;
            r.address[6] = source_address;
            break;
        }
        case 0x1a108U: { // 3c3c002f move.w #$2f, d6
            next = 0x1a10cU;
            const auto value = 0x2fU;
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a10cU: { // 4a16 tst.b (a6)
            next = 0x1a10eU;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1a10eU: { // 6a0a bpl.b $1a11a
            next = 0x1a110U;
            if ((r.status & 8U) == 0U) { next = 0x1a11aU; transfer_kind = 1U; }
            break;
        }
        case 0x1a110U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x1a114U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x1a114U: { // 51cefff6 dbra d6, $1a10c
            next = 0x1a118U;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x1a10cU;
            break;
        }
        case 0x1a118U: { // 4e75 rts 
            next = 0x1a11aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a118U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a11aU: { // 1cbc0080 move.b #$80, (a6)
            next = 0x1a11eU;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a11eU: { // 2d7c0001a19e0002 move.l #$1a19e, $2(a6)
            next = 0x1a126U;
            const auto value = 0x1a19eU;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a126U: { // 1d7c0000000b move.b #$0, $b(a6)
            next = 0x1a12cU;
            const auto value = 0x0U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a12cU: { // 302d000c move.w $c(a5), d0
            next = 0x1a130U;
            const auto source_address = r.address[5] + 0xcU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a130U: { // d058 add.w (a0)+, d0
            next = 0x1a132U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a132U: { // 3d40000c move.w d0, $c(a6)
            next = 0x1a136U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a136U: { // 302d000e move.w $e(a5), d0
            next = 0x1a13aU;
            const auto source_address = r.address[5] + 0xeU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a13aU: { // d058 add.w (a0)+, d0
            next = 0x1a13cU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a13cU: { // 3d40000e move.w d0, $e(a6)
            next = 0x1a140U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a140U: { // 3d6d001a001a move.w $1a(a5), $1a(a6)
            next = 0x1a146U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a146U: { // 426e0064 clr.w $64(a6)
            next = 0x1a14aU;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a14aU: { // 426e0068 clr.w $68(a6)
            next = 0x1a14eU;
            const auto destination_address = r.address[6] + 0x68U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a14eU: { // 3d58006e move.w (a0)+, $6e(a6)
            next = 0x1a152U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[6] + 0x6eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a152U: { // 3d580066 move.w (a0)+, $66(a6)
            next = 0x1a156U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[6] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a156U: { // 2d58005c move.l (a0)+, $5c(a6)
            next = 0x1a15aU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6] + 0x5cU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a15aU: { // 3d7c00d20070 move.w #$d2, $70(a6)
            next = 0x1a160U;
            const auto value = 0xd2U;
            const auto destination_address = r.address[6] + 0x70U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a160U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x1a164U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x1a164U: { // 51cfffa6 dbra d7, $1a10c
            next = 0x1a168U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x1a10cU;
            break;
        }
        case 0x1a168U: { // 4e75 rts 
            next = 0x1a16aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a168U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a0e0U:
        case 0x1a0e8U:
        case 0x1a0eeU:
        case 0x1a0f4U:
        case 0x1a0faU:
        case 0x1a0feU:
        case 0x1a102U:
        case 0x1a104U:
        case 0x1a108U:
        case 0x1a10cU:
        case 0x1a10eU:
        case 0x1a110U:
        case 0x1a114U:
        case 0x1a118U:
        case 0x1a11aU:
        case 0x1a11eU:
        case 0x1a126U:
        case 0x1a12cU:
        case 0x1a130U:
        case 0x1a132U:
        case 0x1a136U:
        case 0x1a13aU:
        case 0x1a13cU:
        case 0x1a140U:
        case 0x1a146U:
        case 0x1a14aU:
        case 0x1a14eU:
        case 0x1a152U:
        case 0x1a156U:
        case 0x1a15aU:
        case 0x1a160U:
        case 0x1a164U:
        case 0x1a168U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
