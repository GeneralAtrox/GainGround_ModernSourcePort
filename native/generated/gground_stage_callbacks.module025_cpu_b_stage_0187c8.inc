
FunctionResult cpu_b_stage_0187c8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x187c8U: { // 303c0200 move.w #$200, d0
            next = 0x187ccU;
            const auto value = 0x200U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187ccU: { // 323c0004 move.w #$4, d1
            next = 0x187d0U;
            const auto value = 0x4U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187d0U: { // 4a2d0051 tst.b $51(a5)
            next = 0x187d4U;
            const auto destination_address = r.address[5] + 0x51U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x187d4U: { // 6708 beq.b $187de
            next = 0x187d6U;
            if ((r.status & 4U) != 0U) { next = 0x187deU; transfer_kind = 1U; }
            break;
        }
        case 0x187d6U: { // 302d0054 move.w $54(a5), d0
            next = 0x187daU;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187daU: { // 323c0080 move.w #$80, d1
            next = 0x187deU;
            const auto value = 0x80U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187deU: { // 906d005a sub.w $5a(a5), d0
            next = 0x187e2U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x187e2U: { // 6738 beq.b $1881c
            next = 0x187e4U;
            if ((r.status & 4U) != 0U) { next = 0x1881cU; transfer_kind = 1U; }
            break;
        }
        case 0x187e4U: { // 024007ff andi.w #$7ff, d0
            next = 0x187e8U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x187e8U: { // 0c400400 cmpi.w #$400, d0
            next = 0x187ecU;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x187ecU: { // 6a08 bpl.b $187f6
            next = 0x187eeU;
            if ((r.status & 8U) == 0U) { next = 0x187f6U; transfer_kind = 1U; }
            break;
        }
        case 0x187eeU: { // b041 cmp.w d1, d0
            next = 0x187f0U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x187f0U: { // 6b02 bmi.b $187f4
            next = 0x187f2U;
            if ((r.status & 8U) != 0U) { next = 0x187f4U; transfer_kind = 1U; }
            break;
        }
        case 0x187f2U: { // 3001 move.w d1, d0
            next = 0x187f4U;
            const auto value = r.data[1];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x187f4U: { // 600c bra.b $18802
            next = 0x187f6U;
            if (true) { next = 0x18802U; transfer_kind = 1U; }
            break;
        }
        case 0x187f6U: { // 04400800 subi.w #$800, d0
            next = 0x187faU;
            const auto source_value = 0x800U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x187faU: { // 4441 neg.w d1
            next = 0x187fcU;
            const auto old = r.data[1];
            const auto value = m.sub(0U, old, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x187fcU: { // b041 cmp.w d1, d0
            next = 0x187feU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x187feU: { // 6a02 bpl.b $18802
            next = 0x18800U;
            if ((r.status & 8U) == 0U) { next = 0x18802U; transfer_kind = 1U; }
            break;
        }
        case 0x18800U: { // 3001 move.w d1, d0
            next = 0x18802U;
            const auto value = r.data[1];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18802U: { // d06d005a add.w $5a(a5), d0
            next = 0x18806U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18806U: { // 024007ff andi.w #$7ff, d0
            next = 0x1880aU;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1880aU: { // 3b40005a move.w d0, $5a(a5)
            next = 0x1880eU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1880eU: { // 06400080 addi.w #$80, d0
            next = 0x18812U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18812U: { // 02400700 andi.w #$700, d0
            next = 0x18816U;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18816U: { // ec48 lsr.w #$6, d0
            next = 0x18818U;
            m.shift_word(0U, 6U, false, false);
            break;
        }
        case 0x18818U: { // 3b400064 move.w d0, $64(a5)
            next = 0x1881cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1881cU: { // 4e75 rts 
            next = 0x1881eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1881cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x187c8U:
        case 0x187ccU:
        case 0x187d0U:
        case 0x187d4U:
        case 0x187d6U:
        case 0x187daU:
        case 0x187deU:
        case 0x187e2U:
        case 0x187e4U:
        case 0x187e8U:
        case 0x187ecU:
        case 0x187eeU:
        case 0x187f0U:
        case 0x187f2U:
        case 0x187f4U:
        case 0x187f6U:
        case 0x187faU:
        case 0x187fcU:
        case 0x187feU:
        case 0x18800U:
        case 0x18802U:
        case 0x18806U:
        case 0x1880aU:
        case 0x1880eU:
        case 0x18812U:
        case 0x18816U:
        case 0x18818U:
        case 0x1881cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
