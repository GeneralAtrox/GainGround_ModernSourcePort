
static bool cpu_b_stage_gap_01bfce_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1bfceU: { // 4a2d003f tst.b $3f(a5)
            next = 0x1bfd2U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x1bfd2U: { // 6704 beq.b $1bfd8
            next = 0x1bfd4U;
            if ((r.status & 4U) != 0U) { next = 0x1bfd8U; transfer_kind = 1U; }
            break;
        }
        case 0x1bfd4U: { // 60001a04 bra.w $1d9da
            next = 0x1bfd8U;
            next = 0x1d9daU; transfer_kind = 1U;
            break;
        }
        case 0x1bfd8U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1bfdcU;
            m.logic(m.byte((r.address[5] + 0x3eU)), 8U);
            break;
        }
        case 0x1bfdcU: { // 6704 beq.b $1bfe2
            next = 0x1bfdeU;
            if ((r.status & 4U) != 0U) { next = 0x1bfe2U; transfer_kind = 1U; }
            break;
        }
        case 0x1bfdeU: { // 60001a0a bra.w $1d9ea
            next = 0x1bfe2U;
            next = 0x1d9eaU; transfer_kind = 1U;
            break;
        }
        case 0x1bfe2U: { // 08ed00020078 bset.b #$2, $78(a5)
            next = 0x1bfe8U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x78U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x1bfe8U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1bfecU;
            const auto v1 = m.add(m.word((r.address[5] + 0x54U)), 0x1U, 16U);
            m.word((r.address[5] + 0x54U), v1);
            break;
        }
        case 0x1bfecU: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1bff2U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 1U)) & 0xffU);
            break;
        }
        case 0x1bff2U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1bff8U;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1bff8U: { // 661c bne.b $1c016
            next = 0x1bffaU;
            if ((r.status & 4U) == 0U) { next = 0x1c016U; transfer_kind = 1U; }
            break;
        }
        case 0x1bffaU: { // 302d0064 move.w $64(a5), d0
            next = 0x1bffeU;
            const auto v1 = m.word((r.address[5] + 0x64U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1bffeU: { // 0440000a subi.w #$a, d0
            next = 0x1c002U;
            const auto v1 = m.sub((r.data[0] & 0xffffU), 0xaU, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1c002U: { // b06d001a cmp.w $1a(a5), d0
            next = 0x1c006U;
            (void)m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x1aU)), 16U, true);
            break;
        }
        case 0x1c006U: { // 6602 bne.b $1c00a
            next = 0x1c008U;
            if ((r.status & 4U) == 0U) { next = 0x1c00aU; transfer_kind = 1U; }
            break;
        }
        case 0x1c00aU: { // 3b40001a move.w d0, $1a(a5)
            next = 0x1c00eU;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x1aU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1c00eU: { // 3b7cfff00016 move.w #$fff0, $16(a5)
            next = 0x1c014U;
            const auto v1 = 0xfff0U;
            m.word((r.address[5] + 0x16U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1c016U: { // 302d0064 move.w $64(a5), d0
            next = 0x1c01aU;
            const auto v1 = m.word((r.address[5] + 0x64U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1c01aU: { // b06d001a cmp.w $1a(a5), d0
            next = 0x1c01eU;
            (void)m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x1aU)), 16U, true);
            break;
        }
        case 0x1c01eU: { // 6602 bne.b $1c022
            next = 0x1c020U;
            if ((r.status & 4U) == 0U) { next = 0x1c022U; transfer_kind = 1U; }
            break;
        }
        case 0x1c022U: { // 3b40001a move.w d0, $1a(a5)
            next = 0x1c026U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x1aU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1c026U: { // 3b7cfff80016 move.w #$fff8, $16(a5)
            next = 0x1c02cU;
            const auto v1 = 0xfff8U;
            m.word((r.address[5] + 0x16U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1d9daU: { // 2b7c0001ed980002 move.l #$1ed98, $2(a5)
            next = 0x1d9e2U;
            const auto v1 = 0x1ed98U;
            m.lng((r.address[5] + 0x2U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x1d9e2U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x1d9e8U;
            const auto v1 = 0x0U;
            m.word((r.address[5] + 0x54U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1d9eaU: { // 1b7c00010054 move.b #$1, $54(a5)
            next = 0x1d9f0U;
            const auto v1 = 0x1U;
            m.byte((r.address[5] + 0x54U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1d9f0U: { // 532d003e subq.b #$1, $3e(a5)
            next = 0x1d9f4U;
            const auto v1 = m.sub(m.byte((r.address[5] + 0x3eU)), 0x1U, 8U);
            m.byte((r.address[5] + 0x3eU), v1);
            break;
        }
        case 0x1d9f4U: { // 022d00fe003e andi.b #$fe, $3e(a5)
            next = 0x1d9faU;
            const auto v1 = (m.byte((r.address[5] + 0x3eU)) & 0xfeU) & 0xffU;
            m.byte((r.address[5] + 0x3eU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1d9faU: { // 08ed00010040 bset.b #$1, $40(a5)
            next = 0x1da00U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 1U)) & 0xffU);
            break;
        }
        case 0x1da00U: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1da06U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 3U)) & 0xffU);
            break;
        }
        case 0x1da06U: { // 066d0080005c addi.w #$80, $5c(a5)
            next = 0x1da0cU;
            const auto v1 = m.add(m.word((r.address[5] + 0x5cU)), 0x80U, 16U);
            m.word((r.address[5] + 0x5cU), v1);
            break;
        }
        case 0x1da0cU: { // 026d0780005c andi.w #$780, $5c(a5)
            next = 0x1da12U;
            const auto v1 = (m.word((r.address[5] + 0x5cU)) & 0x780U) & 0xffffU;
            m.word((r.address[5] + 0x5cU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1da12U: { // 1b7c0009005e move.b #$9, $5e(a5)
            next = 0x1da18U;
            const auto v1 = 0x9U;
            m.byte((r.address[5] + 0x5eU), v1);
            m.logic(v1, 8U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_gap_01bfce(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c008U: { // 4e75 rts 
            next = 0x1c00aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c008U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c014U: { // 4e75 rts 
            next = 0x1c016U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c014U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c020U: { // 4e75 rts 
            next = 0x1c022U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c020U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c02cU: { // 4e75 rts 
            next = 0x1c02eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c02cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1d9e8U: { // 4e75 rts 
            next = 0x1d9eaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1d9e8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1da18U: { // 4e75 rts 
            next = 0x1da1aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1da18U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_gap_01bfce_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1bfceU:
        case 0x1bfd2U:
        case 0x1bfd4U:
        case 0x1bfd8U:
        case 0x1bfdcU:
        case 0x1bfdeU:
        case 0x1bfe2U:
        case 0x1bfe8U:
        case 0x1bfecU:
        case 0x1bff2U:
        case 0x1bff8U:
        case 0x1bffaU:
        case 0x1bffeU:
        case 0x1c002U:
        case 0x1c006U:
        case 0x1c008U:
        case 0x1c00aU:
        case 0x1c00eU:
        case 0x1c014U:
        case 0x1c016U:
        case 0x1c01aU:
        case 0x1c01eU:
        case 0x1c020U:
        case 0x1c022U:
        case 0x1c026U:
        case 0x1c02cU:
        case 0x1d9daU:
        case 0x1d9e2U:
        case 0x1d9e8U:
        case 0x1d9eaU:
        case 0x1d9f0U:
        case 0x1d9f4U:
        case 0x1d9faU:
        case 0x1da00U:
        case 0x1da06U:
        case 0x1da0cU:
        case 0x1da12U:
        case 0x1da18U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
