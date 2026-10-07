
FunctionResult cpu_b_stage_01ea1a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1ea1aU: { // 610001d4 bsr.w $1ebf0
            next = 0x1ea1eU;
            const auto result = m.call(c, 363U, 0x1ea1aU, 0x1ebf0U, 0x1ea1eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ea1eU: { // 7200 moveq #$0, d1
            next = 0x1ea20U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1ea20U: { // 302d005c move.w $5c(a5), d0
            next = 0x1ea24U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ea24U: { // 06400080 addi.w #$80, d0
            next = 0x1ea28U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ea28U: { // 02400700 andi.w #$700, d0
            next = 0x1ea2cU;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ea2cU: { // e040 asr.w #$8, d0
            next = 0x1ea2eU;
            m.shift_word(0U, 8U, false, true);
            break;
        }
        case 0x1ea2eU: { // 5340 subq.w #$1, d0
            next = 0x1ea30U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ea30U: { // 6b0a bmi.b $1ea3c
            next = 0x1ea32U;
            if ((r.status & 8U) != 0U) { next = 0x1ea3cU; transfer_kind = 1U; }
            break;
        }
        case 0x1ea32U: { // 3407 move.w d7, d2
            next = 0x1ea34U;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ea34U: { // e54a lsl.w #$2, d2
            next = 0x1ea36U;
            m.shift_word(2U, 2U, true, false);
            break;
        }
        case 0x1ea36U: { // d242 add.w d2, d1
            next = 0x1ea38U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1ea38U: { // 51c8fffc dbra d0, $1ea36
            next = 0x1ea3cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1ea36U;
            break;
        }
        case 0x1ea3cU: { // 4a2d003e tst.b $3e(a5)
            next = 0x1ea40U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1ea40U: { // 6600f7b0 bne.w $1e1f2
            next = 0x1ea44U;
            if ((r.status & 4U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1ea44U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1ea4aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1ea4aU: { // 67000018 beq.w $1ea64
            next = 0x1ea4eU;
            if ((r.status & 4U) != 0U) { next = 0x1ea64U; transfer_kind = 1U; }
            break;
        }
        case 0x1ea4eU: { // 526d0056 addq.w #$1, $56(a5)
            next = 0x1ea52U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1ea52U: { // 302d0056 move.w $56(a5), d0
            next = 0x1ea56U;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ea56U: { // 02400020 andi.w #$20, d0
            next = 0x1ea5aU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ea5aU: { // 6600f796 bne.w $1e1f2
            next = 0x1ea5eU;
            if ((r.status & 4U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1ea5eU: { // d247 add.w d7, d1
            next = 0x1ea60U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1ea60U: { // 6000f790 bra.w $1e1f2
            next = 0x1ea64U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1ea64U: { // 3207 move.w d7, d1
            next = 0x1ea66U;
            const auto value = r.data[7];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ea66U: { // eb49 lsl.w #$5, d1
            next = 0x1ea68U;
            m.shift_word(1U, 5U, true, false);
            break;
        }
        case 0x1ea68U: { // 6000f788 bra.w $1e1f2
            next = 0x1ea6cU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1ea1aU:
        case 0x1ea1eU:
        case 0x1ea20U:
        case 0x1ea24U:
        case 0x1ea28U:
        case 0x1ea2cU:
        case 0x1ea2eU:
        case 0x1ea30U:
        case 0x1ea32U:
        case 0x1ea34U:
        case 0x1ea36U:
        case 0x1ea38U:
        case 0x1ea3cU:
        case 0x1ea40U:
        case 0x1ea44U:
        case 0x1ea4aU:
        case 0x1ea4eU:
        case 0x1ea52U:
        case 0x1ea56U:
        case 0x1ea5aU:
        case 0x1ea5eU:
        case 0x1ea60U:
        case 0x1ea64U:
        case 0x1ea66U:
        case 0x1ea68U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
