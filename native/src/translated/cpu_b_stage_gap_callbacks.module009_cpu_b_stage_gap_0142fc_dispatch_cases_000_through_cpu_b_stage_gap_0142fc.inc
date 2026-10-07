
static bool cpu_b_stage_gap_0142fc_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x142fcU: { // 49f900000d1c lea.l $d1c.l, a4
            next = 0x14302U;
            r.address[4] = 0xd1cU;
            break;
        }
        case 0x14302U: { // 302d0020 move.w $20(a5), d0
            next = 0x14306U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14306U: { // d8c0 adda.w d0, a4
            next = 0x14308U;
            r.address[4] += static_cast<std::uint32_t>(static_cast<std::int16_t>((r.data[0] & 0xffffU)));
            break;
        }
        case 0x14308U: { // 3014 move.w (a4), d0
            next = 0x1430aU;
            const auto v1 = m.word(r.address[4]);
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1430aU: { // e540 asl.w #$2, d0
            next = 0x1430cU;
            m.asl_word(0, 2U);
            break;
        }
        case 0x1430cU: { // 4efb0002 jmp $14310(pc, d0.w)
            next = 0x14310U;
            next = 0x14310U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])); transfer_kind = 1U;
            break;
        }
        case 0x14310U: { // 6000000e bra.w $14320
            next = 0x14314U;
            next = 0x14320U; transfer_kind = 1U;
            break;
        }
        case 0x14314U: { // 60000028 bra.w $1433e
            next = 0x14318U;
            next = 0x1433eU; transfer_kind = 1U;
            break;
        }
        case 0x14318U: { // 6000002a bra.w $14344
            next = 0x1431cU;
            next = 0x14344U; transfer_kind = 1U;
            break;
        }
        case 0x1431cU: { // 60000038 bra.w $14356
            next = 0x14320U;
            next = 0x14356U; transfer_kind = 1U;
            break;
        }
        case 0x14320U: { // 3b7c0c060008 move.w #$c06, $8(a5)
            next = 0x14326U;
            const auto v1 = 0xc06U;
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14326U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1432aU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x1432aU: { // 674a beq.b $14376
            next = 0x1432cU;
            if ((r.status & 4U) != 0U) { next = 0x14376U; transfer_kind = 1U; }
            break;
        }
        case 0x1432cU: { // 38bc0001 move.w #$1, (a4)
            next = 0x14330U;
            const auto v1 = 0x1U;
            m.word(r.address[4], v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14330U: { // 426c0002 clr.w $2(a4)
            next = 0x14334U;
            (void)m.word((r.address[4] + 0x2U));
            m.word((r.address[4] + 0x2U), 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x14334U: { // 1b7c0004000b move.b #$4, $b(a5)
            next = 0x1433aU;
            const auto v1 = 0x4U;
            m.byte((r.address[5] + 0xbU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1433aU: { // 7000 moveq #$0, d0
            next = 0x1433cU;
            r.data[0] = 0x0U; m.logic(r.data[0], 32U);
            break;
        }
        case 0x1433cU: { // 602e bra.b $1436c
            next = 0x1433eU;
            next = 0x1436cU; transfer_kind = 1U;
            break;
        }
        case 0x1433eU: { // 302c0002 move.w $2(a4), d0
            next = 0x14342U;
            const auto v1 = m.word((r.address[4] + 0x2U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14342U: { // 6028 bra.b $1436c
            next = 0x14344U;
            next = 0x1436cU; transfer_kind = 1U;
            break;
        }
        case 0x14344U: { // 422d003f clr.b $3f(a5)
            next = 0x14348U;
            (void)m.byte((r.address[5] + 0x3fU));
            m.byte((r.address[5] + 0x3fU), 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x14348U: { // 1b7c0004000b move.b #$4, $b(a5)
            next = 0x1434eU;
            const auto v1 = 0x4U;
            m.byte((r.address[5] + 0xbU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1434eU: { // 3b7c00140046 move.w #$14, $46(a5)
            next = 0x14354U;
            const auto v1 = 0x14U;
            m.word((r.address[5] + 0x46U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14354U: { // 6020 bra.b $14376
            next = 0x14356U;
            next = 0x14376U; transfer_kind = 1U;
            break;
        }
        case 0x14356U: { // 1b7c000a000b move.b #$a, $b(a5)
            next = 0x1435cU;
            const auto v1 = 0xaU;
            m.byte((r.address[5] + 0xbU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1435cU: { // 4a2d003f tst.b $3f(a5)
            next = 0x14360U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x14360U: { // 66ca bne.b $1432c
            next = 0x14362U;
            if ((r.status & 4U) == 0U) { next = 0x1432cU; transfer_kind = 1U; }
            break;
        }
        case 0x14362U: { // 302c0002 move.w $2(a4), d0
            next = 0x14366U;
            const auto v1 = m.word((r.address[4] + 0x2U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14366U: { // 4640 not.w d0
            next = 0x14368U;
            const auto v1 = (~(r.data[0] & 0xffffU)) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14368U: { // 02400007 andi.w #$7, d0
            next = 0x1436cU;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x7U) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1436cU: { // e240 asr.w #$1, d0
            next = 0x1436eU;
            m.shift_word(0, 1U, false, true);
            break;
        }
        case 0x1436eU: { // 06400c07 addi.w #$c07, d0
            next = 0x14372U;
            const auto v1 = m.add((r.data[0] & 0xffffU), 0xc07U, 16U);
            m.dw(0, v1);
            break;
        }
        case 0x14372U: { // 3b400008 move.w d0, $8(a5)
            next = 0x14376U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_gap_0142fc(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x14376U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1437cU;
            const auto result = m.indirect_call(c, 0x14376U, 0x15df2U, 0x1437cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1437cU: { // 4e75 rts 
            next = 0x1437eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1437cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_gap_0142fc_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x142fcU:
        case 0x14302U:
        case 0x14306U:
        case 0x14308U:
        case 0x1430aU:
        case 0x1430cU:
        case 0x14310U:
        case 0x14314U:
        case 0x14318U:
        case 0x1431cU:
        case 0x14320U:
        case 0x14326U:
        case 0x1432aU:
        case 0x1432cU:
        case 0x14330U:
        case 0x14334U:
        case 0x1433aU:
        case 0x1433cU:
        case 0x1433eU:
        case 0x14342U:
        case 0x14344U:
        case 0x14348U:
        case 0x1434eU:
        case 0x14354U:
        case 0x14356U:
        case 0x1435cU:
        case 0x14360U:
        case 0x14362U:
        case 0x14366U:
        case 0x14368U:
        case 0x1436cU:
        case 0x1436eU:
        case 0x14372U:
        case 0x14376U:
        case 0x1437cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
