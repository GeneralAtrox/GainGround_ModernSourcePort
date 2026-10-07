
FunctionResult cpu_b_stage_01ea6c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1ea6cU: { // 61000182 bsr.w $1ebf0
            next = 0x1ea70U;
            const auto result = m.call(c, 363U, 0x1ea6cU, 0x1ebf0U, 0x1ea70U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1ea70U: { // 7000 moveq #$0, d0
            next = 0x1ea72U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1ea72U: { // 102d0036 move.b $36(a5), d0
            next = 0x1ea76U;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1ea76U: { // 3200 move.w d0, d1
            next = 0x1ea78U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ea78U: { // e540 asl.w #$2, d0
            next = 0x1ea7aU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1ea7aU: { // e341 asl.w #$1, d1
            next = 0x1ea7cU;
            m.asl_word(1U, 1U);
            break;
        }
        case 0x1ea7cU: { // d041 add.w d1, d0
            next = 0x1ea7eU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ea7eU: { // 322d0056 move.w $56(a5), d1
            next = 0x1ea82U;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ea82U: { // d041 add.w d1, d0
            next = 0x1ea84U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1ea84U: { // 0c401800 cmpi.w #$1800, d0
            next = 0x1ea88U;
            const auto source_value = 0x1800U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1ea88U: { // 6d02 blt.b $1ea8c
            next = 0x1ea8aU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1ea8cU; transfer_kind = 1U; }
            break;
        }
        case 0x1ea8aU: { // 7000 moveq #$0, d0
            next = 0x1ea8cU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1ea8cU: { // 3b400056 move.w d0, $56(a5)
            next = 0x1ea90U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ea90U: { // e048 lsr.w #$8, d0
            next = 0x1ea92U;
            m.shift_word(0U, 8U, false, false);
            break;
        }
        case 0x1ea92U: { // 7200 moveq #$0, d1
            next = 0x1ea94U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1ea94U: { // 9247 sub.w d7, d1
            next = 0x1ea96U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1ea96U: { // d247 add.w d7, d1
            next = 0x1ea98U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1ea98U: { // 51c8fffc dbra d0, $1ea96
            next = 0x1ea9cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1ea96U;
            break;
        }
        case 0x1ea9cU: { // 6000f754 bra.w $1e1f2
            next = 0x1eaa0U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1ea6cU:
        case 0x1ea70U:
        case 0x1ea72U:
        case 0x1ea76U:
        case 0x1ea78U:
        case 0x1ea7aU:
        case 0x1ea7cU:
        case 0x1ea7eU:
        case 0x1ea82U:
        case 0x1ea84U:
        case 0x1ea88U:
        case 0x1ea8aU:
        case 0x1ea8cU:
        case 0x1ea90U:
        case 0x1ea92U:
        case 0x1ea94U:
        case 0x1ea96U:
        case 0x1ea98U:
        case 0x1ea9cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
