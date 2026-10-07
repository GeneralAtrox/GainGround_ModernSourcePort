
FunctionResult cpu_b_stage_018c28(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18c30U: { // 6100fcd8 bsr.w $1890a
            next = 0x18c34U;
            const auto result = m.call(c, 880U, 0x18c30U, 0x1890aU, 0x18c34U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c38U: { // 4e75 rts 
            next = 0x18c3aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18c38U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18c44U: { // 4eb900016ff8 jsr $16ff8.l
            next = 0x18c4aU;
            const auto result = m.call(c, 308U, 0x18c44U, 0x16ff8U, 0x18c4aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c76U: { // 610004ee bsr.w $19166
            next = 0x18c7aU;
            const auto result = m.call(c, 900U, 0x18c76U, 0x19166U, 0x18c7aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c7aU: { // 6100041c bsr.w $19098
            next = 0x18c7eU;
            const auto result = m.call(c, 899U, 0x18c7aU, 0x19098U, 0x18c7eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c82U: { // 61000716 bsr.w $1939a
            next = 0x18c86U;
            const auto result = m.call(c, 904U, 0x18c82U, 0x1939aU, 0x18c86U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18c86U: { // 4e75 rts 
            next = 0x18c88U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18c86U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_018c28_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18c28U:
        case 0x18c2cU:
        case 0x18c2eU:
        case 0x18c30U:
        case 0x18c34U:
        case 0x18c38U:
        case 0x18c3aU:
        case 0x18c3eU:
        case 0x18c40U:
        case 0x18c44U:
        case 0x18c4aU:
        case 0x18c50U:
        case 0x18c54U:
        case 0x18c5aU:
        case 0x18c5eU:
        case 0x18c62U:
        case 0x18c64U:
        case 0x18c66U:
        case 0x18c6aU:
        case 0x18c6cU:
        case 0x18c70U:
        case 0x18c76U:
        case 0x18c7aU:
        case 0x18c7eU:
        case 0x18c82U:
        case 0x18c86U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_019474_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19474U: { // 536d0070 subq.w #$1, $70(a5)
            next = 0x19478U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19478U: { // 086d00000068 bchg.b #$0, $68(a5)
            next = 0x1947eU;
            const auto destination_address = r.address[5] + 0x68U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x1947eU: { // 6636 bne.b $194b6
            next = 0x19480U;
            if ((r.status & 4U) == 0U) { next = 0x194b6U; transfer_kind = 1U; }
            break;
        }
        case 0x19480U: { // 302d0064 move.w $64(a5), d0
            next = 0x19484U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19484U: { // 5240 addq.w #$1, d0
            next = 0x19486U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19486U: { // b06d006e cmp.w $6e(a5), d0
            next = 0x1948aU;
            const auto source_address = r.address[5] + 0x6eU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1948aU: { // 6b0c bmi.b $19498
            next = 0x1948cU;
            if ((r.status & 8U) != 0U) { next = 0x19498U; transfer_kind = 1U; }
            break;
        }
        case 0x1948cU: { // 7000 moveq #$0, d0
            next = 0x1948eU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1948eU: { // 4a6d0070 tst.w $70(a5)
            next = 0x19492U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x19492U: { // 6e04 bgt.b $19498
            next = 0x19494U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19498U; transfer_kind = 1U; }
            break;
        }
        case 0x19494U: { // 4255 clr.w (a5)
            next = 0x19496U;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19498U: { // 3b400064 move.w d0, $64(a5)
            next = 0x1949cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1949cU: { // d040 add.w d0, d0
            next = 0x1949eU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1949eU: { // 3200 move.w d0, d1
            next = 0x194a0U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x194a0U: { // d040 add.w d0, d0
            next = 0x194a2U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x194a2U: { // d041 add.w d1, d0
            next = 0x194a4U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x194a4U: { // 41fa1c02 lea.l $1b0a8(pc), a0
            next = 0x194a8U;
            const auto source_address = 0x1b0a8U;
            r.address[0] = source_address;
            break;
        }
        case 0x194a8U: { // d0c0 adda.w d0, a0
            next = 0x194aaU;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x194aaU: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x194aeU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x194aeU: { // 3b580008 move.w (a0)+, $8(a5)
            next = 0x194b2U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x194b2U: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x194b6U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_019474(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19496U: { // 4e75 rts 
            next = 0x19498U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19496U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x194b6U: { // 4eb900015df2 jsr $15df2.l
            next = 0x194bcU;
            const auto result = m.call(c, 282U, 0x194b6U, 0x15df2U, 0x194bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x194bcU: { // 4e75 rts 
            next = 0x194beU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x194bcU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_019474_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19474U:
        case 0x19478U:
        case 0x1947eU:
        case 0x19480U:
        case 0x19484U:
        case 0x19486U:
        case 0x1948aU:
        case 0x1948cU:
        case 0x1948eU:
        case 0x19492U:
        case 0x19494U:
        case 0x19496U:
        case 0x19498U:
        case 0x1949cU:
        case 0x1949eU:
        case 0x194a0U:
        case 0x194a2U:
        case 0x194a4U:
        case 0x194a8U:
        case 0x194aaU:
        case 0x194aeU:
        case 0x194b2U:
        case 0x194b6U:
        case 0x194bcU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
