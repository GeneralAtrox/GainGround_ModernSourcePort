
FunctionResult cpu_b_stage_01f800(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1f806U: { // 4e75 rts 
            next = 0x1f808U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f806U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f81cU: { // 4e75 rts 
            next = 0x1f81eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f81cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f81eU: { // 610002c6 bsr.w $1fae6
            next = 0x1f822U;
            const auto result = m.call(c, 374U, 0x1f81eU, 0x1fae6U, 0x1f822U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f824U: { // 4e75 rts 
            next = 0x1f826U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f824U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f848U: { // 6100e20e bsr.w $1da58
            next = 0x1f84cU;
            const auto result = m.call(c, 347U, 0x1f848U, 0x1da58U, 0x1f84cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f84cU: { // 4e75 rts 
            next = 0x1f84eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f84cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f860U: { // 4e75 rts 
            next = 0x1f862U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f860U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f890U: { // 4e75 rts 
            next = 0x1f892U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f890U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f8a2U: { // 61000c1c bsr.w $204c0
            next = 0x1f8a6U;
            const auto result = m.call(c, 384U, 0x1f8a2U, 0x204c0U, 0x1f8a6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f8b8U: { // 4e75 rts 
            next = 0x1f8baU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f8b8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f8daU: { // 4e75 rts 
            next = 0x1f8dcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f8daU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f8dcU: { // 6100e17a bsr.w $1da58
            next = 0x1f8e0U;
            const auto result = m.call(c, 347U, 0x1f8dcU, 0x1da58U, 0x1f8e0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f8e6U: { // 61000ada bsr.w $203c2
            next = 0x1f8eaU;
            const auto result = m.call(c, 382U, 0x1f8e6U, 0x203c2U, 0x1f8eaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f8eeU: { // 6100f816 bsr.w $1f106
            next = 0x1f8f2U;
            const auto result = m.call(c, 369U, 0x1f8eeU, 0x1f106U, 0x1f8f2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f8f2U: { // 61000290 bsr.w $1fb84
            next = 0x1f8f6U;
            const auto result = m.call(c, 376U, 0x1f8f2U, 0x1fb84U, 0x1f8f6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f936U: { // 61000ab0 bsr.w $203e8
            next = 0x1f93aU;
            const auto result = m.call(c, 694U, 0x1f936U, 0x203e8U, 0x1f93aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f93eU: { // 6100002c bsr.w $1f96c
            next = 0x1f942U;
            const auto result = m.call(c, 688U, 0x1f93eU, 0x1f96cU, 0x1f942U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f942U: { // 6100005a bsr.w $1f99e
            next = 0x1f946U;
            const auto result = m.call(c, 689U, 0x1f942U, 0x1f99eU, 0x1f946U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f958U: { // 4eb9000207c2 jsr $207c2.l
            next = 0x1f95eU;
            const auto result = m.call(c, 388U, 0x1f958U, 0x207c2U, 0x1f95eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f96aU: { // 4e75 rts 
            next = 0x1f96cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f96aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01f800_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01f800_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_01f800_dispatch_cases_002(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01f800_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

FunctionResult cpu_b_stage_01fa68(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1fa68U: { // 3b6d005c007a move.w $5c(a5), $7a(a5)
            next = 0x1fa6eU;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x7aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa6eU: { // 3b6d005a005c move.w $5a(a5), $5c(a5)
            next = 0x1fa74U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa74U: { // 6100f786 bsr.w $1f1fc
            next = 0x1fa78U;
            const auto result = m.call(c, 371U, 0x1fa74U, 0x1f1fcU, 0x1fa78U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1fa78U: { // 3b6d005c005a move.w $5c(a5), $5a(a5)
            next = 0x1fa7eU;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa7eU: { // 3b6d007a005c move.w $7a(a5), $5c(a5)
            next = 0x1fa84U;
            const auto source_address = r.address[5] + 0x7aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa84U: { // 4e75 rts 
            next = 0x1fa86U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1fa84U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1fa68U:
        case 0x1fa6eU:
        case 0x1fa74U:
        case 0x1fa78U:
        case 0x1fa7eU:
        case 0x1fa84U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
