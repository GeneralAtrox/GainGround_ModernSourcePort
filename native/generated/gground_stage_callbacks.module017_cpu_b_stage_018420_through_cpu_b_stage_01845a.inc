
FunctionResult cpu_b_stage_018420(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18420U: { // 4dd5 lea.l (a5), a6
            next = 0x18422U;
            const auto source_address = r.address[5];
            r.address[6] = source_address;
            break;
        }
        case 0x18422U: { // 49fa2e2c lea.l $1b250(pc), a4
            next = 0x18426U;
            const auto source_address = 0x1b250U;
            r.address[4] = source_address;
            break;
        }
        case 0x18426U: { // 7e01 moveq #$1, d7
            next = 0x18428U;
            r.data[7] = 0x1U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x18428U: { // 225c movea.l (a4)+, a1
            next = 0x1842aU;
            const auto source_address = r.address[4];
            const auto value = m.lng(source_address);
            r.address[4] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x1842aU: { // 61000068 bsr.w $18494
            next = 0x1842eU;
            const auto result = m.call(c, 649U, 0x1842aU, 0x18494U, 0x1842eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1842eU: { // 61000096 bsr.w $184c6
            next = 0x18432U;
            const auto result = m.call(c, 650U, 0x1842eU, 0x184c6U, 0x18432U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18432U: { // 45d6 lea.l (a6), a2
            next = 0x18434U;
            const auto source_address = r.address[6];
            r.address[2] = source_address;
            break;
        }
        case 0x18434U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x18438U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x18438U: { // 7a00 moveq #$0, d5
            next = 0x1843aU;
            r.data[5] = 0x0U;
            m.logic(r.data[5], 32U);
            break;
        }
        case 0x1843aU: { // 7c02 moveq #$2, d6
            next = 0x1843cU;
            r.data[6] = 0x2U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x1843cU: { // 225c movea.l (a4)+, a1
            next = 0x1843eU;
            const auto source_address = r.address[4];
            const auto value = m.lng(source_address);
            r.address[4] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x1843eU: { // 61000054 bsr.w $18494
            next = 0x18442U;
            const auto result = m.call(c, 649U, 0x1843eU, 0x18494U, 0x18442U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18442U: { // 3d4a0036 move.w a2, $36(a6)
            next = 0x18446U;
            const auto value = r.address[2];
            const auto destination_address = r.address[6] + 0x36U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18446U: { // 3d45003a move.w d5, $3a(a6)
            next = 0x1844aU;
            const auto value = r.data[5];
            const auto destination_address = r.address[6] + 0x3aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1844aU: { // 4dee0080 lea.l $80(a6), a6
            next = 0x1844eU;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x1844eU: { // 5c45 addq.w #$6, d5
            next = 0x18450U;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[5];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x18450U: { // 51ceffea dbra d6, $1843c
            next = 0x18454U;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x1843cU;
            break;
        }
        case 0x18454U: { // 51cfffd2 dbra d7, $18428
            next = 0x18458U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x18428U;
            break;
        }
        case 0x18458U: { // 4e75 rts 
            next = 0x1845aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18458U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18420U:
        case 0x18422U:
        case 0x18426U:
        case 0x18428U:
        case 0x1842aU:
        case 0x1842eU:
        case 0x18432U:
        case 0x18434U:
        case 0x18438U:
        case 0x1843aU:
        case 0x1843cU:
        case 0x1843eU:
        case 0x18442U:
        case 0x18446U:
        case 0x1844aU:
        case 0x1844eU:
        case 0x18450U:
        case 0x18454U:
        case 0x18458U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_01845a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1845aU: { // 4dd5 lea.l (a5), a6
            next = 0x1845cU;
            const auto source_address = r.address[5];
            r.address[6] = source_address;
            break;
        }
        case 0x1845cU: { // 49fa309c lea.l $1b4fa(pc), a4
            next = 0x18460U;
            const auto source_address = 0x1b4faU;
            r.address[4] = source_address;
            break;
        }
        case 0x18460U: { // 225c movea.l (a4)+, a1
            next = 0x18462U;
            const auto source_address = r.address[4];
            const auto value = m.lng(source_address);
            r.address[4] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x18462U: { // 61000030 bsr.w $18494
            next = 0x18466U;
            const auto result = m.call(c, 649U, 0x18462U, 0x18494U, 0x18466U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18466U: { // 6100005e bsr.w $184c6
            next = 0x1846aU;
            const auto result = m.call(c, 650U, 0x18466U, 0x184c6U, 0x1846aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1846aU: { // 45d6 lea.l (a6), a2
            next = 0x1846cU;
            const auto source_address = r.address[6];
            r.address[2] = source_address;
            break;
        }
        case 0x1846cU: { // 4dee0080 lea.l $80(a6), a6
            next = 0x18470U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x18470U: { // 7c00 moveq #$0, d6
            next = 0x18472U;
            r.data[6] = 0x0U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x18472U: { // 7e04 moveq #$4, d7
            next = 0x18474U;
            r.data[7] = 0x4U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x18474U: { // 225c movea.l (a4)+, a1
            next = 0x18476U;
            const auto source_address = r.address[4];
            const auto value = m.lng(source_address);
            r.address[4] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x18476U: { // 6100001c bsr.w $18494
            next = 0x1847aU;
            const auto result = m.call(c, 649U, 0x18476U, 0x18494U, 0x1847aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1847aU: { // 3d4a0036 move.w a2, $36(a6)
            next = 0x1847eU;
            const auto value = r.address[2];
            const auto destination_address = r.address[6] + 0x36U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1847eU: { // 3d46003a move.w d6, $3a(a6)
            next = 0x18482U;
            const auto value = r.data[6];
            const auto destination_address = r.address[6] + 0x3aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18482U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x18486U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x18486U: { // 5c46 addq.w #$6, d6
            next = 0x18488U;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[6];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(6U, value);
            break;
        }
        case 0x18488U: { // 51cfffea dbra d7, $18474
            next = 0x1848cU;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x18474U;
            break;
        }
        case 0x1848cU: { // 4eb900013e38 jsr $13e38.l
            next = 0x18492U;
            const auto result = m.call(c, 643U, 0x1848cU, 0x13e38U, 0x18492U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18492U: { // 4e75 rts 
            next = 0x18494U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18492U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1845aU:
        case 0x1845cU:
        case 0x18460U:
        case 0x18462U:
        case 0x18466U:
        case 0x1846aU:
        case 0x1846cU:
        case 0x18470U:
        case 0x18472U:
        case 0x18474U:
        case 0x18476U:
        case 0x1847aU:
        case 0x1847eU:
        case 0x18482U:
        case 0x18486U:
        case 0x18488U:
        case 0x1848cU:
        case 0x18492U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
