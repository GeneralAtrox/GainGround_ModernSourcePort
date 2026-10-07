
FunctionResult cpu_b_stage_01faa2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1faa2U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1faa6U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1faa6U: { // 6702 beq.b $1faaa
            next = 0x1faa8U;
            if ((r.status & 4U) != 0U) { next = 0x1faaaU; transfer_kind = 1U; }
            break;
        }
        case 0x1faa8U: { // 4e75 rts 
            next = 0x1faaaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1faa8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1faaaU: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1fab0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1fab0U: { // 6600f578 bne.w $1f02a
            next = 0x1fab4U;
            if ((r.status & 4U) == 0U) { next = 0x1f02aU; transfer_kind = 1U; }
            break;
        }
        case 0x1fab4U: { // 082d00000078 btst.b #$0, $78(a5)
            next = 0x1fabaU;
            const auto destination_address = r.address[5] + 0x78U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1fabaU: { // 6602 bne.b $1fabe
            next = 0x1fabcU;
            if ((r.status & 4U) == 0U) { next = 0x1fabeU; transfer_kind = 1U; }
            break;
        }
        case 0x1fabcU: { // 4e75 rts 
            next = 0x1fabeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1fabcU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1fabeU: { // 206d0066 movea.l $66(a5), a0
            next = 0x1fac2U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1fac2U: { // 1b6800120072 move.b $12(a0), $72(a5)
            next = 0x1fac8U;
            const auto source_address = r.address[0] + 0x12U;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x72U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1fac8U: { // 08ed00010041 bset.b #$1, $41(a5)
            next = 0x1faceU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1faceU: { // 08ad00000041 bclr.b #$0, $41(a5)
            next = 0x1fad4U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1fad4U: { // 08ed00020041 bset.b #$2, $41(a5)
            next = 0x1fadaU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1fadaU: { // 3b7c00060074 move.w #$6, $74(a5)
            next = 0x1fae0U;
            const auto value = 0x6U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fae0U: { // 6100df76 bsr.w $1da58
            next = 0x1fae4U;
            const auto result = m.call(c, 347U, 0x1fae0U, 0x1da58U, 0x1fae4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1fae4U: { // 4e75 rts 
            next = 0x1fae6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1fae4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1faa2U:
        case 0x1faa6U:
        case 0x1faa8U:
        case 0x1faaaU:
        case 0x1fab0U:
        case 0x1fab4U:
        case 0x1fabaU:
        case 0x1fabcU:
        case 0x1fabeU:
        case 0x1fac2U:
        case 0x1fac8U:
        case 0x1faceU:
        case 0x1fad4U:
        case 0x1fadaU:
        case 0x1fae0U:
        case 0x1fae4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_01fa86(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1fa86U: { // 3b6d005c007a move.w $5c(a5), $7a(a5)
            next = 0x1fa8cU;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x7aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa8cU: { // 3b6d005a005c move.w $5a(a5), $5c(a5)
            next = 0x1fa92U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa92U: { // 610e bsr.b $1faa2
            next = 0x1fa94U;
            const auto result = m.call(c, 693U, 0x1fa92U, 0x1faa2U, 0x1fa94U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1fa94U: { // 3b6d005c005a move.w $5c(a5), $5a(a5)
            next = 0x1fa9aU;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1fa9aU: { // 3b6d007a005c move.w $7a(a5), $5c(a5)
            next = 0x1faa0U;
            const auto source_address = r.address[5] + 0x7aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1faa0U: { // 4e75 rts 
            next = 0x1faa2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1faa0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1fa86U:
        case 0x1fa8cU:
        case 0x1fa92U:
        case 0x1fa94U:
        case 0x1fa9aU:
        case 0x1faa0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_020cc6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20cc6U: { // 6100b044 bsr.w $1bd0c
            next = 0x20ccaU;
            const auto result = m.call(c, 326U, 0x20cc6U, 0x1bd0cU, 0x20ccaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ccaU: { // 6100cef8 bsr.w $1dbc4
            next = 0x20cceU;
            const auto result = m.call(c, 352U, 0x20ccaU, 0x1dbc4U, 0x20cceU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20cceU: { // 6100cf20 bsr.w $1dbf0
            next = 0x20cd2U;
            const auto result = m.call(c, 353U, 0x20cceU, 0x1dbf0U, 0x20cd2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20cd2U: { // 6100d15c bsr.w $1de30
            next = 0x20cd6U;
            const auto result = m.call(c, 355U, 0x20cd2U, 0x1de30U, 0x20cd6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20cd6U: { // 6100d416 bsr.w $1e0ee
            next = 0x20cdaU;
            const auto result = m.call(c, 356U, 0x20cd6U, 0x1e0eeU, 0x20cdaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20cdaU: { // 6100d448 bsr.w $1e124
            next = 0x20cdeU;
            const auto result = m.call(c, 357U, 0x20cdaU, 0x1e124U, 0x20cdeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20cdeU: { // 6100d4a0 bsr.w $1e180
            next = 0x20ce2U;
            const auto result = m.call(c, 359U, 0x20cdeU, 0x1e180U, 0x20ce2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ce2U: { // 4e75 rts 
            next = 0x20ce4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20ce2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20cc6U:
        case 0x20ccaU:
        case 0x20cceU:
        case 0x20cd2U:
        case 0x20cd6U:
        case 0x20cdaU:
        case 0x20cdeU:
        case 0x20ce2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
