
FunctionResult cpu_b_stage_0199ea(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x199eaU: { // 302d006a move.w $6a(a5), d0
            next = 0x199eeU;
            const auto source_address = r.address[5] + 0x6aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x199eeU: { // b06d0008 cmp.w $8(a5), d0
            next = 0x199f2U;
            const auto source_address = r.address[5] + 0x8U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x199f2U: { // 660e bne.b $19a02
            next = 0x199f4U;
            if ((r.status & 4U) == 0U) { next = 0x19a02U; transfer_kind = 1U; }
            break;
        }
        case 0x199f4U: { // 303c0c3a move.w #$c3a, d0
            next = 0x199f8U;
            const auto value = 0xc3aU;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x199f8U: { // 322d006c move.w $6c(a5), d1
            next = 0x199fcU;
            const auto source_address = r.address[5] + 0x6cU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x199fcU: { // b26d0046 cmp.w $46(a5), d1
            next = 0x19a00U;
            const auto source_address = r.address[5] + 0x46U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19a00U: { // 670a beq.b $19a0c
            next = 0x19a02U;
            if ((r.status & 4U) != 0U) { next = 0x19a0cU; transfer_kind = 1U; }
            break;
        }
        case 0x19a02U: { // 3b6d0046006c move.w $46(a5), $6c(a5)
            next = 0x19a08U;
            const auto source_address = r.address[5] + 0x46U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a08U: { // 3b400008 move.w d0, $8(a5)
            next = 0x19a0cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19a0cU: { // 4e75 rts 
            next = 0x19a0eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19a0cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x199eaU:
        case 0x199eeU:
        case 0x199f2U:
        case 0x199f4U:
        case 0x199f8U:
        case 0x199fcU:
        case 0x19a00U:
        case 0x19a02U:
        case 0x19a08U:
        case 0x19a0cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0194d6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x194d6U: { // 536d0070 subq.w #$1, $70(a5)
            next = 0x194daU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x194daU: { // 6e10 bgt.b $194ec
            next = 0x194dcU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x194ecU; transfer_kind = 1U; }
            break;
        }
        case 0x194dcU: { // 4255 clr.w (a5)
            next = 0x194deU;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x194deU: { // 6100f42a bsr.w $1890a
            next = 0x194e2U;
            const auto result = m.call(c, 880U, 0x194deU, 0x1890aU, 0x194e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x194e2U: { // 53380d1b subq.b #$1, $d1b.w
            next = 0x194e6U;
            const auto source_value = 0x1U;
            const auto destination_address = 0xd1bU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x194e6U: { // 52380d2d addq.b #$1, $d2d.w
            next = 0x194eaU;
            const auto source_value = 0x1U;
            const auto destination_address = 0xd2dU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x194eaU: { // 4e75 rts 
            next = 0x194ecU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x194eaU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x194ecU: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x194f0U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x194f0U: { // 6e28 bgt.b $1951a
            next = 0x194f2U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1951aU; transfer_kind = 1U; }
            break;
        }
        case 0x194f2U: { // 303c0014 move.w #$14, d0
            next = 0x194f6U;
            const auto value = 0x14U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x194f6U: { // 4eb900016ff8 jsr $16ff8.l
            next = 0x194fcU;
            const auto result = m.call(c, 308U, 0x194f6U, 0x16ff8U, 0x194fcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x194fcU: { // 3b7c00080068 move.w #$8, $68(a5)
            next = 0x19502U;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19502U: { // 526d0064 addq.w #$1, $64(a5)
            next = 0x19506U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19506U: { // 026d00030064 andi.w #$3, $64(a5)
            next = 0x1950cU;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1950cU: { // 302d0064 move.w $64(a5), d0
            next = 0x19510U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19510U: { // 206d004c movea.l $4c(a5), a0
            next = 0x19514U;
            const auto source_address = r.address[5] + 0x4cU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x19514U: { // 1b7000000059 move.b (a0, d0.w), $59(a5)
            next = 0x1951aU;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x59U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1951aU: { // 6100049a bsr.w $199b6
            next = 0x1951eU;
            const auto result = m.call(c, 914U, 0x1951aU, 0x199b6U, 0x1951eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1951eU: { // 610004ca bsr.w $199ea
            next = 0x19522U;
            const auto result = m.call(c, 915U, 0x1951eU, 0x199eaU, 0x19522U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19522U: { // 41fa1f2c lea.l $1b450(pc), a0
            next = 0x19526U;
            const auto source_address = 0x1b450U;
            r.address[0] = source_address;
            break;
        }
        case 0x19526U: { // 6100fe72 bsr.w $1939a
            next = 0x1952aU;
            const auto result = m.call(c, 904U, 0x19526U, 0x1939aU, 0x1952aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1952aU: { // 4e75 rts 
            next = 0x1952cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1952aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x194d6U:
        case 0x194daU:
        case 0x194dcU:
        case 0x194deU:
        case 0x194e2U:
        case 0x194e6U:
        case 0x194eaU:
        case 0x194ecU:
        case 0x194f0U:
        case 0x194f2U:
        case 0x194f6U:
        case 0x194fcU:
        case 0x19502U:
        case 0x19506U:
        case 0x1950cU:
        case 0x19510U:
        case 0x19514U:
        case 0x1951aU:
        case 0x1951eU:
        case 0x19522U:
        case 0x19526U:
        case 0x1952aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
