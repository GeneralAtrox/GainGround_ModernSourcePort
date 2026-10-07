
static bool cpu_b_stage_019286_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19286U: { // 2a2d001e move.l $1e(a5), d5
            next = 0x1928aU;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[5] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1928aU: { // daad0012 add.l $12(a5), d5
            next = 0x1928eU;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[5];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[5] = value;
            break;
        }
        case 0x1928eU: { // 4845 swap d5
            next = 0x19290U;
            const auto value = (r.data[5] << 16U) | (r.data[5] >> 16U);
            r.data[5] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x19290U: { // 1c14 move.b (a4), d6
            next = 0x19292U;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            m.db(6U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19292U: { // 4886 ext.w d6
            next = 0x19294U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[6])));
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19294U: { // 9a46 sub.w d6, d5
            next = 0x19296U;
            const auto source_value = r.data[6];
            const auto destination_value = r.data[5];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x19296U: { // ba41 cmp.w d1, d5
            next = 0x19298U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[5];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19298U: { // 6c3c bge.b $192d6
            next = 0x1929aU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x192d6U; transfer_kind = 1U; }
            break;
        }
        case 0x1929aU: { // 1c2c0001 move.b $1(a4), d6
            next = 0x1929eU;
            const auto source_address = r.address[4] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(6U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1929eU: { // 4886 ext.w d6
            next = 0x192a0U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[6])));
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x192a0U: { // da46 add.w d6, d5
            next = 0x192a2U;
            const auto source_value = r.data[6];
            const auto destination_value = r.data[5];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x192a2U: { // ba40 cmp.w d0, d5
            next = 0x192a4U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[5];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x192a4U: { // 6f30 ble.b $192d6
            next = 0x192a6U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x192d6U; transfer_kind = 1U; }
            break;
        }
        case 0x192a6U: { // 2a2d0026 move.l $26(a5), d5
            next = 0x192aaU;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[5] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x192aaU: { // daad001a add.l $1a(a5), d5
            next = 0x192aeU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[5];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[5] = value;
            break;
        }
        case 0x192aeU: { // 4845 swap d5
            next = 0x192b0U;
            const auto value = (r.data[5] << 16U) | (r.data[5] >> 16U);
            r.data[5] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x192b0U: { // 1c2c0004 move.b $4(a4), d6
            next = 0x192b4U;
            const auto source_address = r.address[4] + 0x4U;
            const auto value = m.byte(source_address);
            m.db(6U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x192b4U: { // 4886 ext.w d6
            next = 0x192b6U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[6])));
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x192b6U: { // 9a46 sub.w d6, d5
            next = 0x192b8U;
            const auto source_value = r.data[6];
            const auto destination_value = r.data[5];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x192b8U: { // ba43 cmp.w d3, d5
            next = 0x192baU;
            const auto source_value = r.data[3];
            const auto destination_value = r.data[5];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x192baU: { // 6c1a bge.b $192d6
            next = 0x192bcU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x192d6U; transfer_kind = 1U; }
            break;
        }
        case 0x192bcU: { // 1c2c0005 move.b $5(a4), d6
            next = 0x192c0U;
            const auto source_address = r.address[4] + 0x5U;
            const auto value = m.byte(source_address);
            m.db(6U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x192c0U: { // 4886 ext.w d6
            next = 0x192c2U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[6])));
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x192c2U: { // da46 add.w d6, d5
            next = 0x192c4U;
            const auto source_value = r.data[6];
            const auto destination_value = r.data[5];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x192c4U: { // ba42 cmp.w d2, d5
            next = 0x192c6U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[5];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x192c6U: { // 6f0e ble.b $192d6
            next = 0x192c8U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x192d6U; transfer_kind = 1U; }
            break;
        }
        case 0x192c8U: { // 42ad001e clr.l $1e(a5)
            next = 0x192ccU;
            const auto destination_address = r.address[5] + 0x1eU;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0x192ccU: { // 42ad0026 clr.l $26(a5)
            next = 0x192d0U;
            const auto destination_address = r.address[5] + 0x26U;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0x192d0U: { // 44fc0001 move.w #$1, ccr
            next = 0x192d4U;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x1U & 0x1fU));
            break;
        }
        case 0x192d6U: { // 44fc0000 move.w #$0, ccr
            next = 0x192daU;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_019286(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x192d4U: { // 4e75 rts 
            next = 0x192d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x192d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x192daU: { // 4e75 rts 
            next = 0x192dcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x192daU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_019286_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19286U:
        case 0x1928aU:
        case 0x1928eU:
        case 0x19290U:
        case 0x19292U:
        case 0x19294U:
        case 0x19296U:
        case 0x19298U:
        case 0x1929aU:
        case 0x1929eU:
        case 0x192a0U:
        case 0x192a2U:
        case 0x192a4U:
        case 0x192a6U:
        case 0x192aaU:
        case 0x192aeU:
        case 0x192b0U:
        case 0x192b4U:
        case 0x192b6U:
        case 0x192b8U:
        case 0x192baU:
        case 0x192bcU:
        case 0x192c0U:
        case 0x192c2U:
        case 0x192c4U:
        case 0x192c6U:
        case 0x192c8U:
        case 0x192ccU:
        case 0x192d0U:
        case 0x192d4U:
        case 0x192d6U:
        case 0x192daU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
