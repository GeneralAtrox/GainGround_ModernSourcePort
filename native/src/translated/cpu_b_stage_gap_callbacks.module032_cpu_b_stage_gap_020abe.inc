
FunctionResult cpu_b_stage_gap_020abe(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20abeU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x20ac4U;
            const auto v1 = m.byte((r.address[5] + 0x41U));
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | (((v1 >> 3U) & 1U) ? 0U : 4U));
            break;
        }
        case 0x20ac4U: { // 6728 beq.b $20aee
            next = 0x20ac6U;
            if ((r.status & 4U) != 0U) { next = 0x20aeeU; transfer_kind = 1U; }
            break;
        }
        case 0x20ac6U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20acaU;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x20acaU: { // 6708 beq.b $20ad4
            next = 0x20accU;
            if ((r.status & 4U) != 0U) { next = 0x20ad4U; transfer_kind = 1U; }
            break;
        }
        case 0x20accU: { // 4df83400 lea.l $3400.w, a6
            next = 0x20ad0U;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x20ad0U: { // 522e0028 addq.b #$1, $28(a6)
            next = 0x20ad4U;
            const auto v1 = m.add(m.byte((r.address[6] + 0x28U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x28U), v1);
            break;
        }
        case 0x20ad4U: { // 6100c420 bsr.w $1cef6
            next = 0x20ad8U;
            const auto result = m.call(c, 334U, 0x20ad4U, 0x1cef6U, 0x20ad8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ad8U: { // 6100d0ea bsr.w $1dbc4
            next = 0x20adcU;
            const auto result = m.call(c, 352U, 0x20ad8U, 0x1dbc4U, 0x20adcU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20adcU: { // 6100d112 bsr.w $1dbf0
            next = 0x20ae0U;
            const auto result = m.call(c, 353U, 0x20adcU, 0x1dbf0U, 0x20ae0U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ae0U: { // 6100d642 bsr.w $1e124
            next = 0x20ae4U;
            const auto result = m.call(c, 357U, 0x20ae0U, 0x1e124U, 0x20ae4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ae4U: { // 6100d81e bsr.w $1e304
            next = 0x20ae8U;
            const auto result = m.call(c, 360U, 0x20ae4U, 0x1e304U, 0x20ae8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ae8U: { // 6100e1fa bsr.w $1ece4
            next = 0x20aecU;
            const auto result = m.call(c, 365U, 0x20ae8U, 0x1ece4U, 0x20aecU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20aecU: { // 4e75 rts 
            next = 0x20aeeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20aecU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20aeeU: { // 4a2d003f tst.b $3f(a5)
            next = 0x20af2U;
            m.logic(m.byte((r.address[5] + 0x3fU)), 8U);
            break;
        }
        case 0x20af2U: { // 6708 beq.b $20afc
            next = 0x20af4U;
            if ((r.status & 4U) != 0U) { next = 0x20afcU; transfer_kind = 1U; }
            break;
        }
        case 0x20af4U: { // 4df83400 lea.l $3400.w, a6
            next = 0x20af8U;
            r.address[6] = 0x3400U;
            break;
        }
        case 0x20af8U: { // 522e0028 addq.b #$1, $28(a6)
            next = 0x20afcU;
            const auto v1 = m.add(m.byte((r.address[6] + 0x28U)), 0x1U, 8U);
            m.byte((r.address[6] + 0x28U), v1);
            break;
        }
        case 0x20afcU: { // 6100af8e bsr.w $1ba8c
            next = 0x20b00U;
            const auto result = m.call(c, 562U, 0x20afcU, 0x1ba8cU, 0x20b00U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20b00U: { // 6100d0c2 bsr.w $1dbc4
            next = 0x20b04U;
            const auto result = m.call(c, 352U, 0x20b00U, 0x1dbc4U, 0x20b04U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20b04U: { // 6100d0ea bsr.w $1dbf0
            next = 0x20b08U;
            const auto result = m.call(c, 353U, 0x20b04U, 0x1dbf0U, 0x20b08U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20b08U: { // 6100d326 bsr.w $1de30
            next = 0x20b0cU;
            const auto result = m.call(c, 355U, 0x20b08U, 0x1de30U, 0x20b0cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20b0cU: { // 6100d5e0 bsr.w $1e0ee
            next = 0x20b10U;
            const auto result = m.call(c, 356U, 0x20b0cU, 0x1e0eeU, 0x20b10U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20b10U: { // 6100d612 bsr.w $1e124
            next = 0x20b14U;
            const auto result = m.call(c, 357U, 0x20b10U, 0x1e124U, 0x20b14U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20b14U: { // 6100d7ee bsr.w $1e304
            next = 0x20b18U;
            const auto result = m.call(c, 360U, 0x20b14U, 0x1e304U, 0x20b18U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20b18U: { // 4e75 rts 
            next = 0x20b1aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20b18U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20abeU:
        case 0x20ac4U:
        case 0x20ac6U:
        case 0x20acaU:
        case 0x20accU:
        case 0x20ad0U:
        case 0x20ad4U:
        case 0x20ad8U:
        case 0x20adcU:
        case 0x20ae0U:
        case 0x20ae4U:
        case 0x20ae8U:
        case 0x20aecU:
        case 0x20aeeU:
        case 0x20af2U:
        case 0x20af4U:
        case 0x20af8U:
        case 0x20afcU:
        case 0x20b00U:
        case 0x20b04U:
        case 0x20b08U:
        case 0x20b0cU:
        case 0x20b10U:
        case 0x20b14U:
        case 0x20b18U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
