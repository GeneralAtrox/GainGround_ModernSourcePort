
FunctionResult cpu_b_stage_020eb8(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20eb8U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x20ebeU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x20ebeU: { // 6728 beq.b $20ee8
            next = 0x20ec0U;
            if ((r.status & 4U) != 0U) { next = 0x20ee8U; transfer_kind = 1U; }
            break;
        }
        case 0x20ec0U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20ec4U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20ec4U: { // 6708 beq.b $20ece
            next = 0x20ec6U;
            if ((r.status & 4U) != 0U) { next = 0x20eceU; transfer_kind = 1U; }
            break;
        }
        case 0x20ec6U: { // 4df83400 lea.l $3400.w, a6
            next = 0x20ecaU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x20ecaU: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x20eceU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x22U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x20eceU: { // 6100c026 bsr.w $1cef6
            next = 0x20ed2U;
            const auto result = m.call(c, 334U, 0x20eceU, 0x1cef6U, 0x20ed2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ed2U: { // 6100ccf0 bsr.w $1dbc4
            next = 0x20ed6U;
            const auto result = m.call(c, 352U, 0x20ed2U, 0x1dbc4U, 0x20ed6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ed6U: { // 6100cd18 bsr.w $1dbf0
            next = 0x20edaU;
            const auto result = m.call(c, 353U, 0x20ed6U, 0x1dbf0U, 0x20edaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20edaU: { // 6100d248 bsr.w $1e124
            next = 0x20edeU;
            const auto result = m.call(c, 357U, 0x20edaU, 0x1e124U, 0x20edeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20edeU: { // 6100d424 bsr.w $1e304
            next = 0x20ee2U;
            const auto result = m.call(c, 360U, 0x20edeU, 0x1e304U, 0x20ee2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ee2U: { // 6100de00 bsr.w $1ece4
            next = 0x20ee6U;
            const auto result = m.call(c, 365U, 0x20ee2U, 0x1ece4U, 0x20ee6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20ee6U: { // 4e75 rts 
            next = 0x20ee8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20ee6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20ee8U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20eecU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20eecU: { // 6708 beq.b $20ef6
            next = 0x20eeeU;
            if ((r.status & 4U) != 0U) { next = 0x20ef6U; transfer_kind = 1U; }
            break;
        }
        case 0x20eeeU: { // 4df83400 lea.l $3400.w, a6
            next = 0x20ef2U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x20ef2U: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x20ef6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x22U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x20ef6U: { // 6100aec8 bsr.w $1bdc0
            next = 0x20efaU;
            const auto result = m.call(c, 327U, 0x20ef6U, 0x1bdc0U, 0x20efaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20efaU: { // 6100e300 bsr.w $1f1fc
            next = 0x20efeU;
            const auto result = m.call(c, 371U, 0x20efaU, 0x1f1fcU, 0x20efeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20efeU: { // 6100d4a6 bsr.w $1e3a6
            next = 0x20f02U;
            const auto result = m.call(c, 361U, 0x20efeU, 0x1e3a6U, 0x20f02U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f02U: { // 4e75 rts 
            next = 0x20f04U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20f02U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20eb8U:
        case 0x20ebeU:
        case 0x20ec0U:
        case 0x20ec4U:
        case 0x20ec6U:
        case 0x20ecaU:
        case 0x20eceU:
        case 0x20ed2U:
        case 0x20ed6U:
        case 0x20edaU:
        case 0x20edeU:
        case 0x20ee2U:
        case 0x20ee6U:
        case 0x20ee8U:
        case 0x20eecU:
        case 0x20eeeU:
        case 0x20ef2U:
        case 0x20ef6U:
        case 0x20efaU:
        case 0x20efeU:
        case 0x20f02U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
