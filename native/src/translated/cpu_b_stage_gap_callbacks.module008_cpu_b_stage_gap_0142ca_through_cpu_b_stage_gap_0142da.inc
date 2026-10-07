
FunctionResult cpu_b_stage_gap_0142ca(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x142caU: { // 086d0000001e bchg.b #$0, $1e(a5)
            next = 0x142d0U;
            const auto v1 = m.byte((r.address[5] + 0x1eU));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 0U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x1eU), (v1 ^ (1U << 0U)) & 0xffU);
            break;
        }
        case 0x142d0U: { // 6708 beq.b $142da
            next = 0x142d2U;
            if ((r.status & 4U) != 0U) { next = 0x142daU; transfer_kind = 1U; }
            break;
        }
        case 0x142d2U: { // 41f900025584 lea.l $25584.l, a0
            next = 0x142d8U;
            r.address[0] = 0x25584U;
            break;
        }
        case 0x142d8U: { // 6006 bra.b $142e0
            next = 0x142daU;
            next = 0x142e0U; transfer_kind = 1U;
            break;
        }
        case 0x142daU: { // 41f900025594 lea.l $25594.l, a0
            next = 0x142e0U;
            r.address[0] = 0x25594U;
            break;
        }
        case 0x142e0U: { // 302d0020 move.w $20(a5), d0
            next = 0x142e4U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x142e4U: { // 20300000 move.l (a0, d0.w), d0
            next = 0x142e8U;
            const auto v1 = m.lng((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x142e8U: { // 7402 moveq #$2, d2
            next = 0x142eaU;
            r.data[2] = 0x2U; m.logic(r.data[2], 32U);
            break;
        }
        case 0x142eaU: { // 4eb900015e4e jsr $15e4e.l
            next = 0x142f0U;
            const auto result = m.indirect_call(c, 0x142eaU, 0x15e4eU, 0x142f0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x142f0U: { // 068000200000 addi.l #$200000, d0
            next = 0x142f6U;
            const auto v1 = m.add(r.data[0], 0x200000U, 32U);
            r.data[0] = v1;
            break;
        }
        case 0x142f6U: { // 51cafff2 dbra d2, $142ea
            next = 0x142faU;
            const auto v1 = static_cast<std::uint16_t>(r.data[2] - 1U); m.dw(2, v1);
            if (v1 != 0xffffU) { next = 0x142eaU; transfer_kind = 1U; }
            break;
        }
        case 0x142faU: { // 4e75 rts 
            next = 0x142fcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x142faU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x142caU:
        case 0x142d0U:
        case 0x142d2U:
        case 0x142d8U:
        case 0x142daU:
        case 0x142e0U:
        case 0x142e4U:
        case 0x142e8U:
        case 0x142eaU:
        case 0x142f0U:
        case 0x142f6U:
        case 0x142faU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_gap_0142da(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x142daU: { // 41f900025594 lea.l $25594.l, a0
            next = 0x142e0U;
            r.address[0] = 0x25594U;
            break;
        }
        case 0x142e0U: { // 302d0020 move.w $20(a5), d0
            next = 0x142e4U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x142e4U: { // 20300000 move.l (a0, d0.w), d0
            next = 0x142e8U;
            const auto v1 = m.lng((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            r.data[0] = v1;
            m.logic(v1, 32U);
            break;
        }
        case 0x142e8U: { // 7402 moveq #$2, d2
            next = 0x142eaU;
            r.data[2] = 0x2U; m.logic(r.data[2], 32U);
            break;
        }
        case 0x142eaU: { // 4eb900015e4e jsr $15e4e.l
            next = 0x142f0U;
            const auto result = m.indirect_call(c, 0x142eaU, 0x15e4eU, 0x142f0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x142f0U: { // 068000200000 addi.l #$200000, d0
            next = 0x142f6U;
            const auto v1 = m.add(r.data[0], 0x200000U, 32U);
            r.data[0] = v1;
            break;
        }
        case 0x142f6U: { // 51cafff2 dbra d2, $142ea
            next = 0x142faU;
            const auto v1 = static_cast<std::uint16_t>(r.data[2] - 1U); m.dw(2, v1);
            if (v1 != 0xffffU) { next = 0x142eaU; transfer_kind = 1U; }
            break;
        }
        case 0x142faU: { // 4e75 rts 
            next = 0x142fcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x142faU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x142daU:
        case 0x142e0U:
        case 0x142e4U:
        case 0x142e8U:
        case 0x142eaU:
        case 0x142f0U:
        case 0x142f6U:
        case 0x142faU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
