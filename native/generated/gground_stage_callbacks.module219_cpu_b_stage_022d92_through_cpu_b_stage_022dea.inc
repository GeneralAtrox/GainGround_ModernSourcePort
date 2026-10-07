
FunctionResult cpu_b_stage_022d92(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22d92U: { // 61000b34 bsr.w $238c8
            next = 0x22d96U;
            const auto result = m.call(c, 416U, 0x22d92U, 0x238c8U, 0x22d96U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22d96U: { // 2b7c00022dda0002 move.l #$22dda, $2(a5)
            next = 0x22d9eU;
            const auto value = 0x22ddaU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22d9eU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22da2U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22da2U: { // 41f900024256 lea.l $24256.l, a0
            next = 0x22da8U;
            const auto source_address = 0x24256U;
            r.address[0] = source_address;
            break;
        }
        case 0x22da8U: { // 7007 moveq #$7, d0
            next = 0x22daaU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22daaU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22dacU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22dacU: { // 51c8fffc dbra d0, $22daa
            next = 0x22db0U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22daaU;
            break;
        }
        case 0x22db0U: { // 61000a7a bsr.w $2382c
            next = 0x22db4U;
            const auto result = m.call(c, 413U, 0x22db0U, 0x2382cU, 0x22db4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22db4U: { // 4e75 rts 
            next = 0x22db6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22db4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22d92U:
        case 0x22d96U:
        case 0x22d9eU:
        case 0x22da2U:
        case 0x22da8U:
        case 0x22daaU:
        case 0x22dacU:
        case 0x22db0U:
        case 0x22db4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022db6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22db6U: { // 61000b10 bsr.w $238c8
            next = 0x22dbaU;
            const auto result = m.call(c, 416U, 0x22db6U, 0x238c8U, 0x22dbaU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22dbaU: { // 2b7c00022dda0002 move.l #$22dda, $2(a5)
            next = 0x22dc2U;
            const auto value = 0x22ddaU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22dc2U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22dc6U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22dc6U: { // 41f900024266 lea.l $24266.l, a0
            next = 0x22dccU;
            const auto source_address = 0x24266U;
            r.address[0] = source_address;
            break;
        }
        case 0x22dccU: { // 7007 moveq #$7, d0
            next = 0x22dceU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22dceU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22dd0U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22dd0U: { // 51c8fffc dbra d0, $22dce
            next = 0x22dd4U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22dceU;
            break;
        }
        case 0x22dd4U: { // 61000a56 bsr.w $2382c
            next = 0x22dd8U;
            const auto result = m.call(c, 413U, 0x22dd4U, 0x2382cU, 0x22dd8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22dd8U: { // 4e75 rts 
            next = 0x22ddaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22dd8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22db6U:
        case 0x22dbaU:
        case 0x22dc2U:
        case 0x22dc6U:
        case 0x22dccU:
        case 0x22dceU:
        case 0x22dd0U:
        case 0x22dd4U:
        case 0x22dd8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022e56(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22e56U: { // 6100098a bsr.w $237e2
            next = 0x22e5aU;
            const auto result = m.call(c, 412U, 0x22e56U, 0x237e2U, 0x22e5aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22e5aU: { // 45f9000242be lea.l $242be.l, a2
            next = 0x22e60U;
            const auto source_address = 0x242beU;
            r.address[2] = source_address;
            break;
        }
        case 0x22e60U: { // 61000ac8 bsr.w $2392a
            next = 0x22e64U;
            const auto result = m.call(c, 419U, 0x22e60U, 0x2392aU, 0x22e64U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22e64U: { // 4e75 rts 
            next = 0x22e66U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22e64U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22e56U:
        case 0x22e5aU:
        case 0x22e60U:
        case 0x22e64U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022dea(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22deaU: { // 61000adc bsr.w $238c8
            next = 0x22deeU;
            const auto result = m.call(c, 416U, 0x22deaU, 0x238c8U, 0x22deeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22deeU: { // 2b7c00022e560002 move.l #$22e56, $2(a5)
            next = 0x22df6U;
            const auto value = 0x22e56U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22df6U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22dfaU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22dfaU: { // 41f90002428e lea.l $2428e.l, a0
            next = 0x22e00U;
            const auto source_address = 0x2428eU;
            r.address[0] = source_address;
            break;
        }
        case 0x22e00U: { // 7007 moveq #$7, d0
            next = 0x22e02U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22e02U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22e04U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22e04U: { // 51c8fffc dbra d0, $22e02
            next = 0x22e08U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22e02U;
            break;
        }
        case 0x22e08U: { // 61000a22 bsr.w $2382c
            next = 0x22e0cU;
            const auto result = m.call(c, 413U, 0x22e08U, 0x2382cU, 0x22e0cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22e0cU: { // 4e75 rts 
            next = 0x22e0eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22e0cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22deaU:
        case 0x22deeU:
        case 0x22df6U:
        case 0x22dfaU:
        case 0x22e00U:
        case 0x22e02U:
        case 0x22e04U:
        case 0x22e08U:
        case 0x22e0cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
