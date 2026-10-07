
FunctionResult cpu_b_stage_019b62(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19b66U: { // 4eb900012bb0 jsr $12bb0.l
            next = 0x19b6cU;
            const auto result = m.call(c, 258U, 0x19b66U, 0x12bb0U, 0x19b6cU);
            if (result.status == TranslationStatus::complete && result.control == 8U)
                return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19b88U: { // 4eb900016372 jsr $16372.l
            next = 0x19b8eU;
            const auto result = m.call(c, 306U, 0x19b88U, 0x16372U, 0x19b8eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19bf0U: { // 4eb900015d24 jsr $15d24.l
            next = 0x19bf6U;
            const auto result = m.call(c, 280U, 0x19bf0U, 0x15d24U, 0x19bf6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19bfaU: { // 6100f79e bsr.w $1939a
            next = 0x19bfeU;
            const auto result = m.call(c, 904U, 0x19bfaU, 0x1939aU, 0x19bfeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19bfeU: { // 4eb900015df2 jsr $15df2.l
            next = 0x19c04U;
            const auto result = m.call(c, 282U, 0x19bfeU, 0x15df2U, 0x19c04U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19c04U: { // 4e75 rts 
            next = 0x19c06U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19c04U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19c06U: { // 6100efce bsr.w $18bd6
            next = 0x19c0aU;
            const auto result = m.call(c, 891U, 0x19c06U, 0x18bd6U, 0x19c0aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19c0aU: { // 4e75 rts 
            next = 0x19c0cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19c0aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_019b62_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_019b62_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_019b62_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_019806_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x19806U:
    case 0x19808U:
    case 0x1980cU:
    case 0x19810U:
    case 0x19814U:
    case 0x19818U:
    case 0x1981eU:
    case 0x19820U:
    case 0x19824U:
    case 0x19828U:
    case 0x1982aU:
    case 0x1982eU:
    case 0x19834U:
    case 0x19836U:
    case 0x1983aU:
    case 0x1983cU:
    case 0x1983eU:
    case 0x19842U:
    case 0x19844U:
    case 0x19848U:
    case 0x1984cU:
    case 0x1984eU:
    case 0x19850U:
    case 0x19852U:
    case 0x19854U:
    case 0x19858U:
    case 0x1985cU:
    case 0x19860U:
    case 0x19866U:
    case 0x19868U:
    case 0x1986aU:
    case 0x19870U:
    case 0x19872U:
    case 0x19876U:
    case 0x19878U:
    case 0x1987cU:
    case 0x19880U:
    case 0x19884U:
    case 0x19886U:
    case 0x1988aU:
    case 0x1988eU:
    case 0x19892U:
    case 0x19896U:
    case 0x19898U:
    case 0x1989cU:
    case 0x1989eU:
    case 0x198a2U:
    case 0x198a8U:
    case 0x198aeU:
    case 0x198b2U:
    case 0x198b8U:
    case 0x198c0U:
    case 0x198c6U:
    case 0x198ccU:
    case 0x198d0U:
    case 0x198d4U:
    case 0x198d8U:
    case 0x198dcU:
    case 0x198e0U:
    case 0x198e4U:
    case 0x198e8U:
    case 0x198ecU:
    case 0x198f0U:
    case 0x198f4U:
    case 0x198f8U:
    case 0x198fcU:
    case 0x19900U:
    case 0x19904U:
    case 0x19908U:
    case 0x1990cU:
    case 0x19912U:
    case 0x19918U:
    case 0x1991cU:
    case 0x19922U:
    case 0x19928U:
    case 0x1992cU:
    case 0x19930U:
    case 0x19932U:
    case 0x19936U:
        return true;
    default: return false;
    }
}
