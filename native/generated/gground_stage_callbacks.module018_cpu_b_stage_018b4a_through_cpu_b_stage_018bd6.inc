
FunctionResult cpu_b_stage_018b4a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18b4aU: { // 303c002f move.w #$2f, d0
            next = 0x18b4eU;
            const auto value = 0x2fU;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18b4eU: { // 4df85400 lea.l $5400.w, a6
            next = 0x18b52U;
            const auto source_address = 0x5400U;
            r.address[6] = source_address;
            break;
        }
        case 0x18b52U: { // 4a16 tst.b (a6)
            next = 0x18b54U;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x18b54U: { // 6a08 bpl.b $18b5e
            next = 0x18b56U;
            if ((r.status & 8U) == 0U) { next = 0x18b5eU; transfer_kind = 1U; }
            break;
        }
        case 0x18b56U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x18b5aU;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x18b5aU: { // 51c8fff6 dbra d0, $18b52
            next = 0x18b5eU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x18b52U;
            break;
        }
        case 0x18b5eU: { // 4e75 rts 
            next = 0x18b60U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18b5eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18b4aU:
        case 0x18b4eU:
        case 0x18b52U:
        case 0x18b54U:
        case 0x18b56U:
        case 0x18b5aU:
        case 0x18b5eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_018bde(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18bdeU: { // 512d0010 subq.b #$8, $10(a5)
            next = 0x18be2U;
            const auto source_value = 0x8U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x18be2U: { // 6f06 ble.b $18bea
            next = 0x18be4U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x18beaU; transfer_kind = 1U; }
            break;
        }
        case 0x18be4U: { // 512d0011 subq.b #$8, $11(a5)
            next = 0x18be8U;
            const auto source_value = 0x8U;
            const auto destination_address = r.address[5] + 0x11U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x18be8U: { // 6e06 bgt.b $18bf0
            next = 0x18beaU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18bf0U; transfer_kind = 1U; }
            break;
        }
        case 0x18beaU: { // 08950007 bclr.b #$7, (a5)
            next = 0x18beeU;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x18beeU: { // 4e75 rts 
            next = 0x18bf0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18beeU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18bf0U: { // 08950000 bclr.b #$0, (a5)
            next = 0x18bf4U;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x18bf4U: { // 4eb900015d24 jsr $15d24.l
            next = 0x18bfaU;
            const auto result = m.call(c, 280U, 0x18bf4U, 0x15d24U, 0x18bfaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18bfaU: { // 4eb900015df2 jsr $15df2.l
            next = 0x18c00U;
            const auto result = m.call(c, 282U, 0x18bfaU, 0x15df2U, 0x18c00U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c00U: { // 4e75 rts 
            next = 0x18c02U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18c00U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18bdeU:
        case 0x18be2U:
        case 0x18be4U:
        case 0x18be8U:
        case 0x18beaU:
        case 0x18beeU:
        case 0x18bf0U:
        case 0x18bf4U:
        case 0x18bfaU:
        case 0x18c00U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_018bd6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18bd6U: { // 2b7c00018bde0002 move.l #$18bde, $2(a5)
            next = 0x18bdeU;
            const auto value = 0x18bdeU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18bd6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
