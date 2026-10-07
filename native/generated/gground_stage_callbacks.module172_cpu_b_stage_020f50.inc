
FunctionResult cpu_b_stage_020f50(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20f50U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x20f56U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x20f56U: { // 6728 beq.b $20f80
            next = 0x20f58U;
            if ((r.status & 4U) != 0U) { next = 0x20f80U; transfer_kind = 1U; }
            break;
        }
        case 0x20f58U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20f5cU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20f5cU: { // 6708 beq.b $20f66
            next = 0x20f5eU;
            if ((r.status & 4U) != 0U) { next = 0x20f66U; transfer_kind = 1U; }
            break;
        }
        case 0x20f5eU: { // 4df83400 lea.l $3400.w, a6
            next = 0x20f62U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x20f62U: { // 522e002a addq.b #$1, $2a(a6)
            next = 0x20f66U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x2aU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x20f66U: { // 6100bf8e bsr.w $1cef6
            next = 0x20f6aU;
            const auto result = m.call(c, 334U, 0x20f66U, 0x1cef6U, 0x20f6aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f6aU: { // 6100cc58 bsr.w $1dbc4
            next = 0x20f6eU;
            const auto result = m.call(c, 352U, 0x20f6aU, 0x1dbc4U, 0x20f6eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f6eU: { // 6100cc80 bsr.w $1dbf0
            next = 0x20f72U;
            const auto result = m.call(c, 353U, 0x20f6eU, 0x1dbf0U, 0x20f72U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f72U: { // 6100d1b0 bsr.w $1e124
            next = 0x20f76U;
            const auto result = m.call(c, 357U, 0x20f72U, 0x1e124U, 0x20f76U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f76U: { // 6100d38c bsr.w $1e304
            next = 0x20f7aU;
            const auto result = m.call(c, 360U, 0x20f76U, 0x1e304U, 0x20f7aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f7aU: { // 6100dd68 bsr.w $1ece4
            next = 0x20f7eU;
            const auto result = m.call(c, 365U, 0x20f7aU, 0x1ece4U, 0x20f7eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f7eU: { // 4e75 rts 
            next = 0x20f80U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20f7eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20f80U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20f84U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20f84U: { // 6708 beq.b $20f8e
            next = 0x20f86U;
            if ((r.status & 4U) != 0U) { next = 0x20f8eU; transfer_kind = 1U; }
            break;
        }
        case 0x20f86U: { // 4df83400 lea.l $3400.w, a6
            next = 0x20f8aU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x20f8aU: { // 522e002a addq.b #$1, $2a(a6)
            next = 0x20f8eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x2aU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x20f8eU: { // 6100ae30 bsr.w $1bdc0
            next = 0x20f92U;
            const auto result = m.call(c, 327U, 0x20f8eU, 0x1bdc0U, 0x20f92U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f92U: { // 6100e268 bsr.w $1f1fc
            next = 0x20f96U;
            const auto result = m.call(c, 371U, 0x20f92U, 0x1f1fcU, 0x20f96U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f96U: { // 6100d40e bsr.w $1e3a6
            next = 0x20f9aU;
            const auto result = m.call(c, 361U, 0x20f96U, 0x1e3a6U, 0x20f9aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f9aU: { // 4e75 rts 
            next = 0x20f9cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20f9aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20f50U:
        case 0x20f56U:
        case 0x20f58U:
        case 0x20f5cU:
        case 0x20f5eU:
        case 0x20f62U:
        case 0x20f66U:
        case 0x20f6aU:
        case 0x20f6eU:
        case 0x20f72U:
        case 0x20f76U:
        case 0x20f7aU:
        case 0x20f7eU:
        case 0x20f80U:
        case 0x20f84U:
        case 0x20f86U:
        case 0x20f8aU:
        case 0x20f8eU:
        case 0x20f92U:
        case 0x20f96U:
        case 0x20f9aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
