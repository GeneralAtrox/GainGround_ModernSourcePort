
static bool cpu_b_stage_019c0e_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19c1aU: { // 49fa19e6 lea.l $1b602(pc), a4
            next = 0x19c1eU;
            const auto source_address = 0x1b602U;
            r.address[4] = source_address;
            break;
        }
        case 0x19c22U: { // 41fa19e4 lea.l $1b608(pc), a0
            next = 0x19c26U;
            const auto source_address = 0x1b608U;
            r.address[0] = source_address;
            break;
        }
        case 0x19c26U: { // 43fa19da lea.l $1b602(pc), a1
            next = 0x19c2aU;
            const auto source_address = 0x1b602U;
            r.address[1] = source_address;
            break;
        }
        case 0x19c36U: { // 4a2d003f tst.b $3f(a5)
            next = 0x19c3aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x19c3aU: { // 6700008a beq.w $19cc6
            next = 0x19c3eU;
            if ((r.status & 4U) != 0U) { next = 0x19cc6U; transfer_kind = 1U; }
            break;
        }
        case 0x19c42U: { // 60000082 bra.w $19cc6
            next = 0x19c46U;
            if (true) { next = 0x19cc6U; transfer_kind = 1U; }
            break;
        }
        case 0x19ccaU: { // 41fa1936 lea.l $1b602(pc), a0
            next = 0x19cceU;
            const auto source_address = 0x1b602U;
            r.address[0] = source_address;
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_019c0e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19c0eU: { // 610000c4 bsr.w $19cd4
            next = 0x19c12U;
            const auto result = m.call(c, 922U, 0x19c0eU, 0x19cd4U, 0x19c12U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19c12U: { // 61000352 bsr.w $19f66
            next = 0x19c16U;
            const auto result = m.call(c, 923U, 0x19c12U, 0x19f66U, 0x19c16U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19c16U: { // 6100036a bsr.w $19f82
            next = 0x19c1aU;
            const auto result = m.call(c, 924U, 0x19c16U, 0x19f82U, 0x19c1aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19c1eU: { // 6100f588 bsr.w $191a8
            next = 0x19c22U;
            const auto result = m.call(c, 901U, 0x19c1eU, 0x191a8U, 0x19c22U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19c2aU: { // 6100f6b0 bsr.w $192dc
            next = 0x19c2eU;
            const auto result = m.call(c, 903U, 0x19c2aU, 0x192dcU, 0x19c2eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19c2eU: { // 61000370 bsr.w $19fa0
            next = 0x19c32U;
            const auto result = m.call(c, 925U, 0x19c2eU, 0x19fa0U, 0x19c32U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19c32U: { // 61000398 bsr.w $19fcc
            next = 0x19c36U;
            const auto result = m.call(c, 926U, 0x19c32U, 0x19fccU, 0x19c36U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19c3eU: { // 610004a0 bsr.w $1a0e0
            next = 0x19c42U;
            const auto result = m.call(c, 927U, 0x19c3eU, 0x1a0e0U, 0x19c42U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19cc6U: { // 610004a2 bsr.w $1a16a
            next = 0x19ccaU;
            const auto result = m.call(c, 928U, 0x19cc6U, 0x1a16aU, 0x19ccaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19cceU: { // 6100f6ca bsr.w $1939a
            next = 0x19cd2U;
            const auto result = m.call(c, 904U, 0x19cceU, 0x1939aU, 0x19cd2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19cd2U: { // 4e75 rts 
            next = 0x19cd4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19cd2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_019c0e_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19c0eU:
        case 0x19c12U:
        case 0x19c16U:
        case 0x19c1aU:
        case 0x19c1eU:
        case 0x19c22U:
        case 0x19c26U:
        case 0x19c2aU:
        case 0x19c2eU:
        case 0x19c32U:
        case 0x19c36U:
        case 0x19c3aU:
        case 0x19c3eU:
        case 0x19c42U:
        case 0x19cc6U:
        case 0x19ccaU:
        case 0x19cceU:
        case 0x19cd2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01a1f6_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1a1f6U:
    case 0x1a1faU:
    case 0x1a1fcU:
    case 0x1a1feU:
    case 0x1a200U:
    case 0x1a202U:
    case 0x1a206U:
    case 0x1a208U:
    case 0x1a20cU:
    case 0x1a210U:
    case 0x1a214U:
    case 0x1a218U:
    case 0x1a21aU:
    case 0x1a21cU:
    case 0x1a21eU:
    case 0x1a222U:
    case 0x1a226U:
    case 0x1a228U:
    case 0x1a22cU:
    case 0x1a230U:
    case 0x1a232U:
    case 0x1a234U:
    case 0x1a238U:
    case 0x1a23cU:
    case 0x1a23eU:
    case 0x1a242U:
    case 0x1a246U:
    case 0x1a24cU:
    case 0x1a252U:
    case 0x1a254U:
    case 0x1a256U:
    case 0x1a258U:
    case 0x1a25cU:
    case 0x1a260U:
    case 0x1a262U:
    case 0x1a264U:
    case 0x1a268U:
    case 0x1a26cU:
    case 0x1a270U:
    case 0x1a272U:
    case 0x1a276U:
    case 0x1a27aU:
    case 0x1a280U:
    case 0x1a286U:
    case 0x1a288U:
    case 0x1a28cU:
    case 0x1a28eU:
    case 0x1a294U:
    case 0x1a296U:
    case 0x1a29aU:
    case 0x1a29cU:
    case 0x1a2a2U:
    case 0x1a2a6U:
    case 0x1a2acU:
    case 0x1a2aeU:
    case 0x1a2b2U:
    case 0x1a2b6U:
    case 0x1a2baU:
    case 0x1a2beU:
    case 0x1a2c4U:
    case 0x1a2c8U:
        return true;
    default: return false;
    }
}
