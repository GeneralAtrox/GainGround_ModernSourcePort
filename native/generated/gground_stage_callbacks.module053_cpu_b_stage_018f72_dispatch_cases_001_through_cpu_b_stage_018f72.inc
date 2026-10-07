
static bool cpu_b_stage_018f72_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18ff0U: { // 3d400034 move.w d0, $34(a6)
            next = 0x18ff4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x34U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ff4U: { // 302d000e move.w $e(a5), d0
            next = 0x18ff8U;
            const auto source_address = r.address[5] + 0xeU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ff8U: { // d058 add.w (a0)+, d0
            next = 0x18ffaU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18ffaU: { // 3d40000e move.w d0, $e(a6)
            next = 0x18ffeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18ffeU: { // 3d400012 move.w d0, $12(a6)
            next = 0x19002U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19002U: { // 907c0010 sub.w #$10, d0
            next = 0x19006U;
            const auto source_value = 0x10U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19006U: { // 3d40002a move.w d0, $2a(a6)
            next = 0x1900aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1900aU: { // d07c0020 add.w #$20, d0
            next = 0x1900eU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1900eU: { // 3d40002c move.w d0, $2c(a6)
            next = 0x19012U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19012U: { // 426e002e clr.w $2e(a6)
            next = 0x19016U;
            const auto destination_address = r.address[6] + 0x2eU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19016U: { // 3d7c00200030 move.w #$20, $30(a6)
            next = 0x1901cU;
            const auto value = 0x20U;
            const auto destination_address = r.address[6] + 0x30U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1901cU: { // 422e003f clr.b $3f(a6)
            next = 0x19020U;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x19020U: { // 3d7c00020068 move.w #$2, $68(a6)
            next = 0x19026U;
            const auto value = 0x2U;
            const auto destination_address = r.address[6] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19026U: { // 3d7c00050066 move.w #$5, $66(a6)
            next = 0x1902cU;
            const auto value = 0x5U;
            const auto destination_address = r.address[6] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_018f72(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18f8aU: { // 6100fbbe bsr.w $18b4a
            next = 0x18f8eU;
            const auto result = m.call(c, 889U, 0x18f8aU, 0x18b4aU, 0x18f8eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1902cU: { // 4e75 rts 
            next = 0x1902eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1902cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_018f72_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_018f72_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_018f72_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
