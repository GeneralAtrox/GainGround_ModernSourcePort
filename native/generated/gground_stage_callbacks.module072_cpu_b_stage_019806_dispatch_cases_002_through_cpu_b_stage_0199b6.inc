
static bool cpu_b_stage_019806_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19904U: { // 3d400034 move.w d0, $34(a6)
            next = 0x19908U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x34U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19908U: { // 426e001c clr.w $1c(a6)
            next = 0x1990cU;
            const auto destination_address = r.address[6] + 0x1cU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1990cU: { // 3d6d0056005a move.w $56(a5), $5a(a6)
            next = 0x19912U;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19912U: { // 3d6d00720072 move.w $72(a5), $72(a6)
            next = 0x19918U;
            const auto source_address = r.address[5] + 0x72U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x72U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19918U: { // 422e003f clr.b $3f(a6)
            next = 0x1991cU;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1991cU: { // 3d7c00010068 move.w #$1, $68(a6)
            next = 0x19922U;
            const auto value = 0x1U;
            const auto destination_address = r.address[6] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19922U: { // 3d7c005a0066 move.w #$5a, $66(a6)
            next = 0x19928U;
            const auto value = 0x5aU;
            const auto destination_address = r.address[6] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19928U: { // 426e0064 clr.w $64(a6)
            next = 0x1992cU;
            const auto destination_address = r.address[6] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1992cU: { // 4a2d0051 tst.b $51(a5)
            next = 0x19930U;
            const auto destination_address = r.address[5] + 0x51U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x19930U: { // 6e04 bgt.b $19936
            next = 0x19932U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19936U; transfer_kind = 1U; }
            break;
        }
        case 0x19932U: { // 426d0040 clr.w $40(a5)
            next = 0x19936U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_019806(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19824U: { // 6100f324 bsr.w $18b4a
            next = 0x19828U;
            const auto result = m.call(c, 889U, 0x19824U, 0x18b4aU, 0x19828U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19936U: { // 4e75 rts 
            next = 0x19938U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19936U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_019806_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_019806_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_019806_dispatch_cases_002(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_019806_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

FunctionResult cpu_b_stage_0199b6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x199b6U: { // 41f900038a24 lea.l $38a24.l, a0
            next = 0x199bcU;
            const auto source_address = 0x38a24U;
            r.address[0] = source_address;
            break;
        }
        case 0x199bcU: { // 302d0058 move.w $58(a5), d0
            next = 0x199c0U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x199c0U: { // 6b26 bmi.b $199e8
            next = 0x199c2U;
            if ((r.status & 8U) != 0U) { next = 0x199e8U; transfer_kind = 1U; }
            break;
        }
        case 0x199c2U: { // e740 asl.w #$3, d0
            next = 0x199c4U;
            m.asl_word(0U, 3U);
            break;
        }
        case 0x199c4U: { // 3200 move.w d0, d1
            next = 0x199c6U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x199c6U: { // d040 add.w d0, d0
            next = 0x199c8U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x199c8U: { // d041 add.w d1, d0
            next = 0x199caU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x199caU: { // d0c0 adda.w d0, a0
            next = 0x199ccU;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x199ccU: { // 2b48005c move.l a0, $5c(a5)
            next = 0x199d0U;
            const auto value = r.address[0];
            const auto destination_address = r.address[5] + 0x5cU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x199d0U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x199d4U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x199d4U: { // 6b12 bmi.b $199e8
            next = 0x199d6U;
            if ((r.status & 8U) != 0U) { next = 0x199e8U; transfer_kind = 1U; }
            break;
        }
        case 0x199d6U: { // 5648 addq.w #$3, a0
            next = 0x199d8U;
            const auto source_value = 0x3U;
            r.address[0] += source_value;
            break;
        }
        case 0x199d8U: { // 1b500001 move.b (a0), $1(a5)
            next = 0x199dcU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x199dcU: { // 4eb900015d24 jsr $15d24.l
            next = 0x199e2U;
            const auto result = m.call(c, 280U, 0x199dcU, 0x15d24U, 0x199e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x199e2U: { // 4eb900015df2 jsr $15df2.l
            next = 0x199e8U;
            const auto result = m.call(c, 282U, 0x199e2U, 0x15df2U, 0x199e8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x199e8U: { // 4e75 rts 
            next = 0x199eaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x199e8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x199b6U:
        case 0x199bcU:
        case 0x199c0U:
        case 0x199c2U:
        case 0x199c4U:
        case 0x199c6U:
        case 0x199c8U:
        case 0x199caU:
        case 0x199ccU:
        case 0x199d0U:
        case 0x199d4U:
        case 0x199d6U:
        case 0x199d8U:
        case 0x199dcU:
        case 0x199e2U:
        case 0x199e8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
