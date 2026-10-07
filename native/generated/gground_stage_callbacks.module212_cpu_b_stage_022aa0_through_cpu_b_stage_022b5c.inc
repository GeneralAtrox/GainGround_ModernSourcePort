
FunctionResult cpu_b_stage_022aa0(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22aa0U: { // 61000e26 bsr.w $238c8
            next = 0x22aa4U;
            const auto result = m.call(c, 416U, 0x22aa0U, 0x238c8U, 0x22aa4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22aa4U: { // 2b7c00022ac40002 move.l #$22ac4, $2(a5)
            next = 0x22aacU;
            const auto value = 0x22ac4U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22aacU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22ab0U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22ab0U: { // 41f90002407a lea.l $2407a.l, a0
            next = 0x22ab6U;
            const auto source_address = 0x2407aU;
            r.address[0] = source_address;
            break;
        }
        case 0x22ab6U: { // 7007 moveq #$7, d0
            next = 0x22ab8U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22ab8U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22abaU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22abaU: { // 51c8fffc dbra d0, $22ab8
            next = 0x22abeU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22ab8U;
            break;
        }
        case 0x22abeU: { // 61000d6c bsr.w $2382c
            next = 0x22ac2U;
            const auto result = m.call(c, 413U, 0x22abeU, 0x2382cU, 0x22ac2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22ac2U: { // 4e75 rts 
            next = 0x22ac4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22ac2U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22aa0U:
        case 0x22aa4U:
        case 0x22aacU:
        case 0x22ab0U:
        case 0x22ab6U:
        case 0x22ab8U:
        case 0x22abaU:
        case 0x22abeU:
        case 0x22ac2U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022b5c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22b5cU: { // 61000c84 bsr.w $237e2
            next = 0x22b60U;
            const auto result = m.call(c, 412U, 0x22b5cU, 0x237e2U, 0x22b60U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22b60U: { // 302d001c move.w $1c(a5), d0
            next = 0x22b64U;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x22b64U: { // e540 asl.w #$2, d0
            next = 0x22b66U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x22b66U: { // 4efb0002 jmp $22b6a(pc, d0.w)
            next = 0x22b6aU;
            next = (0x22b6aU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x22b6aU: { // 60000008 bra.w $22b74
            next = 0x22b6eU;
            if (true) { next = 0x22b74U; transfer_kind = 1U; }
            break;
        }
        case 0x22b6eU: { // 6000001e bra.w $22b8e
            next = 0x22b72U;
            if (true) { next = 0x22b8eU; transfer_kind = 1U; }
            break;
        }
        case 0x22b72U: { // 4e75 rts 
            next = 0x22b74U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22b72U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22b74U: { // 202d000a move.l $a(a5), d0
            next = 0x22b78U;
            const auto source_address = r.address[5] + 0xaU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x22b78U: { // 028001010100 andi.l #$1010100, d0
            next = 0x22b7eU;
            r.data[0] &= 0x1010100U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22b7eU: { // 6602 bne.b $22b82
            next = 0x22b80U;
            if ((r.status & 4U) == 0U) { next = 0x22b82U; transfer_kind = 1U; }
            break;
        }
        case 0x22b80U: { // 4e75 rts 
            next = 0x22b82U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22b80U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22b82U: { // 1b7c00020022 move.b #$2, $22(a5)
            next = 0x22b88U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x22U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x22b88U: { // 526d001c addq.w #$1, $1c(a5)
            next = 0x22b8cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x1cU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x22b8cU: { // 4e75 rts 
            next = 0x22b8eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22b8cU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22b8eU: { // 45f900024116 lea.l $24116.l, a2
            next = 0x22b94U;
            const auto source_address = 0x24116U;
            r.address[2] = source_address;
            break;
        }
        case 0x22b94U: { // 61000d94 bsr.w $2392a
            next = 0x22b98U;
            const auto result = m.call(c, 419U, 0x22b94U, 0x2392aU, 0x22b98U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22b98U: { // 4e75 rts 
            next = 0x22b9aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22b98U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22b5cU:
        case 0x22b60U:
        case 0x22b64U:
        case 0x22b66U:
        case 0x22b6aU:
        case 0x22b6eU:
        case 0x22b72U:
        case 0x22b74U:
        case 0x22b78U:
        case 0x22b7eU:
        case 0x22b80U:
        case 0x22b82U:
        case 0x22b88U:
        case 0x22b8cU:
        case 0x22b8eU:
        case 0x22b94U:
        case 0x22b98U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
