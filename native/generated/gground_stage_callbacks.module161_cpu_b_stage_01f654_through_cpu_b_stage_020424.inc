
FunctionResult cpu_b_stage_01f654(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1f57cU: { // 4e75 rts 
            next = 0x1f57eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f57cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f596U: { // 4e75 rts 
            next = 0x1f598U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f596U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f5acU: { // 4e75 rts 
            next = 0x1f5aeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f5acU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f5beU: { // 61000f00 bsr.w $204c0
            next = 0x1f5c2U;
            const auto result = m.call(c, 384U, 0x1f5beU, 0x204c0U, 0x1f5c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f5d4U: { // 4e75 rts 
            next = 0x1f5d6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f5d4U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f5f6U: { // 4e75 rts 
            next = 0x1f5f8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f5f6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f5feU: { // 6100f600 bsr.w $1ec00
            next = 0x1f602U;
            const auto result = m.call(c, 364U, 0x1f5feU, 0x1ec00U, 0x1f602U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f638U: { // 61000d88 bsr.w $203c2
            next = 0x1f63cU;
            const auto result = m.call(c, 382U, 0x1f638U, 0x203c2U, 0x1f63cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f640U: { // 6100fac4 bsr.w $1f106
            next = 0x1f644U;
            const auto result = m.call(c, 369U, 0x1f640U, 0x1f106U, 0x1f644U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f644U: { // 6100fb00 bsr.w $1f146
            next = 0x1f648U;
            const auto result = m.call(c, 370U, 0x1f644U, 0x1f146U, 0x1f648U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f652U: { // 4e75 rts 
            next = 0x1f654U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f652U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f65aU: { // 4e75 rts 
            next = 0x1f65cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f65aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f670U: { // 4e75 rts 
            next = 0x1f672U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f670U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f672U: { // 61000472 bsr.w $1fae6
            next = 0x1f676U;
            const auto result = m.call(c, 374U, 0x1f672U, 0x1fae6U, 0x1f676U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f678U: { // 4e75 rts 
            next = 0x1f67aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f678U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1f69cU: { // 6100e3ba bsr.w $1da58
            next = 0x1f6a0U;
            const auto result = m.call(c, 347U, 0x1f69cU, 0x1da58U, 0x1f6a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1f6a0U: { // 4e75 rts 
            next = 0x1f6a2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1f6a0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01f654_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01f654_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_01f654_dispatch_cases_002(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01f654_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

FunctionResult cpu_b_stage_01fa38(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1fa38U: { // 7200 moveq #$0, d1
            next = 0x1fa3aU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1fa3aU: { // 303c001c move.w #$1c, d0
            next = 0x1fa3eU;
            const auto value = 0x1cU;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa3eU: { // 43ee000c lea.l $c(a6), a1
            next = 0x1fa42U;
            const auto source_address = r.address[6] + 0xcU;
            r.address[1] = source_address;
            break;
        }
        case 0x1fa42U: { // 22c1 move.l d1, (a1)+
            next = 0x1fa44U;
            const auto value = r.data[1];
            const auto destination_address = r.address[1];
            m.lng(destination_address, value);
            r.address[1] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x1fa44U: { // 51c8fffc dbra d0, $1fa42
            next = 0x1fa48U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1fa42U;
            break;
        }
        case 0x1fa48U: { // 4e75 rts 
            next = 0x1fa4aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1fa48U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1fa38U:
        case 0x1fa3aU:
        case 0x1fa3eU:
        case 0x1fa42U:
        case 0x1fa44U:
        case 0x1fa48U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_020424(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20424U: { // 30280020 move.w $20(a0), d0
            next = 0x20428U;
            const auto source_address = r.address[0] + 0x20U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x20428U: { // 06400101 addi.w #$101, d0
            next = 0x2042cU;
            const auto source_value = 0x101U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x2042cU: { // 0c400404 cmpi.w #$404, d0
            next = 0x20430U;
            const auto source_value = 0x404U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x20430U: { // 6d04 blt.b $20436
            next = 0x20432U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x20436U; transfer_kind = 1U; }
            break;
        }
        case 0x20432U: { // 303c0101 move.w #$101, d0
            next = 0x20436U;
            const auto value = 0x101U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x20436U: { // 31400020 move.w d0, $20(a0)
            next = 0x2043aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[0] + 0x20U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x2043aU: { // 4e75 rts 
            next = 0x2043cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2043aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20424U:
        case 0x20428U:
        case 0x2042cU:
        case 0x20430U:
        case 0x20432U:
        case 0x20436U:
        case 0x2043aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
