
FunctionResult cpu_b_stage_01a3b4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a3bcU: { // 4eb90001a814 jsr $1a814.l
            next = 0x1a3c2U;
            const auto result = m.call(c, 943U, 0x1a3bcU, 0x1a814U, 0x1a3c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a3eaU: { // 4eb90001631a jsr $1631a.l
            next = 0x1a3f0U;
            const auto result = m.call(c, 304U, 0x1a3eaU, 0x1631aU, 0x1a3f0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a3f0U: { // 4eb9000163ba jsr $163ba.l
            next = 0x1a3f6U;
            const auto result = m.call(c, 867U, 0x1a3f0U, 0x163baU, 0x1a3f6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a42eU: { // 4eb900016372 jsr $16372.l
            next = 0x1a434U;
            const auto result = m.call(c, 306U, 0x1a42eU, 0x16372U, 0x1a434U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a48cU: { // 4eb900015d24 jsr $15d24.l
            next = 0x1a492U;
            const auto result = m.call(c, 280U, 0x1a48cU, 0x15d24U, 0x1a492U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a496U: { // 6100ef02 bsr.w $1939a
            next = 0x1a49aU;
            const auto result = m.call(c, 904U, 0x1a496U, 0x1939aU, 0x1a49aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a49aU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a4a0U;
            const auto result = m.call(c, 282U, 0x1a49aU, 0x15df2U, 0x1a4a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a4a0U: { // 4e75 rts 
            next = 0x1a4a2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a4a0U, r.program_counter)) return *event;
            return result;
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
        default:
            if (!(cpu_b_stage_01a3b4_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01a3b4_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01a3b4_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_01a500_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a500U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x1a504U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a504U: { // 6e2e bgt.b $1a534
            next = 0x1a506U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a534U; transfer_kind = 1U; }
            break;
        }
        case 0x1a506U: { // 3b7c00030068 move.w #$3, $68(a5)
            next = 0x1a50cU;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a50cU: { // 5c6d0064 addq.w #$6, $64(a5)
            next = 0x1a510U;
            const auto source_value = 0x6U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a510U: { // 0c6d002a0064 cmpi.w #$2a, $64(a5)
            next = 0x1a516U;
            const auto source_value = 0x2aU;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a516U: { // 6b04 bmi.b $1a51c
            next = 0x1a518U;
            if ((r.status & 8U) != 0U) { next = 0x1a51cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a518U: { // 4255 clr.w (a5)
            next = 0x1a51aU;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a51cU: { // 302d0064 move.w $64(a5), d0
            next = 0x1a520U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a520U: { // 41fa1270 lea.l $1b792(pc), a0
            next = 0x1a524U;
            const auto source_address = 0x1b792U;
            r.address[0] = source_address;
            break;
        }
        case 0x1a524U: { // d0c0 adda.w d0, a0
            next = 0x1a526U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1a526U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1a52aU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a52aU: { // 3b580010 move.w (a0)+, $10(a5)
            next = 0x1a52eU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a52eU: { // 3018 move.w (a0)+, d0
            next = 0x1a530U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a530U: { // d16d000c add.w d0, $c(a5)
            next = 0x1a534U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0xcU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01a500(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a51aU: { // 4e75 rts 
            next = 0x1a51cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a51aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a534U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a53aU;
            const auto result = m.call(c, 282U, 0x1a534U, 0x15df2U, 0x1a53aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a53aU: { // 4e75 rts 
            next = 0x1a53cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a53aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01a500_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a500U:
        case 0x1a504U:
        case 0x1a506U:
        case 0x1a50cU:
        case 0x1a510U:
        case 0x1a516U:
        case 0x1a518U:
        case 0x1a51aU:
        case 0x1a51cU:
        case 0x1a520U:
        case 0x1a524U:
        case 0x1a526U:
        case 0x1a52aU:
        case 0x1a52eU:
        case 0x1a530U:
        case 0x1a534U:
        case 0x1a53aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
