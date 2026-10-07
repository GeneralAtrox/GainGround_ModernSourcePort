
FunctionResult cpu_b_stage_01c85e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c85eU: { // 4eb900015e28 jsr $15e28.l
            next = 0x1c864U;
            const auto result = m.call(c, 283U, 0x1c85eU, 0x15e28U, 0x1c864U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1c8e6U: { // 4e75 rts 
            next = 0x1c8e8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c8e6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01c85e_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01c85e_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1c85eU:
        case 0x1c864U:
        case 0x1c866U:
        case 0x1c86aU:
        case 0x1c86cU:
        case 0x1c86eU:
        case 0x1c870U:
        case 0x1c872U:
        case 0x1c874U:
        case 0x1c876U:
        case 0x1c878U:
        case 0x1c87aU:
        case 0x1c87eU:
        case 0x1c882U:
        case 0x1c884U:
        case 0x1c888U:
        case 0x1c88cU:
        case 0x1c890U:
        case 0x1c894U:
        case 0x1c898U:
        case 0x1c89cU:
        case 0x1c8a0U:
        case 0x1c8a4U:
        case 0x1c8a6U:
        case 0x1c8a8U:
        case 0x1c8aaU:
        case 0x1c8acU:
        case 0x1c8b0U:
        case 0x1c8b4U:
        case 0x1c8b8U:
        case 0x1c8baU:
        case 0x1c8beU:
        case 0x1c8c0U:
        case 0x1c8c4U:
        case 0x1c8c6U:
        case 0x1c8caU:
        case 0x1c8ceU:
        case 0x1c8d2U:
        case 0x1c8d4U:
        case 0x1c8d8U:
        case 0x1c8daU:
        case 0x1c8deU:
        case 0x1c8e2U:
        case 0x1c8e6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01c76a_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1c76aU:
    case 0x1c76eU:
    case 0x1c774U:
    case 0x1c776U:
    case 0x1c77aU:
    case 0x1c77cU:
    case 0x1c780U:
    case 0x1c786U:
    case 0x1c78cU:
    case 0x1c790U:
    case 0x1c794U:
    case 0x1c79aU:
    case 0x1c79cU:
    case 0x1c7a2U:
    case 0x1c7a8U:
    case 0x1c7aeU:
    case 0x1c7b4U:
    case 0x1c7baU:
    case 0x1c7beU:
    case 0x1c7c4U:
    case 0x1c7c8U:
    case 0x1c7ccU:
    case 0x1c7d0U:
    case 0x1c7d2U:
    case 0x1c7d6U:
    case 0x1c7daU:
    case 0x1c7dcU:
    case 0x1c7e0U:
    case 0x1c7e2U:
    case 0x1c7e6U:
    case 0x1c7ecU:
    case 0x1c7eeU:
    case 0x1c7f2U:
    case 0x1c7f6U:
    case 0x1c7fcU:
    case 0x1c802U:
    case 0x1c804U:
    case 0x1c808U:
    case 0x1c80aU:
    case 0x1c80eU:
    case 0x1c812U:
    case 0x1c814U:
    case 0x1c818U:
    case 0x1c81aU:
    case 0x1c820U:
    case 0x1c822U:
    case 0x1c824U:
    case 0x1c82aU:
    case 0x1c830U:
    case 0x1c836U:
    case 0x1c83cU:
    case 0x1c842U:
    case 0x1c848U:
    case 0x1c84eU:
    case 0x1c852U:
    case 0x1c854U:
    case 0x1c858U:
    case 0x1c85cU:
        return true;
    default: return false;
    }
}
