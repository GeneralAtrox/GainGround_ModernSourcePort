
static bool cpu_b_stage_0191a8_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19230U: { // b02e000a cmp.b $a(a6), d0
            next = 0x19234U;
            const auto source_address = r.address[6] + 0xaU;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x19234U: { // 6700004a beq.w $19280
            next = 0x19238U;
            if ((r.status & 4U) != 0U) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x19238U: { // 4a2e003f tst.b $3f(a6)
            next = 0x1923cU;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1923cU: { // 66000042 bne.w $19280
            next = 0x19240U;
            if ((r.status & 4U) == 0U) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x19240U: { // 302e002a move.w $2a(a6), d0
            next = 0x19244U;
            const auto source_address = r.address[6] + 0x2aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19244U: { // 322e002c move.w $2c(a6), d1
            next = 0x19248U;
            const auto source_address = r.address[6] + 0x2cU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19248U: { // 342e0032 move.w $32(a6), d2
            next = 0x1924cU;
            const auto source_address = r.address[6] + 0x32U;
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1924cU: { // 362e0034 move.w $34(a6), d3
            next = 0x19250U;
            const auto source_address = r.address[6] + 0x34U;
            const auto value = m.word(source_address);
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19254U: { // 652e bcs.b $19284
            next = 0x19256U;
            if ((r.status & 1U) != 0U) { next = 0x19284U; transfer_kind = 1U; }
            break;
        }
        case 0x19256U: { // 6028 bra.b $19280
            next = 0x19258U;
            if (true) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x19258U: { // 4a2e003f tst.b $3f(a6)
            next = 0x1925cU;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1925cU: { // 6622 bne.b $19280
            next = 0x1925eU;
            if ((r.status & 4U) == 0U) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x1925eU: { // 302e0012 move.w $12(a6), d0
            next = 0x19262U;
            const auto source_address = r.address[6] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19262U: { // 3200 move.w d0, d1
            next = 0x19264U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19264U: { // 907c000a sub.w #$a, d0
            next = 0x19268U;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19268U: { // d27c000a add.w #$a, d1
            next = 0x1926cU;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1926cU: { // 342e001a move.w $1a(a6), d2
            next = 0x19270U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19270U: { // 3602 move.w d2, d3
            next = 0x19272U;
            const auto value = r.data[2];
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19272U: { // 947c0009 sub.w #$9, d2
            next = 0x19276U;
            const auto source_value = 0x9U;
            const auto destination_value = r.data[2];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x19276U: { // d67c0009 add.w #$9, d3
            next = 0x1927aU;
            const auto source_value = 0x9U;
            const auto destination_value = r.data[3];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(3U, value);
            break;
        }
        case 0x1927eU: { // 6504 bcs.b $19284
            next = 0x19280U;
            if ((r.status & 1U) != 0U) { next = 0x19284U; transfer_kind = 1U; }
            break;
        }
        case 0x19280U: { // 51cfff5a dbra d7, $191dc
            next = 0x19284U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x191dcU;
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_0191a8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19250U: { // 61000034 bsr.w $19286
            next = 0x19254U;
            const auto result = m.call(c, 902U, 0x19250U, 0x19286U, 0x19254U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1927aU: { // 6100000a bsr.w $19286
            next = 0x1927eU;
            const auto result = m.call(c, 902U, 0x1927aU, 0x19286U, 0x1927eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19284U: { // 4e75 rts 
            next = 0x19286U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19284U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_0191a8_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_0191a8_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_0191a8_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_0192dc_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x192dcU:
    case 0x192e0U:
    case 0x192e2U:
    case 0x192e6U:
    case 0x192e8U:
    case 0x192eaU:
    case 0x192ecU:
    case 0x192eeU:
    case 0x192f2U:
    case 0x192f4U:
    case 0x192f8U:
    case 0x192faU:
    case 0x192feU:
    case 0x19302U:
    case 0x19306U:
    case 0x19308U:
    case 0x1930cU:
    case 0x1930eU:
    case 0x19312U:
    case 0x19314U:
    case 0x19316U:
    case 0x1931aU:
    case 0x1931cU:
    case 0x19320U:
    case 0x19322U:
    case 0x19326U:
    case 0x1932aU:
    case 0x1932cU:
    case 0x1932eU:
    case 0x19330U:
    case 0x19334U:
    case 0x19336U:
    case 0x19338U:
    case 0x1933eU:
    case 0x19342U:
    case 0x19344U:
    case 0x19346U:
    case 0x1934aU:
    case 0x1934cU:
    case 0x1934eU:
    case 0x19354U:
    case 0x19358U:
    case 0x1935aU:
    case 0x1935eU:
    case 0x19360U:
    case 0x19362U:
    case 0x19368U:
    case 0x1936cU:
    case 0x1936eU:
    case 0x19370U:
    case 0x19376U:
    case 0x1937aU:
    case 0x1937cU:
    case 0x19380U:
    case 0x19384U:
    case 0x19388U:
    case 0x1938cU:
    case 0x1938eU:
    case 0x19392U:
    case 0x19396U:
        return true;
    default: return false;
    }
}
