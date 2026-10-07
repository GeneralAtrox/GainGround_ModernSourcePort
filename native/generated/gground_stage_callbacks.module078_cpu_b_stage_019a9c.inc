
FunctionResult cpu_b_stage_019a9c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19a9cU: { // 3c6d0036 movea.w $36(a5), a6
            next = 0x19aa0U;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x19aa0U: { // 4a16 tst.b (a6)
            next = 0x19aa2U;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x19aa2U: { // 6b04 bmi.b $19aa8
            next = 0x19aa4U;
            if ((r.status & 8U) != 0U) { next = 0x19aa8U; transfer_kind = 1U; }
            break;
        }
        case 0x19aa4U: { // 4255 clr.w (a5)
            next = 0x19aa6U;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19aa6U: { // 4e75 rts 
            next = 0x19aa8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19aa6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19aa8U: { // 286e005c movea.l $5c(a6), a4
            next = 0x19aacU;
            const auto source_address = r.address[6] + 0x5cU;
            const auto value = m.lng(source_address);
            r.address[4] = value;
            break;
        }
        case 0x19aacU: { // 5c4c addq.w #$6, a4
            next = 0x19aaeU;
            const auto source_value = 0x6U;
            r.address[4] += source_value;
            break;
        }
        case 0x19aaeU: { // d8ed003a adda.w $3a(a5), a4
            next = 0x19ab2U;
            const auto source_address = r.address[5] + 0x3aU;
            const auto source_value = m.word(source_address);
            r.address[4] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x19ab2U: { // 3b5c0006 move.w (a4)+, $6(a5)
            next = 0x19ab6U;
            const auto source_address = r.address[4];
            const auto value = m.word(source_address);
            r.address[4] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ab6U: { // 6b5a bmi.b $19b12
            next = 0x19ab8U;
            if ((r.status & 8U) != 0U) { next = 0x19b12U; transfer_kind = 1U; }
            break;
        }
        case 0x19ab8U: { // 101c move.b (a4)+, d0
            next = 0x19abaU;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19abaU: { // 4880 ext.w d0
            next = 0x19abcU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19abcU: { // 3200 move.w d0, d1
            next = 0x19abeU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19abeU: { // d06e000c add.w $c(a6), d0
            next = 0x19ac2U;
            const auto source_address = r.address[6] + 0xcU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19ac2U: { // 3b40000c move.w d0, $c(a5)
            next = 0x19ac6U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ac6U: { // 101c move.b (a4)+, d0
            next = 0x19ac8U;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19ac8U: { // 4880 ext.w d0
            next = 0x19acaU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19acaU: { // d06e000e add.w $e(a6), d0
            next = 0x19aceU;
            const auto source_address = r.address[6] + 0xeU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19aceU: { // 3b40000e move.w d0, $e(a5)
            next = 0x19ad2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ad2U: { // 3b6e00080008 move.w $8(a6), $8(a5)
            next = 0x19ad8U;
            const auto source_address = r.address[6] + 0x8U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ad8U: { // 301c move.w (a4)+, d0
            next = 0x19adaU;
            const auto source_address = r.address[4];
            const auto value = m.word(source_address);
            r.address[4] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19adaU: { // 1b400001 move.b d0, $1(a5)
            next = 0x19adeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19adeU: { // 3b6e001a001a move.w $1a(a6), $1a(a5)
            next = 0x19ae4U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19ae4U: { // 4a2e003f tst.b $3f(a6)
            next = 0x19ae8U;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x19ae8U: { // 6708 beq.b $19af2
            next = 0x19aeaU;
            if ((r.status & 4U) != 0U) { next = 0x19af2U; transfer_kind = 1U; }
            break;
        }
        case 0x19aeaU: { // 1b7c0000000b move.b #$0, $b(a5)
            next = 0x19af0U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19af0U: { // 601a bra.b $19b0c
            next = 0x19af2U;
            if (true) { next = 0x19b0cU; transfer_kind = 1U; }
            break;
        }
        case 0x19af2U: { // 422d003f clr.b $3f(a5)
            next = 0x19af6U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x19af6U: { // 3b6d000e0012 move.w $e(a5), $12(a5)
            next = 0x19afcU;
            const auto source_address = r.address[5] + 0xeU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19afcU: { // d27c0018 add.w #$18, d1
            next = 0x19b00U;
            const auto source_value = 0x18U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x19b00U: { // 3b410016 move.w d1, $16(a5)
            next = 0x19b04U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19b04U: { // 41fa1984 lea.l $1b48a(pc), a0
            next = 0x19b08U;
            const auto source_address = 0x1b48aU;
            r.address[0] = source_address;
            break;
        }
        case 0x19b08U: { // 6100f890 bsr.w $1939a
            next = 0x19b0cU;
            const auto result = m.call(c, 904U, 0x19b08U, 0x1939aU, 0x19b0cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19b0cU: { // 4eb900015df2 jsr $15df2.l
            next = 0x19b12U;
            const auto result = m.call(c, 282U, 0x19b0cU, 0x15df2U, 0x19b12U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19b12U: { // 08950000 bclr.b #$0, (a5)
            next = 0x19b16U;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x19b16U: { // 4e75 rts 
            next = 0x19b18U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19b16U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19a9cU:
        case 0x19aa0U:
        case 0x19aa2U:
        case 0x19aa4U:
        case 0x19aa6U:
        case 0x19aa8U:
        case 0x19aacU:
        case 0x19aaeU:
        case 0x19ab2U:
        case 0x19ab6U:
        case 0x19ab8U:
        case 0x19abaU:
        case 0x19abcU:
        case 0x19abeU:
        case 0x19ac2U:
        case 0x19ac6U:
        case 0x19ac8U:
        case 0x19acaU:
        case 0x19aceU:
        case 0x19ad2U:
        case 0x19ad8U:
        case 0x19adaU:
        case 0x19adeU:
        case 0x19ae4U:
        case 0x19ae8U:
        case 0x19aeaU:
        case 0x19af0U:
        case 0x19af2U:
        case 0x19af6U:
        case 0x19afcU:
        case 0x19b00U:
        case 0x19b04U:
        case 0x19b08U:
        case 0x19b0cU:
        case 0x19b12U:
        case 0x19b16U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
