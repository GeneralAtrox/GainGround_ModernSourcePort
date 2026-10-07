
FunctionResult cpu_b_stage_021c48(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21c48U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21c4cU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21c4cU: { // 6716 beq.b $21c64
            next = 0x21c4eU;
            if ((r.status & 4U) != 0U) { next = 0x21c64U; transfer_kind = 1U; }
            break;
        }
        case 0x21c4eU: { // 4a6d0012 tst.w $12(a5)
            next = 0x21c52U;
            const auto destination_address = r.address[5] + 0x12U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x21c52U: { // 6a08 bpl.b $21c5c
            next = 0x21c54U;
            if ((r.status & 8U) == 0U) { next = 0x21c5cU; transfer_kind = 1U; }
            break;
        }
        case 0x21c54U: { // 1b7c0000003f move.b #$0, $3f(a5)
            next = 0x21c5aU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x3fU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21c5aU: { // 6008 bra.b $21c64
            next = 0x21c5cU;
            if (true) { next = 0x21c64U; transfer_kind = 1U; }
            break;
        }
        case 0x21c5cU: { // 4df83400 lea.l $3400.w, a6
            next = 0x21c60U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21c60U: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x21c64U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21c64U: { // 1b7c0009000b move.b #$9, $b(a5)
            next = 0x21c6aU;
            const auto value = 0x9U;
            const auto destination_address = r.address[5] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21c6aU: { // 6100a2b0 bsr.w $1bf1c
            next = 0x21c6eU;
            const auto result = m.call(c, 328U, 0x21c6aU, 0x1bf1cU, 0x21c6eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c6eU: { // 6100d58c bsr.w $1f1fc
            next = 0x21c72U;
            const auto result = m.call(c, 371U, 0x21c6eU, 0x1f1fcU, 0x21c72U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c72U: { // 6100bf50 bsr.w $1dbc4
            next = 0x21c76U;
            const auto result = m.call(c, 352U, 0x21c72U, 0x1dbc4U, 0x21c76U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c76U: { // 6100bf78 bsr.w $1dbf0
            next = 0x21c7aU;
            const auto result = m.call(c, 353U, 0x21c76U, 0x1dbf0U, 0x21c7aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c7aU: { // 6100c1b4 bsr.w $1de30
            next = 0x21c7eU;
            const auto result = m.call(c, 355U, 0x21c7aU, 0x1de30U, 0x21c7eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c7eU: { // 6100c46e bsr.w $1e0ee
            next = 0x21c82U;
            const auto result = m.call(c, 356U, 0x21c7eU, 0x1e0eeU, 0x21c82U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c82U: { // 6100c4a0 bsr.w $1e124
            next = 0x21c86U;
            const auto result = m.call(c, 357U, 0x21c82U, 0x1e124U, 0x21c86U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c86U: { // 6100c4f8 bsr.w $1e180
            next = 0x21c8aU;
            const auto result = m.call(c, 359U, 0x21c86U, 0x1e180U, 0x21c8aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21c8aU: { // 4e75 rts 
            next = 0x21c8cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21c8aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21c48U:
        case 0x21c4cU:
        case 0x21c4eU:
        case 0x21c52U:
        case 0x21c54U:
        case 0x21c5aU:
        case 0x21c5cU:
        case 0x21c60U:
        case 0x21c64U:
        case 0x21c6aU:
        case 0x21c6eU:
        case 0x21c72U:
        case 0x21c76U:
        case 0x21c7aU:
        case 0x21c7eU:
        case 0x21c82U:
        case 0x21c86U:
        case 0x21c8aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021c8c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21c8cU: { // 4a2d003f tst.b $3f(a5)
            next = 0x21c90U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21c90U: { // 6718 beq.b $21caa
            next = 0x21c92U;
            if ((r.status & 4U) != 0U) { next = 0x21caaU; transfer_kind = 1U; }
            break;
        }
        case 0x21c92U: { // 0c6d0180001a cmpi.w #$180, $1a(a5)
            next = 0x21c98U;
            const auto source_value = 0x180U;
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x21c98U: { // 6d08 blt.b $21ca2
            next = 0x21c9aU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x21ca2U; transfer_kind = 1U; }
            break;
        }
        case 0x21c9aU: { // 1b7c0000003f move.b #$0, $3f(a5)
            next = 0x21ca0U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x3fU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21ca0U: { // 6008 bra.b $21caa
            next = 0x21ca2U;
            if (true) { next = 0x21caaU; transfer_kind = 1U; }
            break;
        }
        case 0x21ca2U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21ca6U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21ca6U: { // 522e0026 addq.b #$1, $26(a6)
            next = 0x21caaU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x26U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21caaU: { // 1b7c0009000b move.b #$9, $b(a5)
            next = 0x21cb0U;
            const auto value = 0x9U;
            const auto destination_address = r.address[5] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x21cb0U: { // 6100a26a bsr.w $1bf1c
            next = 0x21cb4U;
            const auto result = m.call(c, 328U, 0x21cb0U, 0x1bf1cU, 0x21cb4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cb4U: { // 6100d546 bsr.w $1f1fc
            next = 0x21cb8U;
            const auto result = m.call(c, 371U, 0x21cb4U, 0x1f1fcU, 0x21cb8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cb8U: { // 6100bf0a bsr.w $1dbc4
            next = 0x21cbcU;
            const auto result = m.call(c, 352U, 0x21cb8U, 0x1dbc4U, 0x21cbcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cbcU: { // 6100bf32 bsr.w $1dbf0
            next = 0x21cc0U;
            const auto result = m.call(c, 353U, 0x21cbcU, 0x1dbf0U, 0x21cc0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cc0U: { // 6100c16e bsr.w $1de30
            next = 0x21cc4U;
            const auto result = m.call(c, 355U, 0x21cc0U, 0x1de30U, 0x21cc4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cc4U: { // 6100c428 bsr.w $1e0ee
            next = 0x21cc8U;
            const auto result = m.call(c, 356U, 0x21cc4U, 0x1e0eeU, 0x21cc8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cc8U: { // 6100c45a bsr.w $1e124
            next = 0x21cccU;
            const auto result = m.call(c, 357U, 0x21cc8U, 0x1e124U, 0x21cccU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cccU: { // 6100c4b2 bsr.w $1e180
            next = 0x21cd0U;
            const auto result = m.call(c, 359U, 0x21cccU, 0x1e180U, 0x21cd0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21cd0U: { // 4e75 rts 
            next = 0x21cd2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21cd0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21c8cU:
        case 0x21c90U:
        case 0x21c92U:
        case 0x21c98U:
        case 0x21c9aU:
        case 0x21ca0U:
        case 0x21ca2U:
        case 0x21ca6U:
        case 0x21caaU:
        case 0x21cb0U:
        case 0x21cb4U:
        case 0x21cb8U:
        case 0x21cbcU:
        case 0x21cc0U:
        case 0x21cc4U:
        case 0x21cc8U:
        case 0x21cccU:
        case 0x21cd0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
