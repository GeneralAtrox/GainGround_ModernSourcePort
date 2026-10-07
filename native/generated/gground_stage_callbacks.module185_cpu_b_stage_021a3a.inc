
FunctionResult cpu_b_stage_021a3a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21a3aU: { // 4a2d003f tst.b $3f(a5)
            next = 0x21a3eU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21a3eU: { // 6708 beq.b $21a48
            next = 0x21a40U;
            if ((r.status & 4U) != 0U) { next = 0x21a48U; transfer_kind = 1U; }
            break;
        }
        case 0x21a40U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21a44U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21a44U: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x21a48U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21a48U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x21a4eU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21a4eU: { // 671a beq.b $21a6a
            next = 0x21a50U;
            if ((r.status & 4U) != 0U) { next = 0x21a6aU; transfer_kind = 1U; }
            break;
        }
        case 0x21a50U: { // 6100b4a4 bsr.w $1cef6
            next = 0x21a54U;
            const auto result = m.call(c, 334U, 0x21a50U, 0x1cef6U, 0x21a54U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a54U: { // 6100c16e bsr.w $1dbc4
            next = 0x21a58U;
            const auto result = m.call(c, 352U, 0x21a54U, 0x1dbc4U, 0x21a58U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a58U: { // 6100c196 bsr.w $1dbf0
            next = 0x21a5cU;
            const auto result = m.call(c, 353U, 0x21a58U, 0x1dbf0U, 0x21a5cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a5cU: { // 6100c6c6 bsr.w $1e124
            next = 0x21a60U;
            const auto result = m.call(c, 357U, 0x21a5cU, 0x1e124U, 0x21a60U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a60U: { // 6100ce78 bsr.w $1e8da
            next = 0x21a64U;
            const auto result = m.call(c, 676U, 0x21a60U, 0x1e8daU, 0x21a64U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a64U: { // 6100d27e bsr.w $1ece4
            next = 0x21a68U;
            const auto result = m.call(c, 365U, 0x21a64U, 0x1ece4U, 0x21a68U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a68U: { // 4e75 rts 
            next = 0x21a6aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21a68U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21a6aU: { // 6100a0c0 bsr.w $1bb2c
            next = 0x21a6eU;
            const auto result = m.call(c, 564U, 0x21a6aU, 0x1bb2cU, 0x21a6eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a6eU: { // 6100c67e bsr.w $1e0ee
            next = 0x21a72U;
            const auto result = m.call(c, 356U, 0x21a6eU, 0x1e0eeU, 0x21a72U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a72U: { // 6100d568 bsr.w $1efdc
            next = 0x21a76U;
            const auto result = m.call(c, 368U, 0x21a72U, 0x1efdcU, 0x21a76U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a76U: { // 6100c14c bsr.w $1dbc4
            next = 0x21a7aU;
            const auto result = m.call(c, 352U, 0x21a76U, 0x1dbc4U, 0x21a7aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a7aU: { // 6100c174 bsr.w $1dbf0
            next = 0x21a7eU;
            const auto result = m.call(c, 353U, 0x21a7aU, 0x1dbf0U, 0x21a7eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a7eU: { // 6100c3b0 bsr.w $1de30
            next = 0x21a82U;
            const auto result = m.call(c, 355U, 0x21a7eU, 0x1de30U, 0x21a82U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a82U: { // 6100c6a0 bsr.w $1e124
            next = 0x21a86U;
            const auto result = m.call(c, 357U, 0x21a82U, 0x1e124U, 0x21a86U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a86U: { // 6100ce52 bsr.w $1e8da
            next = 0x21a8aU;
            const auto result = m.call(c, 676U, 0x21a86U, 0x1e8daU, 0x21a8aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21a8aU: { // 4e75 rts 
            next = 0x21a8cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21a8aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21a3aU:
        case 0x21a3eU:
        case 0x21a40U:
        case 0x21a44U:
        case 0x21a48U:
        case 0x21a4eU:
        case 0x21a50U:
        case 0x21a54U:
        case 0x21a58U:
        case 0x21a5cU:
        case 0x21a60U:
        case 0x21a64U:
        case 0x21a68U:
        case 0x21a6aU:
        case 0x21a6eU:
        case 0x21a72U:
        case 0x21a76U:
        case 0x21a7aU:
        case 0x21a7eU:
        case 0x21a82U:
        case 0x21a86U:
        case 0x21a8aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
