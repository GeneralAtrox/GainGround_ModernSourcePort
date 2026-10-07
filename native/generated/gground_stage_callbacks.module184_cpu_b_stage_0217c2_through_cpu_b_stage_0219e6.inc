
FunctionResult cpu_b_stage_0217c2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x217c2U: { // 6100a80a bsr.w $1bfce
            next = 0x217c6U;
            const auto result = m.call(c, 652U, 0x217c2U, 0x1bfceU, 0x217c6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x217c6U: { // 6100c428 bsr.w $1dbf0
            next = 0x217caU;
            const auto result = m.call(c, 353U, 0x217c6U, 0x1dbf0U, 0x217caU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x217caU: { // 6100da30 bsr.w $1f1fc
            next = 0x217ceU;
            const auto result = m.call(c, 371U, 0x217caU, 0x1f1fcU, 0x217ceU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x217ceU: { // 6100d02a bsr.w $1e7fa
            next = 0x217d2U;
            const auto result = m.call(c, 675U, 0x217ceU, 0x1e7faU, 0x217d2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x217d2U: { // 4e75 rts 
            next = 0x217d4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x217d2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x217c2U:
        case 0x217c6U:
        case 0x217caU:
        case 0x217ceU:
        case 0x217d2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0219d4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x219d4U: { // 6100a6d0 bsr.w $1c0a6
            next = 0x219d8U;
            const auto result = m.call(c, 653U, 0x219d4U, 0x1c0a6U, 0x219d8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219d8U: { // 6100c216 bsr.w $1dbf0
            next = 0x219dcU;
            const auto result = m.call(c, 353U, 0x219d8U, 0x1dbf0U, 0x219dcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219dcU: { // 6100d81e bsr.w $1f1fc
            next = 0x219e0U;
            const auto result = m.call(c, 371U, 0x219dcU, 0x1f1fcU, 0x219e0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219e0U: { // 6100ce18 bsr.w $1e7fa
            next = 0x219e4U;
            const auto result = m.call(c, 675U, 0x219e0U, 0x1e7faU, 0x219e4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x219e4U: { // 4e75 rts 
            next = 0x219e6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x219e4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x219d4U:
        case 0x219d8U:
        case 0x219dcU:
        case 0x219e0U:
        case 0x219e4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0219e6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x219e6U: { // 4a2d003f tst.b $3f(a5)
            next = 0x219eaU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x219eaU: { // 6708 beq.b $219f4
            next = 0x219ecU;
            if ((r.status & 4U) != 0U) { next = 0x219f4U; transfer_kind = 1U; }
            break;
        }
        case 0x219ecU: { // 4df83400 lea.l $3400.w, a6
            next = 0x219f0U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x219f0U: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x219f4U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x22U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x219f4U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x219faU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x219faU: { // 671a beq.b $21a16
            next = 0x219fcU;
            if ((r.status & 4U) != 0U) { next = 0x21a16U; transfer_kind = 1U; }
            break;
        }
        case 0x219fcU: { // 6100b4f8 bsr.w $1cef6
            next = 0x21a00U;
            const auto result = m.call(c, 334U, 0x219fcU, 0x1cef6U, 0x21a00U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a00U: { // 6100c1c2 bsr.w $1dbc4
            next = 0x21a04U;
            const auto result = m.call(c, 352U, 0x21a00U, 0x1dbc4U, 0x21a04U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a04U: { // 6100c1ea bsr.w $1dbf0
            next = 0x21a08U;
            const auto result = m.call(c, 353U, 0x21a04U, 0x1dbf0U, 0x21a08U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a08U: { // 6100c71a bsr.w $1e124
            next = 0x21a0cU;
            const auto result = m.call(c, 357U, 0x21a08U, 0x1e124U, 0x21a0cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a0cU: { // 6100cecc bsr.w $1e8da
            next = 0x21a10U;
            const auto result = m.call(c, 676U, 0x21a0cU, 0x1e8daU, 0x21a10U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a10U: { // 6100d2d2 bsr.w $1ece4
            next = 0x21a14U;
            const auto result = m.call(c, 365U, 0x21a10U, 0x1ece4U, 0x21a14U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a14U: { // 4e75 rts 
            next = 0x21a16U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21a14U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21a16U: { // 6100a114 bsr.w $1bb2c
            next = 0x21a1aU;
            const auto result = m.call(c, 564U, 0x21a16U, 0x1bb2cU, 0x21a1aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a1aU: { // 6100c6d2 bsr.w $1e0ee
            next = 0x21a1eU;
            const auto result = m.call(c, 356U, 0x21a1aU, 0x1e0eeU, 0x21a1eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a1eU: { // 6100d5bc bsr.w $1efdc
            next = 0x21a22U;
            const auto result = m.call(c, 368U, 0x21a1eU, 0x1efdcU, 0x21a22U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a22U: { // 6100c1a0 bsr.w $1dbc4
            next = 0x21a26U;
            const auto result = m.call(c, 352U, 0x21a22U, 0x1dbc4U, 0x21a26U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a26U: { // 6100c1c8 bsr.w $1dbf0
            next = 0x21a2aU;
            const auto result = m.call(c, 353U, 0x21a26U, 0x1dbf0U, 0x21a2aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a2aU: { // 6100c404 bsr.w $1de30
            next = 0x21a2eU;
            const auto result = m.call(c, 355U, 0x21a2aU, 0x1de30U, 0x21a2eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a2eU: { // 6100c6f4 bsr.w $1e124
            next = 0x21a32U;
            const auto result = m.call(c, 357U, 0x21a2eU, 0x1e124U, 0x21a32U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a32U: { // 6100cea6 bsr.w $1e8da
            next = 0x21a36U;
            const auto result = m.call(c, 676U, 0x21a32U, 0x1e8daU, 0x21a36U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a36U: { // 4e75 rts 
            next = 0x21a38U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21a36U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x219e6U:
        case 0x219eaU:
        case 0x219ecU:
        case 0x219f0U:
        case 0x219f4U:
        case 0x219faU:
        case 0x219fcU:
        case 0x21a00U:
        case 0x21a04U:
        case 0x21a08U:
        case 0x21a0cU:
        case 0x21a10U:
        case 0x21a14U:
        case 0x21a16U:
        case 0x21a1aU:
        case 0x21a1eU:
        case 0x21a22U:
        case 0x21a26U:
        case 0x21a2aU:
        case 0x21a2eU:
        case 0x21a32U:
        case 0x21a36U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
