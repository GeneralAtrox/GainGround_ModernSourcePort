
static bool cpu_b_stage_gap_01437e_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1437eU: { // 49f900000d1c lea.l $d1c.l, a4
            next = 0x14384U;
            r.address[4] = 0xd1cU;
            break;
        }
        case 0x14384U: { // 302d0020 move.w $20(a5), d0
            next = 0x14388U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14388U: { // d8c0 adda.w d0, a4
            next = 0x1438aU;
            r.address[4] += static_cast<std::uint32_t>(static_cast<std::int16_t>((r.data[0] & 0xffffU)));
            break;
        }
        case 0x1438aU: { // 3014 move.w (a4), d0
            next = 0x1438cU;
            const auto v1 = m.word(r.address[4]);
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1438cU: { // e540 asl.w #$2, d0
            next = 0x1438eU;
            m.asl_word(0, 2U);
            break;
        }
        case 0x1438eU: { // 4efb0002 jmp $14392(pc, d0.w)
            next = 0x14392U;
            next = 0x14392U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0])); transfer_kind = 1U;
            break;
        }
        case 0x14392U: { // 6000000e bra.w $143a2
            next = 0x14396U;
            next = 0x143a2U; transfer_kind = 1U;
            break;
        }
        case 0x14396U: { // 60000026 bra.w $143be
            next = 0x1439aU;
            next = 0x143beU; transfer_kind = 1U;
            break;
        }
        case 0x1439aU: { // 6000005a bra.w $143f6
            next = 0x1439eU;
            next = 0x143f6U; transfer_kind = 1U;
            break;
        }
        case 0x1439eU: { // 6000002e bra.w $143ce
            next = 0x143a2U;
            next = 0x143ceU; transfer_kind = 1U;
            break;
        }
        case 0x143a2U: { // 1b7c000b000b move.b #$b, $b(a5)
            next = 0x143a8U;
            const auto v1 = 0xbU;
            m.byte((r.address[5] + 0xbU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x143a8U: { // 422d003f clr.b $3f(a5)
            next = 0x143acU;
            (void)m.byte((r.address[5] + 0x3fU));
            m.byte((r.address[5] + 0x3fU), 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x143acU: { // 41fa1904 lea.l $15cb2(pc), a0
            next = 0x143b0U;
            r.address[0] = 0x15cb2U;
            break;
        }
        case 0x143b0U: { // 582d003d addq.b #$4, $3d(a5)
            next = 0x143b4U;
            const auto v1 = m.add(m.byte((r.address[5] + 0x3dU)), 0x4U, 8U);
            m.byte((r.address[5] + 0x3dU), v1);
            break;
        }
        case 0x143b4U: { // 102d003d move.b $3d(a5), d0
            next = 0x143b8U;
            const auto v1 = m.byte((r.address[5] + 0x3dU));
            m.db(0, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x143b8U: { // 0240000c andi.w #$c, d0
            next = 0x143bcU;
            const auto v1 = ((r.data[0] & 0xffffU) & 0xcU) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x143bcU: { // 6026 bra.b $143e4
            next = 0x143beU;
            next = 0x143e4U; transfer_kind = 1U;
            break;
        }
        case 0x143beU: { // 422d003f clr.b $3f(a5)
            next = 0x143c2U;
            (void)m.byte((r.address[5] + 0x3fU));
            m.byte((r.address[5] + 0x3fU), 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x143c2U: { // 302c0002 move.w $2(a4), d0
            next = 0x143c6U;
            const auto v1 = m.word((r.address[4] + 0x2U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x143c6U: { // e540 asl.w #$2, d0
            next = 0x143c8U;
            m.asl_word(0, 2U);
            break;
        }
        case 0x143c8U: { // 41fa18f8 lea.l $15cc2(pc), a0
            next = 0x143ccU;
            r.address[0] = 0x15cc2U;
            break;
        }
        case 0x143ccU: { // 6016 bra.b $143e4
            next = 0x143ceU;
            next = 0x143e4U; transfer_kind = 1U;
            break;
        }
        case 0x143ceU: { // 1b7c0000000b move.b #$0, $b(a5)
            next = 0x143d4U;
            const auto v1 = 0x0U;
            m.byte((r.address[5] + 0xbU), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x143d4U: { // 302c0002 move.w $2(a4), d0
            next = 0x143d8U;
            const auto v1 = m.word((r.address[4] + 0x2U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x143d8U: { // 4640 not.w d0
            next = 0x143daU;
            const auto v1 = (~(r.data[0] & 0xffffU)) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x143daU: { // e540 asl.w #$2, d0
            next = 0x143dcU;
            m.asl_word(0, 2U);
            break;
        }
        case 0x143dcU: { // 0240001c andi.w #$1c, d0
            next = 0x143e0U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0x1cU) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x143e0U: { // 41fa18e0 lea.l $15cc2(pc), a0
            next = 0x143e4U;
            r.address[0] = 0x15cc2U;
            break;
        }
        case 0x143e4U: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x143eaU;
            const auto v1 = m.word((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x143eaU: { // 3b7000020008 move.w $2(a0, d0.w), $8(a5)
            next = 0x143f0U;
            const auto v1 = m.word((r.address[0] + 0x2U + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_gap_01437e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x143f0U: { // 4eb900015df2 jsr $15df2.l
            next = 0x143f6U;
            const auto result = m.indirect_call(c, 0x143f0U, 0x15df2U, 0x143f6U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x143f6U: { // 4e75 rts 
            next = 0x143f8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x143f6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_gap_01437e_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1437eU:
        case 0x14384U:
        case 0x14388U:
        case 0x1438aU:
        case 0x1438cU:
        case 0x1438eU:
        case 0x14392U:
        case 0x14396U:
        case 0x1439aU:
        case 0x1439eU:
        case 0x143a2U:
        case 0x143a8U:
        case 0x143acU:
        case 0x143b0U:
        case 0x143b4U:
        case 0x143b8U:
        case 0x143bcU:
        case 0x143beU:
        case 0x143c2U:
        case 0x143c6U:
        case 0x143c8U:
        case 0x143ccU:
        case 0x143ceU:
        case 0x143d4U:
        case 0x143d8U:
        case 0x143daU:
        case 0x143dcU:
        case 0x143e0U:
        case 0x143e4U:
        case 0x143eaU:
        case 0x143f0U:
        case 0x143f6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
