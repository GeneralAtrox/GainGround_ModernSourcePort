
FunctionResult cpu_b_stage_01d0d6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1d0f0U: { // 61000d3e bsr.w $1de30
            next = 0x1d0f4U;
            const auto result = m.call(c, 355U, 0x1d0f0U, 0x1de30U, 0x1d0f4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1d12aU: { // 4e75 rts 
            next = 0x1d12cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1d12aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01d0d6_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1d0d6U:
        case 0x1d0daU:
        case 0x1d0dcU:
        case 0x1d0e0U:
        case 0x1d0e4U:
        case 0x1d0e6U:
        case 0x1d0eaU:
        case 0x1d0f0U:
        case 0x1d0f4U:
        case 0x1d0faU:
        case 0x1d0fcU:
        case 0x1d102U:
        case 0x1d104U:
        case 0x1d106U:
        case 0x1d10cU:
        case 0x1d112U:
        case 0x1d118U:
        case 0x1d11eU:
        case 0x1d124U:
        case 0x1d12aU:
        case 0x1d12cU:
        case 0x1d132U:
        case 0x1d138U:
        case 0x1d13eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01e472_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1e472U:
    case 0x1e476U:
    case 0x1e478U:
    case 0x1e47cU:
    case 0x1e480U:
    case 0x1e484U:
    case 0x1e488U:
    case 0x1e48cU:
    case 0x1e48eU:
    case 0x1e490U:
    case 0x1e494U:
    case 0x1e496U:
    case 0x1e49aU:
    case 0x1e49eU:
    case 0x1e4a2U:
    case 0x1e4a6U:
    case 0x1e4acU:
    case 0x1e4aeU:
    case 0x1e4b2U:
    case 0x1e4b6U:
    case 0x1e4b8U:
    case 0x1e4baU:
    case 0x1e4bcU:
    case 0x1e4beU:
    case 0x1e4c0U:
    case 0x1e4c4U:
    case 0x1e4c8U:
    case 0x1e4caU:
    case 0x1e4ceU:
    case 0x1e4d2U:
    case 0x1e4d8U:
    case 0x1e4dcU:
    case 0x1e4deU:
    case 0x1e4e2U:
    case 0x1e4e6U:
    case 0x1e4eaU:
    case 0x1e4eeU:
    case 0x1e4f0U:
    case 0x1e4f4U:
    case 0x1e4f8U:
    case 0x1e4fcU:
    case 0x1e4feU:
    case 0x1e502U:
    case 0x1e506U:
    case 0x1e50cU:
    case 0x1e512U:
        return true;
    default: return false;
    }
}
