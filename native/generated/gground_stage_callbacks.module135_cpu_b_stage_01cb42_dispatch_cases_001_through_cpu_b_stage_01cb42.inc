
static bool cpu_b_stage_01cb42_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1cbdeU: { // 522d0058 addq.b #$1, $58(a5)
            next = 0x1cbe2U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1cbe2U: { // 3b7c00000074 move.w #$0, $74(a5)
            next = 0x1cbe8U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cbe8U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1cbeeU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cbf0U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1cbf4U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1cbf4U: { // 6704 beq.b $1cbfa
            next = 0x1cbf6U;
            if ((r.status & 4U) != 0U) { next = 0x1cbfaU; transfer_kind = 1U; }
            break;
        }
        case 0x1cbf6U: { // 60000df2 bra.w $1d9ea
            next = 0x1cbfaU;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1cbfaU: { // 4a6d005a tst.w $5a(a5)
            next = 0x1cbfeU;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1cbfeU: { // 6706 beq.b $1cc06
            next = 0x1cc00U;
            if ((r.status & 4U) != 0U) { next = 0x1cc06U; transfer_kind = 1U; }
            break;
        }
        case 0x1cc00U: { // 536d005a subq.w #$1, $5a(a5)
            next = 0x1cc04U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1cc06U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1cc0cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1cc0cU: { // 6702 beq.b $1cc10
            next = 0x1cc0eU;
            if ((r.status & 4U) != 0U) { next = 0x1cc10U; transfer_kind = 1U; }
            break;
        }
        case 0x1cc10U: { // 08ed00050041 bset.b #$5, $41(a5)
            next = 0x1cc16U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cc18U: { // 0c2d00200058 cmpi.b #$20, $58(a5)
            next = 0x1cc1eU;
            const auto source_value = 0x20U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1cc1eU: { // 6c16 bge.b $1cc36
            next = 0x1cc20U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1cc36U; transfer_kind = 1U; }
            break;
        }
        case 0x1cc20U: { // 522d0058 addq.b #$1, $58(a5)
            next = 0x1cc24U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1cc24U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1cc2aU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cc2aU: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1cc30U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cc30U: { // 422d003e clr.b $3e(a5)
            next = 0x1cc34U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1cc36U: { // 08ad00030041 bclr.b #$3, $41(a5)
            next = 0x1cc3cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cc3cU: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1cc42U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1cc42U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1cc48U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cc48U: { // 1b7c00000060 move.b #$0, $60(a5)
            next = 0x1cc4eU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x60U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1cc4eU: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1cc54U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cc56U: { // 0c2d00700058 cmpi.b #$70, $58(a5)
            next = 0x1cc5cU;
            const auto source_value = 0x70U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1cc5cU: { // 6d02 blt.b $1cc60
            next = 0x1cc5eU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1cc60U; transfer_kind = 1U; }
            break;
        }
        case 0x1cc60U: { // 522d0058 addq.b #$1, $58(a5)
            next = 0x1cc64U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01cb42(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1cb6aU: { // 4eb90001fae6 jsr $1fae6.l
            next = 0x1cb70U;
            const auto result = m.call(c, 374U, 0x1cb6aU, 0x1fae6U, 0x1cb70U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1cb92U: { // 4e75 rts 
            next = 0x1cb94U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cb92U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cbbaU: { // 4e75 rts 
            next = 0x1cbbcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cbbaU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cbc8U: { // 4e75 rts 
            next = 0x1cbcaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cbc8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cbeeU: { // 4e75 rts 
            next = 0x1cbf0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cbeeU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cc04U: { // 4e75 rts 
            next = 0x1cc06U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cc04U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cc0eU: { // 4e75 rts 
            next = 0x1cc10U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cc0eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cc16U: { // 4e75 rts 
            next = 0x1cc18U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cc16U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cc34U: { // 4e75 rts 
            next = 0x1cc36U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cc34U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cc54U: { // 4e75 rts 
            next = 0x1cc56U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cc54U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cc5eU: { // 4e75 rts 
            next = 0x1cc60U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cc5eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cc64U: { // 4e75 rts 
            next = 0x1cc66U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cc64U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01cb42_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01cb42_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01cb42_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
