
FunctionResult cpu_b_stage_01bde0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1be00U: { // 4e75 rts 
            next = 0x1be02U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1be00U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1be14U: { // 4e75 rts 
            next = 0x1be16U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1be14U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1be48U: { // 610015b8 bsr.w $1d402
            next = 0x1be4cU;
            const auto result = m.call(c, 337U, 0x1be48U, 0x1d402U, 0x1be4cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1be5eU: { // 61001cfc bsr.w $1db5c
            next = 0x1be62U;
            const auto result = m.call(c, 351U, 0x1be5eU, 0x1db5cU, 0x1be62U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1be80U: { // 4e75 rts 
            next = 0x1be82U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1be80U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1bea0U: { // 4e75 rts 
            next = 0x1bea2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1bea0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1bea8U: { // 61001bae bsr.w $1da58
            next = 0x1beacU;
            const auto result = m.call(c, 347U, 0x1bea8U, 0x1da58U, 0x1beacU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1bec0U: { // 4e75 rts 
            next = 0x1bec2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1bec0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1bed2U: { // 4e75 rts 
            next = 0x1bed4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1bed2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1bedeU: { // 61001a40 bsr.w $1d920
            next = 0x1bee2U;
            const auto result = m.call(c, 668U, 0x1bedeU, 0x1d920U, 0x1bee2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1beecU: { // 4e75 rts 
            next = 0x1beeeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1beecU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1bf1aU: { // 4e75 rts 
            next = 0x1bf1cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1bf1aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01bde0_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01bde0_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_01bde0_dispatch_cases_002(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01bde0_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_01bfce_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1bfceU: { // 4a2d003f tst.b $3f(a5)
            next = 0x1bfd2U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1bfd2U: { // 6704 beq.b $1bfd8
            next = 0x1bfd4U;
            if ((r.status & 4U) != 0U) { next = 0x1bfd8U; transfer_kind = 1U; }
            break;
        }
        case 0x1bfd4U: { // 60001a04 bra.w $1d9da
            next = 0x1bfd8U;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1bfd8U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1bfdcU;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1bfdcU: { // 6704 beq.b $1bfe2
            next = 0x1bfdeU;
            if ((r.status & 4U) != 0U) { next = 0x1bfe2U; transfer_kind = 1U; }
            break;
        }
        case 0x1bfdeU: { // 60001a0a bra.w $1d9ea
            next = 0x1bfe2U;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1bfe2U: { // 08ed00020078 bset.b #$2, $78(a5)
            next = 0x1bfe8U;
            const auto destination_address = r.address[5] + 0x78U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1bfe8U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1bfecU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1bfecU: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1bff2U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1bff2U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1bff8U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1bff8U: { // 661c bne.b $1c016
            next = 0x1bffaU;
            if ((r.status & 4U) == 0U) { next = 0x1c016U; transfer_kind = 1U; }
            break;
        }
        case 0x1bffaU: { // 302d0064 move.w $64(a5), d0
            next = 0x1bffeU;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1bffeU: { // 0440000a subi.w #$a, d0
            next = 0x1c002U;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c002U: { // b06d001a cmp.w $1a(a5), d0
            next = 0x1c006U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1c006U: { // 6602 bne.b $1c00a
            next = 0x1c008U;
            if ((r.status & 4U) == 0U) { next = 0x1c00aU; transfer_kind = 1U; }
            break;
        }
        case 0x1c00aU: { // 3b40001a move.w d0, $1a(a5)
            next = 0x1c00eU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c00eU: { // 3b7cfff00016 move.w #$fff0, $16(a5)
            next = 0x1c014U;
            const auto value = 0xfff0U;
            const auto destination_address = r.address[5] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c016U: { // 302d0064 move.w $64(a5), d0
            next = 0x1c01aU;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c01aU: { // b06d001a cmp.w $1a(a5), d0
            next = 0x1c01eU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1c01eU: { // 6602 bne.b $1c022
            next = 0x1c020U;
            if ((r.status & 4U) == 0U) { next = 0x1c022U; transfer_kind = 1U; }
            break;
        }
        case 0x1c022U: { // 3b40001a move.w d0, $1a(a5)
            next = 0x1c026U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c026U: { // 3b7cfff80016 move.w #$fff8, $16(a5)
            next = 0x1c02cU;
            const auto value = 0xfff8U;
            const auto destination_address = r.address[5] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
