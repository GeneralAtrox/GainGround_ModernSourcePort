
FunctionResult cpu_b_stage_021452(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21452U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x21458U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21458U: { // 6728 beq.b $21482
            next = 0x2145aU;
            if ((r.status & 4U) != 0U) { next = 0x21482U; transfer_kind = 1U; }
            break;
        }
        case 0x2145aU: { // 4a2d003f tst.b $3f(a5)
            next = 0x2145eU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x2145eU: { // 6708 beq.b $21468
            next = 0x21460U;
            if ((r.status & 4U) != 0U) { next = 0x21468U; transfer_kind = 1U; }
            break;
        }
        case 0x21460U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21464U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21464U: { // 522e002c addq.b #$1, $2c(a6)
            next = 0x21468U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x2cU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21468U: { // 6100ba8c bsr.w $1cef6
            next = 0x2146cU;
            const auto result = m.call(c, 334U, 0x21468U, 0x1cef6U, 0x2146cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2146cU: { // 6100c756 bsr.w $1dbc4
            next = 0x21470U;
            const auto result = m.call(c, 352U, 0x2146cU, 0x1dbc4U, 0x21470U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21470U: { // 6100c77e bsr.w $1dbf0
            next = 0x21474U;
            const auto result = m.call(c, 353U, 0x21470U, 0x1dbf0U, 0x21474U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21474U: { // 6100ccae bsr.w $1e124
            next = 0x21478U;
            const auto result = m.call(c, 357U, 0x21474U, 0x1e124U, 0x21478U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21478U: { // 6100cd06 bsr.w $1e180
            next = 0x2147cU;
            const auto result = m.call(c, 359U, 0x21478U, 0x1e180U, 0x2147cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2147cU: { // 6100d866 bsr.w $1ece4
            next = 0x21480U;
            const auto result = m.call(c, 365U, 0x2147cU, 0x1ece4U, 0x21480U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21480U: { // 4e75 rts 
            next = 0x21482U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21480U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21482U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21486U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21486U: { // 6708 beq.b $21490
            next = 0x21488U;
            if ((r.status & 4U) != 0U) { next = 0x21490U; transfer_kind = 1U; }
            break;
        }
        case 0x21488U: { // 4df83400 lea.l $3400.w, a6
            next = 0x2148cU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x2148cU: { // 522e002c addq.b #$1, $2c(a6)
            next = 0x21490U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x2cU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21490U: { // 6100a69a bsr.w $1bb2c
            next = 0x21494U;
            const auto result = m.call(c, 564U, 0x21490U, 0x1bb2cU, 0x21494U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21494U: { // 6100cc58 bsr.w $1e0ee
            next = 0x21498U;
            const auto result = m.call(c, 356U, 0x21494U, 0x1e0eeU, 0x21498U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21498U: { // 6100c72a bsr.w $1dbc4
            next = 0x2149cU;
            const auto result = m.call(c, 352U, 0x21498U, 0x1dbc4U, 0x2149cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2149cU: { // 6100c752 bsr.w $1dbf0
            next = 0x214a0U;
            const auto result = m.call(c, 353U, 0x2149cU, 0x1dbf0U, 0x214a0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214a0U: { // 6100c98e bsr.w $1de30
            next = 0x214a4U;
            const auto result = m.call(c, 355U, 0x214a0U, 0x1de30U, 0x214a4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214a4U: { // 6100cc7e bsr.w $1e124
            next = 0x214a8U;
            const auto result = m.call(c, 357U, 0x214a4U, 0x1e124U, 0x214a8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214a8U: { // 6100ccd6 bsr.w $1e180
            next = 0x214acU;
            const auto result = m.call(c, 359U, 0x214a8U, 0x1e180U, 0x214acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214acU: { // 4e75 rts 
            next = 0x214aeU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x214acU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21452U:
        case 0x21458U:
        case 0x2145aU:
        case 0x2145eU:
        case 0x21460U:
        case 0x21464U:
        case 0x21468U:
        case 0x2146cU:
        case 0x21470U:
        case 0x21474U:
        case 0x21478U:
        case 0x2147cU:
        case 0x21480U:
        case 0x21482U:
        case 0x21486U:
        case 0x21488U:
        case 0x2148cU:
        case 0x21490U:
        case 0x21494U:
        case 0x21498U:
        case 0x2149cU:
        case 0x214a0U:
        case 0x214a4U:
        case 0x214a8U:
        case 0x214acU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
