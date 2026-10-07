
FunctionResult cpu_b_stage_021042(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21042U: { // 6100ad9c bsr.w $1bde0
            next = 0x21046U;
            const auto result = m.call(c, 651U, 0x21042U, 0x1bde0U, 0x21046U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21046U: { // 6100cb7c bsr.w $1dbc4
            next = 0x2104aU;
            const auto result = m.call(c, 352U, 0x21046U, 0x1dbc4U, 0x2104aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2104aU: { // 6100cba4 bsr.w $1dbf0
            next = 0x2104eU;
            const auto result = m.call(c, 353U, 0x2104aU, 0x1dbf0U, 0x2104eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2104eU: { // 6100cde0 bsr.w $1de30
            next = 0x21052U;
            const auto result = m.call(c, 355U, 0x2104eU, 0x1de30U, 0x21052U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21052U: { // 6100d0d0 bsr.w $1e124
            next = 0x21056U;
            const auto result = m.call(c, 357U, 0x21052U, 0x1e124U, 0x21056U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21056U: { // 4a2d0058 tst.b $58(a5)
            next = 0x2105aU;
            const auto destination_address = r.address[5] + 0x58U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2105aU: { // 6602 bne.b $2105e
            next = 0x2105cU;
            if ((r.status & 4U) == 0U) { next = 0x2105eU; transfer_kind = 1U; }
            break;
        }
        case 0x2105cU: { // 4e75 rts 
            next = 0x2105eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2105cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x2105eU: { // 6100d412 bsr.w $1e472
            next = 0x21062U;
            const auto result = m.call(c, 669U, 0x2105eU, 0x1e472U, 0x21062U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21062U: { // 4e75 rts 
            next = 0x21064U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21062U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21042U:
        case 0x21046U:
        case 0x2104aU:
        case 0x2104eU:
        case 0x21052U:
        case 0x21056U:
        case 0x2105aU:
        case 0x2105cU:
        case 0x2105eU:
        case 0x21062U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021094(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21094U: { // 6100b2e8 bsr.w $1c37e
            next = 0x21098U;
            const auto result = m.call(c, 331U, 0x21094U, 0x1c37eU, 0x21098U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21098U: { // 6100d054 bsr.w $1e0ee
            next = 0x2109cU;
            const auto result = m.call(c, 356U, 0x21098U, 0x1e0eeU, 0x2109cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2109cU: { // 6100e1ac bsr.w $1f24a
            next = 0x210a0U;
            const auto result = m.call(c, 372U, 0x2109cU, 0x1f24aU, 0x210a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x210a0U: { // 6100cb22 bsr.w $1dbc4
            next = 0x210a4U;
            const auto result = m.call(c, 352U, 0x210a0U, 0x1dbc4U, 0x210a4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x210a4U: { // 6100cb4a bsr.w $1dbf0
            next = 0x210a8U;
            const auto result = m.call(c, 353U, 0x210a4U, 0x1dbf0U, 0x210a8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x210a8U: { // 6100cd86 bsr.w $1de30
            next = 0x210acU;
            const auto result = m.call(c, 355U, 0x210a8U, 0x1de30U, 0x210acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x210acU: { // 6100d076 bsr.w $1e124
            next = 0x210b0U;
            const auto result = m.call(c, 357U, 0x210acU, 0x1e124U, 0x210b0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x210b0U: { // 6100d252 bsr.w $1e304
            next = 0x210b4U;
            const auto result = m.call(c, 360U, 0x210b0U, 0x1e304U, 0x210b4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x210b4U: { // 4e75 rts 
            next = 0x210b6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x210b4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21094U:
        case 0x21098U:
        case 0x2109cU:
        case 0x210a0U:
        case 0x210a4U:
        case 0x210a8U:
        case 0x210acU:
        case 0x210b0U:
        case 0x210b4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_021116(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21116U: { // 6100aca8 bsr.w $1bdc0
            next = 0x2111aU;
            const auto result = m.call(c, 327U, 0x21116U, 0x1bdc0U, 0x2111aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2111aU: { // 6100e12e bsr.w $1f24a
            next = 0x2111eU;
            const auto result = m.call(c, 372U, 0x2111aU, 0x1f24aU, 0x2111eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2111eU: { // 6100d4ce bsr.w $1e5ee
            next = 0x21122U;
            const auto result = m.call(c, 672U, 0x2111eU, 0x1e5eeU, 0x21122U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21122U: { // 4e75 rts 
            next = 0x21124U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21122U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21116U:
        case 0x2111aU:
        case 0x2111eU:
        case 0x21122U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
