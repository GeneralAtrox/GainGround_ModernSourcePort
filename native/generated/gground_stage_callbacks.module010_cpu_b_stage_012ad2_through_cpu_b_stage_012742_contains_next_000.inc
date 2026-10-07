
FunctionResult cpu_b_stage_012ad2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x12b2aU: { // 4e75 rts
            next = 0x12b2cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12b2aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x12b30U: { // 4e75 rts
            next = 0x12b32U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12b30U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_012ad2_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x12ad2U:
        case 0x12ad6U:
        case 0x12adaU:
        case 0x12adeU:
        case 0x12ae0U:
        case 0x12ae2U:
        case 0x12ae6U:
        case 0x12ae8U:
        case 0x12aecU:
        case 0x12aeeU:
        case 0x12af2U:
        case 0x12af6U:
        case 0x12afaU:
        case 0x12afcU:
        case 0x12afeU:
        case 0x12b02U:
        case 0x12b04U:
        case 0x12b08U:
        case 0x12b0aU:
        case 0x12b0eU:
        case 0x12b12U:
        case 0x12b16U:
        case 0x12b18U:
        case 0x12b1aU:
        case 0x12b1eU:
        case 0x12b20U:
        case 0x12b24U:
        case 0x12b26U:
        case 0x12b2aU:
        case 0x12b2cU:
        case 0x12b30U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_012742_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x12742U:
    case 0x12746U:
    case 0x1274aU:
    case 0x1274eU:
    case 0x12752U:
    case 0x12756U:
    case 0x12758U:
    case 0x1275cU:
    case 0x12760U:
    case 0x12764U:
    case 0x12768U:
    case 0x1276cU:
    case 0x12770U:
    case 0x12774U:
    case 0x12778U:
    case 0x1277cU:
    case 0x12780U:
    case 0x12786U:
    case 0x1278cU:
    case 0x1278eU:
    case 0x12792U:
    case 0x12794U:
    case 0x1279aU:
    case 0x127a0U:
    case 0x127a4U:
    case 0x127a6U:
    case 0x127aaU:
    case 0x127acU:
    case 0x127b0U:
    case 0x127b4U:
    case 0x127b6U:
    case 0x127b8U:
    case 0x127baU:
    case 0x127bcU:
    case 0x127c0U:
    case 0x127c4U:
    case 0x127c8U:
    case 0x127ceU:
    case 0x127d0U:
    case 0x127d6U:
    case 0x127d8U:
    case 0x127dcU:
    case 0x127e0U:
    case 0x127e4U:
    case 0x127e8U:
    case 0x127ecU:
    case 0x127f2U:
    case 0x127f4U:
    case 0x127f6U:
    case 0x127faU:
    case 0x127fcU:
    case 0x127feU:
    case 0x12802U:
    case 0x12804U:
    case 0x12808U:
    case 0x1280aU:
    case 0x1280cU:
    case 0x12810U:
    case 0x12814U:
    case 0x12818U:
    case 0x1281cU:
    case 0x12822U:
    case 0x12828U:
    case 0x1282eU:
    case 0x12834U:
    case 0x1283aU:
        return true;
    default: return false;
    }
}
