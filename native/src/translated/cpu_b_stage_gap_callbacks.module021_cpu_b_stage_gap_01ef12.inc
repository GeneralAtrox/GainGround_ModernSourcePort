
FunctionResult cpu_b_stage_gap_01ef12(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1ef12U: { // 41f900031898 lea.l $31898.l, a0
            next = 0x1ef18U;
            r.address[0] = 0x31898U;
            break;
        }
        case 0x1ef18U: { // 302d0054 move.w $54(a5), d0
            next = 0x1ef1cU;
            const auto v1 = m.word((r.address[5] + 0x54U));
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ef1cU: { // b018 cmp.b (a0)+, d0
            next = 0x1ef1eU;
            const auto ea1 = r.address[0]; r.address[0] += 1U;
            (void)m.sub((r.data[0] & 0xffU), m.byte(ea1), 8U, true);
            break;
        }
        case 0x1ef1eU: { // 6f06 ble.b $1ef26
            next = 0x1ef20U;
            if (((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U)) { next = 0x1ef26U; transfer_kind = 1U; }
            break;
        }
        case 0x1ef20U: { // d0fc0005 adda.w #$5, a0
            next = 0x1ef24U;
            r.address[0] += 0x5U;
            break;
        }
        case 0x1ef24U: { // 60f6 bra.b $1ef1c
            next = 0x1ef26U;
            next = 0x1ef1cU; transfer_kind = 1U;
            break;
        }
        case 0x1ef26U: { // 303c0c00 move.w #$c00, d0
            next = 0x1ef2aU;
            const auto v1 = 0xc00U;
            m.dw(0, v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ef2aU: { // d018 add.b (a0)+, d0
            next = 0x1ef2cU;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = m.add((r.data[0] & 0xffU), m.byte(ea2), 8U);
            m.db(0, v1);
            break;
        }
        case 0x1ef2cU: { // 3b400008 move.w d0, $8(a5)
            next = 0x1ef30U;
            const auto v1 = (r.data[0] & 0xffffU);
            m.word((r.address[5] + 0x8U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ef30U: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x1ef34U;
            const auto ea2 = r.address[0]; r.address[0] += 2U;
            const auto v1 = m.word(ea2);
            m.word((r.address[5] + 0x6U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ef34U: { // 6b26 bmi.b $1ef5c
            next = 0x1ef36U;
            if ((r.status & 8U) != 0U) { next = 0x1ef5cU; transfer_kind = 1U; }
            break;
        }
        case 0x1ef36U: { // 1218 move.b (a0)+, d1
            next = 0x1ef38U;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = m.byte(ea2);
            m.db(1, v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1ef38U: { // 6722 beq.b $1ef5c
            next = 0x1ef3aU;
            if ((r.status & 4U) != 0U) { next = 0x1ef5cU; transfer_kind = 1U; }
            break;
        }
        case 0x1ef3aU: { // 08d50007 bset.b #$7, (a5)
            next = 0x1ef3eU;
            const auto v1 = m.byte(r.address[5]);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 7U) & 1U) ? 0U : 4U));
            m.byte(r.address[5], (v1 | (1U << 7U)) & 0xffU);
            break;
        }
        case 0x1ef3eU: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x1ef42U;
            const auto ea2 = r.address[0]; r.address[0] += 1U;
            const auto v1 = m.byte(ea2);
            m.byte((r.address[5] + 0x1U), v1);
            m.logic(v1, 8U);
            break;
        }
        case 0x1ef42U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1ef46U;
            const auto v1 = m.add(m.word((r.address[5] + 0x54U)), 0x1U, 16U);
            m.word((r.address[5] + 0x54U), v1);
            break;
        }
        case 0x1ef46U: { // 6100fd9c bsr.w $1ece4
            next = 0x1ef4aU;
            const auto result = m.call(c, 365U, 0x1ef46U, 0x1ece4U, 0x1ef4aU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ef4aU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1ef50U;
            const auto result = m.indirect_call(c, 0x1ef4aU, 0x15df2U, 0x1ef50U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ef50U: { // 3b7c01000046 move.w #$100, $46(a5)
            next = 0x1ef56U;
            const auto v1 = 0x100U;
            m.word((r.address[5] + 0x46U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x1ef56U: { // 422d003f clr.b $3f(a5)
            next = 0x1ef5aU;
            (void)m.byte((r.address[5] + 0x3fU));
            m.byte((r.address[5] + 0x3fU), 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1ef5aU: { // 4e75 rts 
            next = 0x1ef5cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1ef5aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1ef5cU: { // 08950007 bclr.b #$7, (a5)
            next = 0x1ef60U;
            const auto v1 = m.byte(r.address[5]);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 7U) & 1U) ? 0U : 4U));
            m.byte(r.address[5], (v1 & ~(1U << 7U)) & 0xffU);
            break;
        }
        case 0x1ef60U: { // 4e75 rts 
            next = 0x1ef62U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1ef60U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1ef12U:
        case 0x1ef18U:
        case 0x1ef1cU:
        case 0x1ef1eU:
        case 0x1ef20U:
        case 0x1ef24U:
        case 0x1ef26U:
        case 0x1ef2aU:
        case 0x1ef2cU:
        case 0x1ef30U:
        case 0x1ef34U:
        case 0x1ef36U:
        case 0x1ef38U:
        case 0x1ef3aU:
        case 0x1ef3eU:
        case 0x1ef42U:
        case 0x1ef46U:
        case 0x1ef4aU:
        case 0x1ef50U:
        case 0x1ef56U:
        case 0x1ef5aU:
        case 0x1ef5cU:
        case 0x1ef60U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
