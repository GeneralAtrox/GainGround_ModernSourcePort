
FunctionResult cpu_b_stage_021f1a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21f1aU: { // 4a2d003f tst.b $3f(a5)
            next = 0x21f1eU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21f1eU: { // 671e beq.b $21f3e
            next = 0x21f20U;
            if ((r.status & 4U) != 0U) { next = 0x21f3eU; transfer_kind = 1U; }
            break;
        }
        case 0x21f20U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21f24U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21f24U: { // 522e0028 addq.b #$1, $28(a6)
            next = 0x21f28U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x28U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21f28U: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x21f30U;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x21f30U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x21f36U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x21f36U: { // 7200 moveq #$0, d1
            next = 0x21f38U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x21f38U: { // 6100c3ca bsr.w $1e304
            next = 0x21f3cU;
            const auto result = m.call(c, 360U, 0x21f38U, 0x1e304U, 0x21f3cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f3cU: { // 4e75 rts 
            next = 0x21f3eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21f3cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21f3eU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x21f44U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21f44U: { // 671a beq.b $21f60
            next = 0x21f46U;
            if ((r.status & 4U) != 0U) { next = 0x21f60U; transfer_kind = 1U; }
            break;
        }
        case 0x21f46U: { // 6100afae bsr.w $1cef6
            next = 0x21f4aU;
            const auto result = m.call(c, 334U, 0x21f46U, 0x1cef6U, 0x21f4aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f4aU: { // 6100bc78 bsr.w $1dbc4
            next = 0x21f4eU;
            const auto result = m.call(c, 352U, 0x21f4aU, 0x1dbc4U, 0x21f4eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f4eU: { // 6100bca0 bsr.w $1dbf0
            next = 0x21f52U;
            const auto result = m.call(c, 353U, 0x21f4eU, 0x1dbf0U, 0x21f52U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f52U: { // 6100c1d0 bsr.w $1e124
            next = 0x21f56U;
            const auto result = m.call(c, 357U, 0x21f52U, 0x1e124U, 0x21f56U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f56U: { // 6100c3ac bsr.w $1e304
            next = 0x21f5aU;
            const auto result = m.call(c, 360U, 0x21f56U, 0x1e304U, 0x21f5aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f5aU: { // 6100cd88 bsr.w $1ece4
            next = 0x21f5eU;
            const auto result = m.call(c, 365U, 0x21f5aU, 0x1ece4U, 0x21f5eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f5eU: { // 4e75 rts 
            next = 0x21f60U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21f5eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21f60U: { // 61009e5e bsr.w $1bdc0
            next = 0x21f64U;
            const auto result = m.call(c, 327U, 0x21f60U, 0x1bdc0U, 0x21f64U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f64U: { // 6100d296 bsr.w $1f1fc
            next = 0x21f68U;
            const auto result = m.call(c, 371U, 0x21f64U, 0x1f1fcU, 0x21f68U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f68U: { // 6100c39a bsr.w $1e304
            next = 0x21f6cU;
            const auto result = m.call(c, 360U, 0x21f68U, 0x1e304U, 0x21f6cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21f6cU: { // 4e75 rts 
            next = 0x21f6eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21f6cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21f1aU:
        case 0x21f1eU:
        case 0x21f20U:
        case 0x21f24U:
        case 0x21f28U:
        case 0x21f30U:
        case 0x21f36U:
        case 0x21f38U:
        case 0x21f3cU:
        case 0x21f3eU:
        case 0x21f44U:
        case 0x21f46U:
        case 0x21f4aU:
        case 0x21f4eU:
        case 0x21f52U:
        case 0x21f56U:
        case 0x21f5aU:
        case 0x21f5eU:
        case 0x21f60U:
        case 0x21f64U:
        case 0x21f68U:
        case 0x21f6cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
