
static bool cpu_b_stage_0192dc_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19344U: { // 3601 move.w d1, d3
            next = 0x19346U;
            const auto value = r.data[1];
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19346U: { // 14290005 move.b $5(a1), d2
            next = 0x1934aU;
            const auto source_address = r.address[1] + 0x5U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1934aU: { // 4882 ext.w d2
            next = 0x1934cU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[2])));
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1934cU: { // d242 add.w d2, d1
            next = 0x1934eU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x19354U: { // 08100000 btst.b #$0, (a0)
            next = 0x19358U;
            const auto destination_address = r.address[0];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x19358U: { // 6634 bne.b $1938e
            next = 0x1935aU;
            if ((r.status & 4U) == 0U) { next = 0x1938eU; transfer_kind = 1U; }
            break;
        }
        case 0x1935aU: { // 14290001 move.b $1(a1), d2
            next = 0x1935eU;
            const auto source_address = r.address[1] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1935eU: { // 4882 ext.w d2
            next = 0x19360U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[2])));
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19360U: { // d042 add.w d2, d0
            next = 0x19362U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19368U: { // 08100000 btst.b #$0, (a0)
            next = 0x1936cU;
            const auto destination_address = r.address[0];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1936cU: { // 6620 bne.b $1938e
            next = 0x1936eU;
            if ((r.status & 4U) == 0U) { next = 0x1938eU; transfer_kind = 1U; }
            break;
        }
        case 0x1936eU: { // 3203 move.w d3, d1
            next = 0x19370U;
            const auto value = r.data[3];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19376U: { // 08100000 btst.b #$0, (a0)
            next = 0x1937aU;
            const auto destination_address = r.address[0];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1937aU: { // 6612 bne.b $1938e
            next = 0x1937cU;
            if ((r.status & 4U) == 0U) { next = 0x1938eU; transfer_kind = 1U; }
            break;
        }
        case 0x1937cU: { // 202d001e move.l $1e(a5), d0
            next = 0x19380U;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x19380U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x19384U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x19384U: { // 202d0026 move.l $26(a5), d0
            next = 0x19388U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x19388U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x1938cU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1938eU: { // 42ad001e clr.l $1e(a5)
            next = 0x19392U;
            const auto destination_address = r.address[5] + 0x1eU;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0x19392U: { // 42ad0026 clr.l $26(a5)
            next = 0x19396U;
            const auto destination_address = r.address[5] + 0x26U;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_0192dc(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19338U: { // 4eb900015ede jsr $15ede.l
            next = 0x1933eU;
            const auto result = m.call(c, 287U, 0x19338U, 0x15edeU, 0x1933eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1934eU: { // 4eb900015ede jsr $15ede.l
            next = 0x19354U;
            const auto result = m.call(c, 287U, 0x1934eU, 0x15edeU, 0x19354U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19362U: { // 4eb900015ede jsr $15ede.l
            next = 0x19368U;
            const auto result = m.call(c, 287U, 0x19362U, 0x15edeU, 0x19368U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19370U: { // 4eb900015ede jsr $15ede.l
            next = 0x19376U;
            const auto result = m.call(c, 287U, 0x19370U, 0x15edeU, 0x19376U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1938cU: { // 4e75 rts 
            next = 0x1938eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1938cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19396U: { // 4e75 rts 
            next = 0x19398U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19396U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_0192dc_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_0192dc_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_0192dc_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
