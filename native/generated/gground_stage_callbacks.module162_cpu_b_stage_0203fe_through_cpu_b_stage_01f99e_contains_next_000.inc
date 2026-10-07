
FunctionResult cpu_b_stage_0203fe(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x203feU: { // 41f83400 lea.l $3400.w, a0
            next = 0x20402U;
            const auto source_address = 0x3400U;
            r.address[0] = source_address;
            break;
        }
        case 0x20402U: { // 2028000a move.l $a(a0), d0
            next = 0x20406U;
            const auto source_address = r.address[0] + 0xaU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x20406U: { // 028001010100 andi.l #$1010100, d0
            next = 0x2040cU;
            r.data[0] &= 0x1010100U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2040cU: { // 6602 bne.b $20410
            next = 0x2040eU;
            if ((r.status & 4U) == 0U) { next = 0x20410U; transfer_kind = 1U; }
            break;
        }
        case 0x2040eU: { // 4e75 rts 
            next = 0x20410U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2040eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20410U: { // 61000012 bsr.w $20424
            next = 0x20414U;
            const auto result = m.call(c, 696U, 0x20410U, 0x20424U, 0x20414U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20414U: { // 02400003 andi.w #$3, d0
            next = 0x20418U;
            const auto source_value = 0x3U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x20418U: { // 06400009 addi.w #$9, d0
            next = 0x2041cU;
            const auto source_value = 0x9U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x2041cU: { // 4a300000 tst.b (a0, d0.w)
            next = 0x20420U;
            const auto destination_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20420U: { // 67dc beq.b $203fe
            next = 0x20422U;
            if ((r.status & 4U) != 0U) { next = 0x203feU; transfer_kind = 1U; }
            break;
        }
        case 0x20422U: { // 4e75 rts 
            next = 0x20424U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20422U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x203feU:
        case 0x20402U:
        case 0x20406U:
        case 0x2040cU:
        case 0x2040eU:
        case 0x20410U:
        case 0x20414U:
        case 0x20418U:
        case 0x2041cU:
        case 0x20420U:
        case 0x20422U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01f96c_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1f970U: { // 206d0066 movea.l $66(a5), a0
            next = 0x1f974U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1f974U: { // 2068001e movea.l $1e(a0), a0
            next = 0x1f978U;
            const auto source_address = r.address[0] + 0x1eU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1f980U: { // 523900003436 addq.b #$1, $3436.l
            next = 0x1f986U;
            const auto source_value = 0x1U;
            const auto destination_address = 0x3436U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1f986U: { // 08ee00010078 bset.b #$1, $78(a6)
            next = 0x1f98cU;
            const auto destination_address = r.address[6] + 0x78U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f98cU: { // 08ee00030041 bset.b #$3, $41(a6)
            next = 0x1f992U;
            const auto destination_address = r.address[6] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f992U: { // 2d6e00480006 move.l $48(a6), $6(a6)
            next = 0x1f998U;
            const auto source_address = r.address[6] + 0x48U;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[6] + 0x6U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1f998U: { // 5dae0006 subq.l #$6, $6(a6)
            next = 0x1f99cU;
            const auto source_value = 0x6U;
            const auto destination_address = r.address[6] + 0x6U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.sub(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01f96c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1f96cU: { // 610000ca bsr.w $1fa38
            next = 0x1f970U;
            const auto result = m.call(c, 690U, 0x1f96cU, 0x1fa38U, 0x1f970U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f978U: { // 6100c040 bsr.w $1b9ba
            next = 0x1f97cU;
            const auto result = m.call(c, 325U, 0x1f978U, 0x1b9baU, 0x1f97cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f97cU: { // 61000a80 bsr.w $203fe
            next = 0x1f980U;
            const auto result = m.call(c, 695U, 0x1f97cU, 0x203feU, 0x1f980U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f99cU: { // 4e75 rts 
            next = 0x1f99eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f99cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01f96c_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1f96cU:
        case 0x1f970U:
        case 0x1f974U:
        case 0x1f978U:
        case 0x1f97cU:
        case 0x1f980U:
        case 0x1f986U:
        case 0x1f98cU:
        case 0x1f992U:
        case 0x1f998U:
        case 0x1f99cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01f99e_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1f99eU:
    case 0x1f9a2U:
    case 0x1f9a6U:
    case 0x1f9a8U:
    case 0x1f9aaU:
    case 0x1f9acU:
    case 0x1f9aeU:
    case 0x1f9b4U:
    case 0x1f9b6U:
    case 0x1f9baU:
    case 0x1f9bcU:
    case 0x1f9c0U:
    case 0x1f9c4U:
    case 0x1f9c8U:
    case 0x1f9ceU:
    case 0x1f9d2U:
    case 0x1f9d4U:
    case 0x1f9d8U:
    case 0x1f9dcU:
    case 0x1f9e0U:
    case 0x1f9e2U:
    case 0x1f9e4U:
    case 0x1f9e8U:
    case 0x1f9eaU:
    case 0x1f9eeU:
    case 0x1f9f2U:
    case 0x1f9f6U:
    case 0x1f9faU:
    case 0x1f9feU:
    case 0x1fa00U:
    case 0x1fa04U:
    case 0x1fa08U:
    case 0x1fa0cU:
    case 0x1fa10U:
    case 0x1fa14U:
    case 0x1fa1aU:
    case 0x1fa1cU:
    case 0x1fa20U:
    case 0x1fa22U:
    case 0x1fa26U:
    case 0x1fa28U:
    case 0x1fa2cU:
    case 0x1fa2eU:
    case 0x1fa32U:
    case 0x1fa36U:
        return true;
    default: return false;
    }
}
