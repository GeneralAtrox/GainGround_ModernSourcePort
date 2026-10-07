
static bool cpu_b_stage_01e5ee_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e5f2U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1e5f8U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e5f8U: { // 6708 beq.b $1e602
            next = 0x1e5faU;
            if ((r.status & 4U) != 0U) { next = 0x1e602U; transfer_kind = 1U; }
            break;
        }
        case 0x1e5faU: { // 082d00000041 btst.b #$0, $41(a5)
            next = 0x1e600U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e600U: { // 6732 beq.b $1e634
            next = 0x1e602U;
            if ((r.status & 4U) != 0U) { next = 0x1e634U; transfer_kind = 1U; }
            break;
        }
        case 0x1e602U: { // 302d005c move.w $5c(a5), d0
            next = 0x1e606U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e606U: { // 06400080 addi.w #$80, d0
            next = 0x1e60aU;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e60aU: { // 02400700 andi.w #$700, d0
            next = 0x1e60eU;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e60eU: { // e048 lsr.w #$8, d0
            next = 0x1e610U;
            m.shift_word(0U, 8U, false, false);
            break;
        }
        case 0x1e610U: { // 3407 move.w d7, d2
            next = 0x1e612U;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e612U: { // d442 add.w d2, d2
            next = 0x1e614U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e614U: { // 7200 moveq #$0, d1
            next = 0x1e616U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e616U: { // 9242 sub.w d2, d1
            next = 0x1e618U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e618U: { // d242 add.w d2, d1
            next = 0x1e61aU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e61aU: { // 51c8fffc dbra d0, $1e618
            next = 0x1e61eU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e618U;
            break;
        }
        case 0x1e61eU: { // 526d0056 addq.w #$1, $56(a5)
            next = 0x1e622U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1e622U: { // 302d0056 move.w $56(a5), d0
            next = 0x1e626U;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e626U: { // 02400008 andi.w #$8, d0
            next = 0x1e62aU;
            const auto source_value = 0x8U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e62aU: { // 6700fbc6 beq.w $1e1f2
            next = 0x1e62eU;
            if ((r.status & 4U) != 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e62eU: { // d247 add.w d7, d1
            next = 0x1e630U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e630U: { // 6000fbc0 bra.w $1e1f2
            next = 0x1e634U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e634U: { // 302d005c move.w $5c(a5), d0
            next = 0x1e638U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e638U: { // 06400080 addi.w #$80, d0
            next = 0x1e63cU;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e63cU: { // 02400700 andi.w #$700, d0
            next = 0x1e640U;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e640U: { // e048 lsr.w #$8, d0
            next = 0x1e642U;
            m.shift_word(0U, 8U, false, false);
            break;
        }
        case 0x1e642U: { // 7200 moveq #$0, d1
            next = 0x1e644U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1e644U: { // 3407 move.w d7, d2
            next = 0x1e646U;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e646U: { // d442 add.w d2, d2
            next = 0x1e648U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e648U: { // 9242 sub.w d2, d1
            next = 0x1e64aU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e64aU: { // d242 add.w d2, d1
            next = 0x1e64cU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e64cU: { // 51c8fffc dbra d0, $1e64a
            next = 0x1e650U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e64aU;
            break;
        }
        case 0x1e650U: { // 0c6d00030074 cmpi.w #$3, $74(a5)
            next = 0x1e656U;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e656U: { // 6e00fb9a bgt.w $1e1f2
            next = 0x1e65aU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e65aU: { // d247 add.w d7, d1
            next = 0x1e65cU;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e65cU: { // 6000fb94 bra.w $1e1f2
            next = 0x1e660U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01e5ee(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e5eeU: { // 61000600 bsr.w $1ebf0
            next = 0x1e5f2U;
            const auto result = m.call(c, 363U, 0x1e5eeU, 0x1ebf0U, 0x1e5f2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        default:
            if (!(cpu_b_stage_01e5ee_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1e5eeU:
        case 0x1e5f2U:
        case 0x1e5f8U:
        case 0x1e5faU:
        case 0x1e600U:
        case 0x1e602U:
        case 0x1e606U:
        case 0x1e60aU:
        case 0x1e60eU:
        case 0x1e610U:
        case 0x1e612U:
        case 0x1e614U:
        case 0x1e616U:
        case 0x1e618U:
        case 0x1e61aU:
        case 0x1e61eU:
        case 0x1e622U:
        case 0x1e626U:
        case 0x1e62aU:
        case 0x1e62eU:
        case 0x1e630U:
        case 0x1e634U:
        case 0x1e638U:
        case 0x1e63cU:
        case 0x1e640U:
        case 0x1e642U:
        case 0x1e644U:
        case 0x1e646U:
        case 0x1e648U:
        case 0x1e64aU:
        case 0x1e64cU:
        case 0x1e650U:
        case 0x1e656U:
        case 0x1e65aU:
        case 0x1e65cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
