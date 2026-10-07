
FunctionResult cpu_b_stage_gap_0141d6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x141d6U: { // 536d0022 subq.w #$1, $22(a5)
            next = 0x141daU;
            const auto v1 = m.sub(m.word((r.address[5] + 0x22U)), 0x1U, 16U);
            m.word((r.address[5] + 0x22U), v1);
            break;
        }
        case 0x141daU: { // 6e16 bgt.b $141f2
            next = 0x141dcU;
            if (((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U)) { next = 0x141f2U; transfer_kind = 1U; }
            break;
        }
        case 0x141dcU: { // 3b7c00040022 move.w #$4, $22(a5)
            next = 0x141e2U;
            const auto v1 = 0x4U;
            m.word((r.address[5] + 0x22U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x141e2U: { // 526d0020 addq.w #$1, $20(a5)
            next = 0x141e6U;
            const auto v1 = m.add(m.word((r.address[5] + 0x20U)), 0x1U, 16U);
            m.word((r.address[5] + 0x20U), v1);
            break;
        }
        case 0x141e6U: { // 0c6d00040020 cmpi.w #$4, $20(a5)
            next = 0x141ecU;
            (void)m.sub(m.word((r.address[5] + 0x20U)), 0x4U, 16U, true);
            break;
        }
        case 0x141ecU: { // 6b04 bmi.b $141f2
            next = 0x141eeU;
            if ((r.status & 8U) != 0U) { next = 0x141f2U; transfer_kind = 1U; }
            break;
        }
        case 0x141eeU: { // 4255 clr.w (a5)
            next = 0x141f0U;
            (void)m.word(r.address[5]);
            m.word(r.address[5], 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x141f0U: { // 4e75 rts 
            next = 0x141f2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x141f0U, r.program_counter)) return *event;
            return result;
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
        case 0x141d6U:
        case 0x141daU:
        case 0x141dcU:
        case 0x141e2U:
        case 0x141e6U:
        case 0x141ecU:
        case 0x141eeU:
        case 0x141f0U:
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
