
static bool cpu_b_stage_gap_01c02e_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c02eU: { // 4a2d003f tst.b $3f(a5)
            next = 0x1c032U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x1c032U: { // 6704 beq.b $1c038
            next = 0x1c034U;
            if ((r.status & 4U) != 0U) { next = 0x1c038U; transfer_kind = 1U; }
            break;
        }
        case 0x1c034U: { // 600019a4 bra.w $1d9da
            next = 0x1c038U;
            next = 0x1d9daU; transfer_kind = 1U;
            break;
        }
        case 0x1c038U: { // 082d00030078 btst.b #$3, $78(a5)
            next = 0x1c03eU;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x1c03eU: { // 6638 bne.b $1c078
            next = 0x1c040U;
            if ((r.status & 4U) == 0U) { next = 0x1c078U; transfer_kind = 1U; }
            break;
        }
        case 0x1c044U: { // 206d006e movea.l $6e(a5), a0
            next = 0x1c048U;
            const auto v1 = m.lng((r.address[5] + 0x6eU));
            r.address[0] = v1;
            break;
        }
        case 0x1c048U: { // 7000 moveq #$0, d0
            next = 0x1c04aU;
            r.data[0] = 0x0U; m.logic(r.data[0], 32U);
            break;
        }
        case 0x1c04aU: { // 10280006 move.b $6(a0), d0
            next = 0x1c04eU;
            const auto v1 = m.byte((r.address[0] + 0x6U));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1c04eU: { // e948 lsl.w #$4, d0
            next = 0x1c050U;
            m.shift_word(0, 4U, true, false);
            break;
        }
        case 0x1c050U: { // b06d0054 cmp.w $54(a5), d0
            next = 0x1c054U;
            (void)m.sub((r.data[0] & 0xffffU), m.word((r.address[5] + 0x54U)), 16U, true);
            break;
        }
        case 0x1c054U: { // 6702 beq.b $1c058
            next = 0x1c056U;
            if ((r.status & 4U) != 0U) { next = 0x1c058U; transfer_kind = 1U; }
            break;
        }
        case 0x1c058U: { // 08ed00030078 bset.b #$3, $78(a5)
            next = 0x1c05eU;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x78U), (v1 | (1U << 3U)) & 0xffU);
            break;
        }
        case 0x1c05eU: { // 1b7c00000058 move.b #$0, $58(a5)
            next = 0x1c064U;
            const auto v1 = 0x0U;
            m.byte((r.address[5] + 0x58U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1c064U: { // 3b6d0064001a move.w $64(a5), $1a(a5)
            next = 0x1c06aU;
            const auto v1 = m.word((r.address[5] + 0x64U));
            m.word((r.address[5] + 0x1aU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1c06aU: { // 066d0010001a addi.w #$10, $1a(a5)
            next = 0x1c070U;
            const auto v1 = m.add(m.word((r.address[5] + 0x1aU)), 0x10U, 16U);
            m.word((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x1c070U: { // 3b7c00000016 move.w #$0, $16(a5)
            next = 0x1c076U;
            const auto v1 = 0x0U;
            m.word((r.address[5] + 0x16U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1c078U: { // 526d0016 addq.w #$1, $16(a5)
            next = 0x1c07cU;
            const auto v1 = m.add(m.word((r.address[5] + 0x16U)), 0x1U, 16U);
            m.word((r.address[5] + 0x16U), v1);
            break;
        }
        case 0x1c07cU: { // 546d001a addq.w #$2, $1a(a5)
            next = 0x1c080U;
            const auto v1 = m.add(m.word((r.address[5] + 0x1aU)), 0x2U, 16U);
            m.word((r.address[5] + 0x1aU), v1);
            break;
        }
        case 0x1c080U: { // 102d0058 move.b $58(a5), d0
            next = 0x1c084U;
            const auto v1 = m.byte((r.address[5] + 0x58U));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1c084U: { // 5200 addq.b #$1, d0
            next = 0x1c086U;
            const auto v1 = m.add((r.data[0] & 0xffU), 0x1U, 8U);
            m.db(0, v1);
            break;
        }
        case 0x1c086U: { // 1b400058 move.b d0, $58(a5)
            next = 0x1c08aU;
            const auto v1 = (r.data[0] & 0xffU);
            m.byte((r.address[5] + 0x58U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1c08aU: { // 0c000017 cmpi.b #$17, d0
            next = 0x1c08eU;
            (void)m.sub((r.data[0] & 0xffU), 0x17U, 8U, true);
            break;
        }
        case 0x1c08eU: { // 6702 beq.b $1c092
            next = 0x1c090U;
            if ((r.status & 4U) != 0U) { next = 0x1c092U; transfer_kind = 1U; }
            break;
        }
        case 0x1c092U: { // 08ad00020078 bclr.b #$2, $78(a5)
            next = 0x1c098U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 2U) & 1U) ? 0U : 4U));
            m.byte((r.address[5] + 0x78U), (v1 & ~(1U << 2U)) & 0xffU);
            break;
        }
        case 0x1c098U: { // 3b7c00080016 move.w #$8, $16(a5)
            next = 0x1c09eU;
            const auto v1 = 0x8U;
            m.word((r.address[5] + 0x16U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1c09eU: { // 066d00600064 addi.w #$60, $64(a5)
            next = 0x1c0a4U;
            const auto v1 = m.add(m.word((r.address[5] + 0x64U)), 0x60U, 16U);
            m.word((r.address[5] + 0x64U), v1);
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

FunctionResult cpu_b_stage_gap_01c02e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c040U: { // 6100ff8c bsr.w $1bfce
            next = 0x1c044U;
            const auto result = m.call(c, 696U, 0x1c040U, 0x1bfceU, 0x1c044U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1c056U: { // 4e75 rts 
            next = 0x1c058U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c056U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c076U: { // 4e75 rts 
            next = 0x1c078U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c076U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c090U: { // 4e75 rts 
            next = 0x1c092U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c090U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1c0a4U: { // 4e75 rts 
            next = 0x1c0a6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c0a4U, r.program_counter)) return *event;
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
            if (!(cpu_b_stage_gap_01c02e_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1c02eU:
        case 0x1c032U:
        case 0x1c034U:
        case 0x1c038U:
        case 0x1c03eU:
        case 0x1c040U:
        case 0x1c044U:
        case 0x1c048U:
        case 0x1c04aU:
        case 0x1c04eU:
        case 0x1c050U:
        case 0x1c054U:
        case 0x1c056U:
        case 0x1c058U:
        case 0x1c05eU:
        case 0x1c064U:
        case 0x1c06aU:
        case 0x1c070U:
        case 0x1c076U:
        case 0x1c078U:
        case 0x1c07cU:
        case 0x1c080U:
        case 0x1c084U:
        case 0x1c086U:
        case 0x1c08aU:
        case 0x1c08eU:
        case 0x1c090U:
        case 0x1c092U:
        case 0x1c098U:
        case 0x1c09eU:
        case 0x1c0a4U:
        case 0x1d9daU:
        case 0x1d9e2U:
        case 0x1d9e8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
