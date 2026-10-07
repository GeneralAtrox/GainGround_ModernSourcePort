
FunctionResult cpu_b_stage_01a664(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a3a6U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1a3acU;
            const auto result = m.call(c, 280U, 0x1a3a6U, 0x15d24U, 0x1a3acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a3acU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a3b2U;
            const auto result = m.call(c, 282U, 0x1a3acU, 0x15df2U, 0x1a3b2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a3b2U: { // 4e75 rts 
            next = 0x1a3b4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a3b2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a66cU: { // 610000cc bsr.w $1a73a
            next = 0x1a670U;
            const auto result = m.call(c, 941U, 0x1a66cU, 0x1a73aU, 0x1a670U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a674U: { // 4eb900012bb0 jsr $12bb0.l
            next = 0x1a67aU;
            const auto result = m.call(c, 258U, 0x1a674U, 0x12bb0U, 0x1a67aU);
            if (result.status == TranslationStatus::complete && result.control == 8U)
                return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a6a0U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1a6a6U;
            const auto result = m.call(c, 280U, 0x1a6a0U, 0x15d24U, 0x1a6a6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a6aaU: { // 6100ecee bsr.w $1939a
            next = 0x1a6aeU;
            const auto result = m.call(c, 904U, 0x1a6aaU, 0x1939aU, 0x1a6aeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a6aeU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a6b4U;
            const auto result = m.call(c, 282U, 0x1a6aeU, 0x15df2U, 0x1a6b4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a6b4U: { // 4e75 rts 
            next = 0x1a6b6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a6b4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01a664_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a37cU:
        case 0x1a382U:
        case 0x1a38aU:
        case 0x1a38eU:
        case 0x1a394U:
        case 0x1a3a2U:
        case 0x1a3a6U:
        case 0x1a3acU:
        case 0x1a3b2U:
        case 0x1a664U:
        case 0x1a668U:
        case 0x1a66cU:
        case 0x1a670U:
        case 0x1a674U:
        case 0x1a67aU:
        case 0x1a67eU:
        case 0x1a682U:
        case 0x1a686U:
        case 0x1a68aU:
        case 0x1a68eU:
        case 0x1a692U:
        case 0x1a696U:
        case 0x1a69aU:
        case 0x1a6a0U:
        case 0x1a6a6U:
        case 0x1a6aaU:
        case 0x1a6aeU:
        case 0x1a6b4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
