
FunctionResult cpu_b_stage_01a7c6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a7c6U: { // 302d0012 move.w $12(a5), d0
            next = 0x1a7caU;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a7caU: { // 0c40ffe0 cmpi.w #$ffe0, d0
            next = 0x1a7ceU;
            const auto source_value = 0xffe0U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a7ceU: { // 6b16 bmi.b $1a7e6
            next = 0x1a7d0U;
            if ((r.status & 8U) != 0U) { next = 0x1a7e6U; transfer_kind = 1U; }
            break;
        }
        case 0x1a7d0U: { // 0c4001a0 cmpi.w #$1a0, d0
            next = 0x1a7d4U;
            const auto source_value = 0x1a0U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a7d4U: { // 6e10 bgt.b $1a7e6
            next = 0x1a7d6U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a7e6U; transfer_kind = 1U; }
            break;
        }
        case 0x1a7d6U: { // 302d001a move.w $1a(a5), d0
            next = 0x1a7daU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a7daU: { // 0c40ffe0 cmpi.w #$ffe0, d0
            next = 0x1a7deU;
            const auto source_value = 0xffe0U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a7deU: { // 6b06 bmi.b $1a7e6
            next = 0x1a7e0U;
            if ((r.status & 8U) != 0U) { next = 0x1a7e6U; transfer_kind = 1U; }
            break;
        }
        case 0x1a7e0U: { // 0c400210 cmpi.w #$210, d0
            next = 0x1a7e4U;
            const auto source_value = 0x210U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a7e4U: { // 6b06 bmi.b $1a7ec
            next = 0x1a7e6U;
            if ((r.status & 8U) != 0U) { next = 0x1a7ecU; transfer_kind = 1U; }
            break;
        }
        case 0x1a7e6U: { // 584f addq.w #$4, a7
            next = 0x1a7e8U;
            const auto source_value = 0x4U;
            r.address[7] += source_value;
            break;
        }
        case 0x1a7e8U: { // 4215 clr.b (a5)
            next = 0x1a7eaU;
            const auto destination_address = r.address[5];
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1a7eaU: { // 4e75 rts 
            next = 0x1a7ecU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a7eaU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a7ecU: { // 302d0012 move.w $12(a5), d0
            next = 0x1a7f0U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a7f0U: { // 322d001a move.w $1a(a5), d1
            next = 0x1a7f4U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a7f4U: { // 342d002e move.w $2e(a5), d2
            next = 0x1a7f8U;
            const auto source_address = r.address[5] + 0x2eU;
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a7f8U: { // d442 add.w d2, d2
            next = 0x1a7faU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1a7faU: { // 4eb900015ede jsr $15ede.l
            next = 0x1a800U;
            const auto result = m.call(c, 287U, 0x1a7faU, 0x15edeU, 0x1a800U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a800U: { // 1610 move.b (a0), d3
            next = 0x1a802U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            m.db(3U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a802U: { // 4883 ext.w d3
            next = 0x1a804U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[3])));
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a804U: { // b443 cmp.w d3, d2
            next = 0x1a806U;
            const auto source_value = r.data[3];
            const auto destination_value = r.data[2];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a806U: { // 6b06 bmi.b $1a80e
            next = 0x1a808U;
            if ((r.status & 8U) != 0U) { next = 0x1a80eU; transfer_kind = 1U; }
            break;
        }
        case 0x1a808U: { // 44fc0000 move.w #$0, ccr
            next = 0x1a80cU;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
        case 0x1a80cU: { // 4e75 rts 
            next = 0x1a80eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a80cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a80eU: { // 44fc0001 move.w #$1, ccr
            next = 0x1a812U;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x1U & 0x1fU));
            break;
        }
        case 0x1a812U: { // 4e75 rts 
            next = 0x1a814U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a812U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a7c6U:
        case 0x1a7caU:
        case 0x1a7ceU:
        case 0x1a7d0U:
        case 0x1a7d4U:
        case 0x1a7d6U:
        case 0x1a7daU:
        case 0x1a7deU:
        case 0x1a7e0U:
        case 0x1a7e4U:
        case 0x1a7e6U:
        case 0x1a7e8U:
        case 0x1a7eaU:
        case 0x1a7ecU:
        case 0x1a7f0U:
        case 0x1a7f4U:
        case 0x1a7f8U:
        case 0x1a7faU:
        case 0x1a800U:
        case 0x1a802U:
        case 0x1a804U:
        case 0x1a806U:
        case 0x1a808U:
        case 0x1a80cU:
        case 0x1a80eU:
        case 0x1a812U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
