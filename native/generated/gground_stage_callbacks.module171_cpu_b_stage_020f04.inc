
FunctionResult cpu_b_stage_020f04(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x20f04U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x20f0aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x20f0aU: { // 6728 beq.b $20f34
            next = 0x20f0cU;
            if ((r.status & 4U) != 0U) { next = 0x20f34U; transfer_kind = 1U; }
            break;
        }
        case 0x20f0cU: { // 4a2d003f tst.b $3f(a5)
            next = 0x20f10U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20f10U: { // 6708 beq.b $20f1a
            next = 0x20f12U;
            if ((r.status & 4U) != 0U) { next = 0x20f1aU; transfer_kind = 1U; }
            break;
        }
        case 0x20f12U: { // 4df83400 lea.l $3400.w, a6
            next = 0x20f16U;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x20f16U: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x20f1aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x20f1aU: { // 6100bfda bsr.w $1cef6
            next = 0x20f1eU;
            const auto result = m.call(c, 334U, 0x20f1aU, 0x1cef6U, 0x20f1eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f1eU: { // 6100cca4 bsr.w $1dbc4
            next = 0x20f22U;
            const auto result = m.call(c, 352U, 0x20f1eU, 0x1dbc4U, 0x20f22U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f22U: { // 6100cccc bsr.w $1dbf0
            next = 0x20f26U;
            const auto result = m.call(c, 353U, 0x20f22U, 0x1dbf0U, 0x20f26U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f26U: { // 6100d1fc bsr.w $1e124
            next = 0x20f2aU;
            const auto result = m.call(c, 357U, 0x20f26U, 0x1e124U, 0x20f2aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f2aU: { // 6100d3d8 bsr.w $1e304
            next = 0x20f2eU;
            const auto result = m.call(c, 360U, 0x20f2aU, 0x1e304U, 0x20f2eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f2eU: { // 6100ddb4 bsr.w $1ece4
            next = 0x20f32U;
            const auto result = m.call(c, 365U, 0x20f2eU, 0x1ece4U, 0x20f32U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f32U: { // 4e75 rts 
            next = 0x20f34U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20f32U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x20f34U: { // 4a2d003f tst.b $3f(a5)
            next = 0x20f38U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x20f38U: { // 6708 beq.b $20f42
            next = 0x20f3aU;
            if ((r.status & 4U) != 0U) { next = 0x20f42U; transfer_kind = 1U; }
            break;
        }
        case 0x20f3aU: { // 4df83400 lea.l $3400.w, a6
            next = 0x20f3eU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x20f3eU: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x20f42U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x20f42U: { // 6100ae7c bsr.w $1bdc0
            next = 0x20f46U;
            const auto result = m.call(c, 327U, 0x20f42U, 0x1bdc0U, 0x20f46U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f46U: { // 6100e2b4 bsr.w $1f1fc
            next = 0x20f4aU;
            const auto result = m.call(c, 371U, 0x20f46U, 0x1f1fcU, 0x20f4aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f4aU: { // 6100d45a bsr.w $1e3a6
            next = 0x20f4eU;
            const auto result = m.call(c, 361U, 0x20f4aU, 0x1e3a6U, 0x20f4eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x20f4eU: { // 4e75 rts 
            next = 0x20f50U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x20f4eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x20f04U:
        case 0x20f0aU:
        case 0x20f0cU:
        case 0x20f10U:
        case 0x20f12U:
        case 0x20f16U:
        case 0x20f1aU:
        case 0x20f1eU:
        case 0x20f22U:
        case 0x20f26U:
        case 0x20f2aU:
        case 0x20f2eU:
        case 0x20f32U:
        case 0x20f34U:
        case 0x20f38U:
        case 0x20f3aU:
        case 0x20f3eU:
        case 0x20f42U:
        case 0x20f46U:
        case 0x20f4aU:
        case 0x20f4eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
