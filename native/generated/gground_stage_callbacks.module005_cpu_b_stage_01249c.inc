
FunctionResult cpu_b_stage_01249c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1249cU: { // 6100039e bsr.w $1283c
            next = 0x124a0U;
            const auto result = m.call(c, 253U, 0x1249cU, 0x1283cU, 0x124a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x124a4U: { // 6100068c bsr.w $12b32
            next = 0x124a8U;
            const auto result = m.call(c, 257U, 0x124a4U, 0x12b32U, 0x124a8U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x124f6U: { // 4eb900015d24 jsr $15d24.l
            next = 0x124fcU;
            const auto result = m.call(c, 280U, 0x124f6U, 0x15d24U, 0x124fcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x124fcU: { // 4eb900015d3c jsr $15d3c.l
            next = 0x12502U;
            const auto result = m.call(c, 281U, 0x124fcU, 0x15d3cU, 0x12502U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12502U: { // 4eb900015df2 jsr $15df2.l
            next = 0x12508U;
            const auto result = m.call(c, 282U, 0x12502U, 0x15df2U, 0x12508U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12508U: { // 4e75 rts
            next = 0x1250aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12508U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01249c_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1249cU:
        case 0x124a0U:
        case 0x124a4U:
        case 0x124a8U:
        case 0x124acU:
        case 0x124b0U:
        case 0x124b4U:
        case 0x124b8U:
        case 0x124bcU:
        case 0x124c0U:
        case 0x124c4U:
        case 0x124caU:
        case 0x124ccU:
        case 0x124d0U:
        case 0x124d6U:
        case 0x124d8U:
        case 0x124deU:
        case 0x124e2U:
        case 0x124e4U:
        case 0x124e8U:
        case 0x124eaU:
        case 0x124eeU:
        case 0x124f2U:
        case 0x124f6U:
        case 0x124fcU:
        case 0x12502U:
        case 0x12508U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
