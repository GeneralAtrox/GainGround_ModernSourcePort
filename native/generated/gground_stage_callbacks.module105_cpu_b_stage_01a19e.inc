
FunctionResult cpu_b_stage_01a19e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a19eU: { // 536d0070 subq.w #$1, $70(a5)
            next = 0x1a1a2U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a1a2U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x1a1a6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a1a6U: { // 6e46 bgt.b $1a1ee
            next = 0x1a1a8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a1eeU; transfer_kind = 1U; }
            break;
        }
        case 0x1a1a8U: { // 303c0014 move.w #$14, d0
            next = 0x1a1acU;
            const auto value = 0x14U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a1acU: { // 4eb900016ff8 jsr $16ff8.l
            next = 0x1a1b2U;
            const auto result = m.call(c, 308U, 0x1a1acU, 0x16ff8U, 0x1a1b2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a1b2U: { // 3b6d00660068 move.w $66(a5), $68(a5)
            next = 0x1a1b8U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a1b8U: { // 302d0064 move.w $64(a5), d0
            next = 0x1a1bcU;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a1bcU: { // 5240 addq.w #$1, d0
            next = 0x1a1beU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a1beU: { // b06d006e cmp.w $6e(a5), d0
            next = 0x1a1c2U;
            const auto source_address = r.address[5] + 0x6eU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a1c2U: { // 6b0c bmi.b $1a1d0
            next = 0x1a1c4U;
            if ((r.status & 8U) != 0U) { next = 0x1a1d0U; transfer_kind = 1U; }
            break;
        }
        case 0x1a1c4U: { // 7000 moveq #$0, d0
            next = 0x1a1c6U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1a1c6U: { // 4a6d0070 tst.w $70(a5)
            next = 0x1a1caU;
            const auto destination_address = r.address[5] + 0x70U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1a1caU: { // 6e04 bgt.b $1a1d0
            next = 0x1a1ccU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a1d0U; transfer_kind = 1U; }
            break;
        }
        case 0x1a1ccU: { // 4255 clr.w (a5)
            next = 0x1a1ceU;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a1ceU: { // 4e75 rts 
            next = 0x1a1d0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a1ceU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a1d0U: { // 3b400064 move.w d0, $64(a5)
            next = 0x1a1d4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a1d4U: { // d040 add.w d0, d0
            next = 0x1a1d6U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a1d6U: { // 3200 move.w d0, d1
            next = 0x1a1d8U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a1d8U: { // d040 add.w d0, d0
            next = 0x1a1daU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a1daU: { // d041 add.w d1, d0
            next = 0x1a1dcU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a1dcU: { // 206d005c movea.l $5c(a5), a0
            next = 0x1a1e0U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1a1e0U: { // d0c0 adda.w d0, a0
            next = 0x1a1e2U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1a1e2U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1a1e6U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a1e6U: { // 3b580008 move.w (a0)+, $8(a5)
            next = 0x1a1eaU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a1eaU: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x1a1eeU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a1eeU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a1f4U;
            const auto result = m.call(c, 282U, 0x1a1eeU, 0x15df2U, 0x1a1f4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a1f4U: { // 4e75 rts 
            next = 0x1a1f6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a1f4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a19eU:
        case 0x1a1a2U:
        case 0x1a1a6U:
        case 0x1a1a8U:
        case 0x1a1acU:
        case 0x1a1b2U:
        case 0x1a1b8U:
        case 0x1a1bcU:
        case 0x1a1beU:
        case 0x1a1c2U:
        case 0x1a1c4U:
        case 0x1a1c6U:
        case 0x1a1caU:
        case 0x1a1ccU:
        case 0x1a1ceU:
        case 0x1a1d0U:
        case 0x1a1d4U:
        case 0x1a1d6U:
        case 0x1a1d8U:
        case 0x1a1daU:
        case 0x1a1dcU:
        case 0x1a1e0U:
        case 0x1a1e2U:
        case 0x1a1e6U:
        case 0x1a1eaU:
        case 0x1a1eeU:
        case 0x1a1f4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
