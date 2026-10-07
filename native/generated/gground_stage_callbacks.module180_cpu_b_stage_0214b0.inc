
FunctionResult cpu_b_stage_0214b0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x214b0U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x214b6U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x214b6U: { // 6728 beq.b $214e0
            next = 0x214b8U;
            if ((r.status & 4U) != 0U) { next = 0x214e0U; transfer_kind = 1U; }
            break;
        }
        case 0x214b8U: { // 4a2d003f tst.b $3f(a5)
            next = 0x214bcU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x214bcU: { // 6708 beq.b $214c6
            next = 0x214beU;
            if ((r.status & 4U) != 0U) { next = 0x214c6U; transfer_kind = 1U; }
            break;
        }
        case 0x214beU: { // 4df83400 lea.l $3400.w, a6
            next = 0x214c2U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x214c2U: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x214c6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x22U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x214c6U: { // 6100ba2e bsr.w $1cef6
            next = 0x214caU;
            const auto result = m.call(c, 334U, 0x214c6U, 0x1cef6U, 0x214caU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214caU: { // 6100c6f8 bsr.w $1dbc4
            next = 0x214ceU;
            const auto result = m.call(c, 352U, 0x214caU, 0x1dbc4U, 0x214ceU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214ceU: { // 6100c720 bsr.w $1dbf0
            next = 0x214d2U;
            const auto result = m.call(c, 353U, 0x214ceU, 0x1dbf0U, 0x214d2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214d2U: { // 6100cc50 bsr.w $1e124
            next = 0x214d6U;
            const auto result = m.call(c, 357U, 0x214d2U, 0x1e124U, 0x214d6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214d6U: { // 6100ce2c bsr.w $1e304
            next = 0x214daU;
            const auto result = m.call(c, 360U, 0x214d6U, 0x1e304U, 0x214daU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214daU: { // 6100d808 bsr.w $1ece4
            next = 0x214deU;
            const auto result = m.call(c, 365U, 0x214daU, 0x1ece4U, 0x214deU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214deU: { // 4e75 rts 
            next = 0x214e0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x214deU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x214e0U: { // 4a2d003f tst.b $3f(a5)
            next = 0x214e4U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x214e4U: { // 6708 beq.b $214ee
            next = 0x214e6U;
            if ((r.status & 4U) != 0U) { next = 0x214eeU; transfer_kind = 1U; }
            break;
        }
        case 0x214e6U: { // 4df83400 lea.l $3400.w, a6
            next = 0x214eaU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x214eaU: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x214eeU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x22U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x214eeU: { // 1b6d005f005e move.b $5f(a5), $5e(a5)
            next = 0x214f4U;
            const auto source_address = r.address[5] + 0x5fU;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x5eU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x214f4U: { // 6100a8ca bsr.w $1bdc0
            next = 0x214f8U;
            const auto result = m.call(c, 327U, 0x214f4U, 0x1bdc0U, 0x214f8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214f8U: { // 6100dd50 bsr.w $1f24a
            next = 0x214fcU;
            const auto result = m.call(c, 372U, 0x214f8U, 0x1f24aU, 0x214fcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x214fcU: { // 6100cea8 bsr.w $1e3a6
            next = 0x21500U;
            const auto result = m.call(c, 361U, 0x214fcU, 0x1e3a6U, 0x21500U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21500U: { // 4e75 rts 
            next = 0x21502U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21500U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x214b0U:
        case 0x214b6U:
        case 0x214b8U:
        case 0x214bcU:
        case 0x214beU:
        case 0x214c2U:
        case 0x214c6U:
        case 0x214caU:
        case 0x214ceU:
        case 0x214d2U:
        case 0x214d6U:
        case 0x214daU:
        case 0x214deU:
        case 0x214e0U:
        case 0x214e4U:
        case 0x214e6U:
        case 0x214eaU:
        case 0x214eeU:
        case 0x214f4U:
        case 0x214f8U:
        case 0x214fcU:
        case 0x21500U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
