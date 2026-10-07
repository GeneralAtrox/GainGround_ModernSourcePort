
static bool cpu_b_stage_01c652_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c6f2U: { // 6704 beq.b $1c6f8
            next = 0x1c6f4U;
            if ((r.status & 4U) != 0U) { next = 0x1c6f8U; transfer_kind = 1U; }
            break;
        }
        case 0x1c6f4U: { // 600012f4 bra.w $1d9ea
            next = 0x1c6f8U;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1c6f8U: { // 4a6d005a tst.w $5a(a5)
            next = 0x1c6fcU;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1c6fcU: { // 6706 beq.b $1c704
            next = 0x1c6feU;
            if ((r.status & 4U) != 0U) { next = 0x1c704U; transfer_kind = 1U; }
            break;
        }
        case 0x1c6feU: { // 536d005a subq.w #$1, $5a(a5)
            next = 0x1c702U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c704U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1c70aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c70aU: { // 6702 beq.b $1c70e
            next = 0x1c70cU;
            if ((r.status & 4U) != 0U) { next = 0x1c70eU; transfer_kind = 1U; }
            break;
        }
        case 0x1c70eU: { // 08ad00030041 bclr.b #$3, $41(a5)
            next = 0x1c714U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c714U: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1c71aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c71aU: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1c720U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c720U: { // 1b7c00000060 move.b #$0, $60(a5)
            next = 0x1c726U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x60U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c726U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c72cU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c72cU: { // 3b7c000e0058 move.w #$e, $58(a5)
            next = 0x1c732U;
            const auto value = 0xeU;
            const auto destination_address = r.address[5] + 0x58U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c732U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1c738U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01c652(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c668U: { // 61001298 bsr.w $1d902
            next = 0x1c66cU;
            const auto result = m.call(c, 344U, 0x1c668U, 0x1d902U, 0x1c66cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1c68cU: { // 4e75 rts 
            next = 0x1c68eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c68cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c6acU: { // 6100008c bsr.w $1c73a
            next = 0x1c6b0U;
            const auto result = m.call(c, 658U, 0x1c6acU, 0x1c73aU, 0x1c6b0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1c6c4U: { // 4e75 rts 
            next = 0x1c6c6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c6c4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c6ecU: { // 4e75 rts 
            next = 0x1c6eeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c6ecU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c702U: { // 4e75 rts 
            next = 0x1c704U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c702U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c70cU: { // 4e75 rts 
            next = 0x1c70eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c70cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c738U: { // 4e75 rts 
            next = 0x1c73aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c738U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01c652_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01c652_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01c652_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
