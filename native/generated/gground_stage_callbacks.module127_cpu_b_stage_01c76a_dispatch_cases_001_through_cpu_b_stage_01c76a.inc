
static bool cpu_b_stage_01c76a_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c7fcU: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c802U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c804U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1c808U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c808U: { // 6704 beq.b $1c80e
            next = 0x1c80aU;
            if ((r.status & 4U) != 0U) { next = 0x1c80eU; transfer_kind = 1U; }
            break;
        }
        case 0x1c80aU: { // 600011de bra.w $1d9ea
            next = 0x1c80eU;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1c80eU: { // 4a6d005a tst.w $5a(a5)
            next = 0x1c812U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1c812U: { // 6706 beq.b $1c81a
            next = 0x1c814U;
            if ((r.status & 4U) != 0U) { next = 0x1c81aU; transfer_kind = 1U; }
            break;
        }
        case 0x1c814U: { // 536d005a subq.w #$1, $5a(a5)
            next = 0x1c818U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c81aU: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1c820U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c820U: { // 6702 beq.b $1c824
            next = 0x1c822U;
            if ((r.status & 4U) != 0U) { next = 0x1c824U; transfer_kind = 1U; }
            break;
        }
        case 0x1c824U: { // 08ad00030041 bclr.b #$3, $41(a5)
            next = 0x1c82aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c82aU: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1c830U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c830U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1c836U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c836U: { // 1b7c00000060 move.b #$0, $60(a5)
            next = 0x1c83cU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x60U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c83cU: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c842U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c842U: { // 3b7c000e0058 move.w #$e, $58(a5)
            next = 0x1c848U;
            const auto value = 0xeU;
            const auto destination_address = r.address[5] + 0x58U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c848U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1c84eU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c84eU: { // 206d006e movea.l $6e(a5), a0
            next = 0x1c852U;
            const auto source_address = r.address[5] + 0x6eU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c852U: { // 7000 moveq #$0, d0
            next = 0x1c854U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1c854U: { // 10280007 move.b $7(a0), d0
            next = 0x1c858U;
            const auto source_address = r.address[0] + 0x7U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c858U: { // 3b40005a move.w d0, $5a(a5)
            next = 0x1c85cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01c76a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c79aU: { // 4e75 rts 
            next = 0x1c79cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c79aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c7baU: { // 610000a2 bsr.w $1c85e
            next = 0x1c7beU;
            const auto result = m.call(c, 660U, 0x1c7baU, 0x1c85eU, 0x1c7beU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1c7daU: { // 4e75 rts 
            next = 0x1c7dcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c7daU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c802U: { // 4e75 rts 
            next = 0x1c804U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c802U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c818U: { // 4e75 rts 
            next = 0x1c81aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c818U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c822U: { // 4e75 rts 
            next = 0x1c824U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c822U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c85cU: { // 4e75 rts 
            next = 0x1c85eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c85cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01c76a_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01c76a_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01c76a_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
