
FunctionResult cpu_b_stage_021126(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21126U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x2112cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x2112cU: { // 661a bne.b $21148
            next = 0x2112eU;
            if ((r.status & 4U) == 0U) { next = 0x21148U; transfer_kind = 1U; }
            break;
        }
        case 0x2112eU: { // 6100bfa6 bsr.w $1d0d6
            next = 0x21132U;
            const auto result = m.call(c, 667U, 0x2112eU, 0x1d0d6U, 0x21132U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21132U: { // 6100ca90 bsr.w $1dbc4
            next = 0x21136U;
            const auto result = m.call(c, 352U, 0x21132U, 0x1dbc4U, 0x21136U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21136U: { // 6100cab8 bsr.w $1dbf0
            next = 0x2113aU;
            const auto result = m.call(c, 353U, 0x21136U, 0x1dbf0U, 0x2113aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2113aU: { // 6100cfe8 bsr.w $1e124
            next = 0x2113eU;
            const auto result = m.call(c, 357U, 0x2113aU, 0x1e124U, 0x2113eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2113eU: { // 6100d040 bsr.w $1e180
            next = 0x21142U;
            const auto result = m.call(c, 359U, 0x2113eU, 0x1e180U, 0x21142U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21142U: { // 6100dba0 bsr.w $1ece4
            next = 0x21146U;
            const auto result = m.call(c, 365U, 0x21142U, 0x1ece4U, 0x21146U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21146U: { // 4e75 rts 
            next = 0x21148U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21146U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21148U: { // 6100abc2 bsr.w $1bd0c
            next = 0x2114cU;
            const auto result = m.call(c, 326U, 0x21148U, 0x1bd0cU, 0x2114cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2114cU: { // 6100ca76 bsr.w $1dbc4
            next = 0x21150U;
            const auto result = m.call(c, 352U, 0x2114cU, 0x1dbc4U, 0x21150U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21150U: { // 6100ca9e bsr.w $1dbf0
            next = 0x21154U;
            const auto result = m.call(c, 353U, 0x21150U, 0x1dbf0U, 0x21154U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21154U: { // 6100ccda bsr.w $1de30
            next = 0x21158U;
            const auto result = m.call(c, 355U, 0x21154U, 0x1de30U, 0x21158U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21158U: { // 6100cfca bsr.w $1e124
            next = 0x2115cU;
            const auto result = m.call(c, 357U, 0x21158U, 0x1e124U, 0x2115cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2115cU: { // 6100d022 bsr.w $1e180
            next = 0x21160U;
            const auto result = m.call(c, 359U, 0x2115cU, 0x1e180U, 0x21160U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21160U: { // 4e75 rts 
            next = 0x21162U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21160U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21126U:
        case 0x2112cU:
        case 0x2112eU:
        case 0x21132U:
        case 0x21136U:
        case 0x2113aU:
        case 0x2113eU:
        case 0x21142U:
        case 0x21146U:
        case 0x21148U:
        case 0x2114cU:
        case 0x21150U:
        case 0x21154U:
        case 0x21158U:
        case 0x2115cU:
        case 0x21160U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021278(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21278U: { // 6100aca2 bsr.w $1bf1c
            next = 0x2127cU;
            const auto result = m.call(c, 328U, 0x21278U, 0x1bf1cU, 0x2127cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2127cU: { // 6100c946 bsr.w $1dbc4
            next = 0x21280U;
            const auto result = m.call(c, 352U, 0x2127cU, 0x1dbc4U, 0x21280U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21280U: { // 6100c96e bsr.w $1dbf0
            next = 0x21284U;
            const auto result = m.call(c, 353U, 0x21280U, 0x1dbf0U, 0x21284U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21284U: { // 6100cbaa bsr.w $1de30
            next = 0x21288U;
            const auto result = m.call(c, 355U, 0x21284U, 0x1de30U, 0x21288U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21288U: { // 6100ce64 bsr.w $1e0ee
            next = 0x2128cU;
            const auto result = m.call(c, 356U, 0x21288U, 0x1e0eeU, 0x2128cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2128cU: { // 6100ce96 bsr.w $1e124
            next = 0x21290U;
            const auto result = m.call(c, 357U, 0x2128cU, 0x1e124U, 0x21290U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21290U: { // 6100ceee bsr.w $1e180
            next = 0x21294U;
            const auto result = m.call(c, 359U, 0x21290U, 0x1e180U, 0x21294U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21294U: { // 4e75 rts 
            next = 0x21296U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21294U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21278U:
        case 0x2127cU:
        case 0x21280U:
        case 0x21284U:
        case 0x21288U:
        case 0x2128cU:
        case 0x21290U:
        case 0x21294U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
