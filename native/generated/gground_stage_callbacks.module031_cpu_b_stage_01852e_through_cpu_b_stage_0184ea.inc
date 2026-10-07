
FunctionResult cpu_b_stage_01852e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1852eU: { // 61000094 bsr.w $185c4
            next = 0x18532U;
            const auto result = m.call(c, 871U, 0x1852eU, 0x185c4U, 0x18532U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18532U: { // 61000284 bsr.w $187b8
            next = 0x18536U;
            const auto result = m.call(c, 873U, 0x18532U, 0x187b8U, 0x18536U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18536U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1853aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1853aU: { // 6778 beq.b $185b4
            next = 0x1853cU;
            if ((r.status & 4U) != 0U) { next = 0x185b4U; transfer_kind = 1U; }
            break;
        }
        case 0x1853cU: { // 6100031a bsr.w $18858
            next = 0x18540U;
            const auto result = m.call(c, 877U, 0x1853cU, 0x18858U, 0x18540U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x18540U: { // 6072 bra.b $185b4
            next = 0x18542U;
            if (true) { next = 0x185b4U; transfer_kind = 1U; }
            break;
        }
        case 0x185b4U: { // 61000268 bsr.w $1881e
            next = 0x185b8U;
            const auto result = m.call(c, 875U, 0x185b4U, 0x1881eU, 0x185b8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185b8U: { // 6100027a bsr.w $18834
            next = 0x185bcU;
            const auto result = m.call(c, 876U, 0x185b8U, 0x18834U, 0x185bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185bcU: { // 4eb900015df2 jsr $15df2.l
            next = 0x185c2U;
            const auto result = m.call(c, 282U, 0x185bcU, 0x15df2U, 0x185c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185c2U: { // 4e75 rts 
            next = 0x185c4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x185c2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1852eU:
        case 0x18532U:
        case 0x18536U:
        case 0x1853aU:
        case 0x1853cU:
        case 0x18540U:
        case 0x185b4U:
        case 0x185b8U:
        case 0x185bcU:
        case 0x185c2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_0184ea_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x184eaU: { // 536d0048 subq.w #$1, $48(a5)
            next = 0x184eeU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x48U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x184eeU: { // 6e000046 bgt.w $18536
            next = 0x184f2U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18536U; transfer_kind = 1U; }
            break;
        }
        case 0x184f2U: { // 3b7c00050048 move.w #$5, $48(a5)
            next = 0x184f8U;
            const auto value = 0x5U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x184f8U: { // 526d006a addq.w #$1, $6a(a5)
            next = 0x184fcU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x6aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x184fcU: { // 302d004a move.w $4a(a5), d0
            next = 0x18500U;
            const auto source_address = r.address[5] + 0x4aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18500U: { // 206d004c movea.l $4c(a5), a0
            next = 0x18504U;
            const auto source_address = r.address[5] + 0x4cU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x18504U: { // d0c0 adda.w d0, a0
            next = 0x18506U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x18506U: { // 3418 move.w (a0)+, d2
            next = 0x18508U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18508U: { // 2010 move.l (a0), d0
            next = 0x1850aU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x18510U: { // 068000200000 addi.l #$200000, d0
            next = 0x18516U;
            const auto source_value = 0x200000U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[0] = value;
            break;
        }
        case 0x18516U: { // 51cafff2 dbra d2, $1850a
            next = 0x1851aU;
            m.dw(2U, r.data[2] - 1U);
            if ((r.data[2] & 0xffffU) != 0xffffU) next = 0x1850aU;
            break;
        }
        case 0x1851aU: { // 5c6d004a addq.w #$6, $4a(a5)
            next = 0x1851eU;
            const auto source_value = 0x6U;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1851eU: { // 0c6d005a004a cmpi.w #$5a, $4a(a5)
            next = 0x18524U;
            const auto source_value = 0x5aU;
            const auto destination_address = r.address[5] + 0x4aU;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18524U: { // 6b10 bmi.b $18536
            next = 0x18526U;
            if ((r.status & 8U) != 0U) { next = 0x18536U; transfer_kind = 1U; }
            break;
        }
        case 0x18526U: { // 2b7c0001852e0002 move.l #$1852e, $2(a5)
            next = 0x1852eU;
            const auto value = 0x1852eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x18536U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1853aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1853aU: { // 6778 beq.b $185b4
            next = 0x1853cU;
            if ((r.status & 4U) != 0U) { next = 0x185b4U; transfer_kind = 1U; }
            break;
        }
        case 0x18540U: { // 6072 bra.b $185b4
            next = 0x18542U;
            if (true) { next = 0x185b4U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_0184ea(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1850aU: { // 4eb900015e4e jsr $15e4e.l
            next = 0x18510U;
            const auto result = m.call(c, 284U, 0x1850aU, 0x15e4eU, 0x18510U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1853cU: { // 6100031a bsr.w $18858
            next = 0x18540U;
            const auto result = m.call(c, 877U, 0x1853cU, 0x18858U, 0x18540U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185b4U: { // 61000268 bsr.w $1881e
            next = 0x185b8U;
            const auto result = m.call(c, 875U, 0x185b4U, 0x1881eU, 0x185b8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185b8U: { // 6100027a bsr.w $18834
            next = 0x185bcU;
            const auto result = m.call(c, 876U, 0x185b8U, 0x18834U, 0x185bcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185bcU: { // 4eb900015df2 jsr $15df2.l
            next = 0x185c2U;
            const auto result = m.call(c, 282U, 0x185bcU, 0x15df2U, 0x185c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x185c2U: { // 4e75 rts 
            next = 0x185c4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x185c2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_0184ea_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x184eaU:
        case 0x184eeU:
        case 0x184f2U:
        case 0x184f8U:
        case 0x184fcU:
        case 0x18500U:
        case 0x18504U:
        case 0x18506U:
        case 0x18508U:
        case 0x1850aU:
        case 0x18510U:
        case 0x18516U:
        case 0x1851aU:
        case 0x1851eU:
        case 0x18524U:
        case 0x18526U:
        case 0x18536U:
        case 0x1853aU:
        case 0x1853cU:
        case 0x18540U:
        case 0x185b4U:
        case 0x185b8U:
        case 0x185bcU:
        case 0x185c2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
