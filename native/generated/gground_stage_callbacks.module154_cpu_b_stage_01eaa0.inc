
FunctionResult cpu_b_stage_01eaa0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1eaa0U: { // 6100014e bsr.w $1ebf0
            next = 0x1eaa4U;
            const auto result = m.call(c, 363U, 0x1eaa0U, 0x1ebf0U, 0x1eaa4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1eaa4U: { // 082d00000041 btst.b #$0, $41(a5)
            next = 0x1eaaaU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1eaaaU: { // 6730 beq.b $1eadc
            next = 0x1eaacU;
            if ((r.status & 4U) != 0U) { next = 0x1eadcU; transfer_kind = 1U; }
            break;
        }
        case 0x1eaacU: { // 7000 moveq #$0, d0
            next = 0x1eaaeU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1eaaeU: { // 102d0036 move.b $36(a5), d0
            next = 0x1eab2U;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1eab2U: { // 3200 move.w d0, d1
            next = 0x1eab4U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eab4U: { // e540 asl.w #$2, d0
            next = 0x1eab6U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1eab6U: { // e341 asl.w #$1, d1
            next = 0x1eab8U;
            m.asl_word(1U, 1U);
            break;
        }
        case 0x1eab8U: { // d041 add.w d1, d0
            next = 0x1eabaU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1eabaU: { // 322d0056 move.w $56(a5), d1
            next = 0x1eabeU;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eabeU: { // d041 add.w d1, d0
            next = 0x1eac0U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1eac0U: { // 024007ff andi.w #$7ff, d0
            next = 0x1eac4U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1eac4U: { // 3b400056 move.w d0, $56(a5)
            next = 0x1eac8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eac8U: { // 02400300 andi.w #$300, d0
            next = 0x1eaccU;
            const auto source_value = 0x300U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1eaccU: { // e158 rol.w #$8, d0
            next = 0x1eaceU;
            const auto old = r.data[0] & 0xffffU;
            const auto value = ((old << 8U) | (old >> 8U)) & 0xffffU;
            m.dw(0U, value);
            m.logic(value, 16U);
            r.status = static_cast<std::uint16_t>(r.status | (value & 1U));
            break;
        }
        case 0x1eaceU: { // 7200 moveq #$0, d1
            next = 0x1ead0U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1ead0U: { // 9247 sub.w d7, d1
            next = 0x1ead2U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1ead2U: { // d247 add.w d7, d1
            next = 0x1ead4U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1ead4U: { // 51c8fffc dbra d0, $1ead2
            next = 0x1ead8U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1ead2U;
            break;
        }
        case 0x1ead8U: { // 6000f718 bra.w $1e1f2
            next = 0x1eadcU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1eadcU: { // 3207 move.w d7, d1
            next = 0x1eadeU;
            const auto value = r.data[7];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eadeU: { // e549 lsl.w #$2, d1
            next = 0x1eae0U;
            m.shift_word(1U, 2U, true, false);
            break;
        }
        case 0x1eae0U: { // 0c6d00030074 cmpi.w #$3, $74(a5)
            next = 0x1eae6U;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1eae6U: { // 6e00f70a bgt.w $1e1f2
            next = 0x1eaeaU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1eaeaU: { // d247 add.w d7, d1
            next = 0x1eaecU;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1eaecU: { // 6000f704 bra.w $1e1f2
            next = 0x1eaf0U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1eaa0U:
        case 0x1eaa4U:
        case 0x1eaaaU:
        case 0x1eaacU:
        case 0x1eaaeU:
        case 0x1eab2U:
        case 0x1eab4U:
        case 0x1eab6U:
        case 0x1eab8U:
        case 0x1eabaU:
        case 0x1eabeU:
        case 0x1eac0U:
        case 0x1eac4U:
        case 0x1eac8U:
        case 0x1eaccU:
        case 0x1eaceU:
        case 0x1ead0U:
        case 0x1ead2U:
        case 0x1ead4U:
        case 0x1ead8U:
        case 0x1eadcU:
        case 0x1eadeU:
        case 0x1eae0U:
        case 0x1eae6U:
        case 0x1eaeaU:
        case 0x1eaecU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
