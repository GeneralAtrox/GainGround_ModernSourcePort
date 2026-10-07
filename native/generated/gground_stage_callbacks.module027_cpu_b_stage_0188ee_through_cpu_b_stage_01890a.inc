
FunctionResult cpu_b_stage_0188ee(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x188eeU: { // 3419 move.w (a1)+, d2
            next = 0x188f0U;
            const auto source_address = r.address[1];
            const auto value = m.word(source_address);
            r.address[1] += 2U;
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188f0U: { // 3619 move.w (a1)+, d3
            next = 0x188f2U;
            const auto source_address = r.address[1];
            const auto value = m.word(source_address);
            r.address[1] += 2U;
            m.dw(3U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188f2U: { // 45d0 lea.l (a0), a2
            next = 0x188f4U;
            const auto source_address = r.address[0];
            r.address[2] = source_address;
            break;
        }
        case 0x188f4U: { // 3202 move.w d2, d1
            next = 0x188f6U;
            const auto value = r.data[2];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188f6U: { // 3019 move.w (a1)+, d0
            next = 0x188f8U;
            const auto source_address = r.address[1];
            const auto value = m.word(source_address);
            r.address[1] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x188f8U: { // 8044 or.w d4, d0
            next = 0x188faU;
            const auto source_value = r.data[4];
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x188faU: { // 34c0 move.w d0, (a2)+
            next = 0x188fcU;
            const auto value = r.data[0];
            const auto destination_address = r.address[2];
            m.word(destination_address, value);
            r.address[2] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0x188fcU: { // 51c9fff8 dbra d1, $188f6
            next = 0x18900U;
            m.dw(1U, r.data[1] - 1U);
            if ((r.data[1] & 0xffffU) != 0xffffU) next = 0x188f6U;
            break;
        }
        case 0x18900U: { // 41e80080 lea.l $80(a0), a0
            next = 0x18904U;
            const auto source_address = r.address[0] + 0x80U;
            r.address[0] = source_address;
            break;
        }
        case 0x18904U: { // 51cbffec dbra d3, $188f2
            next = 0x18908U;
            m.dw(3U, r.data[3] - 1U);
            if ((r.data[3] & 0xffffU) != 0xffffU) next = 0x188f2U;
            break;
        }
        case 0x18908U: { // 4e75 rts 
            next = 0x1890aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18908U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x188eeU:
        case 0x188f0U:
        case 0x188f2U:
        case 0x188f4U:
        case 0x188f6U:
        case 0x188f8U:
        case 0x188faU:
        case 0x188fcU:
        case 0x18900U:
        case 0x18904U:
        case 0x18908U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0188dc(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x188dcU: { // 41f900200000 lea.l $200000.l, a0
            next = 0x188e2U;
            const auto source_address = 0x200000U;
            r.address[0] = source_address;
            break;
        }
        case 0x188e2U: { // d0ed0062 adda.w $62(a5), a0
            next = 0x188e6U;
            const auto source_address = r.address[5] + 0x62U;
            const auto source_value = m.word(source_address);
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x188e6U: { // 43fa2304 lea.l $1abec(pc), a1
            next = 0x188eaU;
            const auto source_address = 0x1abecU;
            r.address[1] = source_address;
            break;
        }
        case 0x188eaU: { // 382d0060 move.w $60(a5), d4
            next = 0x188eeU;
            const auto source_address = r.address[5] + 0x60U;
            const auto value = m.word(source_address);
            m.dw(4U, value);
            m.logic(value, 16U);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x188dcU:
        case 0x188e2U:
        case 0x188e6U:
        case 0x188eaU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_01890a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1890aU: { // 30380c14 move.w $c14.w, d0
            next = 0x1890eU;
            const auto source_address = 0xc14U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1890eU: { // 7201 moveq #$1, d1
            next = 0x18910U;
            r.data[1] = 0x1U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x18910U: { // 44fc0000 move.w #$0, ccr
            next = 0x18914U;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
        case 0x18914U: { // 8101 sbcd.b d1, d0
            next = 0x18916U;
            const auto source_value = r.data[1];
            const auto value = m.sbcd(r.data[0], source_value);
            m.db(0U, value);
            break;
        }
        case 0x18916U: { // 31c00c14 move.w d0, $c14.w
            next = 0x1891aU;
            const auto value = r.data[0];
            const auto destination_address = 0xc14U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1891aU: { // 4e75 rts 
            next = 0x1891cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1891aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1890aU:
        case 0x1890eU:
        case 0x18910U:
        case 0x18914U:
        case 0x18916U:
        case 0x1891aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
