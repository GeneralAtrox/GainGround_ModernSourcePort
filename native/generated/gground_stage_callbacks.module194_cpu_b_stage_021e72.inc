
FunctionResult cpu_b_stage_021e72(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x21e72U: { // 4a2d003f tst.b $3f(a5)
            next = 0x21e76U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x21e76U: { // 671e beq.b $21e96
            next = 0x21e78U;
            if ((r.status & 4U) != 0U) { next = 0x21e96U; transfer_kind = 1U; }
            break;
        }
        case 0x21e78U: { // 4df83400 lea.l $3400.w, a6
            next = 0x21e7cU;
            const auto source_address = 0x3400U;
            r.address[6] = source_address;
            break;
        }
        case 0x21e7cU: { // 522e0024 addq.b #$1, $24(a6)
            next = 0x21e80U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[6] + 0x24U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x21e80U: { // 2b7c0001eeb60002 move.l #$1eeb6, $2(a5)
            next = 0x21e88U;
            const auto value = 0x1eeb6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x21e88U: { // 3b7c00000054 move.w #$0, $54(a5)
            next = 0x21e8eU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x21e8eU: { // 7200 moveq #$0, d1
            next = 0x21e90U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x21e90U: { // 6100c472 bsr.w $1e304
            next = 0x21e94U;
            const auto result = m.call(c, 360U, 0x21e90U, 0x1e304U, 0x21e94U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21e94U: { // 4e75 rts 
            next = 0x21e96U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21e94U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21e96U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x21e9cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x21e9cU: { // 671a beq.b $21eb8
            next = 0x21e9eU;
            if ((r.status & 4U) != 0U) { next = 0x21eb8U; transfer_kind = 1U; }
            break;
        }
        case 0x21e9eU: { // 6100b056 bsr.w $1cef6
            next = 0x21ea2U;
            const auto result = m.call(c, 334U, 0x21e9eU, 0x1cef6U, 0x21ea2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ea2U: { // 6100bd20 bsr.w $1dbc4
            next = 0x21ea6U;
            const auto result = m.call(c, 352U, 0x21ea2U, 0x1dbc4U, 0x21ea6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ea6U: { // 6100bd48 bsr.w $1dbf0
            next = 0x21eaaU;
            const auto result = m.call(c, 353U, 0x21ea6U, 0x1dbf0U, 0x21eaaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21eaaU: { // 6100c278 bsr.w $1e124
            next = 0x21eaeU;
            const auto result = m.call(c, 357U, 0x21eaaU, 0x1e124U, 0x21eaeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21eaeU: { // 6100c454 bsr.w $1e304
            next = 0x21eb2U;
            const auto result = m.call(c, 360U, 0x21eaeU, 0x1e304U, 0x21eb2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21eb2U: { // 6100ce30 bsr.w $1ece4
            next = 0x21eb6U;
            const auto result = m.call(c, 365U, 0x21eb2U, 0x1ece4U, 0x21eb6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21eb6U: { // 4e75 rts 
            next = 0x21eb8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21eb6U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x21eb8U: { // 61009f06 bsr.w $1bdc0
            next = 0x21ebcU;
            const auto result = m.call(c, 327U, 0x21eb8U, 0x1bdc0U, 0x21ebcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ebcU: { // 6100d33e bsr.w $1f1fc
            next = 0x21ec0U;
            const auto result = m.call(c, 371U, 0x21ebcU, 0x1f1fcU, 0x21ec0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ec0U: { // 6100c442 bsr.w $1e304
            next = 0x21ec4U;
            const auto result = m.call(c, 360U, 0x21ec0U, 0x1e304U, 0x21ec4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x21ec4U: { // 4e75 rts 
            next = 0x21ec6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x21ec4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x21e72U:
        case 0x21e76U:
        case 0x21e78U:
        case 0x21e7cU:
        case 0x21e80U:
        case 0x21e88U:
        case 0x21e8eU:
        case 0x21e90U:
        case 0x21e94U:
        case 0x21e96U:
        case 0x21e9cU:
        case 0x21e9eU:
        case 0x21ea2U:
        case 0x21ea6U:
        case 0x21eaaU:
        case 0x21eaeU:
        case 0x21eb2U:
        case 0x21eb6U:
        case 0x21eb8U:
        case 0x21ebcU:
        case 0x21ec0U:
        case 0x21ec4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
