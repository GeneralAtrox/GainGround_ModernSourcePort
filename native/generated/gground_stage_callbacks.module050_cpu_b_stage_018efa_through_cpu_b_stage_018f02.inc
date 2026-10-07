
FunctionResult cpu_b_stage_018efa(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18efaU: { // 6000000a bra.w $18f06
            next = 0x18efeU;
            if (true) { next = 0x18f06U; transfer_kind = 1U; }
            break;
        }
        case 0x18f06U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x18f0aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18f0aU: { // 6e16 bgt.b $18f22
            next = 0x18f0cU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18f22U; transfer_kind = 1U; }
            break;
        }
        case 0x18f0cU: { // 3b7c000a0068 move.w #$a, $68(a5)
            next = 0x18f12U;
            const auto value = 0xaU;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18f12U: { // 526d0066 addq.w #$1, $66(a5)
            next = 0x18f16U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18f16U: { // 026d00030066 andi.w #$3, $66(a5)
            next = 0x18f1cU;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18f1cU: { // 6604 bne.b $18f22
            next = 0x18f1eU;
            if ((r.status & 4U) == 0U) { next = 0x18f22U; transfer_kind = 1U; }
            break;
        }
        case 0x18f1eU: { // 426d0040 clr.w $40(a5)
            next = 0x18f22U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18f22U: { // 4e75 rts 
            next = 0x18f24U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18f22U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18efaU:
        case 0x18f06U:
        case 0x18f0aU:
        case 0x18f0cU:
        case 0x18f12U:
        case 0x18f16U:
        case 0x18f1cU:
        case 0x18f1eU:
        case 0x18f22U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_018efe(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18efeU: { // 60000024 bra.w $18f24
            next = 0x18f02U;
            if (true) { next = 0x18f24U; transfer_kind = 1U; }
            break;
        }
        case 0x18f24U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x18f28U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18f28U: { // 6e16 bgt.b $18f40
            next = 0x18f2aU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18f40U; transfer_kind = 1U; }
            break;
        }
        case 0x18f2aU: { // 3b7c00030068 move.w #$3, $68(a5)
            next = 0x18f30U;
            const auto value = 0x3U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18f30U: { // 526d0066 addq.w #$1, $66(a5)
            next = 0x18f34U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18f34U: { // 0c6d00060066 cmpi.w #$6, $66(a5)
            next = 0x18f3aU;
            const auto source_value = 0x6U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18f3aU: { // 6b04 bmi.b $18f40
            next = 0x18f3cU;
            if ((r.status & 8U) != 0U) { next = 0x18f40U; transfer_kind = 1U; }
            break;
        }
        case 0x18f3cU: { // 426d0066 clr.w $66(a5)
            next = 0x18f40U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18f40U: { // 4e75 rts 
            next = 0x18f42U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18f40U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18efeU:
        case 0x18f24U:
        case 0x18f28U:
        case 0x18f2aU:
        case 0x18f30U:
        case 0x18f34U:
        case 0x18f3aU:
        case 0x18f3cU:
        case 0x18f40U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_018f02(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18f02U: { // 6000003e bra.w $18f42
            next = 0x18f06U;
            if (true) { next = 0x18f42U; transfer_kind = 1U; }
            break;
        }
        case 0x18f42U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x18f46U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18f46U: { // 6e28 bgt.b $18f70
            next = 0x18f48U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18f70U; transfer_kind = 1U; }
            break;
        }
        case 0x18f48U: { // 3b7c00080068 move.w #$8, $68(a5)
            next = 0x18f4eU;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18f4eU: { // 526d0066 addq.w #$1, $66(a5)
            next = 0x18f52U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18f52U: { // 0c6d00040066 cmpi.w #$4, $66(a5)
            next = 0x18f58U;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18f58U: { // 660a bne.b $18f64
            next = 0x18f5aU;
            if ((r.status & 4U) == 0U) { next = 0x18f64U; transfer_kind = 1U; }
            break;
        }
        case 0x18f5aU: { // 1b7c00040051 move.b #$4, $51(a5)
            next = 0x18f60U;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x51U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18f60U: { // 422d0050 clr.b $50(a5)
            next = 0x18f64U;
            const auto destination_address = r.address[5] + 0x50U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x18f64U: { // 0c6d00070066 cmpi.w #$7, $66(a5)
            next = 0x18f6aU;
            const auto source_value = 0x7U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18f6aU: { // 6b04 bmi.b $18f70
            next = 0x18f6cU;
            if ((r.status & 8U) != 0U) { next = 0x18f70U; transfer_kind = 1U; }
            break;
        }
        case 0x18f6cU: { // 426d0040 clr.w $40(a5)
            next = 0x18f70U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18f70U: { // 4e75 rts 
            next = 0x18f72U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18f70U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18f02U:
        case 0x18f42U:
        case 0x18f46U:
        case 0x18f48U:
        case 0x18f4eU:
        case 0x18f52U:
        case 0x18f58U:
        case 0x18f5aU:
        case 0x18f60U:
        case 0x18f64U:
        case 0x18f6aU:
        case 0x18f6cU:
        case 0x18f70U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
