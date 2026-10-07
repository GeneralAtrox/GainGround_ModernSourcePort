
FunctionResult cpu_b_stage_022ac4(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22ac4U: { // 61000d1c bsr.w $237e2
            next = 0x22ac8U;
            const auto result = m.call(c, 412U, 0x22ac4U, 0x237e2U, 0x22ac8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22ac8U: { // 302d001c move.w $1c(a5), d0
            next = 0x22accU;
            const auto source_address = r.address[5] + 0x1cU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x22accU: { // e540 asl.w #$2, d0
            next = 0x22aceU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x22aceU: { // 4efb0002 jmp $22ad2(pc, d0.w)
            next = 0x22ad2U;
            next = (0x22ad2U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x22ad2U: { // 60000006 bra.w $22ada
            next = 0x22ad6U;
            if (true) { next = 0x22adaU; transfer_kind = 1U; }
            break;
        }
        case 0x22ad6U: { // 6000000c bra.w $22ae4
            next = 0x22adaU;
            if (true) { next = 0x22ae4U; transfer_kind = 1U; }
            break;
        }
        case 0x22adaU: { // 0c6d012c0008 cmpi.w #$12c, $8(a5)
            next = 0x22ae0U;
            const auto source_value = 0x12cU;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x22ae0U: { // 6e02 bgt.b $22ae4
            next = 0x22ae2U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x22ae4U; transfer_kind = 1U; }
            break;
        }
        case 0x22ae2U: { // 4e75 rts 
            next = 0x22ae4U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22ae2U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x22ae4U: { // 45f90002408a lea.l $2408a.l, a2
            next = 0x22aeaU;
            const auto source_address = 0x2408aU;
            r.address[2] = source_address;
            break;
        }
        case 0x22aeaU: { // 61000e3e bsr.w $2392a
            next = 0x22aeeU;
            const auto result = m.call(c, 419U, 0x22aeaU, 0x2392aU, 0x22aeeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22aeeU: { // 4e75 rts 
            next = 0x22af0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22aeeU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22ac4U:
        case 0x22ac8U:
        case 0x22accU:
        case 0x22aceU:
        case 0x22ad2U:
        case 0x22ad6U:
        case 0x22adaU:
        case 0x22ae0U:
        case 0x22ae2U:
        case 0x22ae4U:
        case 0x22aeaU:
        case 0x22aeeU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022a58(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22a58U: { // 61000e6e bsr.w $238c8
            next = 0x22a5cU;
            const auto result = m.call(c, 416U, 0x22a58U, 0x238c8U, 0x22a5cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22a5cU: { // 2b7c00022ac40002 move.l #$22ac4, $2(a5)
            next = 0x22a64U;
            const auto value = 0x22ac4U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22a64U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22a68U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22a68U: { // 41f90002405a lea.l $2405a.l, a0
            next = 0x22a6eU;
            const auto source_address = 0x2405aU;
            r.address[0] = source_address;
            break;
        }
        case 0x22a6eU: { // 7007 moveq #$7, d0
            next = 0x22a70U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22a70U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22a72U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22a72U: { // 51c8fffc dbra d0, $22a70
            next = 0x22a76U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22a70U;
            break;
        }
        case 0x22a76U: { // 61000db4 bsr.w $2382c
            next = 0x22a7aU;
            const auto result = m.call(c, 413U, 0x22a76U, 0x2382cU, 0x22a7aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22a7aU: { // 4e75 rts 
            next = 0x22a7cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22a7aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22a58U:
        case 0x22a5cU:
        case 0x22a64U:
        case 0x22a68U:
        case 0x22a6eU:
        case 0x22a70U:
        case 0x22a72U:
        case 0x22a76U:
        case 0x22a7aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022a7c(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22a7cU: { // 61000e4a bsr.w $238c8
            next = 0x22a80U;
            const auto result = m.call(c, 416U, 0x22a7cU, 0x238c8U, 0x22a80U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22a80U: { // 2b7c00022ac40002 move.l #$22ac4, $2(a5)
            next = 0x22a88U;
            const auto value = 0x22ac4U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22a88U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22a8cU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22a8cU: { // 41f90002406a lea.l $2406a.l, a0
            next = 0x22a92U;
            const auto source_address = 0x2406aU;
            r.address[0] = source_address;
            break;
        }
        case 0x22a92U: { // 7007 moveq #$7, d0
            next = 0x22a94U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22a94U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22a96U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22a96U: { // 51c8fffc dbra d0, $22a94
            next = 0x22a9aU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22a94U;
            break;
        }
        case 0x22a9aU: { // 61000d90 bsr.w $2382c
            next = 0x22a9eU;
            const auto result = m.call(c, 413U, 0x22a9aU, 0x2382cU, 0x22a9eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22a9eU: { // 4e75 rts 
            next = 0x22aa0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22a9eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22a7cU:
        case 0x22a80U:
        case 0x22a88U:
        case 0x22a8cU:
        case 0x22a92U:
        case 0x22a94U:
        case 0x22a96U:
        case 0x22a9aU:
        case 0x22a9eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
