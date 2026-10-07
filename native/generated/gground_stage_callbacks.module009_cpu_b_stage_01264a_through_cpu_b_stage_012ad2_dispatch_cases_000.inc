
FunctionResult cpu_b_stage_01264a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x12626U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1262cU;
            const auto result = m.call(c, 280U, 0x12626U, 0x15d24U, 0x1262cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1262cU: { // 4eb900015d3c jsr $15d3c.l
            next = 0x12632U;
            const auto result = m.call(c, 281U, 0x1262cU, 0x15d3cU, 0x12632U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12632U: { // 4eb900015df2 jsr $15df2.l
            next = 0x12638U;
            const auto result = m.call(c, 282U, 0x12632U, 0x15df2U, 0x12638U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12638U: { // 4e75 rts
            next = 0x1263aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12638U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1263eU: { // 4eb900016ff8 jsr $16ff8.l
            next = 0x12644U;
            const auto result = m.call(c, 308U, 0x1263eU, 0x16ff8U, 0x12644U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12644U: { // 61000708 bsr.w $12d4e
            next = 0x12648U;
            const auto result = m.call(c, 264U, 0x12644U, 0x12d4eU, 0x12648U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12648U: { // 4e75 rts
            next = 0x1264aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12648U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1264aU: { // 610001f0 bsr.w $1283c
            next = 0x1264eU;
            const auto result = m.call(c, 253U, 0x1264aU, 0x1283cU, 0x1264eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12650U: { // 610004e0 bsr.w $12b32
            next = 0x12654U;
            const auto result = m.call(c, 257U, 0x12650U, 0x12b32U, 0x12654U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1266cU: { // 4eb900016372 jsr $16372.l
            next = 0x12672U;
            const auto result = m.call(c, 306U, 0x1266cU, 0x16372U, 0x12672U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        default:
            if (!(cpu_b_stage_01264a_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01264a_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01264a_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_012ad2_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x12ad2U: { // 323cfff9 move.w #$fff9, d1
            next = 0x12ad6U;
            const auto value = 0xfff9U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12ad6U: { // 343c0007 move.w #$7, d2
            next = 0x12adaU;
            const auto value = 0x7U;
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12adaU: { // 302e0012 move.w $12(a6), d0
            next = 0x12adeU;
            const auto source_address = r.address[6] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12adeU: { // d240 add.w d0, d1
            next = 0x12ae0U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x12ae0U: { // d440 add.w d0, d2
            next = 0x12ae2U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x12ae2U: { // b26d002c cmp.w $2c(a5), d1
            next = 0x12ae6U;
            const auto source_address = r.address[5] + 0x2cU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x12ae6U: { // 6e44 bgt.b $12b2c
            next = 0x12ae8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x12b2cU; transfer_kind = 1U; }
            break;
        }
        case 0x12ae8U: { // b46d002a cmp.w $2a(a5), d2
            next = 0x12aecU;
            const auto source_address = r.address[5] + 0x2aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[2];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x12aecU: { // 6b3e bmi.b $12b2c
            next = 0x12aeeU;
            if ((r.status & 8U) != 0U) { next = 0x12b2cU; transfer_kind = 1U; }
            break;
        }
        case 0x12aeeU: { // 323cfff8 move.w #$fff8, d1
            next = 0x12af2U;
            const auto value = 0xfff8U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12af2U: { // 343c000c move.w #$c, d2
            next = 0x12af6U;
            const auto value = 0xcU;
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12af6U: { // 302e0016 move.w $16(a6), d0
            next = 0x12afaU;
            const auto source_address = r.address[6] + 0x16U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12afaU: { // d240 add.w d0, d1
            next = 0x12afcU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x12afcU: { // d440 add.w d0, d2
            next = 0x12afeU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x12afeU: { // b26d0030 cmp.w $30(a5), d1
            next = 0x12b02U;
            const auto source_address = r.address[5] + 0x30U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x12b02U: { // 6e28 bgt.b $12b2c
            next = 0x12b04U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x12b2cU; transfer_kind = 1U; }
            break;
        }
        case 0x12b04U: { // b46d002e cmp.w $2e(a5), d2
            next = 0x12b08U;
            const auto source_address = r.address[5] + 0x2eU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[2];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x12b08U: { // 6b22 bmi.b $12b2c
            next = 0x12b0aU;
            if ((r.status & 8U) != 0U) { next = 0x12b2cU; transfer_kind = 1U; }
            break;
        }
        case 0x12b0aU: { // 323cfffa move.w #$fffa, d1
            next = 0x12b0eU;
            const auto value = 0xfffaU;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12b0eU: { // 343c0006 move.w #$6, d2
            next = 0x12b12U;
            const auto value = 0x6U;
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12b12U: { // 302e001a move.w $1a(a6), d0
            next = 0x12b16U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12b16U: { // d240 add.w d0, d1
            next = 0x12b18U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x12b18U: { // d440 add.w d0, d2
            next = 0x12b1aU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x12b1aU: { // b26d0034 cmp.w $34(a5), d1
            next = 0x12b1eU;
            const auto source_address = r.address[5] + 0x34U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x12b1eU: { // 6e0c bgt.b $12b2c
            next = 0x12b20U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x12b2cU; transfer_kind = 1U; }
            break;
        }
        case 0x12b20U: { // b46d0032 cmp.w $32(a5), d2
            next = 0x12b24U;
            const auto source_address = r.address[5] + 0x32U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[2];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x12b24U: { // 6b06 bmi.b $12b2c
            next = 0x12b26U;
            if ((r.status & 8U) != 0U) { next = 0x12b2cU; transfer_kind = 1U; }
            break;
        }
        case 0x12b26U: { // 44fc0001 move.w #$1, ccr
            next = 0x12b2aU;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x1U & 0x1fU));
            break;
        }
        case 0x12b2cU: { // 44fc0000 move.w #$0, ccr
            next = 0x12b30U;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
    default: return false;
    }
    return true;
}
