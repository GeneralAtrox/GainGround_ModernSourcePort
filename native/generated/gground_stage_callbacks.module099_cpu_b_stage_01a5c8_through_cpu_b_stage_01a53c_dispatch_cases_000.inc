
FunctionResult cpu_b_stage_01a5c8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a5d2U: { // 6100e576 bsr.w $18b4a
            next = 0x1a5d6U;
            const auto result = m.call(c, 889U, 0x1a5d2U, 0x18b4aU, 0x1a5d6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a648U: { // 4eb900016372 jsr $16372.l
            next = 0x1a64eU;
            const auto result = m.call(c, 306U, 0x1a648U, 0x16372U, 0x1a64eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a662U: { // 4e75 rts 
            next = 0x1a664U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a662U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01a5c8_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01a5c8_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a5c8U:
        case 0x1a5ccU:
        case 0x1a5ceU:
        case 0x1a5d0U:
        case 0x1a5d2U:
        case 0x1a5d6U:
        case 0x1a5d8U:
        case 0x1a5dcU:
        case 0x1a5e0U:
        case 0x1a5e8U:
        case 0x1a5eeU:
        case 0x1a5f0U:
        case 0x1a5f4U:
        case 0x1a5f8U:
        case 0x1a5fcU:
        case 0x1a602U:
        case 0x1a606U:
        case 0x1a60cU:
        case 0x1a610U:
        case 0x1a614U:
        case 0x1a618U:
        case 0x1a61cU:
        case 0x1a622U:
        case 0x1a628U:
        case 0x1a62eU:
        case 0x1a632U:
        case 0x1a636U:
        case 0x1a63aU:
        case 0x1a63eU:
        case 0x1a642U:
        case 0x1a646U:
        case 0x1a648U:
        case 0x1a64eU:
        case 0x1a650U:
        case 0x1a652U:
        case 0x1a654U:
        case 0x1a656U:
        case 0x1a65aU:
        case 0x1a65eU:
        case 0x1a662U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01a53c_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a53cU: { // 422d003f clr.b $3f(a5)
            next = 0x1a540U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1a540U: { // 202d001e move.l $1e(a5), d0
            next = 0x1a544U;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a544U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x1a548U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a548U: { // 202d0022 move.l $22(a5), d0
            next = 0x1a54cU;
            const auto source_address = r.address[5] + 0x22U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a54cU: { // 04ad000020000022 subi.l #$2000, $22(a5)
            next = 0x1a554U;
            const auto source_value = 0x2000U;
            const auto destination_address = r.address[5] + 0x22U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.sub(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a554U: { // d1ad0016 add.l d0, $16(a5)
            next = 0x1a558U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x16U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a558U: { // 202d0026 move.l $26(a5), d0
            next = 0x1a55cU;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a55cU: { // d1ad001a add.l d0, $1a(a5)
            next = 0x1a560U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a560U: { // 302d0060 move.w $60(a5), d0
            next = 0x1a564U;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a564U: { // 06400303 addi.w #$303, d0
            next = 0x1a568U;
            const auto source_value = 0x303U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a568U: { // 0c403f3f cmpi.w #$3f3f, d0
            next = 0x1a56cU;
            const auto source_value = 0x3f3fU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a56cU: { // 6f04 ble.b $1a572
            next = 0x1a56eU;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1a572U; transfer_kind = 1U; }
            break;
        }
        case 0x1a56eU: { // 303c3f3f move.w #$3f3f, d0
            next = 0x1a572U;
            const auto value = 0x3f3fU;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a572U: { // 3b400010 move.w d0, $10(a5)
            next = 0x1a576U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a576U: { // 3b400060 move.w d0, $60(a5)
            next = 0x1a57aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x60U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a57aU: { // 302d0016 move.w $16(a5), d0
            next = 0x1a57eU;
            const auto source_address = r.address[5] + 0x16U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a57eU: { // 0c400008 cmpi.w #$8, d0
            next = 0x1a582U;
            const auto source_value = 0x8U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a582U: { // 6f3e ble.b $1a5c2
            next = 0x1a584U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1a5c2U; transfer_kind = 1U; }
            break;
        }
        case 0x1a584U: { // e240 asr.w #$1, d0
            next = 0x1a586U;
            m.shift_word(0U, 1U, false, true);
            break;
        }
        case 0x1a586U: { // d12d0010 add.b d0, $10(a5)
            next = 0x1a58aU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x10U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1a58aU: { // d12d0011 add.b d0, $11(a5)
            next = 0x1a58eU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x11U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1a58eU: { // 302d0064 move.w $64(a5), d0
            next = 0x1a592U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a592U: { // 5440 addq.w #$2, d0
            next = 0x1a594U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a594U: { // 0c400006 cmpi.w #$6, d0
            next = 0x1a598U;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a598U: { // 6b02 bmi.b $1a59c
            next = 0x1a59aU;
            if ((r.status & 8U) != 0U) { next = 0x1a59cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a59aU: { // 7000 moveq #$0, d0
            next = 0x1a59cU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1a59cU: { // 3b400064 move.w d0, $64(a5)
            next = 0x1a5a0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a5a0U: { // 41fa121a lea.l $1b7bc(pc), a0
            next = 0x1a5a4U;
            const auto source_address = 0x1b7bcU;
            r.address[0] = source_address;
            break;
        }
        case 0x1a5a4U: { // 3b7000000008 move.w (a0, d0.w), $8(a5)
            next = 0x1a5aaU;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a5b0U: { // 41fa1366 lea.l $1b918(pc), a0
            next = 0x1a5b4U;
            const auto source_address = 0x1b918U;
            r.address[0] = source_address;
            break;
        }
    default: return false;
    }
    return true;
}
