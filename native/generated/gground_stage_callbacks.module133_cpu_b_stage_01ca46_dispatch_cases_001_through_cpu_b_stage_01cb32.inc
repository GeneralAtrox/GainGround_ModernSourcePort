
static bool cpu_b_stage_01ca46_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1cac4U: { // 08ad00070040 bclr.b #$7, $40(a5)
            next = 0x1cacaU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cacaU: { // 08ad00060040 bclr.b #$6, $40(a5)
            next = 0x1cad0U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x6U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cad0U: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1cad6U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cad6U: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1cadcU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cadcU: { // 08ed00020040 bset.b #$2, $40(a5)
            next = 0x1cae2U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cae2U: { // 302d005c move.w $5c(a5), d0
            next = 0x1cae6U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cae6U: { // 02400400 andi.w #$400, d0
            next = 0x1caeaU;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1caeaU: { // 3b40005c move.w d0, $5c(a5)
            next = 0x1caeeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1caf0U: { // 08ad00070040 bclr.b #$7, $40(a5)
            next = 0x1caf6U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1caf6U: { // 08ad00060040 bclr.b #$6, $40(a5)
            next = 0x1cafcU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x6U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cafcU: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1cb02U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cb02U: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1cb08U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cb08U: { // 08ed00020040 bset.b #$2, $40(a5)
            next = 0x1cb0eU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cb0eU: { // 3b7c0000005c move.w #$0, $5c(a5)
            next = 0x1cb14U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cb14U: { // 0c6d00c00012 cmpi.w #$c0, $12(a5)
            next = 0x1cb1aU;
            const auto source_value = 0xc0U;
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1cb1aU: { // 6e02 bgt.b $1cb1e
            next = 0x1cb1cU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1cb1eU; transfer_kind = 1U; }
            break;
        }
        case 0x1cb1eU: { // 3b7c0400005c move.w #$400, $5c(a5)
            next = 0x1cb24U;
            const auto value = 0x400U;
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cb26U: { // 206d006e movea.l $6e(a5), a0
            next = 0x1cb2aU;
            const auto source_address = r.address[5] + 0x6eU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1cb2aU: { // 1b6800060059 move.b $6(a0), $59(a5)
            next = 0x1cb30U;
            const auto source_address = r.address[0] + 0x6U;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x59U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01ca46(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1ca74U: { // 4e75 rts 
            next = 0x1ca76U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1ca74U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1caa6U: { // 4e75 rts 
            next = 0x1caa8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1caa6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cac2U: { // 4e75 rts 
            next = 0x1cac4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cac2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1caeeU: { // 4e75 rts 
            next = 0x1caf0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1caeeU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cb1cU: { // 4e75 rts 
            next = 0x1cb1eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cb1cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cb24U: { // 4e75 rts 
            next = 0x1cb26U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cb24U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cb30U: { // 4e75 rts 
            next = 0x1cb32U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cb30U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01ca46_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01ca46_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01ca46_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_01cb32_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1cb32U: { // 082d00000078 btst.b #$0, $78(a5)
            next = 0x1cb38U;
            const auto destination_address = r.address[5] + 0x78U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1cb38U: { // 6706 beq.b $1cb40
            next = 0x1cb3aU;
            if ((r.status & 4U) != 0U) { next = 0x1cb40U; transfer_kind = 1U; }
            break;
        }
        case 0x1cb3aU: { // 066d08080036 addi.w #$808, $36(a5)
            next = 0x1cb40U;
            const auto source_value = 0x808U;
            const auto destination_address = r.address[5] + 0x36U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01cb32(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1cb40U: { // 4e75 rts 
            next = 0x1cb42U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cb40U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01cb32_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1cb32U:
        case 0x1cb38U:
        case 0x1cb3aU:
        case 0x1cb40U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
