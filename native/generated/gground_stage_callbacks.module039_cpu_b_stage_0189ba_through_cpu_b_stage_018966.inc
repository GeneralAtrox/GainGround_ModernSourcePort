
FunctionResult cpu_b_stage_0189ba(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x189baU: { // 6100fca8 bsr.w $18664
            next = 0x189beU;
            const auto result = m.call(c, 872U, 0x189baU, 0x18664U, 0x189beU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x189beU: { // 6100fe08 bsr.w $187c8
            next = 0x189c2U;
            const auto result = m.call(c, 874U, 0x189beU, 0x187c8U, 0x189c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x189c2U: { // 4a2d003f tst.b $3f(a5)
            next = 0x189c6U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x189c6U: { // 676c beq.b $18a34
            next = 0x189c8U;
            if ((r.status & 4U) != 0U) { next = 0x18a34U; transfer_kind = 1U; }
            break;
        }
        case 0x189c8U: { // 610000e6 bsr.w $18ab0
            next = 0x189ccU;
            const auto result = m.call(c, 887U, 0x189c8U, 0x18ab0U, 0x189ccU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x189ccU: { // 6066 bra.b $18a34
            next = 0x189ceU;
            if (true) { next = 0x18a34U; transfer_kind = 1U; }
            break;
        }
        case 0x18a34U: { // 6100004e bsr.w $18a84
            next = 0x18a38U;
            const auto result = m.call(c, 886U, 0x18a34U, 0x18a84U, 0x18a38U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a38U: { // 6100fdfa bsr.w $18834
            next = 0x18a3cU;
            const auto result = m.call(c, 876U, 0x18a38U, 0x18834U, 0x18a3cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a3cU: { // 4eb900015df2 jsr $15df2.l
            next = 0x18a42U;
            const auto result = m.call(c, 282U, 0x18a3cU, 0x15df2U, 0x18a42U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a42U: { // 4e75 rts 
            next = 0x18a44U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18a42U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x189baU:
        case 0x189beU:
        case 0x189c2U:
        case 0x189c6U:
        case 0x189c8U:
        case 0x189ccU:
        case 0x18a34U:
        case 0x18a38U:
        case 0x18a3cU:
        case 0x18a42U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_018966(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x18966U: { // 0c38000a0d19 cmpi.b #$a, $d19.w
            next = 0x1896cU;
            const auto source_value = 0xaU;
            const auto destination_address = 0xd19U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1896cU: { // 6e0000ce bgt.w $18a3c
            next = 0x18970U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18a3cU; transfer_kind = 1U; }
            break;
        }
        case 0x18970U: { // 536d0048 subq.w #$1, $48(a5)
            next = 0x18974U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x48U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18974U: { // 6e00004c bgt.w $189c2
            next = 0x18978U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x189c2U; transfer_kind = 1U; }
            break;
        }
        case 0x18978U: { // 3b7c00050048 move.w #$5, $48(a5)
            next = 0x1897eU;
            const auto value = 0x5U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1897eU: { // 526d006a addq.w #$1, $6a(a5)
            next = 0x18982U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x6aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18982U: { // 302d004a move.w $4a(a5), d0
            next = 0x18986U;
            const auto source_address = r.address[5] + 0x4aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18986U: { // 206d004c movea.l $4c(a5), a0
            next = 0x1898aU;
            const auto source_address = r.address[5] + 0x4cU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1898aU: { // d0c0 adda.w d0, a0
            next = 0x1898cU;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1898cU: { // 3418 move.w (a0)+, d2
            next = 0x1898eU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1898eU: { // 2010 move.l (a0), d0
            next = 0x18990U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x18990U: { // 4eb900015e4e jsr $15e4e.l
            next = 0x18996U;
            const auto result = m.call(c, 284U, 0x18990U, 0x15e4eU, 0x18996U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18996U: { // 068000200000 addi.l #$200000, d0
            next = 0x1899cU;
            const auto source_value = 0x200000U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[0] = value;
            break;
        }
        case 0x1899cU: { // 51cafff2 dbra d2, $18990
            next = 0x189a0U;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0x18990U;
            break;
        }
        case 0x189a0U: { // 5c6d004a addq.w #$6, $4a(a5)
            next = 0x189a4U;
            const auto source_value = 0x6U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x189a4U: { // 0c6d005a004a cmpi.w #$5a, $4a(a5)
            next = 0x189aaU;
            const auto source_value = 0x5aU;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x189aaU: { // 6b16 bmi.b $189c2
            next = 0x189acU;
            if ((r.status & 8U) != 0U) { next = 0x189c2U; transfer_kind = 1U; }
            break;
        }
        case 0x189acU: { // 2b7c000189ba0002 move.l #$189ba, $2(a5)
            next = 0x189b4U;
            const auto value = 0x189baU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x189b4U: { // 1b7c000a000b move.b #$a, $b(a5)
            next = 0x189baU;
            const auto value = 0xaU;
            const auto destination_address = r.address[5] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x189c2U: { // 4a2d003f tst.b $3f(a5)
            next = 0x189c6U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x189c6U: { // 676c beq.b $18a34
            next = 0x189c8U;
            if ((r.status & 4U) != 0U) { next = 0x18a34U; transfer_kind = 1U; }
            break;
        }
        case 0x189c8U: { // 610000e6 bsr.w $18ab0
            next = 0x189ccU;
            const auto result = m.call(c, 887U, 0x189c8U, 0x18ab0U, 0x189ccU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x189ccU: { // 6066 bra.b $18a34
            next = 0x189ceU;
            if (true) { next = 0x18a34U; transfer_kind = 1U; }
            break;
        }
        case 0x18a34U: { // 6100004e bsr.w $18a84
            next = 0x18a38U;
            const auto result = m.call(c, 886U, 0x18a34U, 0x18a84U, 0x18a38U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a38U: { // 6100fdfa bsr.w $18834
            next = 0x18a3cU;
            const auto result = m.call(c, 876U, 0x18a38U, 0x18834U, 0x18a3cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a3cU: { // 4eb900015df2 jsr $15df2.l
            next = 0x18a42U;
            const auto result = m.call(c, 282U, 0x18a3cU, 0x15df2U, 0x18a42U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a42U: { // 4e75 rts 
            next = 0x18a44U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18a42U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x18966U:
        case 0x1896cU:
        case 0x18970U:
        case 0x18974U:
        case 0x18978U:
        case 0x1897eU:
        case 0x18982U:
        case 0x18986U:
        case 0x1898aU:
        case 0x1898cU:
        case 0x1898eU:
        case 0x18990U:
        case 0x18996U:
        case 0x1899cU:
        case 0x189a0U:
        case 0x189a4U:
        case 0x189aaU:
        case 0x189acU:
        case 0x189b4U:
        case 0x189c2U:
        case 0x189c6U:
        case 0x189c8U:
        case 0x189ccU:
        case 0x18a34U:
        case 0x18a38U:
        case 0x18a3cU:
        case 0x18a42U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
