
FunctionResult cpu_b_stage_01e9ac(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e9acU: { // 61000242 bsr.w $1ebf0
            next = 0x1e9b0U;
            const auto result = m.call(c, 363U, 0x1e9acU, 0x1ebf0U, 0x1e9b0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e9b0U: { // 7200 moveq #$0, d1
            next = 0x1e9b2U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e9b2U: { // 302d005c move.w $5c(a5), d0
            next = 0x1e9b6U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e9b6U: { // 06400080 addi.w #$80, d0
            next = 0x1e9baU;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e9baU: { // 02400700 andi.w #$700, d0
            next = 0x1e9beU;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e9beU: { // e040 asr.w #$8, d0
            next = 0x1e9c0U;
            m.shift_word(0U, 8U, false, true);
            break;
        }
        case 0x1e9c0U: { // 5340 subq.w #$1, d0
            next = 0x1e9c2U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e9c2U: { // 6b0a bmi.b $1e9ce
            next = 0x1e9c4U;
            if ((r.status & 8U) != 0U) { next = 0x1e9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1e9c4U: { // 3407 move.w d7, d2
            next = 0x1e9c6U;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e9c6U: { // e54a lsl.w #$2, d2
            next = 0x1e9c8U;
            m.shift_word(2U, 2U, true, false);
            break;
        }
        case 0x1e9c8U: { // d242 add.w d2, d1
            next = 0x1e9caU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e9caU: { // 51c8fffc dbra d0, $1e9c8
            next = 0x1e9ceU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e9c8U;
            break;
        }
        case 0x1e9ceU: { // 4a2d003e tst.b $3e(a5)
            next = 0x1e9d2U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1e9d2U: { // 6600f81e bne.w $1e1f2
            next = 0x1e9d6U;
            if ((r.status & 4U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e9d6U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1e9dcU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e9dcU: { // 6726 beq.b $1ea04
            next = 0x1e9deU;
            if ((r.status & 4U) != 0U) { next = 0x1ea04U; transfer_kind = 1U; }
            break;
        }
        case 0x1e9deU: { // 082d00020041 btst.b #$2, $41(a5)
            next = 0x1e9e4U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e9e4U: { // 671e beq.b $1ea04
            next = 0x1e9e6U;
            if ((r.status & 4U) != 0U) { next = 0x1ea04U; transfer_kind = 1U; }
            break;
        }
        case 0x1e9e6U: { // d247 add.w d7, d1
            next = 0x1e9e8U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e9e8U: { // 0c6d00040074 cmpi.w #$4, $74(a5)
            next = 0x1e9eeU;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e9eeU: { // 6e00f802 bgt.w $1e1f2
            next = 0x1e9f2U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e9f2U: { // d247 add.w d7, d1
            next = 0x1e9f4U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e9f4U: { // 0c6d00020074 cmpi.w #$2, $74(a5)
            next = 0x1e9faU;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e9faU: { // 6e00f7f6 bgt.w $1e1f2
            next = 0x1e9feU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e9feU: { // d247 add.w d7, d1
            next = 0x1ea00U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1ea00U: { // 6000f7f0 bra.w $1e1f2
            next = 0x1ea04U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1ea04U: { // 526d0056 addq.w #$1, $56(a5)
            next = 0x1ea08U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1ea08U: { // 302d0056 move.w $56(a5), d0
            next = 0x1ea0cU;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ea0cU: { // 02400008 andi.w #$8, d0
            next = 0x1ea10U;
            const auto source_value = 0x8U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ea10U: { // 6600f7e0 bne.w $1e1f2
            next = 0x1ea14U;
            if ((r.status & 4U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1ea14U: { // d247 add.w d7, d1
            next = 0x1ea16U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1ea16U: { // 6000f7da bra.w $1e1f2
            next = 0x1ea1aU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1e9acU:
        case 0x1e9b0U:
        case 0x1e9b2U:
        case 0x1e9b6U:
        case 0x1e9baU:
        case 0x1e9beU:
        case 0x1e9c0U:
        case 0x1e9c2U:
        case 0x1e9c4U:
        case 0x1e9c6U:
        case 0x1e9c8U:
        case 0x1e9caU:
        case 0x1e9ceU:
        case 0x1e9d2U:
        case 0x1e9d6U:
        case 0x1e9dcU:
        case 0x1e9deU:
        case 0x1e9e4U:
        case 0x1e9e6U:
        case 0x1e9e8U:
        case 0x1e9eeU:
        case 0x1e9f2U:
        case 0x1e9f4U:
        case 0x1e9faU:
        case 0x1e9feU:
        case 0x1ea00U:
        case 0x1ea04U:
        case 0x1ea08U:
        case 0x1ea0cU:
        case 0x1ea10U:
        case 0x1ea14U:
        case 0x1ea16U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
