
FunctionResult cpu_b_stage_01eaf0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1eaf0U: { // 610000fe bsr.w $1ebf0
            next = 0x1eaf4U;
            const auto result = m.call(c, 363U, 0x1eaf0U, 0x1ebf0U, 0x1eaf4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1eaf4U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1eaf8U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1eaf8U: { // 662c bne.b $1eb26
            next = 0x1eafaU;
            if ((r.status & 4U) == 0U) { next = 0x1eb26U; transfer_kind = 1U; }
            break;
        }
        case 0x1eafaU: { // 7000 moveq #$0, d0
            next = 0x1eafcU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1eafcU: { // 102d0058 move.b $58(a5), d0
            next = 0x1eb00U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1eb00U: { // 0c400010 cmpi.w #$10, d0
            next = 0x1eb04U;
            const auto source_value = 0x10U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1eb04U: { // 6d06 blt.b $1eb0c
            next = 0x1eb06U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1eb0cU; transfer_kind = 1U; }
            break;
        }
        case 0x1eb06U: { // 04400020 subi.w #$20, d0
            next = 0x1eb0aU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1eb0aU: { // 4440 neg.w d0
            next = 0x1eb0cU;
            const auto old = r.data[0];
            const auto value = m.sub(0U, old, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1eb0cU: { // 0c40000a cmpi.w #$a, d0
            next = 0x1eb10U;
            const auto source_value = 0xaU;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1eb10U: { // 6f04 ble.b $1eb16
            next = 0x1eb12U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1eb16U; transfer_kind = 1U; }
            break;
        }
        case 0x1eb12U: { // 303c000a move.w #$a, d0
            next = 0x1eb16U;
            const auto value = 0xaU;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eb16U: { // e248 lsr.w #$1, d0
            next = 0x1eb18U;
            m.shift_word(0U, 1U, false, false);
            break;
        }
        case 0x1eb18U: { // 7200 moveq #$0, d1
            next = 0x1eb1aU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1eb1aU: { // 9247 sub.w d7, d1
            next = 0x1eb1cU;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1eb1cU: { // d247 add.w d7, d1
            next = 0x1eb1eU;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1eb1eU: { // 51c8fffc dbra d0, $1eb1c
            next = 0x1eb22U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1eb1cU;
            break;
        }
        case 0x1eb22U: { // 6000f6ce bra.w $1e1f2
            next = 0x1eb26U;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        case 0x1eb26U: { // 7000 moveq #$0, d0
            next = 0x1eb28U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1eb28U: { // 102d0058 move.b $58(a5), d0
            next = 0x1eb2cU;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1eb2cU: { // 0c400014 cmpi.w #$14, d0
            next = 0x1eb30U;
            const auto source_value = 0x14U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1eb30U: { // 6f04 ble.b $1eb36
            next = 0x1eb32U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1eb36U; transfer_kind = 1U; }
            break;
        }
        case 0x1eb32U: { // 303c0014 move.w #$14, d0
            next = 0x1eb36U;
            const auto value = 0x14U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1eb36U: { // d07c000c add.w #$c, d0
            next = 0x1eb3aU;
            const auto source_value = 0xcU;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1eb3aU: { // e248 lsr.w #$1, d0
            next = 0x1eb3cU;
            m.shift_word(0U, 1U, false, false);
            break;
        }
        case 0x1eb3cU: { // 7200 moveq #$0, d1
            next = 0x1eb3eU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1eb3eU: { // 9247 sub.w d7, d1
            next = 0x1eb40U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1eb40U: { // d247 add.w d7, d1
            next = 0x1eb42U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1eb42U: { // 51c8fffc dbra d0, $1eb40
            next = 0x1eb46U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x1eb40U;
            break;
        }
        case 0x1eb46U: { // 6000f6aa bra.w $1e1f2
            next = 0x1eb4aU;
            if (true) { next = 0x1e1f2U; transfer_kind = 1U; }
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1eaf0U:
        case 0x1eaf4U:
        case 0x1eaf8U:
        case 0x1eafaU:
        case 0x1eafcU:
        case 0x1eb00U:
        case 0x1eb04U:
        case 0x1eb06U:
        case 0x1eb0aU:
        case 0x1eb0cU:
        case 0x1eb10U:
        case 0x1eb12U:
        case 0x1eb16U:
        case 0x1eb18U:
        case 0x1eb1aU:
        case 0x1eb1cU:
        case 0x1eb1eU:
        case 0x1eb22U:
        case 0x1eb26U:
        case 0x1eb28U:
        case 0x1eb2cU:
        case 0x1eb30U:
        case 0x1eb32U:
        case 0x1eb36U:
        case 0x1eb3aU:
        case 0x1eb3cU:
        case 0x1eb3eU:
        case 0x1eb40U:
        case 0x1eb42U:
        case 0x1eb46U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
