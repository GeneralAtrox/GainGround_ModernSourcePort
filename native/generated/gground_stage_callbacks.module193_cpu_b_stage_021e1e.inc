
FunctionResult cpu_b_stage_021e1e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21e1eU: { // 4a2d003f tst.b $3f(a5)
            next = 0x21e22U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21e22U: { // 671e beq.b $21e42
            next = 0x21e24U;
            if ((r.status & 4U) != 0U) { next = 0x21e42U; transfer_kind = 1U; }
            break;
        }
        case 0x21e24U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21e28U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21e28U: { // 522e0022 addq.b #$1, $22(a6)
            next = 0x21e2cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x22U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21e2cU: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x21e34U;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x21e34U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x21e3aU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x21e3aU: { // 7200 moveq #$0, d1
            next = 0x21e3cU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x21e3cU: { // 6100c4c6 bsr.w $1e304
            next = 0x21e40U;
            const auto result = m.call(c, 360U, 0x21e3cU, 0x1e304U, 0x21e40U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e40U: { // 4e75 rts 
            next = 0x21e42U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21e40U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21e42U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x21e48U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21e48U: { // 671a beq.b $21e64
            next = 0x21e4aU;
            if ((r.status & 4U) != 0U) { next = 0x21e64U; transfer_kind = 1U; }
            break;
        }
        case 0x21e4aU: { // 6100b0aa bsr.w $1cef6
            next = 0x21e4eU;
            const auto result = m.call(c, 334U, 0x21e4aU, 0x1cef6U, 0x21e4eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e4eU: { // 6100bd74 bsr.w $1dbc4
            next = 0x21e52U;
            const auto result = m.call(c, 352U, 0x21e4eU, 0x1dbc4U, 0x21e52U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e52U: { // 6100bd9c bsr.w $1dbf0
            next = 0x21e56U;
            const auto result = m.call(c, 353U, 0x21e52U, 0x1dbf0U, 0x21e56U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e56U: { // 6100c2cc bsr.w $1e124
            next = 0x21e5aU;
            const auto result = m.call(c, 357U, 0x21e56U, 0x1e124U, 0x21e5aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e5aU: { // 6100c4a8 bsr.w $1e304
            next = 0x21e5eU;
            const auto result = m.call(c, 360U, 0x21e5aU, 0x1e304U, 0x21e5eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e5eU: { // 6100ce84 bsr.w $1ece4
            next = 0x21e62U;
            const auto result = m.call(c, 365U, 0x21e5eU, 0x1ece4U, 0x21e62U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e62U: { // 4e75 rts 
            next = 0x21e64U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21e62U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21e64U: { // 61009f5a bsr.w $1bdc0
            next = 0x21e68U;
            const auto result = m.call(c, 327U, 0x21e64U, 0x1bdc0U, 0x21e68U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e68U: { // 6100d392 bsr.w $1f1fc
            next = 0x21e6cU;
            const auto result = m.call(c, 371U, 0x21e68U, 0x1f1fcU, 0x21e6cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e6cU: { // 6100c496 bsr.w $1e304
            next = 0x21e70U;
            const auto result = m.call(c, 360U, 0x21e6cU, 0x1e304U, 0x21e70U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e70U: { // 4e75 rts 
            next = 0x21e72U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21e70U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21e1eU:
        case 0x21e22U:
        case 0x21e24U:
        case 0x21e28U:
        case 0x21e2cU:
        case 0x21e34U:
        case 0x21e3aU:
        case 0x21e3cU:
        case 0x21e40U:
        case 0x21e42U:
        case 0x21e48U:
        case 0x21e4aU:
        case 0x21e4eU:
        case 0x21e52U:
        case 0x21e56U:
        case 0x21e5aU:
        case 0x21e5eU:
        case 0x21e62U:
        case 0x21e64U:
        case 0x21e68U:
        case 0x21e6cU:
        case 0x21e70U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
