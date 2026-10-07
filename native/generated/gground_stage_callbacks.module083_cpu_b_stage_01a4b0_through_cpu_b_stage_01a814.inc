
FunctionResult cpu_b_stage_01a4b0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a4b0U: { // 4aed003f tas.b $3f(a5)
            next = 0x1a4b4U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x1a4b4U: { // 086d00000068 bchg.b #$0, $68(a5)
            next = 0x1a4baU;
            const auto destination_address = r.address[5] + 0x68U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x1a4baU: { // 6636 bne.b $1a4f2
            next = 0x1a4bcU;
            if ((r.status & 4U) == 0U) { next = 0x1a4f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1a4bcU: { // 586d0064 addq.w #$4, $64(a5)
            next = 0x1a4c0U;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a4c0U: { // 0c6d00200064 cmpi.w #$20, $64(a5)
            next = 0x1a4c6U;
            const auto source_value = 0x20U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a4c6U: { // 650e bcs.b $1a4d6
            next = 0x1a4c8U;
            if ((r.status & 1U) != 0U) { next = 0x1a4d6U; transfer_kind = 1U; }
            break;
        }
        case 0x1a4c8U: { // 303c0014 move.w #$14, d0
            next = 0x1a4ccU;
            const auto value = 0x14U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a4ccU: { // 4eb900016ff8 jsr $16ff8.l
            next = 0x1a4d2U;
            const auto result = m.call(c, 308U, 0x1a4ccU, 0x16ff8U, 0x1a4d2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a4d2U: { // 4215 clr.b (a5)
            next = 0x1a4d4U;
            const auto destination_address = r.address[5];
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1a4d4U: { // 4e75 rts 
            next = 0x1a4d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a4d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a4d6U: { // 302d0064 move.w $64(a5), d0
            next = 0x1a4daU;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a4daU: { // 41fa1256 lea.l $1b732(pc), a0
            next = 0x1a4deU;
            const auto source_address = 0x1b732U;
            r.address[0] = source_address;
            break;
        }
        case 0x1a4deU: { // d0c0 adda.w d0, a0
            next = 0x1a4e0U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1a4e0U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1a4e4U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a4e4U: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x1a4e8U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a4e8U: { // 303c0900 move.w #$900, d0
            next = 0x1a4ecU;
            const auto value = 0x900U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a4ecU: { // 1018 move.b (a0)+, d0
            next = 0x1a4eeU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a4eeU: { // 3b400008 move.w d0, $8(a5)
            next = 0x1a4f2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a4f2U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1a4f8U;
            const auto result = m.call(c, 280U, 0x1a4f2U, 0x15d24U, 0x1a4f8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a4f8U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a4feU;
            const auto result = m.call(c, 282U, 0x1a4f8U, 0x15df2U, 0x1a4feU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a4feU: { // 4e75 rts 
            next = 0x1a500U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a4feU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a4b0U:
        case 0x1a4b4U:
        case 0x1a4baU:
        case 0x1a4bcU:
        case 0x1a4c0U:
        case 0x1a4c6U:
        case 0x1a4c8U:
        case 0x1a4ccU:
        case 0x1a4d2U:
        case 0x1a4d4U:
        case 0x1a4d6U:
        case 0x1a4daU:
        case 0x1a4deU:
        case 0x1a4e0U:
        case 0x1a4e4U:
        case 0x1a4e8U:
        case 0x1a4ecU:
        case 0x1a4eeU:
        case 0x1a4f2U:
        case 0x1a4f8U:
        case 0x1a4feU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01a814_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a814U: { // 302d0012 move.w $12(a5), d0
            next = 0x1a818U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a818U: { // 0c40ffe0 cmpi.w #$ffe0, d0
            next = 0x1a81cU;
            const auto source_value = 0xffe0U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a81cU: { // 6b16 bmi.b $1a834
            next = 0x1a81eU;
            if ((r.status & 8U) != 0U) { next = 0x1a834U; transfer_kind = 1U; }
            break;
        }
        case 0x1a81eU: { // 0c4001a0 cmpi.w #$1a0, d0
            next = 0x1a822U;
            const auto source_value = 0x1a0U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a822U: { // 6e10 bgt.b $1a834
            next = 0x1a824U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a834U; transfer_kind = 1U; }
            break;
        }
        case 0x1a824U: { // 302d001a move.w $1a(a5), d0
            next = 0x1a828U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a828U: { // 0c40ffe0 cmpi.w #$ffe0, d0
            next = 0x1a82cU;
            const auto source_value = 0xffe0U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a82cU: { // 6b06 bmi.b $1a834
            next = 0x1a82eU;
            if ((r.status & 8U) != 0U) { next = 0x1a834U; transfer_kind = 1U; }
            break;
        }
        case 0x1a82eU: { // 0c400210 cmpi.w #$210, d0
            next = 0x1a832U;
            const auto source_value = 0x210U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a832U: { // 6b04 bmi.b $1a838
            next = 0x1a834U;
            if ((r.status & 8U) != 0U) { next = 0x1a838U; transfer_kind = 1U; }
            break;
        }
        case 0x1a834U: { // 584f addq.w #$4, a7
            next = 0x1a836U;
            const auto source_value = 0x4U;
            r.address[7] += source_value;
            break;
        }
        case 0x1a836U: { // 4215 clr.b (a5)
            next = 0x1a838U;
            const auto destination_address = r.address[5];
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01a814(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a838U: { // 4e75 rts 
            next = 0x1a83aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a838U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01a814_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a814U:
        case 0x1a818U:
        case 0x1a81cU:
        case 0x1a81eU:
        case 0x1a822U:
        case 0x1a824U:
        case 0x1a828U:
        case 0x1a82cU:
        case 0x1a82eU:
        case 0x1a832U:
        case 0x1a834U:
        case 0x1a836U:
        case 0x1a838U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
