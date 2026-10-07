
FunctionResult cpu_b_stage_gap_01d04e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1d068U: { // 61000dc6 bsr.w $1de30
            next = 0x1d06cU;
            const auto result = m.call(c, 355U, 0x1d068U, 0x1de30U, 0x1d06cU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1d0a2U: { // 4e75 rts 
            next = 0x1d0a4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1d0a2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1d0d4U: { // 4e75 rts 
            next = 0x1d0d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1d0d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1d9e8U: { // 4e75 rts 
            next = 0x1d9eaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1d9e8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1da18U: { // 4e75 rts 
            next = 0x1da1aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1da18U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_gap_01d04e_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1d04eU:
        case 0x1d052U:
        case 0x1d054U:
        case 0x1d058U:
        case 0x1d05cU:
        case 0x1d05eU:
        case 0x1d062U:
        case 0x1d068U:
        case 0x1d06cU:
        case 0x1d072U:
        case 0x1d074U:
        case 0x1d07aU:
        case 0x1d07cU:
        case 0x1d07eU:
        case 0x1d084U:
        case 0x1d08aU:
        case 0x1d090U:
        case 0x1d096U:
        case 0x1d09cU:
        case 0x1d0a2U:
        case 0x1d0a4U:
        case 0x1d0aaU:
        case 0x1d0b0U:
        case 0x1d0b6U:
        case 0x1d0bcU:
        case 0x1d0c2U:
        case 0x1d0c8U:
        case 0x1d0ceU:
        case 0x1d0d4U:
        case 0x1d9daU:
        case 0x1d9e2U:
        case 0x1d9e8U:
        case 0x1d9eaU:
        case 0x1d9f0U:
        case 0x1d9f4U:
        case 0x1d9faU:
        case 0x1da00U:
        case 0x1da06U:
        case 0x1da0cU:
        case 0x1da12U:
        case 0x1da18U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_gap_01e75a_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1e1f2U:
    case 0x1e1f6U:
    case 0x1e1f8U:
    case 0x1e1fcU:
    case 0x1e200U:
    case 0x1e204U:
    case 0x1e208U:
    case 0x1e20cU:
    case 0x1e20eU:
    case 0x1e212U:
    case 0x1e216U:
    case 0x1e21aU:
    case 0x1e21cU:
    case 0x1e220U:
    case 0x1e224U:
    case 0x1e22aU:
    case 0x1e230U:
    case 0x1e236U:
    case 0x1e75aU:
    case 0x1e75eU:
    case 0x1e760U:
    case 0x1e766U:
    case 0x1e768U:
    case 0x1e76aU:
    case 0x1e76eU:
    case 0x1e770U:
    case 0x1e774U:
    case 0x1e778U:
    case 0x1e77cU:
    case 0x1e780U:
    case 0x1e782U:
    case 0x1e786U:
    case 0x1e78aU:
    case 0x1e78eU:
    case 0x1e790U:
    case 0x1e794U:
    case 0x1e798U:
    case 0x1e79cU:
    case 0x1e7a2U:
    case 0x1e7a8U:
    case 0x1e7aaU:
    case 0x1e7aeU:
    case 0x1e7b2U:
    case 0x1e7b6U:
    case 0x1e7b8U:
    case 0x1e7baU:
    case 0x1e7bcU:
    case 0x1e7beU:
    case 0x1e7c0U:
    case 0x1e7c2U:
    case 0x1e7c6U:
    case 0x1e7caU:
    case 0x1e7ccU:
    case 0x1e7ceU:
    case 0x1e7d0U:
    case 0x1e7d2U:
    case 0x1e7d6U:
    case 0x1e7d8U:
    case 0x1e7dcU:
    case 0x1e7e0U:
    case 0x1e7e4U:
    case 0x1e7e6U:
    case 0x1e7eaU:
    case 0x1e7ecU:
    case 0x1e7eeU:
    case 0x1e7f0U:
    case 0x1e7f2U:
    case 0x1e7f6U:
        return true;
    default: return false;
    }
}
