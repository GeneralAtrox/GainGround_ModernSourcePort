
FunctionResult cpu_b_stage_022f3a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22f3aU: { // 6100098c bsr.w $238c8
            next = 0x22f3eU;
            const auto result = m.call(c, 416U, 0x22f3aU, 0x238c8U, 0x22f3eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22f3eU: { // 2b7c00022f5e0002 move.l #$22f5e, $2(a5)
            next = 0x22f46U;
            const auto value = 0x22f5eU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22f46U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22f4aU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22f4aU: { // 41f900024378 lea.l $24378.l, a0
            next = 0x22f50U;
            const auto source_address = 0x24378U;
            r.address[0] = source_address;
            break;
        }
        case 0x22f50U: { // 7007 moveq #$7, d0
            next = 0x22f52U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22f52U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22f54U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22f54U: { // 51c8fffc dbra d0, $22f52
            next = 0x22f58U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22f52U;
            break;
        }
        case 0x22f58U: { // 610008d2 bsr.w $2382c
            next = 0x22f5cU;
            const auto result = m.call(c, 413U, 0x22f58U, 0x2382cU, 0x22f5cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22f5cU: { // 4e75 rts 
            next = 0x22f5eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22f5cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22f3aU:
        case 0x22f3eU:
        case 0x22f46U:
        case 0x22f4aU:
        case 0x22f50U:
        case 0x22f52U:
        case 0x22f54U:
        case 0x22f58U:
        case 0x22f5cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022fea(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22feaU: { // 610007f6 bsr.w $237e2
            next = 0x22feeU;
            const auto result = m.call(c, 412U, 0x22feaU, 0x237e2U, 0x22feeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22feeU: { // 45f900024400 lea.l $24400.l, a2
            next = 0x22ff4U;
            const auto source_address = 0x24400U;
            r.address[2] = source_address;
            break;
        }
        case 0x22ff4U: { // 61000934 bsr.w $2392a
            next = 0x22ff8U;
            const auto result = m.call(c, 419U, 0x22ff4U, 0x2392aU, 0x22ff8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22ff8U: { // 4e75 rts 
            next = 0x22ffaU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22ff8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22feaU:
        case 0x22feeU:
        case 0x22ff4U:
        case 0x22ff8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022f7e(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22f7eU: { // 61000948 bsr.w $238c8
            next = 0x22f82U;
            const auto result = m.call(c, 416U, 0x22f7eU, 0x238c8U, 0x22f82U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22f82U: { // 2b7c00022fea0002 move.l #$22fea, $2(a5)
            next = 0x22f8aU;
            const auto value = 0x22feaU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22f8aU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22f8eU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22f8eU: { // 41f9000243ca lea.l $243ca.l, a0
            next = 0x22f94U;
            const auto source_address = 0x243caU;
            r.address[0] = source_address;
            break;
        }
        case 0x22f94U: { // 7007 moveq #$7, d0
            next = 0x22f96U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22f96U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22f98U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22f98U: { // 51c8fffc dbra d0, $22f96
            next = 0x22f9cU;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22f96U;
            break;
        }
        case 0x22f9cU: { // 6100088e bsr.w $2382c
            next = 0x22fa0U;
            const auto result = m.call(c, 413U, 0x22f9cU, 0x2382cU, 0x22fa0U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22fa0U: { // 4e75 rts 
            next = 0x22fa2U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22fa0U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22f7eU:
        case 0x22f82U:
        case 0x22f8aU:
        case 0x22f8eU:
        case 0x22f94U:
        case 0x22f96U:
        case 0x22f98U:
        case 0x22f9cU:
        case 0x22fa0U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_022fa2(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x22fa2U: { // 61000924 bsr.w $238c8
            next = 0x22fa6U;
            const auto result = m.call(c, 416U, 0x22fa2U, 0x238c8U, 0x22fa6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22fa6U: { // 2b7c00022fea0002 move.l #$22fea, $2(a5)
            next = 0x22faeU;
            const auto value = 0x22feaU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x22faeU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x22fb2U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x22fb2U: { // 41f9000243dc lea.l $243dc.l, a0
            next = 0x22fb8U;
            const auto source_address = 0x243dcU;
            r.address[0] = source_address;
            break;
        }
        case 0x22fb8U: { // 7007 moveq #$7, d0
            next = 0x22fbaU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x22fbaU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x22fbcU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x22fbcU: { // 51c8fffc dbra d0, $22fba
            next = 0x22fc0U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x22fbaU;
            break;
        }
        case 0x22fc0U: { // 6100086a bsr.w $2382c
            next = 0x22fc4U;
            const auto result = m.call(c, 413U, 0x22fc0U, 0x2382cU, 0x22fc4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x22fc4U: { // 4e75 rts 
            next = 0x22fc6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x22fc4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x22fa2U:
        case 0x22fa6U:
        case 0x22faeU:
        case 0x22fb2U:
        case 0x22fb8U:
        case 0x22fbaU:
        case 0x22fbcU:
        case 0x22fc0U:
        case 0x22fc4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
