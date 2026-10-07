
static bool cpu_b_stage_01c51c_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c5b8U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c5beU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c5c0U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1c5c4U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c5c4U: { // 6704 beq.b $1c5ca
            next = 0x1c5c6U;
            if ((r.status & 4U) != 0U) { next = 0x1c5caU; transfer_kind = 1U; }
            break;
        }
        case 0x1c5c6U: { // 60001422 bra.w $1d9ea
            next = 0x1c5caU;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1c5caU: { // 4a6d005a tst.w $5a(a5)
            next = 0x1c5ceU;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1c5ceU: { // 6706 beq.b $1c5d6
            next = 0x1c5d0U;
            if ((r.status & 4U) != 0U) { next = 0x1c5d6U; transfer_kind = 1U; }
            break;
        }
        case 0x1c5d0U: { // 536d005a subq.w #$1, $5a(a5)
            next = 0x1c5d4U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c5d6U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1c5dcU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c5dcU: { // 6702 beq.b $1c5e0
            next = 0x1c5deU;
            if ((r.status & 4U) != 0U) { next = 0x1c5e0U; transfer_kind = 1U; }
            break;
        }
        case 0x1c5e0U: { // 08ed00050041 bset.b #$5, $41(a5)
            next = 0x1c5e6U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c5e8U: { // 0c2d00400058 cmpi.b #$40, $58(a5)
            next = 0x1c5eeU;
            const auto source_value = 0x40U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1c5eeU: { // 6e16 bgt.b $1c606
            next = 0x1c5f0U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1c606U; transfer_kind = 1U; }
            break;
        }
        case 0x1c5f0U: { // 522d0058 addq.b #$1, $58(a5)
            next = 0x1c5f4U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1c5f4U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c5faU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c5faU: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c600U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c600U: { // 422d003e clr.b $3e(a5)
            next = 0x1c604U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c606U: { // 08ad00030041 bclr.b #$3, $41(a5)
            next = 0x1c60cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c60cU: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1c612U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c612U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1c618U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c618U: { // 1b7c00000060 move.b #$0, $60(a5)
            next = 0x1c61eU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x60U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c61eU: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c624U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01c51c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c532U: { // 610013ce bsr.w $1d902
            next = 0x1c536U;
            const auto result = m.call(c, 344U, 0x1c532U, 0x1d902U, 0x1c536U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1c55cU: { // 4e75 rts 
            next = 0x1c55eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c55cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c57cU: { // 610000a8 bsr.w $1c626
            next = 0x1c580U;
            const auto result = m.call(c, 656U, 0x1c57cU, 0x1c626U, 0x1c580U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1c58eU: { // 4e75 rts 
            next = 0x1c590U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c58eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c5beU: { // 4e75 rts 
            next = 0x1c5c0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c5beU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c5d4U: { // 4e75 rts 
            next = 0x1c5d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c5d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c5deU: { // 4e75 rts 
            next = 0x1c5e0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c5deU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c5e6U: { // 4e75 rts 
            next = 0x1c5e8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c5e6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c604U: { // 4e75 rts 
            next = 0x1c606U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c604U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c624U: { // 4e75 rts 
            next = 0x1c626U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c624U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01c51c_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01c51c_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01c51c_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
