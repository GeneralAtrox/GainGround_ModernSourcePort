
FunctionResult cpu_b_stage_0193e4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x193eeU: { // 4e75 rts 
            next = 0x193f0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x193eeU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19434U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1943aU;
            const auto result = m.call(c, 282U, 0x19434U, 0x15df2U, 0x1943aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1943eU: { // 4e75 rts 
            next = 0x19440U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1943eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_0193e4_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x193e4U:
        case 0x193e8U:
        case 0x193eaU:
        case 0x193ecU:
        case 0x193eeU:
        case 0x193f0U:
        case 0x193f4U:
        case 0x193f6U:
        case 0x193faU:
        case 0x193feU:
        case 0x19400U:
        case 0x19402U:
        case 0x19404U:
        case 0x19408U:
        case 0x1940cU:
        case 0x1940eU:
        case 0x19410U:
        case 0x19414U:
        case 0x19418U:
        case 0x1941eU:
        case 0x19424U:
        case 0x19426U:
        case 0x1942aU:
        case 0x1942eU:
        case 0x19434U:
        case 0x1943aU:
        case 0x1943eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01952c_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1952cU:
    case 0x19530U:
    case 0x19534U:
    case 0x19538U:
    case 0x1953aU:
    case 0x1953eU:
    case 0x19542U:
    case 0x19546U:
    case 0x1954aU:
    case 0x1954eU:
    case 0x19552U:
    case 0x19556U:
    case 0x1955aU:
    case 0x1955eU:
    case 0x19562U:
    case 0x19564U:
    case 0x1956aU:
    case 0x19570U:
    case 0x19574U:
    case 0x1957aU:
    case 0x19582U:
    case 0x1958aU:
    case 0x1958cU:
    case 0x19590U:
    case 0x19594U:
    case 0x19598U:
    case 0x1959aU:
    case 0x1959cU:
    case 0x195a0U:
    case 0x195a4U:
    case 0x195a6U:
    case 0x195acU:
    case 0x195aeU:
    case 0x195b2U:
    case 0x195b6U:
    case 0x195baU:
    case 0x195beU:
    case 0x195c4U:
    case 0x195caU:
    case 0x195ceU:
    case 0x195d0U:
    case 0x195d4U:
    case 0x195daU:
    case 0x195dcU:
    case 0x195deU:
    case 0x195e2U:
    case 0x195e8U:
    case 0x195eeU:
    case 0x195f4U:
    case 0x195faU:
    case 0x195feU:
    case 0x19600U:
    case 0x19604U:
    case 0x1960aU:
    case 0x1960eU:
    case 0x19614U:
    case 0x19616U:
    case 0x1961cU:
    case 0x19622U:
    case 0x19626U:
    case 0x1962cU:
    case 0x1962eU:
    case 0x19634U:
    case 0x1963aU:
    case 0x1963eU:
    case 0x19644U:
    case 0x1964aU:
    case 0x1964cU:
    case 0x19650U:
    case 0x19652U:
    case 0x19658U:
    case 0x1965cU:
    case 0x19662U:
    case 0x19664U:
    case 0x1966aU:
    case 0x19670U:
    case 0x19674U:
    case 0x1967aU:
    case 0x1967cU:
    case 0x19680U:
    case 0x19686U:
    case 0x19688U:
    case 0x1968aU:
    case 0x1968eU:
    case 0x19694U:
    case 0x1969aU:
    case 0x196a0U:
    case 0x196a6U:
    case 0x196aaU:
    case 0x196acU:
    case 0x196b2U:
    case 0x196b8U:
    case 0x196bcU:
    case 0x196c2U:
    case 0x196caU:
    case 0x196d2U:
    case 0x196d4U:
    case 0x196d8U:
    case 0x196deU:
    case 0x196e2U:
    case 0x196e8U:
    case 0x196eeU:
    case 0x196f4U:
        return true;
    default: return false;
    }
}
