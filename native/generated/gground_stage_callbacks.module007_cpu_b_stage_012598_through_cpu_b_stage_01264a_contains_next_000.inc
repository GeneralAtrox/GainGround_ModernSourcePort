
FunctionResult cpu_b_stage_012598(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x12598U: { // 610002a2 bsr.w $1283c
            next = 0x1259cU;
            const auto result = m.call(c, 253U, 0x12598U, 0x1283cU, 0x1259cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x125a0U: { // 6100060e bsr.w $12bb0
            next = 0x125a4U;
            const auto result = m.call(c, 258U, 0x125a0U, 0x12bb0U, 0x125a4U);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12626U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1262cU;
            const auto result = m.call(c, 280U, 0x12626U, 0x15d24U, 0x1262cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1262cU: { // 4eb900015d3c jsr $15d3c.l
            next = 0x12632U;
            const auto result = m.call(c, 281U, 0x1262cU, 0x15d3cU, 0x12632U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12632U: { // 4eb900015df2 jsr $15df2.l
            next = 0x12638U;
            const auto result = m.call(c, 282U, 0x12632U, 0x15df2U, 0x12638U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12638U: { // 4e75 rts
            next = 0x1263aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12638U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1263eU: { // 4eb900016ff8 jsr $16ff8.l
            next = 0x12644U;
            const auto result = m.call(c, 308U, 0x1263eU, 0x16ff8U, 0x12644U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12644U: { // 61000708 bsr.w $12d4e
            next = 0x12648U;
            const auto result = m.call(c, 264U, 0x12644U, 0x12d4eU, 0x12648U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12648U: { // 4e75 rts
            next = 0x1264aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12648U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_012598_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_012598_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x12598U:
        case 0x1259cU:
        case 0x125a0U:
        case 0x125a4U:
        case 0x125a8U:
        case 0x125acU:
        case 0x125b0U:
        case 0x125b2U:
        case 0x125b6U:
        case 0x125baU:
        case 0x125bcU:
        case 0x125c0U:
        case 0x125c2U:
        case 0x125caU:
        case 0x125d2U:
        case 0x125d4U:
        case 0x125dcU:
        case 0x125e0U:
        case 0x125e4U:
        case 0x125e8U:
        case 0x125ecU:
        case 0x125f0U:
        case 0x125f4U:
        case 0x125f8U:
        case 0x125faU:
        case 0x125feU:
        case 0x12602U:
        case 0x12606U:
        case 0x1260cU:
        case 0x12610U:
        case 0x12612U:
        case 0x12616U:
        case 0x1261aU:
        case 0x12620U:
        case 0x12626U:
        case 0x1262cU:
        case 0x12632U:
        case 0x12638U:
        case 0x1263aU:
        case 0x1263eU:
        case 0x12644U:
        case 0x12648U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01264a_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x12610U:
    case 0x12612U:
    case 0x12616U:
    case 0x1261aU:
    case 0x12620U:
    case 0x12626U:
    case 0x1262cU:
    case 0x12632U:
    case 0x12638U:
    case 0x1263aU:
    case 0x1263eU:
    case 0x12644U:
    case 0x12648U:
    case 0x1264aU:
    case 0x1264eU:
    case 0x12650U:
    case 0x12654U:
    case 0x12656U:
    case 0x1265aU:
    case 0x1265eU:
    case 0x12660U:
    case 0x12664U:
    case 0x12668U:
    case 0x1266cU:
    case 0x12672U:
    case 0x12674U:
    case 0x12676U:
    case 0x1267aU:
    case 0x1267cU:
    case 0x1267eU:
    case 0x12680U:
    case 0x12682U:
    case 0x12684U:
    case 0x12688U:
    case 0x1268aU:
    case 0x1268eU:
    case 0x12692U:
    case 0x12696U:
    case 0x1269aU:
    case 0x126a0U:
    case 0x126a4U:
    case 0x126a8U:
    case 0x126acU:
        return true;
    default: return false;
    }
}
