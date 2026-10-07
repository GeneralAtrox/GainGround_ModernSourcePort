
FunctionResult cpu_b_stage_01a53c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a5aaU: { // 4eb900015d24 jsr $15d24.l
            next = 0x1a5b0U;
            const auto result = m.call(c, 280U, 0x1a5aaU, 0x15d24U, 0x1a5b0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a5b4U: { // 4eb90001939a jsr $1939a.l
            next = 0x1a5baU;
            const auto result = m.call(c, 904U, 0x1a5b4U, 0x1939aU, 0x1a5baU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a5baU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a5c0U;
            const auto result = m.call(c, 282U, 0x1a5baU, 0x15df2U, 0x1a5c0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a5c0U: { // 4e75 rts 
            next = 0x1a5c2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a5c0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a5c2U: { // 61000004 bsr.w $1a5c8
            next = 0x1a5c6U;
            const auto result = m.call(c, 938U, 0x1a5c2U, 0x1a5c8U, 0x1a5c6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a5c6U: { // 4e75 rts 
            next = 0x1a5c8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a5c6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01a53c_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a53cU:
        case 0x1a540U:
        case 0x1a544U:
        case 0x1a548U:
        case 0x1a54cU:
        case 0x1a554U:
        case 0x1a558U:
        case 0x1a55cU:
        case 0x1a560U:
        case 0x1a564U:
        case 0x1a568U:
        case 0x1a56cU:
        case 0x1a56eU:
        case 0x1a572U:
        case 0x1a576U:
        case 0x1a57aU:
        case 0x1a57eU:
        case 0x1a582U:
        case 0x1a584U:
        case 0x1a586U:
        case 0x1a58aU:
        case 0x1a58eU:
        case 0x1a592U:
        case 0x1a594U:
        case 0x1a598U:
        case 0x1a59aU:
        case 0x1a59cU:
        case 0x1a5a0U:
        case 0x1a5a4U:
        case 0x1a5aaU:
        case 0x1a5b0U:
        case 0x1a5b4U:
        case 0x1a5baU:
        case 0x1a5c0U:
        case 0x1a5c2U:
        case 0x1a5c6U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_019fcc_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x19fccU:
    case 0x19fd2U:
    case 0x19fd6U:
    case 0x19fdaU:
    case 0x19fdeU:
    case 0x19fe4U:
    case 0x19fe8U:
    case 0x19feaU:
    case 0x19feeU:
    case 0x19ff6U:
    case 0x19ffcU:
    case 0x1a002U:
    case 0x1a008U:
    case 0x1a00cU:
    case 0x1a010U:
    case 0x1a014U:
    case 0x1a018U:
    case 0x1a01cU:
    case 0x1a020U:
    case 0x1a024U:
    case 0x1a028U:
    case 0x1a02cU:
    case 0x1a030U:
    case 0x1a034U:
    case 0x1a038U:
    case 0x1a03cU:
    case 0x1a040U:
    case 0x1a044U:
    case 0x1a046U:
    case 0x1a04aU:
    case 0x1a04eU:
    case 0x1a052U:
    case 0x1a056U:
    case 0x1a058U:
    case 0x1a05aU:
    case 0x1a060U:
    case 0x1a062U:
    case 0x1a066U:
    case 0x1a068U:
    case 0x1a06cU:
    case 0x1a06eU:
    case 0x1a072U:
    case 0x1a076U:
    case 0x1a07aU:
    case 0x1a07cU:
    case 0x1a07eU:
    case 0x1a080U:
    case 0x1a084U:
    case 0x1a08aU:
    case 0x1a090U:
    case 0x1a094U:
    case 0x1a09aU:
    case 0x1a09cU:
    case 0x1a09eU:
    case 0x1a0a0U:
    case 0x1a0a2U:
    case 0x1a0a4U:
    case 0x1a0a6U:
    case 0x1a0a8U:
    case 0x1a0aeU:
    case 0x1a0b0U:
    case 0x1a0b6U:
    case 0x1a0b8U:
    case 0x1a0baU:
    case 0x1a0bcU:
    case 0x1a0beU:
    case 0x1a0c0U:
    case 0x1a0c2U:
    case 0x1a0c4U:
    case 0x1a0c6U:
    case 0x1a0c8U:
    case 0x1a0caU:
    case 0x1a0ceU:
    case 0x1a0d2U:
    case 0x1a0d6U:
    case 0x1a0daU:
    case 0x1a0deU:
        return true;
    default: return false;
    }
}
