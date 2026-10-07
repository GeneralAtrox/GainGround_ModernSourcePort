
FunctionResult cpu_b_stage_021b50(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21b50U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21b54U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21b54U: { // 6708 beq.b $21b5e
            next = 0x21b56U;
            if ((r.status & 4U) != 0U) { next = 0x21b5eU; transfer_kind = 1U; }
            break;
        }
        case 0x21b56U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21b5aU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21b5aU: { // 522e002e addq.b #$1, $2e(a6)
            next = 0x21b5eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x2eU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21b5eU: { // 6100aee6 bsr.w $1ca46
            next = 0x21b62U;
            const auto result = m.call(c, 663U, 0x21b5eU, 0x1ca46U, 0x21b62U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b62U: { // 6100df04 bsr.w $1fa68
            next = 0x21b66U;
            const auto result = m.call(c, 691U, 0x21b62U, 0x1fa68U, 0x21b66U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b66U: { // 6100c05c bsr.w $1dbc4
            next = 0x21b6aU;
            const auto result = m.call(c, 352U, 0x21b66U, 0x1dbc4U, 0x21b6aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b6aU: { // 6100c084 bsr.w $1dbf0
            next = 0x21b6eU;
            const auto result = m.call(c, 353U, 0x21b6aU, 0x1dbf0U, 0x21b6eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b6eU: { // 6100c5b4 bsr.w $1e124
            next = 0x21b72U;
            const auto result = m.call(c, 357U, 0x21b6eU, 0x1e124U, 0x21b72U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b72U: { // 6100cdd4 bsr.w $1e948
            next = 0x21b76U;
            const auto result = m.call(c, 677U, 0x21b72U, 0x1e948U, 0x21b76U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b76U: { // 4e75 rts 
            next = 0x21b78U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21b76U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21b50U:
        case 0x21b54U:
        case 0x21b56U:
        case 0x21b5aU:
        case 0x21b5eU:
        case 0x21b62U:
        case 0x21b66U:
        case 0x21b6aU:
        case 0x21b6eU:
        case 0x21b72U:
        case 0x21b76U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021b78(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21b78U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21b7cU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21b7cU: { // 6708 beq.b $21b86
            next = 0x21b7eU;
            if ((r.status & 4U) != 0U) { next = 0x21b86U; transfer_kind = 1U; }
            break;
        }
        case 0x21b7eU: { // 4df83400 lea.l $3400.w, a6
            next = 0x21b82U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21b82U: { // 522e0030 addq.b #$1, $30(a6)
            next = 0x21b86U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x30U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21b86U: { // 6100aebe bsr.w $1ca46
            next = 0x21b8aU;
            const auto result = m.call(c, 663U, 0x21b86U, 0x1ca46U, 0x21b8aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b8aU: { // 6100dedc bsr.w $1fa68
            next = 0x21b8eU;
            const auto result = m.call(c, 691U, 0x21b8aU, 0x1fa68U, 0x21b8eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b8eU: { // 6100c034 bsr.w $1dbc4
            next = 0x21b92U;
            const auto result = m.call(c, 352U, 0x21b8eU, 0x1dbc4U, 0x21b92U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b92U: { // 6100c05c bsr.w $1dbf0
            next = 0x21b96U;
            const auto result = m.call(c, 353U, 0x21b92U, 0x1dbf0U, 0x21b96U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b96U: { // 6100c58c bsr.w $1e124
            next = 0x21b9aU;
            const auto result = m.call(c, 357U, 0x21b96U, 0x1e124U, 0x21b9aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b9aU: { // 6100cdac bsr.w $1e948
            next = 0x21b9eU;
            const auto result = m.call(c, 677U, 0x21b9aU, 0x1e948U, 0x21b9eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21b9eU: { // 4e75 rts 
            next = 0x21ba0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21b9eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21b78U:
        case 0x21b7cU:
        case 0x21b7eU:
        case 0x21b82U:
        case 0x21b86U:
        case 0x21b8aU:
        case 0x21b8eU:
        case 0x21b92U:
        case 0x21b96U:
        case 0x21b9aU:
        case 0x21b9eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
