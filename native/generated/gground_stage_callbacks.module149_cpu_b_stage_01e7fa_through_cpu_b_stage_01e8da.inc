
FunctionResult cpu_b_stage_01e7fa(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e7faU: { // 610003f4 bsr.w $1ebf0
            next = 0x1e7feU;
            const auto result = m.call(c, 363U, 0x1e7faU, 0x1ebf0U, 0x1e7feU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        default:
            if (!(cpu_b_stage_01e7fa_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01e7fa_dispatch_cases_001(m, c, pc, next, transfer_kind) || cpu_b_stage_01e7fa_dispatch_cases_002(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01e7fa_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

FunctionResult cpu_b_stage_01e8da(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e8daU: { // 082d00000040 btst.b #$0, $40(a5)
            next = 0x1e8e0U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e8e0U: { // 6600f948 bne.w $1e22a
            next = 0x1e8e4U;
            if ((r.status & 4U) == 0U) { next = 0x1e22aU; transfer_kind = 1U; }
            break;
        }
        case 0x1e8e4U: { // 7000 moveq #$0, d0
            next = 0x1e8e6U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1e8e6U: { // 102d0036 move.b $36(a5), d0
            next = 0x1e8eaU;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e8eaU: { // 3200 move.w d0, d1
            next = 0x1e8ecU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e8ecU: { // e540 asl.w #$2, d0
            next = 0x1e8eeU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1e8eeU: { // e341 asl.w #$1, d1
            next = 0x1e8f0U;
            m.asl_word(1U, 1U);
            break;
        }
        case 0x1e8f0U: { // d041 add.w d1, d0
            next = 0x1e8f2U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e8f2U: { // 322d0056 move.w $56(a5), d1
            next = 0x1e8f6U;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e8f6U: { // 3401 move.w d1, d2
            next = 0x1e8f8U;
            const auto value = r.data[1];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e8f8U: { // 02410600 andi.w #$600, d1
            next = 0x1e8fcU;
            const auto source_value = 0x600U;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e8fcU: { // d440 add.w d0, d2
            next = 0x1e8feU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e8feU: { // 024207ff andi.w #$7ff, d2
            next = 0x1e902U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[2];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e902U: { // 3b420056 move.w d2, $56(a5)
            next = 0x1e906U;
            const auto value = r.data[2];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e906U: { // 02420600 andi.w #$600, d2
            next = 0x1e90aU;
            const auto source_value = 0x600U;
            const auto destination_value = r.data[2];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e90aU: { // b242 cmp.w d2, d1
            next = 0x1e90cU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e90cU: { // 660a bne.b $1e918
            next = 0x1e90eU;
            if ((r.status & 4U) == 0U) { next = 0x1e918U; transfer_kind = 1U; }
            break;
        }
        case 0x1e90eU: { // 082d00030040 btst.b #$3, $40(a5)
            next = 0x1e914U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e914U: { // 6700f914 beq.w $1e22a
            next = 0x1e918U;
            if ((r.status & 4U) != 0U) { next = 0x1e22aU; transfer_kind = 1U; }
            break;
        }
        case 0x1e918U: { // 610002d6 bsr.w $1ebf0
            next = 0x1e91cU;
            const auto result = m.call(c, 363U, 0x1e918U, 0x1ebf0U, 0x1e91cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e91cU: { // 7200 moveq #$0, d1
            next = 0x1e91eU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e91eU: { // ef5a rol.w #$7, d2
            next = 0x1e920U;
            const auto old = r.data[2] & 0xffffU;
            const auto value = ((old << 7U) | (old >> 9U)) & 0xffffU;
            m.dw(2U, value);
            m.logic(value, 16U);
            r.status = static_cast<std::uint16_t>(r.status | (value & 1U));
            break;
        }
        case 0x1e920U: { // 5342 subq.w #$1, d2
            next = 0x1e922U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[2];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e922U: { // 6b06 bmi.b $1e92a
            next = 0x1e924U;
            if ((r.status & 8U) != 0U) { next = 0x1e92aU; transfer_kind = 1U; }
            break;
        }
        case 0x1e924U: { // d247 add.w d7, d1
            next = 0x1e926U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e926U: { // 51cafffc dbra d2, $1e924
            next = 0x1e92aU;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0x1e924U;
            break;
        }
        case 0x1e92aU: { // e547 asl.w #$2, d7
            next = 0x1e92cU;
            m.asl_word(7U, 2U);
            break;
        }
        case 0x1e92cU: { // 302d005c move.w $5c(a5), d0
            next = 0x1e930U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e930U: { // 06400080 addi.w #$80, d0
            next = 0x1e934U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e934U: { // e048 lsr.w #$8, d0
            next = 0x1e936U;
            m.shift_word(0U, 8U, false, false);
            break;
        }
        case 0x1e936U: { // 02400007 andi.w #$7, d0
            next = 0x1e93aU;
            const auto source_value = 0x7U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e93aU: { // 5340 subq.w #$1, d0
            next = 0x1e93cU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e93cU: { // 6b06 bmi.b $1e944
            next = 0x1e93eU;
            if ((r.status & 8U) != 0U) { next = 0x1e944U; transfer_kind = 1U; }
            break;
        }
        case 0x1e93eU: { // d247 add.w d7, d1
            next = 0x1e940U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e940U: { // 51c8fffc dbra d0, $1e93e
            next = 0x1e944U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e93eU;
            break;
        }
        case 0x1e944U: { // 6000f8ac bra.w $1e1f2
            next = 0x1e948U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1e8daU:
        case 0x1e8e0U:
        case 0x1e8e4U:
        case 0x1e8e6U:
        case 0x1e8eaU:
        case 0x1e8ecU:
        case 0x1e8eeU:
        case 0x1e8f0U:
        case 0x1e8f2U:
        case 0x1e8f6U:
        case 0x1e8f8U:
        case 0x1e8fcU:
        case 0x1e8feU:
        case 0x1e902U:
        case 0x1e906U:
        case 0x1e90aU:
        case 0x1e90cU:
        case 0x1e90eU:
        case 0x1e914U:
        case 0x1e918U:
        case 0x1e91cU:
        case 0x1e91eU:
        case 0x1e920U:
        case 0x1e922U:
        case 0x1e924U:
        case 0x1e926U:
        case 0x1e92aU:
        case 0x1e92cU:
        case 0x1e930U:
        case 0x1e934U:
        case 0x1e936U:
        case 0x1e93aU:
        case 0x1e93cU:
        case 0x1e93eU:
        case 0x1e940U:
        case 0x1e944U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
