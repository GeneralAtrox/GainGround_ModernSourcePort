
FunctionResult cpu_b_stage_gap_0233b0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x233b0U: { // 61000430 bsr.w $237e2
            next = 0x233b4U;
            const auto result = m.call(c, 412U, 0x233b0U, 0x237e2U, 0x233b4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x233b4U: { // 45f900024718 lea.l $24718.l, a2
            next = 0x233baU;
            r.address[2] = 0x24718U;
            break;
        }
        case 0x233baU: { // 6100056e bsr.w $2392a
            next = 0x233beU;
            const auto result = m.call(c, 419U, 0x233baU, 0x2392aU, 0x233beU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x233beU: { // 4e75 rts 
            next = 0x233c0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x233beU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x233b0U:
        case 0x233b4U:
        case 0x233baU:
        case 0x233beU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

