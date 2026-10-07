
FunctionResult cpu_b_stage_01e6cc(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e6d4U: { // 4e75 rts 
            next = 0x1e6d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1e6d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01e6cc_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01e6cc_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1e6ccU:
        case 0x1e6d2U:
        case 0x1e6d4U:
        case 0x1e6d6U:
        case 0x1e6d8U:
        case 0x1e6dcU:
        case 0x1e6e0U:
        case 0x1e6e2U:
        case 0x1e6e6U:
        case 0x1e6e8U:
        case 0x1e6eaU:
        case 0x1e6ecU:
        case 0x1e6f0U:
        case 0x1e6f2U:
        case 0x1e6f6U:
        case 0x1e6f8U:
        case 0x1e6faU:
        case 0x1e6fcU:
        case 0x1e700U:
        case 0x1e704U:
        case 0x1e708U:
        case 0x1e70cU:
        case 0x1e710U:
        case 0x1e714U:
        case 0x1e718U:
        case 0x1e71aU:
        case 0x1e71cU:
        case 0x1e71eU:
        case 0x1e720U:
        case 0x1e722U:
        case 0x1e724U:
        case 0x1e726U:
        case 0x1e728U:
        case 0x1e72eU:
        case 0x1e730U:
        case 0x1e734U:
        case 0x1e738U:
        case 0x1e73cU:
        case 0x1e740U:
        case 0x1e744U:
        case 0x1e748U:
        case 0x1e74eU:
        case 0x1e752U:
        case 0x1e756U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01e7fa_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1e7faU:
    case 0x1e7feU:
    case 0x1e800U:
    case 0x1e802U:
    case 0x1e804U:
    case 0x1e806U:
    case 0x1e808U:
    case 0x1e80eU:
    case 0x1e810U:
    case 0x1e812U:
    case 0x1e814U:
    case 0x1e816U:
    case 0x1e81aU:
    case 0x1e81cU:
    case 0x1e81eU:
    case 0x1e820U:
    case 0x1e822U:
    case 0x1e826U:
    case 0x1e828U:
    case 0x1e82cU:
    case 0x1e830U:
    case 0x1e832U:
    case 0x1e834U:
    case 0x1e836U:
    case 0x1e838U:
    case 0x1e83cU:
    case 0x1e840U:
    case 0x1e844U:
    case 0x1e848U:
    case 0x1e84aU:
    case 0x1e84cU:
    case 0x1e84eU:
    case 0x1e852U:
    case 0x1e858U:
    case 0x1e85aU:
    case 0x1e860U:
    case 0x1e862U:
    case 0x1e868U:
    case 0x1e86aU:
    case 0x1e86eU:
    case 0x1e872U:
    case 0x1e876U:
    case 0x1e878U:
    case 0x1e87aU:
    case 0x1e87cU:
    case 0x1e880U:
    case 0x1e884U:
    case 0x1e886U:
    case 0x1e888U:
    case 0x1e88aU:
    case 0x1e88cU:
    case 0x1e88eU:
    case 0x1e892U:
    case 0x1e896U:
    case 0x1e89aU:
    case 0x1e89cU:
    case 0x1e89eU:
    case 0x1e8a0U:
    case 0x1e8a2U:
    case 0x1e8a4U:
    case 0x1e8a8U:
    case 0x1e8aeU:
    case 0x1e8b2U:
    case 0x1e8b4U:
    case 0x1e8b8U:
    case 0x1e8baU:
    case 0x1e8bcU:
    case 0x1e8beU:
    case 0x1e8c0U:
    case 0x1e8c2U:
    case 0x1e8c4U:
    case 0x1e8c6U:
    case 0x1e8c8U:
    case 0x1e8ccU:
    case 0x1e8ceU:
    case 0x1e8d0U:
    case 0x1e8d2U:
    case 0x1e8d6U:
        return true;
    default: return false;
    }
}
