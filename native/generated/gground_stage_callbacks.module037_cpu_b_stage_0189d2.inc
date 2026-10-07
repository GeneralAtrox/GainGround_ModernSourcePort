
FunctionResult cpu_b_stage_0189d2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x189d2U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x189d6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x189d6U: { // 6e28 bgt.b $18a00
            next = 0x189d8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18a00U; transfer_kind = 1U; }
            break;
        }
        case 0x189d8U: { // 303c0014 move.w #$14, d0
            next = 0x189dcU;
            const auto value = 0x14U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x189dcU: { // 4eb900016ff8 jsr $16ff8.l
            next = 0x189e2U;
            const auto result = m.call(c, 308U, 0x189dcU, 0x16ff8U, 0x189e2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x189e2U: { // 3b7c00080068 move.w #$8, $68(a5)
            next = 0x189e8U;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x189e8U: { // 302d0066 move.w $66(a5), d0
            next = 0x189ecU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x189ecU: { // 5440 addq.w #$2, d0
            next = 0x189eeU;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x189eeU: { // 02400006 andi.w #$6, d0
            next = 0x189f2U;
            const auto source_value = 0x6U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x189f2U: { // 3b400066 move.w d0, $66(a5)
            next = 0x189f6U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x189f6U: { // 41fa21ec lea.l $1abe4(pc), a0
            next = 0x189faU;
            const auto source_address = 0x1abe4U;
            r.address[0] = source_address;
            break;
        }
        case 0x189faU: { // 3b7000000064 move.w (a0, d0.w), $64(a5)
            next = 0x18a00U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x64U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18a00U: { // 536d0070 subq.w #$1, $70(a5)
            next = 0x18a04U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x70U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x18a04U: { // 302d0070 move.w $70(a5), d0
            next = 0x18a08U;
            const auto source_address = r.address[5] + 0x70U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18a08U: { // 0c40003c cmpi.w #$3c, d0
            next = 0x18a0cU;
            const auto source_value = 0x3cU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18a0cU: { // 6e26 bgt.b $18a34
            next = 0x18a0eU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18a34U; transfer_kind = 1U; }
            break;
        }
        case 0x18a0eU: { // 660a bne.b $18a1a
            next = 0x18a10U;
            if ((r.status & 4U) == 0U) { next = 0x18a1aU; transfer_kind = 1U; }
            break;
        }
        case 0x18a10U: { // 43fa22f8 lea.l $1ad0a(pc), a1
            next = 0x18a14U;
            const auto source_address = 0x1ad0aU;
            r.address[1] = source_address;
            break;
        }
        case 0x18a14U: { // 61000120 bsr.w $18b36
            next = 0x18a18U;
            const auto result = m.call(c, 888U, 0x18a14U, 0x18b36U, 0x18a18U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a18U: { // 4e75 rts 
            next = 0x18a1aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18a18U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x18a1aU: { // 0c40001e cmpi.w #$1e, d0
            next = 0x18a1eU;
            const auto source_value = 0x1eU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18a1eU: { // 6612 bne.b $18a32
            next = 0x18a20U;
            if ((r.status & 4U) == 0U) { next = 0x18a32U; transfer_kind = 1U; }
            break;
        }
        case 0x18a20U: { // 43fa240c lea.l $1ae2e(pc), a1
            next = 0x18a24U;
            const auto source_address = 0x1ae2eU;
            r.address[1] = source_address;
            break;
        }
        case 0x18a24U: { // 61000110 bsr.w $18b36
            next = 0x18a28U;
            const auto result = m.call(c, 888U, 0x18a24U, 0x18b36U, 0x18a28U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a28U: { // 4255 clr.w (a5)
            next = 0x18a2aU;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x18a2aU: { // 6100fede bsr.w $1890a
            next = 0x18a2eU;
            const auto result = m.call(c, 880U, 0x18a2aU, 0x1890aU, 0x18a2eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18a2eU: { // 42380d1b clr.b $d1b.w
            next = 0x18a32U;
            const auto destination_address = 0xd1bU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x18a32U: { // 4e75 rts 
            next = 0x18a34U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x18a32U, r.program_counter)) return *event;
            return result;
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
        case 0x189d2U:
        case 0x189d6U:
        case 0x189d8U:
        case 0x189dcU:
        case 0x189e2U:
        case 0x189e8U:
        case 0x189ecU:
        case 0x189eeU:
        case 0x189f2U:
        case 0x189f6U:
        case 0x189faU:
        case 0x18a00U:
        case 0x18a04U:
        case 0x18a08U:
        case 0x18a0cU:
        case 0x18a0eU:
        case 0x18a10U:
        case 0x18a14U:
        case 0x18a18U:
        case 0x18a1aU:
        case 0x18a1eU:
        case 0x18a20U:
        case 0x18a24U:
        case 0x18a28U:
        case 0x18a2aU:
        case 0x18a2eU:
        case 0x18a32U:
        case 0x18a34U:
        case 0x18a38U:
        case 0x18a3cU:
        case 0x18a42U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
