
FunctionResult cpu_b_stage_013e38(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x13e38U: { // 41fa1a98 lea.l $158d2(pc), a0
            next = 0x13e3cU;
            const auto source_address = 0x158d2U;
            r.address[0] = source_address;
            break;
        }
        case 0x13e3cU: { // 600e bra.b $13e4c
            next = 0x13e3eU;
            if (true) { next = 0x13e4cU; transfer_kind = 1U; }
            break;
        }
        case 0x13e4cU: { // 3e18 move.w (a0)+, d7
            next = 0x13e4eU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x13e4eU: { // 6b3a bmi.b $13e8a
            next = 0x13e50U;
            if ((r.status & 8U) != 0U) { next = 0x13e8aU; transfer_kind = 1U; }
            break;
        }
        case 0x13e50U: { // 1cbc0080 move.b #$80, (a6)
            next = 0x13e54U;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x13e54U: { // 422e0001 clr.b $1(a6)
            next = 0x13e58U;
            const auto destination_address = r.address[6] + 0x1U;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x13e58U: { // 2d7c00015df20002 move.l #$15df2, $2(a6)
            next = 0x13e60U;
            const auto value = 0x15df2U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x13e60U: { // 426e001e clr.w $1e(a6)
            next = 0x13e64U;
            const auto destination_address = r.address[6] + 0x1eU;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x13e64U: { // 426e0020 clr.w $20(a6)
            next = 0x13e68U;
            const auto destination_address = r.address[6] + 0x20U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x13e68U: { // 426e0022 clr.w $22(a6)
            next = 0x13e6cU;
            const auto destination_address = r.address[6] + 0x22U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x13e6cU: { // 422e003f clr.b $3f(a6)
            next = 0x13e70U;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x13e70U: { // 426e0042 clr.w $42(a6)
            next = 0x13e74U;
            const auto destination_address = r.address[6] + 0x42U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x13e74U: { // 426e0044 clr.w $44(a6)
            next = 0x13e78U;
            const auto destination_address = r.address[6] + 0x44U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x13e78U: { // 3d7c80000046 move.w #$8000, $46(a6)
            next = 0x13e7eU;
            const auto value = 0x8000U;
            const auto destination_address = r.address[6] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x13e7eU: { // 2258 movea.l (a0)+, a1
            next = 0x13e80U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            r.address[1] = value;
            break;
        }
        case 0x13e80U: { // 4e91 jsr (a1)
            next = 0x13e82U;
            const auto target_address = r.address[1];
            const auto result = m.indirect_call(c, 0x13e80U, target_address, 0x13e82U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x13e82U: { // 4dee0080 lea.l $80(a6), a6
            next = 0x13e86U;
            const auto source_address = r.address[6] + 0x80U;
            r.address[6] = source_address;
            break;
        }
        case 0x13e86U: { // 51cfffc8 dbra d7, $13e50
            next = 0x13e8aU;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x13e50U;
            break;
        }
        case 0x13e8aU: { // 4e75 rts 
            next = 0x13e8cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x13e8aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x13e38U:
        case 0x13e3cU:
        case 0x13e4cU:
        case 0x13e4eU:
        case 0x13e50U:
        case 0x13e54U:
        case 0x13e58U:
        case 0x13e60U:
        case 0x13e64U:
        case 0x13e68U:
        case 0x13e6cU:
        case 0x13e70U:
        case 0x13e74U:
        case 0x13e78U:
        case 0x13e7eU:
        case 0x13e80U:
        case 0x13e82U:
        case 0x13e86U:
        case 0x13e8aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_013e8c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x13e8cU: { // 3d7c00080046 move.w #$8, $46(a6)
            next = 0x13e92U;
            const auto value = 0x8U;
            const auto destination_address = r.address[6] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x13e92U: { // 601a bra.b $13eae
            next = 0x13e94U;
            if (true) { next = 0x13eaeU; transfer_kind = 1U; }
            break;
        }
        case 0x13eaeU: { // 2d580002 move.l (a0)+, $2(a6)
            next = 0x13eb2U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x13e8cU:
        case 0x13e92U:
        case 0x13eaeU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
