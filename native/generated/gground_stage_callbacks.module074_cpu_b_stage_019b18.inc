
FunctionResult cpu_b_stage_019b18(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x19b18U: { // 536d0070 subq.w #$1, $70(a5)
            next = 0x19b1cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19b1cU: { // 086d00000068 bchg.b #$0, $68(a5)
            next = 0x19b22U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x19b22U: { // 6636 bne.b $19b5a
            next = 0x19b24U;
            if ((r.status & 4U) == 0U) { next = 0x19b5aU; transfer_kind = 1U; }
            break;
        }
        case 0x19b24U: { // 302d0064 move.w $64(a5), d0
            next = 0x19b28U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19b28U: { // 5240 addq.w #$1, d0
            next = 0x19b2aU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19b2aU: { // b06d006e cmp.w $6e(a5), d0
            next = 0x19b2eU;
            const auto source_address = r.address[5] + 0x6eU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19b2eU: { // 6b0c bmi.b $19b3c
            next = 0x19b30U;
            if ((r.status & 8U) != 0U) { next = 0x19b3cU; transfer_kind = 1U; }
            break;
        }
        case 0x19b30U: { // 7000 moveq #$0, d0
            next = 0x19b32U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x19b32U: { // 4a6d0070 tst.w $70(a5)
            next = 0x19b36U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x19b36U: { // 6e04 bgt.b $19b3c
            next = 0x19b38U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19b3cU; transfer_kind = 1U; }
            break;
        }
        case 0x19b38U: { // 4255 clr.w (a5)
            next = 0x19b3aU;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19b3aU: { // 4e75 rts 
            next = 0x19b3cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19b3aU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x19b3cU: { // 3b400064 move.w d0, $64(a5)
            next = 0x19b40U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19b40U: { // d040 add.w d0, d0
            next = 0x19b42U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19b42U: { // 3200 move.w d0, d1
            next = 0x19b44U;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19b44U: { // d040 add.w d0, d0
            next = 0x19b46U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19b46U: { // d041 add.w d1, d0
            next = 0x19b48U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19b48U: { // 41fa1946 lea.l $1b490(pc), a0
            next = 0x19b4cU;
            const auto source_address = 0x1b490U;
            r.address[0] = source_address;
            break;
        }
        case 0x19b4cU: { // d0c0 adda.w d0, a0
            next = 0x19b4eU;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x19b4eU: { // 3b580006 move.w (a0)+, $6(a5)
            next = 0x19b52U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19b52U: { // 3b580008 move.w (a0)+, $8(a5)
            next = 0x19b56U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19b56U: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x19b5aU;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19b5aU: { // 4eb900015df2 jsr $15df2.l
            next = 0x19b60U;
            const auto result = m.call(c, 282U, 0x19b5aU, 0x15df2U, 0x19b60U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19b60U: { // 4e75 rts 
            next = 0x19b62U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x19b60U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x19b18U:
        case 0x19b1cU:
        case 0x19b22U:
        case 0x19b24U:
        case 0x19b28U:
        case 0x19b2aU:
        case 0x19b2eU:
        case 0x19b30U:
        case 0x19b32U:
        case 0x19b36U:
        case 0x19b38U:
        case 0x19b3aU:
        case 0x19b3cU:
        case 0x19b40U:
        case 0x19b42U:
        case 0x19b44U:
        case 0x19b46U:
        case 0x19b48U:
        case 0x19b4cU:
        case 0x19b4eU:
        case 0x19b52U:
        case 0x19b56U:
        case 0x19b5aU:
        case 0x19b60U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
