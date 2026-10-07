
static bool cpu_b_stage_gap_01cfae_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1cfaeU: { // 4a2d003f tst.b $3f(a5)
            next = 0x1cfb2U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x1cfb2U: { // 6704 beq.b $1cfb8
            next = 0x1cfb4U;
            if ((r.status & 4U) != 0U) { next = 0x1cfb8U; transfer_kind = 1U; }
            break;
        }
        case 0x1cfb4U: { // 60000a24 bra.w $1d9da
            next = 0x1cfb8U;
            next = 0x1d9daU; transfer_kind = 1U;
            break;
        }
        case 0x1cfb8U: { // 4a6d0074 tst.w $74(a5)
            next = 0x1cfbcU;
            m.logic(m.word((r.address[5] + 0x74U)), 16U);
            break;
        }
        case 0x1cfbcU: { // 6a74 bpl.b $1d032
            next = 0x1cfbeU;
            if ((r.status & 8U) == 0U) { next = 0x1d032U; transfer_kind = 1U; }
            break;
        }
        case 0x1cfbeU: { // 302d0012 move.w $12(a5), d0
            next = 0x1cfc2U;
            const auto v1 = m.word((r.address[5] + 0x12U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1cfc2U: { // 906d0062 sub.w $62(a5), d0
            next = 0x1cfc6U;
            const auto v1 = m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x62U)), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1cfc6U: { // 5640 addq.w #$3, d0
            next = 0x1cfc8U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x3U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1cfc8U: { // 0240fffc andi.w #$fffc, d0
            next = 0x1cfccU;
            const auto v1 = ((r.data[0] & 0xffffU) & 0xfffcU) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1cfccU: { // 6612 bne.b $1cfe0
            next = 0x1cfceU;
            if ((r.status & 4U) == 0U) { next = 0x1cfe0U; transfer_kind = 1U; }
            break;
        }
        case 0x1cfceU: { // 302d001a move.w $1a(a5), d0
            next = 0x1cfd2U;
            const auto v1 = m.word((r.address[5] + 0x1aU));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1cfd2U: { // 906d0064 sub.w $64(a5), d0
            next = 0x1cfd6U;
            const auto v1 = m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x64U)), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1cfd6U: { // 5640 addq.w #$3, d0
            next = 0x1cfd8U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0x3U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x1cfd8U: { // 0240fffc andi.w #$fffc, d0
            next = 0x1cfdcU;
            const auto v1 = ((r.data[0] & 0xffffU) & 0xfffcU) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1cfdcU: { // 6602 bne.b $1cfe0
            next = 0x1cfdeU;
            if ((r.status & 4U) == 0U) { next = 0x1cfe0U; transfer_kind = 1U; }
            break;
        }
        case 0x1cfdeU: { // 603e bra.b $1d01e
            next = 0x1cfe0U;
            next = 0x1d01eU; transfer_kind = 1U;
            break;
        }
        case 0x1cfe0U: { // 082d00050040 btst.b #$5, $40(a5)
            next = 0x1cfe6U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 5U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1cfe6U: { // 6610 bne.b $1cff8
            next = 0x1cfe8U;
            if ((r.status & 4U) == 0U) { next = 0x1cff8U; transfer_kind = 1U; }
            break;
        }
        case 0x1cfe8U: { // 082d00060040 btst.b #$6, $40(a5)
            next = 0x1cfeeU;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 6U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1cfeeU: { // 660e bne.b $1cffe
            next = 0x1cff0U;
            if ((r.status & 4U) == 0U) { next = 0x1cffeU; transfer_kind = 1U; }
            break;
        }
        case 0x1cff0U: { // 082d00070040 btst.b #$7, $40(a5)
            next = 0x1cff6U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 7U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1cff6U: { // 6606 bne.b $1cffe
            next = 0x1cff8U;
            if ((r.status & 4U) == 0U) { next = 0x1cffeU; transfer_kind = 1U; }
            break;
        }
        case 0x1cff8U: { // 3b6d003a005c move.w $3a(a5), $5c(a5)
            next = 0x1cffeU;
            const auto v1 = m.word((r.address[5] + 0x3aU));
            m.word((r.address[5] + 0x5cU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1cffeU: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1d004U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 1U)) & 0xffU);
            break;
        }
        case 0x1d004U: { // 08ad00050040 bclr.b #$5, $40(a5)
            next = 0x1d00aU;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 5U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 5U)) & 0xffU);
            break;
        }
        case 0x1d00aU: { // 08ad00000040 bclr.b #$0, $40(a5)
            next = 0x1d010U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 0U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 0U)) & 0xffU);
            break;
        }
        case 0x1d010U: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1d016U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 3U)) & 0xffU);
            break;
        }
        case 0x1d016U: { // 08ed00020040 bset.b #$2, $40(a5)
            next = 0x1d01cU;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x1d01eU: { // 3b7c00050074 move.w #$5, $74(a5)
            next = 0x1d024U;
            const auto v1 = 0x5U;
            m.word((r.address[5] + 0x74U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1d024U: { // 08ed00010040 bset.b #$1, $40(a5)
            next = 0x1d02aU;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 | (1U << 1U)) & 0xffU);
            break;
        }
        case 0x1d02aU: { // 08ed00020078 bset.b #$2, $78(a5)
            next = 0x1d030U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x78U), (v1 | (1U << 2U)) & 0xffU);
            break;
        }
        case 0x1d030U: { // 60d8 bra.b $1d00a
            next = 0x1d032U;
            next = 0x1d00aU; transfer_kind = 1U;
            break;
        }
        case 0x1d032U: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x1d036U;
            const auto v1 = m.sub(m.word((r.address[5] + 0x74U)), 0x1U, 16U);
            m.word((r.address[5] + 0x74U), v1);
            break;
        }
        case 0x1d036U: { // 66d2 bne.b $1d00a
            next = 0x1d038U;
            if ((r.status & 4U) == 0U) { next = 0x1d00aU; transfer_kind = 1U; }
            break;
        }
        case 0x1d038U: { // 08ad00030041 bclr.b #$3, $41(a5)
            next = 0x1d03eU;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x41U), (v1 & ~(1U << 3U)) & 0xffU);
            break;
        }
        case 0x1d03eU: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1d044U;
            const auto v1 = m.byte((r.address[5] + 0x40U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x40U), (v1 & ~(1U << 1U)) & 0xffU);
            break;
        }
        case 0x1d044U: { // 1b7c0009000b move.b #$9, $b(a5)
            next = 0x1d04aU;
            const auto v1 = 0x9U;
            m.byte((r.address[5] + 0xbU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1d04aU: { // 6000ffbe bra.w $1d00a
            next = 0x1d04eU;
            next = 0x1d00aU; transfer_kind = 1U;
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
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_gap_01cfae(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1d01cU: { // 4e75 rts 
            next = 0x1d01eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1d01cU, r.program_counter)) return *event;
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
        default:
            if (!(cpu_b_stage_gap_01cfae_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1cfaeU:
        case 0x1cfb2U:
        case 0x1cfb4U:
        case 0x1cfb8U:
        case 0x1cfbcU:
        case 0x1cfbeU:
        case 0x1cfc2U:
        case 0x1cfc6U:
        case 0x1cfc8U:
        case 0x1cfccU:
        case 0x1cfceU:
        case 0x1cfd2U:
        case 0x1cfd6U:
        case 0x1cfd8U:
        case 0x1cfdcU:
        case 0x1cfdeU:
        case 0x1cfe0U:
        case 0x1cfe6U:
        case 0x1cfe8U:
        case 0x1cfeeU:
        case 0x1cff0U:
        case 0x1cff6U:
        case 0x1cff8U:
        case 0x1cffeU:
        case 0x1d004U:
        case 0x1d00aU:
        case 0x1d010U:
        case 0x1d016U:
        case 0x1d01cU:
        case 0x1d01eU:
        case 0x1d024U:
        case 0x1d02aU:
        case 0x1d030U:
        case 0x1d032U:
        case 0x1d036U:
        case 0x1d038U:
        case 0x1d03eU:
        case 0x1d044U:
        case 0x1d04aU:
        case 0x1d9daU:
        case 0x1d9e2U:
        case 0x1d9e8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
