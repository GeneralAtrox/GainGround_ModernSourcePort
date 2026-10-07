
FunctionResult cpu_b_stage_gap_014164(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x14164U: { // 4a380d2c tst.b $d2c.w
            next = 0x14168U;
            m.logic(m.byte(0xd2cU), 8U);
            break;
        }
        case 0x14168U: { // 663a bne.b $141a4
            next = 0x1416aU;
            if ((r.status & 4U) == 0U) { next = 0x141a4U; transfer_kind = 1U; }
            break;
        }
        case 0x1416aU: { // 536d0022 subq.w #$1, $22(a5)
            next = 0x1416eU;
            const auto v1 = m.sub(m.word((r.address[5] + 0x22U)), 0x1U, 16U);
            m.word((r.address[5] + 0x22U), v1);
            break;
        }
        case 0x1416eU: { // 6e000090 bgt.w $14200
            next = 0x14172U;
            if (((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U)) { next = 0x14200U; transfer_kind = 1U; }
            break;
        }
        case 0x14172U: { // 3b7c00020022 move.w #$2, $22(a5)
            next = 0x14178U;
            const auto v1 = 0x2U;
            m.word((r.address[5] + 0x22U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14178U: { // 526d0020 addq.w #$1, $20(a5)
            next = 0x1417cU;
            const auto v1 = m.add(m.word((r.address[5] + 0x20U)), 0x1U, 16U);
            m.word((r.address[5] + 0x20U), v1);
            break;
        }
        case 0x1417cU: { // 302d0020 move.w $20(a5), d0
            next = 0x14180U;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14180U: { // 0240000f andi.w #$f, d0
            next = 0x14184U;
            const auto v1 = ((r.data[0] & 0xffffU) & 0xfU) & 0xffffU;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x14184U: { // 41fa1b7c lea.l $15d02(pc), a0
            next = 0x14188U;
            r.address[0] = 0x15d02U;
            break;
        }
        case 0x14188U: { // 1b7000000009 move.b (a0, d0.w), $9(a5)
            next = 0x1418eU;
            const auto v1 = m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.byte((r.address[5] + 0x9U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1418eU: { // 122d0001 move.b $1(a5), d1
            next = 0x14192U;
            const auto v1 = m.byte((r.address[5] + 0x1U));
            m.db(1, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x14192U: { // 08410001 bchg.b #$1, d1
            next = 0x14196U;
            const auto v1 = r.data[1];
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            r.data[1] = v1 ^ (1U << 1U);
            break;
        }
        case 0x14196U: { // 4a40 tst.w d0
            next = 0x14198U;
            m.logic((r.data[0] & 0xffffU), 16U);
            break;
        }
        case 0x14198U: { // 6704 beq.b $1419e
            next = 0x1419aU;
            if ((r.status & 4U) != 0U) { next = 0x1419eU; transfer_kind = 1U; }
            break;
        }
        case 0x1419aU: { // 122d0017 move.b $17(a5), d1
            next = 0x1419eU;
            const auto v1 = m.byte((r.address[5] + 0x17U));
            m.db(1, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1419eU: { // 1b410001 move.b d1, $1(a5)
            next = 0x141a2U;
            const auto v1 = (r.data[1] & 0xffU);
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x141a2U: { // 605c bra.b $14200
            next = 0x141a4U;
            next = 0x14200U; transfer_kind = 1U;
            break;
        }
        case 0x141a4U: { // 2b7c000141d60002 move.l #$141d6, $2(a5)
            next = 0x141acU;
            const auto v1 = 0x141d6U;
            m.lng((r.address[5] + 0x2U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x141acU: { // 3b7c04d90006 move.w #$4d9, $6(a5)
            next = 0x141b2U;
            const auto v1 = 0x4d9U;
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x141b2U: { // 5b6d000c subq.w #$5, $c(a5)
            next = 0x141b6U;
            const auto v1 = m.sub(m.word((r.address[5] + 0xcU)), 0x5U, 16U);
            m.word((r.address[5] + 0xcU), v1);
            break;
        }
        case 0x141b6U: { // 7009 moveq #$9, d0
            next = 0x141b8U;
            r.data[0] = 0x9U; m.logic(r.data[0], 32U);
            break;
        }
        case 0x141b8U: { // 4a6d0016 tst.w $16(a5)
            next = 0x141bcU;
            m.logic(m.word((r.address[5] + 0x16U)), 16U);
            break;
        }
        case 0x141bcU: { // 6602 bne.b $141c0
            next = 0x141beU;
            if ((r.status & 4U) == 0U) { next = 0x141c0U; transfer_kind = 1U; }
            break;
        }
        case 0x141beU: { // 4440 neg.w d0
            next = 0x141c0U;
            const auto v1 = m.sub(0U, (r.data[0] & 0xffffU), 16U);
            m.dw(0, v1);
            break;
        }
        case 0x141c0U: { // d16d000e add.w d0, $e(a5)
            next = 0x141c4U;
            const auto v1 = m.add(m.word((r.address[5] + 0xeU)), (r.data[0] & 0xffffU), 16U);
            m.word((r.address[5] + 0xeU), v1);
            break;
        }
        case 0x141c4U: { // 3b7c0040001a move.w #$40, $1a(a5)
            next = 0x141caU;
            const auto v1 = 0x40U;
            m.word((r.address[5] + 0x1aU), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x141caU: { // 426d0020 clr.w $20(a5)
            next = 0x141ceU;
            (void)m.word((r.address[5] + 0x20U));
            m.word((r.address[5] + 0x20U), 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x141ceU: { // 3b7c00040022 move.w #$4, $22(a5)
            next = 0x141d4U;
            const auto v1 = 0x4U;
            m.word((r.address[5] + 0x22U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x141d4U: { // 601c bra.b $141f2
            next = 0x141d6U;
            next = 0x141f2U; transfer_kind = 1U;
            break;
        }
        case 0x141f2U: { // 41fa1b1e lea.l $15d12(pc), a0
            next = 0x141f6U;
            r.address[0] = 0x15d12U;
            break;
        }
        case 0x141f6U: { // 302d0020 move.w $20(a5), d0
            next = 0x141faU;
            const auto v1 = m.word((r.address[5] + 0x20U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x141faU: { // 1b7000000009 move.b (a0, d0.w), $9(a5)
            next = 0x14200U;
            const auto v1 = m.byte((r.address[0] + static_cast<std::uint32_t>(static_cast<std::int16_t>(r.data[0]))));
            m.byte((r.address[5] + 0x9U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x14200U: { // 4a2d0009 tst.b $9(a5)
            next = 0x14204U;
            m.logic(m.byte((r.address[5] + 0x9U)), 8U);
            break;
        }
        case 0x14204U: { // 6706 beq.b $1420c
            next = 0x14206U;
            if ((r.status & 4U) != 0U) { next = 0x1420cU; transfer_kind = 1U; }
            break;
        }
        case 0x14206U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1420cU;
            const auto result = m.indirect_call(c, 0x14206U, 0x15df2U, 0x1420cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1420cU: { // 4e75 rts 
            next = 0x1420eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1420cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x14164U:
        case 0x14168U:
        case 0x1416aU:
        case 0x1416eU:
        case 0x14172U:
        case 0x14178U:
        case 0x1417cU:
        case 0x14180U:
        case 0x14184U:
        case 0x14188U:
        case 0x1418eU:
        case 0x14192U:
        case 0x14196U:
        case 0x14198U:
        case 0x1419aU:
        case 0x1419eU:
        case 0x141a2U:
        case 0x141a4U:
        case 0x141acU:
        case 0x141b2U:
        case 0x141b6U:
        case 0x141b8U:
        case 0x141bcU:
        case 0x141beU:
        case 0x141c0U:
        case 0x141c4U:
        case 0x141caU:
        case 0x141ceU:
        case 0x141d4U:
        case 0x141f2U:
        case 0x141f6U:
        case 0x141faU:
        case 0x14200U:
        case 0x14204U:
        case 0x14206U:
        case 0x1420cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
