
FunctionResult cpu_b_stage_019938(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19938U: { // 2b7c000194d60002 move.l #$194d6, $2(a5)
            next = 0x19940U;
            const auto value = 0x194d6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19940U: { // 3b7c00960070 move.w #$96, $70(a5)
            next = 0x19946U;
            const auto value = 0x96U;
            const auto destination_address = r.address[5] + 0x70U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19946U: { // 426d0064 clr.w $64(a5)
            next = 0x1994aU;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1994aU: { // 426d0068 clr.w $68(a5)
            next = 0x1994eU;
            const auto destination_address = r.address[5] + 0x68U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1994eU: { // 52380d1b addq.b #$1, $d1b.w
            next = 0x19952U;
            const auto source_value = 0x1U;
            const auto destination_address = 0xd1bU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x19952U: { // 41fa1ad8 lea.l $1b42c(pc), a0
            next = 0x19956U;
            const auto source_address = 0x1b42cU;
            r.address[0] = source_address;
            break;
        }
        case 0x19956U: { // 7e05 moveq #$5, d7
            next = 0x19958U;
            r.data[7] = 0x5U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x19958U: { // 4df85400 lea.l $5400.w, a6
            next = 0x1995cU;
            const auto source_address = 0x5400U;
            r.address[6] = source_address;
            break;
        }
        case 0x1995cU: { // 3c3c002f move.w #$2f, d6
            next = 0x19960U;
            const auto value = 0x2fU;
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19960U: { // 4a16 tst.b (a6)
            next = 0x19962U;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x19962U: { // 6a0a bpl.b $1996e
            next = 0x19964U;
            if ((r.status & 8U) == 0U) { next = 0x1996eU; transfer_kind = 1U; }
            break;
        }
        case 0x19964U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x19968U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x19968U: { // 51cefff6 dbra d6, $19960
            next = 0x1996cU;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x19960U;
            break;
        }
        case 0x1996cU: { // 4e75 rts 
            next = 0x1996eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1996cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1996eU: { // 1cbc0080 move.b #$80, (a6)
            next = 0x19972U;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19972U: { // 2d7c00019b180002 move.l #$19b18, $2(a6)
            next = 0x1997aU;
            const auto value = 0x19b18U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1997aU: { // 1d7c0000000b move.b #$0, $b(a6)
            next = 0x19980U;
            const auto value = 0x0U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19980U: { // 302d000c move.w $c(a5), d0
            next = 0x19984U;
            const auto source_address = r.address[5] + 0xcU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19984U: { // d058 add.w (a0)+, d0
            next = 0x19986U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19986U: { // 3d40000c move.w d0, $c(a6)
            next = 0x1998aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1998aU: { // 302d000e move.w $e(a5), d0
            next = 0x1998eU;
            const auto source_address = r.address[5] + 0xeU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1998eU: { // d058 add.w (a0)+, d0
            next = 0x19990U;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19990U: { // 3d40000e move.w d0, $e(a6)
            next = 0x19994U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19994U: { // 3d6d001a001a move.w $1a(a5), $1a(a6)
            next = 0x1999aU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1999aU: { // 426e0064 clr.w $64(a6)
            next = 0x1999eU;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1999eU: { // 426e0068 clr.w $68(a6)
            next = 0x199a2U;
            const auto destination_address = r.address[6] + 0x68U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x199a2U: { // 3d58006e move.w (a0)+, $6e(a6)
            next = 0x199a6U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[6] + 0x6eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x199a6U: { // 3d7c00960070 move.w #$96, $70(a6)
            next = 0x199acU;
            const auto value = 0x96U;
            const auto destination_address = r.address[6] + 0x70U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x199acU: { // 4dee0080 lea.l $80(a6), a6
            next = 0x199b0U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x199b0U: { // 51cfffae dbra d7, $19960
            next = 0x199b4U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x19960U;
            break;
        }
        case 0x199b4U: { // 4e75 rts 
            next = 0x199b6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x199b4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19938U:
        case 0x19940U:
        case 0x19946U:
        case 0x1994aU:
        case 0x1994eU:
        case 0x19952U:
        case 0x19956U:
        case 0x19958U:
        case 0x1995cU:
        case 0x19960U:
        case 0x19962U:
        case 0x19964U:
        case 0x19968U:
        case 0x1996cU:
        case 0x1996eU:
        case 0x19972U:
        case 0x1997aU:
        case 0x19980U:
        case 0x19984U:
        case 0x19986U:
        case 0x1998aU:
        case 0x1998eU:
        case 0x19990U:
        case 0x19994U:
        case 0x1999aU:
        case 0x1999eU:
        case 0x199a2U:
        case 0x199a6U:
        case 0x199acU:
        case 0x199b0U:
        case 0x199b4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
