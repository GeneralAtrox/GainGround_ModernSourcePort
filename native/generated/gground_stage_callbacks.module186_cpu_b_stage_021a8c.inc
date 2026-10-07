
FunctionResult cpu_b_stage_021a8c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21a8cU: { // 4a2d003f tst.b $3f(a5)
            next = 0x21a90U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21a90U: { // 6708 beq.b $21a9a
            next = 0x21a92U;
            if ((r.status & 4U) != 0U) { next = 0x21a9aU; transfer_kind = 1U; }
            break;
        }
        case 0x21a92U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21a96U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21a96U: { // 522e0026 addq.b #$1, $26(a6)
            next = 0x21a9aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x26U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21a9aU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x21aa0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21aa0U: { // 671c beq.b $21abe
            next = 0x21aa2U;
            if ((r.status & 4U) != 0U) { next = 0x21abeU; transfer_kind = 1U; }
            break;
        }
        case 0x21aa2U: { // 6100b452 bsr.w $1cef6
            next = 0x21aa6U;
            const auto result = m.call(c, 334U, 0x21aa2U, 0x1cef6U, 0x21aa6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21aa6U: { // 6100c11c bsr.w $1dbc4
            next = 0x21aaaU;
            const auto result = m.call(c, 352U, 0x21aa6U, 0x1dbc4U, 0x21aaaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21aaaU: { // 6100c144 bsr.w $1dbf0
            next = 0x21aaeU;
            const auto result = m.call(c, 353U, 0x21aaaU, 0x1dbf0U, 0x21aaeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21aaeU: { // 6100c674 bsr.w $1e124
            next = 0x21ab2U;
            const auto result = m.call(c, 357U, 0x21aaeU, 0x1e124U, 0x21ab2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ab2U: { // 6100ce26 bsr.w $1e8da
            next = 0x21ab6U;
            const auto result = m.call(c, 676U, 0x21ab2U, 0x1e8daU, 0x21ab6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ab6U: { // 6100d22c bsr.w $1ece4
            next = 0x21abaU;
            const auto result = m.call(c, 365U, 0x21ab6U, 0x1ece4U, 0x21abaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21abaU: { // 4e75 rts 
            next = 0x21abcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21abaU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21abeU: { // 6100a06c bsr.w $1bb2c
            next = 0x21ac2U;
            const auto result = m.call(c, 564U, 0x21abeU, 0x1bb2cU, 0x21ac2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ac2U: { // 6100c62a bsr.w $1e0ee
            next = 0x21ac6U;
            const auto result = m.call(c, 356U, 0x21ac2U, 0x1e0eeU, 0x21ac6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ac6U: { // 6100d514 bsr.w $1efdc
            next = 0x21acaU;
            const auto result = m.call(c, 368U, 0x21ac6U, 0x1efdcU, 0x21acaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21acaU: { // 6100c0f8 bsr.w $1dbc4
            next = 0x21aceU;
            const auto result = m.call(c, 352U, 0x21acaU, 0x1dbc4U, 0x21aceU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21aceU: { // 6100c120 bsr.w $1dbf0
            next = 0x21ad2U;
            const auto result = m.call(c, 353U, 0x21aceU, 0x1dbf0U, 0x21ad2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ad2U: { // 6100c35c bsr.w $1de30
            next = 0x21ad6U;
            const auto result = m.call(c, 355U, 0x21ad2U, 0x1de30U, 0x21ad6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ad6U: { // 6100c64c bsr.w $1e124
            next = 0x21adaU;
            const auto result = m.call(c, 357U, 0x21ad6U, 0x1e124U, 0x21adaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21adaU: { // 6100cdfe bsr.w $1e8da
            next = 0x21adeU;
            const auto result = m.call(c, 676U, 0x21adaU, 0x1e8daU, 0x21adeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21adeU: { // 4e75 rts 
            next = 0x21ae0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21adeU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21a8cU:
        case 0x21a90U:
        case 0x21a92U:
        case 0x21a96U:
        case 0x21a9aU:
        case 0x21aa0U:
        case 0x21aa2U:
        case 0x21aa6U:
        case 0x21aaaU:
        case 0x21aaeU:
        case 0x21ab2U:
        case 0x21ab6U:
        case 0x21abaU:
        case 0x21abeU:
        case 0x21ac2U:
        case 0x21ac6U:
        case 0x21acaU:
        case 0x21aceU:
        case 0x21ad2U:
        case 0x21ad6U:
        case 0x21adaU:
        case 0x21adeU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
