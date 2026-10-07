
FunctionResult cpu_b_stage_gap_021698(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21698U: { // 4a2d003f tst.b $3f(a5)
            next = 0x2169cU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x2169cU: { // 671c beq.b $216ba
            next = 0x2169eU;
            if ((r.status & 4U) != 0U) { next = 0x216baU; transfer_kind = 1U; }
            break;
        }
        case 0x2169eU: { // 4df83400 lea.l $3400.w, a6
            next = 0x216a2U;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x216a2U: { // 532e0036 subq.b #$1, $36(a6)
            next = 0x216a6U;
            const auto v1 = m.sub(m.byte((r.address[6] + 0x36U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x36U), v1);
            break;
        }
        case 0x216a6U: { // 2b7c0001ee780002 move.l #$1ee78, $2(a5)
            next = 0x216aeU;
            const auto v1 = 0x1ee78U;
            m.lng((r.address[5] + 0x2U), v1);
            m.logic(v1, 32U);
            break;
        }
        case 0x216aeU: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x216b4U;
            const auto v1 = 0x0U;
            m.word((r.address[5] + 0x54U), v1);
            m.logic(v1, 16U);
            break;
        }
        case 0x216b4U: { // 6100d0a4 bsr.w $1e75a
            next = 0x216b8U;
            const auto result = m.call(c, 683U, 0x216b4U, 0x1e75aU, 0x216b8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216b8U: { // 4e75 rts 
            next = 0x216baU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x216b8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x216baU: { // 082d00010078 btst.b #$1, $78(a5)
            next = 0x216c0U;
            const auto v1 = m.byte((r.address[5] + 0x78U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 1U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x216c0U: { // 670e beq.b $216d0
            next = 0x216c2U;
            if ((r.status & 4U) != 0U) { next = 0x216d0U; transfer_kind = 1U; }
            break;
        }
        case 0x216c2U: { // 6100eb64 bsr.w $20228
            next = 0x216c6U;
            const auto result = m.call(c, 684U, 0x216c2U, 0x20228U, 0x216c6U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216c6U: { // 6100d092 bsr.w $1e75a
            next = 0x216caU;
            const auto result = m.call(c, 683U, 0x216c6U, 0x1e75aU, 0x216caU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216caU: { // 6100d618 bsr.w $1ece4
            next = 0x216ceU;
            const auto result = m.call(c, 365U, 0x216caU, 0x1ece4U, 0x216ceU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216ceU: { // 4e75 rts 
            next = 0x216d0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x216ceU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x216d0U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x216d6U;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x216d6U: { // 671a beq.b $216f2
            next = 0x216d8U;
            if ((r.status & 4U) != 0U) { next = 0x216f2U; transfer_kind = 1U; }
            break;
        }
        case 0x216d8U: { // 6100b974 bsr.w $1d04e
            next = 0x216dcU;
            const auto result = m.call(c, 685U, 0x216d8U, 0x1d04eU, 0x216dcU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216dcU: { // 6100c4e6 bsr.w $1dbc4
            next = 0x216e0U;
            const auto result = m.call(c, 352U, 0x216dcU, 0x1dbc4U, 0x216e0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216e0U: { // 6100c50e bsr.w $1dbf0
            next = 0x216e4U;
            const auto result = m.call(c, 353U, 0x216e0U, 0x1dbf0U, 0x216e4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216e4U: { // 6100ca3e bsr.w $1e124
            next = 0x216e8U;
            const auto result = m.call(c, 357U, 0x216e4U, 0x1e124U, 0x216e8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216e8U: { // 6100d070 bsr.w $1e75a
            next = 0x216ecU;
            const auto result = m.call(c, 683U, 0x216e8U, 0x1e75aU, 0x216ecU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216ecU: { // 6100d5f6 bsr.w $1ece4
            next = 0x216f0U;
            const auto result = m.call(c, 365U, 0x216ecU, 0x1ece4U, 0x216f0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216f0U: { // 4e75 rts 
            next = 0x216f2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x216f0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x216f2U: { // 6100a438 bsr.w $1bb2c
            next = 0x216f6U;
            const auto result = m.call(c, 564U, 0x216f2U, 0x1bb2cU, 0x216f6U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216f6U: { // 6100c4cc bsr.w $1dbc4
            next = 0x216faU;
            const auto result = m.call(c, 352U, 0x216f6U, 0x1dbc4U, 0x216faU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216faU: { // 6100c4f4 bsr.w $1dbf0
            next = 0x216feU;
            const auto result = m.call(c, 353U, 0x216faU, 0x1dbf0U, 0x216feU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x216feU: { // 6100c730 bsr.w $1de30
            next = 0x21702U;
            const auto result = m.call(c, 355U, 0x216feU, 0x1de30U, 0x21702U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21702U: { // 6100c9ea bsr.w $1e0ee
            next = 0x21706U;
            const auto result = m.call(c, 356U, 0x21702U, 0x1e0eeU, 0x21706U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21706U: { // 6100ca1c bsr.w $1e124
            next = 0x2170aU;
            const auto result = m.call(c, 357U, 0x21706U, 0x1e124U, 0x2170aU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2170aU: { // 6100d04e bsr.w $1e75a
            next = 0x2170eU;
            const auto result = m.call(c, 683U, 0x2170aU, 0x1e75aU, 0x2170eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2170eU: { // 6100d5d4 bsr.w $1ece4
            next = 0x21712U;
            const auto result = m.call(c, 365U, 0x2170eU, 0x1ece4U, 0x21712U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21712U: { // 4e75 rts 
            next = 0x21714U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21712U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21698U:
        case 0x2169cU:
        case 0x2169eU:
        case 0x216a2U:
        case 0x216a6U:
        case 0x216aeU:
        case 0x216b4U:
        case 0x216b8U:
        case 0x216baU:
        case 0x216c0U:
        case 0x216c2U:
        case 0x216c6U:
        case 0x216caU:
        case 0x216ceU:
        case 0x216d0U:
        case 0x216d6U:
        case 0x216d8U:
        case 0x216dcU:
        case 0x216e0U:
        case 0x216e4U:
        case 0x216e8U:
        case 0x216ecU:
        case 0x216f0U:
        case 0x216f2U:
        case 0x216f6U:
        case 0x216faU:
        case 0x216feU:
        case 0x21702U:
        case 0x21706U:
        case 0x2170aU:
        case 0x2170eU:
        case 0x21712U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
