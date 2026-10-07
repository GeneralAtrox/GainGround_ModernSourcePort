
FunctionResult cpu_b_stage_01a396(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a396U: { // 046d08080010 subi.w #$808, $10(a5)
            next = 0x1a39cU;
            const auto source_value = 0x808U;
            const auto destination_address = r.address[5] + 0x10U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a39cU: { // 6e04 bgt.b $1a3a2
            next = 0x1a39eU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1a3a2U; transfer_kind = 1U; }
            break;
        }
        case 0x1a39eU: { // 4215 clr.b (a5)
            next = 0x1a3a0U;
            const auto destination_address = r.address[5];
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1a3a0U: { // 4e75 rts 
            next = 0x1a3a2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a3a0U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a3a2U: { // 08950000 bclr.b #$0, (a5)
            next = 0x1a3a6U;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1a3a6U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1a3acU;
            const auto result = m.call(c, 280U, 0x1a3a6U, 0x15d24U, 0x1a3acU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a3acU: { // 4eb900015df2 jsr $15df2.l
            next = 0x1a3b2U;
            const auto result = m.call(c, 282U, 0x1a3acU, 0x15df2U, 0x1a3b2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1a3b2U: { // 4e75 rts 
            next = 0x1a3b4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a3b2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a396U:
        case 0x1a39cU:
        case 0x1a39eU:
        case 0x1a3a0U:
        case 0x1a3a2U:
        case 0x1a3a6U:
        case 0x1a3acU:
        case 0x1a3b2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
