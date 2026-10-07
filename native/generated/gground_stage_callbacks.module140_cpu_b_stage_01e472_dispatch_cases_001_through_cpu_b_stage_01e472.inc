
static bool cpu_b_stage_01e472_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e4e6U: { // d0fc0002 adda.w #$2, a0
            next = 0x1e4eaU;
            const auto source_value = 0x2U;
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1e4eaU: { // 303c0c00 move.w #$c00, d0
            next = 0x1e4eeU;
            const auto value = 0xc00U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e4eeU: { // d018 add.b (a0)+, d0
            next = 0x1e4f0U;
            const auto source_address = r.address[0];
            const auto source_value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x1e4f0U: { // 3b400008 move.w d0, $8(a5)
            next = 0x1e4f4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e4f4U: { // 102d0001 move.b $1(a5), d0
            next = 0x1e4f8U;
            const auto source_address = r.address[5] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e4f8U: { // 0200fffc andi.b #$fc, d0
            next = 0x1e4fcU;
            const auto source_value = 0xfcU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x1e4fcU: { // 8018 or.b (a0)+, d0
            next = 0x1e4feU;
            const auto source_address = r.address[0];
            const auto source_value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x1e4feU: { // 1b400001 move.b d0, $1(a5)
            next = 0x1e502U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01e472(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e4b2U: { // 6100073c bsr.w $1ebf0
            next = 0x1e4b6U;
            const auto result = m.call(c, 363U, 0x1e4b2U, 0x1ebf0U, 0x1e4b6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e502U: { // 6100fc54 bsr.w $1e158
            next = 0x1e506U;
            const auto result = m.call(c, 358U, 0x1e502U, 0x1e158U, 0x1e506U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e506U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1e50cU;
            const auto result = m.call(c, 280U, 0x1e506U, 0x15d24U, 0x1e50cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e50cU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1e512U;
            const auto result = m.call(c, 282U, 0x1e50cU, 0x15df2U, 0x1e512U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e512U: { // 4e75 rts 
            next = 0x1e514U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1e512U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01e472_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01e472_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01e472_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
