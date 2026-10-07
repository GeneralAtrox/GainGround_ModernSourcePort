
FunctionResult cpu_b_stage_01e948(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e948U: { // 610002a6 bsr.w $1ebf0
            next = 0x1e94cU;
            const auto result = m.call(c, 363U, 0x1e948U, 0x1ebf0U, 0x1e94cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1e94cU: { // 7000 moveq #$0, d0
            next = 0x1e94eU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1e94eU: { // 102d0036 move.b $36(a5), d0
            next = 0x1e952U;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1e952U: { // 3200 move.w d0, d1
            next = 0x1e954U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e954U: { // e540 asl.w #$2, d0
            next = 0x1e956U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1e956U: { // e341 asl.w #$1, d1
            next = 0x1e958U;
            m.asl_word(1U, 1U);
            break;
        }
        case 0x1e958U: { // d041 add.w d1, d0
            next = 0x1e95aU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e95aU: { // 322d0056 move.w $56(a5), d1
            next = 0x1e95eU;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e95eU: { // d041 add.w d1, d0
            next = 0x1e960U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e960U: { // 024007ff andi.w #$7ff, d0
            next = 0x1e964U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e964U: { // 3b400056 move.w d0, $56(a5)
            next = 0x1e968U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e968U: { // 02400600 andi.w #$600, d0
            next = 0x1e96cU;
            const auto source_value = 0x600U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e96cU: { // 7200 moveq #$0, d1
            next = 0x1e96eU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e96eU: { // ef58 rol.w #$7, d0
            next = 0x1e970U;
            const auto old = r.data[0] & 0xffffU;
            const auto value = ((old << 7U) | (old >> 9U)) & 0xffffU;
            m.dw(0U, value);
            m.logic(value, 16U);
            r.status = static_cast<std::uint16_t>(r.status | (value & 1U));
            break;
        }
        case 0x1e970U: { // 9247 sub.w d7, d1
            next = 0x1e972U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e972U: { // d247 add.w d7, d1
            next = 0x1e974U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e974U: { // 51c8fffc dbra d0, $1e972
            next = 0x1e978U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e972U;
            break;
        }
        case 0x1e978U: { // 3407 move.w d7, d2
            next = 0x1e97aU;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e97aU: { // e542 asl.w #$2, d2
            next = 0x1e97cU;
            m.asl_word(2U, 2U);
            break;
        }
        case 0x1e97cU: { // 302d005a move.w $5a(a5), d0
            next = 0x1e980U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e980U: { // 06400080 addi.w #$80, d0
            next = 0x1e984U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e984U: { // e048 lsr.w #$8, d0
            next = 0x1e986U;
            m.shift_word(0U, 8U, false, false);
            break;
        }
        case 0x1e986U: { // 02400007 andi.w #$7, d0
            next = 0x1e98aU;
            const auto source_value = 0x7U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e98aU: { // 9242 sub.w d2, d1
            next = 0x1e98cU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e98cU: { // d242 add.w d2, d1
            next = 0x1e98eU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e98eU: { // 51c8fffc dbra d0, $1e98c
            next = 0x1e992U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e98cU;
            break;
        }
        case 0x1e992U: { // 302d005c move.w $5c(a5), d0
            next = 0x1e996U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e996U: { // 06400200 addi.w #$200, d0
            next = 0x1e99aU;
            const auto source_value = 0x200U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e99aU: { // 02400400 andi.w #$400, d0
            next = 0x1e99eU;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e99eU: { // 6700f852 beq.w $1e1f2
            next = 0x1e9a2U;
            if ((r.status & 4U) != 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e9a2U: { // 3007 move.w d7, d0
            next = 0x1e9a4U;
            const auto value = r.data[7];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e9a4U: { // eb48 lsl.w #$5, d0
            next = 0x1e9a6U;
            m.shift_word(0U, 5U, true, false);
            break;
        }
        case 0x1e9a6U: { // d240 add.w d0, d1
            next = 0x1e9a8U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e9a8U: { // 6000f848 bra.w $1e1f2
            next = 0x1e9acU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1e948U:
        case 0x1e94cU:
        case 0x1e94eU:
        case 0x1e952U:
        case 0x1e954U:
        case 0x1e956U:
        case 0x1e958U:
        case 0x1e95aU:
        case 0x1e95eU:
        case 0x1e960U:
        case 0x1e964U:
        case 0x1e968U:
        case 0x1e96cU:
        case 0x1e96eU:
        case 0x1e970U:
        case 0x1e972U:
        case 0x1e974U:
        case 0x1e978U:
        case 0x1e97aU:
        case 0x1e97cU:
        case 0x1e980U:
        case 0x1e984U:
        case 0x1e986U:
        case 0x1e98aU:
        case 0x1e98cU:
        case 0x1e98eU:
        case 0x1e992U:
        case 0x1e996U:
        case 0x1e99aU:
        case 0x1e99eU:
        case 0x1e9a2U:
        case 0x1e9a4U:
        case 0x1e9a6U:
        case 0x1e9a8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
