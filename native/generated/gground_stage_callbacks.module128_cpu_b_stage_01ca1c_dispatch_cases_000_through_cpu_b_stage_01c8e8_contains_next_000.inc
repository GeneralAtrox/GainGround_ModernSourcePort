
static bool cpu_b_stage_01ca1c_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1ca22U: { // 206d0066 movea.l $66(a5), a0
            next = 0x1ca26U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1ca26U: { // 2068001a movea.l $1a(a0), a0
            next = 0x1ca2aU;
            const auto source_address = r.address[0] + 0x1aU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1ca2aU: { // 3218 move.w (a0)+, d1
            next = 0x1ca2cU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ca2cU: { // c041 and.w d1, d0
            next = 0x1ca2eU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ca2eU: { // d040 add.w d0, d0
            next = 0x1ca30U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ca30U: { // 3200 move.w d0, d1
            next = 0x1ca32U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ca32U: { // d040 add.w d0, d0
            next = 0x1ca34U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ca34U: { // d240 add.w d0, d1
            next = 0x1ca36U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1ca36U: { // d0c1 adda.w d1, a0
            next = 0x1ca38U;
            const auto source_value = r.data[1];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1ca38U: { // 3b580012 move.w (a0)+, $12(a5)
            next = 0x1ca3cU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ca3cU: { // 3b580016 move.w (a0)+, $16(a5)
            next = 0x1ca40U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ca40U: { // 3b58001a move.w (a0)+, $1a(a5)
            next = 0x1ca44U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01ca1c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1ca1cU: { // 4eb900015e28 jsr $15e28.l
            next = 0x1ca22U;
            const auto result = m.call(c, 283U, 0x1ca1cU, 0x15e28U, 0x1ca22U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ca44U: { // 4e75 rts 
            next = 0x1ca46U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1ca44U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01ca1c_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1ca1cU:
        case 0x1ca22U:
        case 0x1ca26U:
        case 0x1ca2aU:
        case 0x1ca2cU:
        case 0x1ca2eU:
        case 0x1ca30U:
        case 0x1ca32U:
        case 0x1ca34U:
        case 0x1ca36U:
        case 0x1ca38U:
        case 0x1ca3cU:
        case 0x1ca40U:
        case 0x1ca44U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01c8e8_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1c8e8U:
    case 0x1c8ecU:
    case 0x1c8f2U:
    case 0x1c8f4U:
    case 0x1c8f8U:
    case 0x1c8faU:
    case 0x1c8feU:
    case 0x1c904U:
    case 0x1c90aU:
    case 0x1c90eU:
    case 0x1c912U:
    case 0x1c918U:
    case 0x1c91aU:
    case 0x1c920U:
    case 0x1c926U:
    case 0x1c92cU:
    case 0x1c932U:
    case 0x1c938U:
    case 0x1c93cU:
    case 0x1c942U:
    case 0x1c946U:
    case 0x1c94aU:
    case 0x1c94eU:
    case 0x1c950U:
    case 0x1c954U:
    case 0x1c958U:
    case 0x1c95aU:
    case 0x1c960U:
    case 0x1c962U:
    case 0x1c968U:
    case 0x1c96aU:
    case 0x1c96eU:
    case 0x1c974U:
    case 0x1c978U:
    case 0x1c97cU:
    case 0x1c982U:
    case 0x1c988U:
    case 0x1c98aU:
    case 0x1c98eU:
    case 0x1c990U:
    case 0x1c994U:
    case 0x1c998U:
    case 0x1c99aU:
    case 0x1c99eU:
    case 0x1c9a2U:
    case 0x1c9a4U:
    case 0x1c9a8U:
    case 0x1c9aaU:
    case 0x1c9b0U:
    case 0x1c9b2U:
    case 0x1c9b4U:
    case 0x1c9baU:
    case 0x1c9bcU:
    case 0x1c9c2U:
    case 0x1c9c4U:
    case 0x1c9c8U:
    case 0x1c9ceU:
    case 0x1c9d4U:
    case 0x1c9d8U:
    case 0x1c9deU:
    case 0x1c9e2U:
    case 0x1c9e4U:
    case 0x1c9eaU:
    case 0x1c9f0U:
    case 0x1c9f6U:
    case 0x1c9fcU:
    case 0x1ca02U:
    case 0x1ca06U:
    case 0x1ca0cU:
    case 0x1ca10U:
    case 0x1ca12U:
    case 0x1ca16U:
    case 0x1ca1aU:
        return true;
    default: return false;
    }
}
