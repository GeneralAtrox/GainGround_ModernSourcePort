
static bool cpu_b_stage_01e660_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1e664U: { // 3207 move.w d7, d1
            next = 0x1e666U;
            const auto value = r.data[7];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e666U: { // eb49 lsl.w #$5, d1
            next = 0x1e668U;
            m.shift_word(1U, 5U, true, false);
            break;
        }
        case 0x1e668U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1e66eU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e66eU: { // 6708 beq.b $1e678
            next = 0x1e670U;
            if ((r.status & 4U) != 0U) { next = 0x1e678U; transfer_kind = 1U; }
            break;
        }
        case 0x1e670U: { // 082d00000041 btst.b #$0, $41(a5)
            next = 0x1e676U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1e676U: { // 6728 beq.b $1e6a0
            next = 0x1e678U;
            if ((r.status & 4U) != 0U) { next = 0x1e6a0U; transfer_kind = 1U; }
            break;
        }
        case 0x1e678U: { // 302d005c move.w $5c(a5), d0
            next = 0x1e67cU;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e67cU: { // 06400080 addi.w #$80, d0
            next = 0x1e680U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e680U: { // 02400700 andi.w #$700, d0
            next = 0x1e684U;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e684U: { // e048 lsr.w #$8, d0
            next = 0x1e686U;
            m.shift_word(0U, 8U, false, false);
            break;
        }
        case 0x1e686U: { // 5340 subq.w #$1, d0
            next = 0x1e688U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e688U: { // 6b0a bmi.b $1e694
            next = 0x1e68aU;
            if ((r.status & 8U) != 0U) { next = 0x1e694U; transfer_kind = 1U; }
            break;
        }
        case 0x1e68aU: { // 3407 move.w d7, d2
            next = 0x1e68cU;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e68cU: { // e34a lsl.w #$1, d2
            next = 0x1e68eU;
            m.shift_word(2U, 1U, true, false);
            break;
        }
        case 0x1e68eU: { // d242 add.w d2, d1
            next = 0x1e690U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e690U: { // 51c8fffc dbra d0, $1e68e
            next = 0x1e694U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e68eU;
            break;
        }
        case 0x1e694U: { // 526d0056 addq.w #$1, $56(a5)
            next = 0x1e698U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x56U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1e698U: { // 302d0056 move.w $56(a5), d0
            next = 0x1e69cU;
            const auto source_address = r.address[5] + 0x56U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e69cU: { // 6000fb54 bra.w $1e1f2
            next = 0x1e6a0U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e6a0U: { // 302d005c move.w $5c(a5), d0
            next = 0x1e6a4U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e6a4U: { // 06400080 addi.w #$80, d0
            next = 0x1e6a8U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e6a8U: { // 02400700 andi.w #$700, d0
            next = 0x1e6acU;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e6acU: { // e048 lsr.w #$8, d0
            next = 0x1e6aeU;
            m.shift_word(0U, 8U, false, false);
            break;
        }
        case 0x1e6aeU: { // 5340 subq.w #$1, d0
            next = 0x1e6b0U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1e6b0U: { // 6b0a bmi.b $1e6bc
            next = 0x1e6b2U;
            if ((r.status & 8U) != 0U) { next = 0x1e6bcU; transfer_kind = 1U; }
            break;
        }
        case 0x1e6b2U: { // 3407 move.w d7, d2
            next = 0x1e6b4U;
            const auto value = r.data[7];
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1e6b4U: { // d442 add.w d2, d2
            next = 0x1e6b6U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1e6b6U: { // d242 add.w d2, d1
            next = 0x1e6b8U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e6b8U: { // 51c8fffc dbra d0, $1e6b6
            next = 0x1e6bcU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1e6b6U;
            break;
        }
        case 0x1e6bcU: { // 0c6d00030074 cmpi.w #$3, $74(a5)
            next = 0x1e6c2U;
            const auto source_value = 0x3U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1e6c2U: { // 6e00fb2e bgt.w $1e1f2
            next = 0x1e6c6U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1e6c6U: { // d247 add.w d7, d1
            next = 0x1e6c8U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1e6c8U: { // 6000fb28 bra.w $1e1f2
            next = 0x1e6ccU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01e660(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1e660U: { // 6100058e bsr.w $1ebf0
            next = 0x1e664U;
            const auto result = m.call(c, 363U, 0x1e660U, 0x1ebf0U, 0x1e664U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        default:
            if (!(cpu_b_stage_01e660_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1e660U:
        case 0x1e664U:
        case 0x1e666U:
        case 0x1e668U:
        case 0x1e66eU:
        case 0x1e670U:
        case 0x1e676U:
        case 0x1e678U:
        case 0x1e67cU:
        case 0x1e680U:
        case 0x1e684U:
        case 0x1e686U:
        case 0x1e688U:
        case 0x1e68aU:
        case 0x1e68cU:
        case 0x1e68eU:
        case 0x1e690U:
        case 0x1e694U:
        case 0x1e698U:
        case 0x1e69cU:
        case 0x1e6a0U:
        case 0x1e6a4U:
        case 0x1e6a8U:
        case 0x1e6acU:
        case 0x1e6aeU:
        case 0x1e6b0U:
        case 0x1e6b2U:
        case 0x1e6b4U:
        case 0x1e6b6U:
        case 0x1e6b8U:
        case 0x1e6bcU:
        case 0x1e6c2U:
        case 0x1e6c6U:
        case 0x1e6c8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
