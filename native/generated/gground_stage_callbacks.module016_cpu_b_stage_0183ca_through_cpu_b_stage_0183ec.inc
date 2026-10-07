
FunctionResult cpu_b_stage_0183ca(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x183caU: { // 4dd5 lea.l (a5), a6
            next = 0x183ccU;
            const auto source_address = r.address[5];
            r.address[6] = source_address;
            break;
        }
        case 0x183ccU: { // 7e05 moveq #$5, d7
            next = 0x183ceU;
            r.data[7] = 0x5U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x183ceU: { // 49fa2500 lea.l $1a8d0(pc), a4
            next = 0x183d2U;
            const auto source_address = 0x1a8d0U;
            r.address[4] = source_address;
            break;
        }
        case 0x183d2U: { // 225c movea.l (a4)+, a1
            next = 0x183d4U;
            const auto source_address = r.address[4];
            const auto value = m.lng(source_address);
            r.address[4] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x183d4U: { // 610000be bsr.w $18494
            next = 0x183d8U;
            const auto result = m.call(c, 649U, 0x183d4U, 0x18494U, 0x183d8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x183d8U: { // 610000ec bsr.w $184c6
            next = 0x183dcU;
            const auto result = m.call(c, 650U, 0x183d8U, 0x184c6U, 0x183dcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x183dcU: { // 4dee0080 lea.l $80(a6), a6
            next = 0x183e0U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x183e0U: { // 51cffff0 dbra d7, $183d2
            next = 0x183e4U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x183d2U;
            break;
        }
        case 0x183e4U: { // 11fc001e0d19 move.b #$1e, $d19.w
            next = 0x183eaU;
            const auto value = 0x1eU;
            const auto destination_address = 0xd19U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x183eaU: { // 4e75 rts 
            next = 0x183ecU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x183eaU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x183caU:
        case 0x183ccU:
        case 0x183ceU:
        case 0x183d2U:
        case 0x183d4U:
        case 0x183d8U:
        case 0x183dcU:
        case 0x183e0U:
        case 0x183e4U:
        case 0x183eaU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0183ec(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x183ecU: { // 4dd5 lea.l (a5), a6
            next = 0x183eeU;
            const auto source_address = r.address[5];
            r.address[6] = source_address;
            break;
        }
        case 0x183eeU: { // 49fa2b62 lea.l $1af52(pc), a4
            next = 0x183f2U;
            const auto source_address = 0x1af52U;
            r.address[4] = source_address;
            break;
        }
        case 0x183f2U: { // 225c movea.l (a4)+, a1
            next = 0x183f4U;
            const auto source_address = r.address[4];
            const auto value = m.lng(source_address);
            r.address[4] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x183f4U: { // 6100009e bsr.w $18494
            next = 0x183f8U;
            const auto result = m.call(c, 649U, 0x183f4U, 0x18494U, 0x183f8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x183f8U: { // 610000cc bsr.w $184c6
            next = 0x183fcU;
            const auto result = m.call(c, 650U, 0x183f8U, 0x184c6U, 0x183fcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x183fcU: { // 45d6 lea.l (a6), a2
            next = 0x183feU;
            const auto source_address = r.address[6];
            r.address[2] = source_address;
            break;
        }
        case 0x183feU: { // 4dee0080 lea.l $80(a6), a6
            next = 0x18402U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x18402U: { // 7c00 moveq #$0, d6
            next = 0x18404U;
            r.data[6] = 0x0U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x18404U: { // 7e03 moveq #$3, d7
            next = 0x18406U;
            r.data[7] = 0x3U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x18406U: { // 225c movea.l (a4)+, a1
            next = 0x18408U;
            const auto source_address = r.address[4];
            const auto value = m.lng(source_address);
            r.address[4] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x18408U: { // 6100008a bsr.w $18494
            next = 0x1840cU;
            const auto result = m.call(c, 649U, 0x18408U, 0x18494U, 0x1840cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1840cU: { // 3d4a0036 move.w a2, $36(a6)
            next = 0x18410U;
            const auto value = r.address[2];
            const auto destination_address = r.address[6] + 0x36U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18410U: { // 3d46003a move.w d6, $3a(a6)
            next = 0x18414U;
            const auto value = r.data[6];
            const auto destination_address = r.address[6] + 0x3aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18414U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x18418U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x18418U: { // 5c46 addq.w #$6, d6
            next = 0x1841aU;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[6];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(6U, value);
            break;
        }
        case 0x1841aU: { // 51cfffea dbra d7, $18406
            next = 0x1841eU;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x18406U;
            break;
        }
        case 0x1841eU: { // 4e75 rts 
            next = 0x18420U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1841eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x183ecU:
        case 0x183eeU:
        case 0x183f2U:
        case 0x183f4U:
        case 0x183f8U:
        case 0x183fcU:
        case 0x183feU:
        case 0x18402U:
        case 0x18404U:
        case 0x18406U:
        case 0x18408U:
        case 0x1840cU:
        case 0x18410U:
        case 0x18414U:
        case 0x18418U:
        case 0x1841aU:
        case 0x1841eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
