
static bool cpu_b_stage_018b60_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18b60U: { // 4a2d003f tst.b $3f(a5)
            next = 0x18b64U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x18b64U: { // 66000066 bne.w $18bcc
            next = 0x18b68U;
            if ((r.status & 4U) == 0U) { next = 0x18bccU; transfer_kind = 1U; }
            break;
        }
        case 0x18b68U: { // 202d001e move.l $1e(a5), d0
            next = 0x18b6cU;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x18b6cU: { // d1ad0012 add.l d0, $12(a5)
            next = 0x18b70U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x18b70U: { // 202d0026 move.l $26(a5), d0
            next = 0x18b74U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x18b74U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x18b78U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x18b78U: { // 202d0022 move.l $22(a5), d0
            next = 0x18b7cU;
            const auto source_address = r.address[5] + 0x22U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x18b7cU: { // d1ad0016 add.l d0, $16(a5)
            next = 0x18b80U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x16U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x18b80U: { // 302d0016 move.w $16(a5), d0
            next = 0x18b84U;
            const auto source_address = r.address[5] + 0x16U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b84U: { // 0c40000a cmpi.w #$a, d0
            next = 0x18b88U;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18b88U: { // 6b42 bmi.b $18bcc
            next = 0x18b8aU;
            if ((r.status & 8U) != 0U) { next = 0x18bccU; transfer_kind = 1U; }
            break;
        }
        case 0x18b8aU: { // 546d0064 addq.w #$2, $64(a5)
            next = 0x18b8eU;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18b8eU: { // 302d0064 move.w $64(a5), d0
            next = 0x18b92U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b92U: { // 02400006 andi.w #$6, d0
            next = 0x18b96U;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18b96U: { // 41fa2034 lea.l $1abcc(pc), a0
            next = 0x18b9aU;
            const auto source_address = 0x1abccU;
            r.address[0] = source_address;
            break;
        }
        case 0x18b9aU: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x18ba0U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ba0U: { // 302d0010 move.w $10(a5), d0
            next = 0x18ba4U;
            const auto source_address = r.address[5] + 0x10U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ba4U: { // d07c0303 add.w #$303, d0
            next = 0x18ba8U;
            const auto source_value = 0x303U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18ba8U: { // 0c403f3f cmpi.w #$3f3f, d0
            next = 0x18bacU;
            const auto source_value = 0x3f3fU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18bacU: { // 6b04 bmi.b $18bb2
            next = 0x18baeU;
            if ((r.status & 8U) != 0U) { next = 0x18bb2U; transfer_kind = 1U; }
            break;
        }
        case 0x18baeU: { // 303c3f3f move.w #$3f3f, d0
            next = 0x18bb2U;
            const auto value = 0x3f3fU;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18bb2U: { // 3b400010 move.w d0, $10(a5)
            next = 0x18bb6U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18bbcU: { // 41fa249a lea.l $1b058(pc), a0
            next = 0x18bc0U;
            const auto source_address = 0x1b058U;
            r.address[0] = source_address;
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_018b60(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18bb6U: { // 4eb900015d24 jsr $15d24.l
            next = 0x18bbcU;
            const auto result = m.call(c, 280U, 0x18bb6U, 0x15d24U, 0x18bbcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18bc0U: { // 610007d8 bsr.w $1939a
            next = 0x18bc4U;
            const auto result = m.call(c, 904U, 0x18bc0U, 0x1939aU, 0x18bc4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18bc4U: { // 4eb900015df2 jsr $15df2.l
            next = 0x18bcaU;
            const auto result = m.call(c, 282U, 0x18bc4U, 0x15df2U, 0x18bcaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18bcaU: { // 4e75 rts 
            next = 0x18bccU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18bcaU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18bccU: { // 61000008 bsr.w $18bd6
            next = 0x18bd0U;
            const auto result = m.call(c, 891U, 0x18bccU, 0x18bd6U, 0x18bd0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18bd0U: { // 4e75 rts 
            next = 0x18bd2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18bd0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_018b60_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18b60U:
        case 0x18b64U:
        case 0x18b68U:
        case 0x18b6cU:
        case 0x18b70U:
        case 0x18b74U:
        case 0x18b78U:
        case 0x18b7cU:
        case 0x18b80U:
        case 0x18b84U:
        case 0x18b88U:
        case 0x18b8aU:
        case 0x18b8eU:
        case 0x18b92U:
        case 0x18b96U:
        case 0x18b9aU:
        case 0x18ba0U:
        case 0x18ba4U:
        case 0x18ba8U:
        case 0x18bacU:
        case 0x18baeU:
        case 0x18bb2U:
        case 0x18bb6U:
        case 0x18bbcU:
        case 0x18bc0U:
        case 0x18bc4U:
        case 0x18bcaU:
        case 0x18bccU:
        case 0x18bd0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
