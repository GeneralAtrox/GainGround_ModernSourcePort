
static bool cpu_b_stage_01a1f6_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a260U: { // 101c move.b (a4)+, d0
            next = 0x1a262U;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a262U: { // 4880 ext.w d0
            next = 0x1a264U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a264U: { // d06e000e add.w $e(a6), d0
            next = 0x1a268U;
            const auto source_address = r.address[6] + 0xeU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a268U: { // 3b40000e move.w d0, $e(a5)
            next = 0x1a26cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a26cU: { // 303c0c00 move.w #$c00, d0
            next = 0x1a270U;
            const auto value = 0xc00U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a270U: { // d01c add.b (a4)+, d0
            next = 0x1a272U;
            const auto source_address = r.address[4];
            const auto source_value = m.byte(source_address);
            r.address[4] += 1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x1a272U: { // 3b400008 move.w d0, $8(a5)
            next = 0x1a276U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a276U: { // 1b5c0001 move.b (a4)+, $1(a5)
            next = 0x1a27aU;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a27aU: { // 3b6e001a001a move.w $1a(a6), $1a(a5)
            next = 0x1a280U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a280U: { // 0c6d0018003a cmpi.w #$18, $3a(a5)
            next = 0x1a286U;
            const auto source_value = 0x18U;
            const auto destination_address = r.address[5] + 0x3aU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a286U: { // 6636 bne.b $1a2be
            next = 0x1a288U;
            if ((r.status & 4U) == 0U) { next = 0x1a2beU; transfer_kind = 1U; }
            break;
        }
        case 0x1a288U: { // 4a2e006a tst.b $6a(a6)
            next = 0x1a28cU;
            const auto destination_address = r.address[6] + 0x6aU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1a28cU: { // 6708 beq.b $1a296
            next = 0x1a28eU;
            if ((r.status & 4U) != 0U) { next = 0x1a296U; transfer_kind = 1U; }
            break;
        }
        case 0x1a28eU: { // 3b7c0c710008 move.w #$c71, $8(a5)
            next = 0x1a294U;
            const auto value = 0xc71U;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a294U: { // 6028 bra.b $1a2be
            next = 0x1a296U;
            if (true) { next = 0x1a2beU; transfer_kind = 1U; }
            break;
        }
        case 0x1a296U: { // 536d0022 subq.w #$1, $22(a5)
            next = 0x1a29aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x22U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a29aU: { // 6e22 bgt.b $1a2be
            next = 0x1a29cU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a2beU; transfer_kind = 1U; }
            break;
        }
        case 0x1a29cU: { // 3b7c00030022 move.w #$3, $22(a5)
            next = 0x1a2a2U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x22U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a2a2U: { // 526d0024 addq.w #$1, $24(a5)
            next = 0x1a2a6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x24U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a2a6U: { // 0c6d00030024 cmpi.w #$3, $24(a5)
            next = 0x1a2acU;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x24U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a2acU: { // 6504 bcs.b $1a2b2
            next = 0x1a2aeU;
            if ((r.status & 1U) != 0U) { next = 0x1a2b2U; transfer_kind = 1U; }
            break;
        }
        case 0x1a2aeU: { // 426d0024 clr.w $24(a5)
            next = 0x1a2b2U;
            const auto destination_address = r.address[5] + 0x24U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a2b2U: { // 303c0c62 move.w #$c62, d0
            next = 0x1a2b6U;
            const auto value = 0xc62U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a2b6U: { // d06d0024 add.w $24(a5), d0
            next = 0x1a2baU;
            const auto source_address = r.address[5] + 0x24U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a2baU: { // 3b400008 move.w d0, $8(a5)
            next = 0x1a2beU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a2c4U: { // 08950000 bclr.b #$0, (a5)
            next = 0x1a2c8U;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01a1f6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a200U: { // 4e75 rts 
            next = 0x1a202U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a200U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a2beU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a2c4U;
            const auto result = m.call(c, 282U, 0x1a2beU, 0x15df2U, 0x1a2c4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a2c8U: { // 4e75 rts 
            next = 0x1a2caU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a2c8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01a1f6_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01a1f6_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01a1f6_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
