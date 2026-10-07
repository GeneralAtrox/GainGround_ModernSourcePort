
FunctionResult cpu_b_stage_019f82(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19f82U: { // 203c00010000 move.l #$10000, d0
            next = 0x19f88U;
            const auto value = 0x10000U;
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x19f88U: { // 322d0048 move.w $48(a5), d1
            next = 0x19f8cU;
            const auto source_address = r.address[5] + 0x48U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19f8cU: { // 670c beq.b $19f9a
            next = 0x19f8eU;
            if ((r.status & 4U) != 0U) { next = 0x19f9aU; transfer_kind = 1U; }
            break;
        }
        case 0x19f8eU: { // 5541 subq.w #$2, d1
            next = 0x19f90U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x19f90U: { // 6706 beq.b $19f98
            next = 0x19f92U;
            if ((r.status & 4U) != 0U) { next = 0x19f98U; transfer_kind = 1U; }
            break;
        }
        case 0x19f92U: { // 42ad0026 clr.l $26(a5)
            next = 0x19f96U;
            const auto destination_address = r.address[5] + 0x26U;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0x19f96U: { // 4e75 rts 
            next = 0x19f98U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19f96U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19f98U: { // 4480 neg.l d0
            next = 0x19f9aU;
            const auto old = r.data[0];
            const auto value = m.sub(0U, old, 32U);
            r.data[0] = value;
            break;
        }
        case 0x19f9aU: { // 2b400026 move.l d0, $26(a5)
            next = 0x19f9eU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19f9eU: { // 4e75 rts 
            next = 0x19fa0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19f9eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19f82U:
        case 0x19f88U:
        case 0x19f8cU:
        case 0x19f8eU:
        case 0x19f90U:
        case 0x19f92U:
        case 0x19f96U:
        case 0x19f98U:
        case 0x19f9aU:
        case 0x19f9eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_019fa0_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19fa0U: { // 4a2d006a tst.b $6a(a5)
            next = 0x19fa4U;
            const auto destination_address = r.address[5] + 0x6aU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x19fa4U: { // 6706 beq.b $19fac
            next = 0x19fa6U;
            if ((r.status & 4U) != 0U) { next = 0x19facU; transfer_kind = 1U; }
            break;
        }
        case 0x19fa6U: { // 422d006a clr.b $6a(a5)
            next = 0x19faaU;
            const auto destination_address = r.address[5] + 0x6aU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x19facU: { // 302d006c move.w $6c(a5), d0
            next = 0x19fb0U;
            const auto source_address = r.address[5] + 0x6cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19fb0U: { // 906d0046 sub.w $46(a5), d0
            next = 0x19fb4U;
            const auto source_address = r.address[5] + 0x46U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19fb4U: { // 6714 beq.b $19fca
            next = 0x19fb6U;
            if ((r.status & 4U) != 0U) { next = 0x19fcaU; transfer_kind = 1U; }
            break;
        }
        case 0x19fb6U: { // 4aed006a tas.b $6a(a5)
            next = 0x19fbaU;
            const auto destination_address = r.address[5] + 0x6aU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x19fbaU: { // d12d0051 add.b d0, $51(a5)
            next = 0x19fbeU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x51U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x19fbeU: { // 6404 bcc.b $19fc4
            next = 0x19fc0U;
            if ((r.status & 1U) == 0U) { next = 0x19fc4U; transfer_kind = 1U; }
            break;
        }
        case 0x19fc0U: { // 50ed0051 st.b $51(a5)
            next = 0x19fc4U;
            const auto destination_address = r.address[5] + 0x51U;
            (void)m.byte(destination_address);
            m.byte(destination_address, 0xffU);
            break;
        }
        case 0x19fc4U: { // 3b6d0046006c move.w $46(a5), $6c(a5)
            next = 0x19fcaU;
            const auto source_address = r.address[5] + 0x46U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_019fa0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19faaU: { // 4e75 rts 
            next = 0x19facU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19faaU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19fcaU: { // 4e75 rts 
            next = 0x19fccU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19fcaU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_019fa0_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19fa0U:
        case 0x19fa4U:
        case 0x19fa6U:
        case 0x19faaU:
        case 0x19facU:
        case 0x19fb0U:
        case 0x19fb4U:
        case 0x19fb6U:
        case 0x19fbaU:
        case 0x19fbeU:
        case 0x19fc0U:
        case 0x19fc4U:
        case 0x19fcaU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
