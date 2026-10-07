
FunctionResult cpu_b_stage_02365a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x2365aU: { // 6100026c bsr.w $238c8
            next = 0x2365eU;
            const auto result = m.call(c, 416U, 0x2365aU, 0x238c8U, 0x2365eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2365eU: { // 2b7c000236c60002 move.l #$236c6, $2(a5)
            next = 0x23666U;
            const auto value = 0x236c6U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x23666U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x2366aU;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x2366aU: { // 41f9000246e8 lea.l $246e8.l, a0
            next = 0x23670U;
            const auto source_address = 0x246e8U;
            r.address[0] = source_address;
            break;
        }
        case 0x23670U: { // 7007 moveq #$7, d0
            next = 0x23672U;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x23672U: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x23674U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x23674U: { // 51c8fffc dbra d0, $23672
            next = 0x23678U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x23672U;
            break;
        }
        case 0x23678U: { // 610001b2 bsr.w $2382c
            next = 0x2367cU;
            const auto result = m.call(c, 413U, 0x23678U, 0x2382cU, 0x2367cU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x2367cU: { // 4e75 rts 
            next = 0x2367eU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x2367cU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x2365aU:
        case 0x2365eU:
        case 0x23666U:
        case 0x2366aU:
        case 0x23670U:
        case 0x23672U:
        case 0x23674U:
        case 0x23678U:
        case 0x2367cU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_023742(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23742U: { // 6100009e bsr.w $237e2
            next = 0x23746U;
            const auto result = m.call(c, 412U, 0x23742U, 0x237e2U, 0x23746U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23746U: { // 45f900024718 lea.l $24718.l, a2
            next = 0x2374cU;
            const auto source_address = 0x24718U;
            r.address[2] = source_address;
            break;
        }
        case 0x2374cU: { // 610001dc bsr.w $2392a
            next = 0x23750U;
            const auto result = m.call(c, 419U, 0x2374cU, 0x2392aU, 0x23750U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23750U: { // 4e75 rts 
            next = 0x23752U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23750U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x23742U:
        case 0x23746U:
        case 0x2374cU:
        case 0x23750U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0236d6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x236d6U: { // 610001f0 bsr.w $238c8
            next = 0x236daU;
            const auto result = m.call(c, 416U, 0x236d6U, 0x238c8U, 0x236daU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x236daU: { // 2b7c000237420002 move.l #$23742, $2(a5)
            next = 0x236e2U;
            const auto value = 0x23742U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x236e2U: { // 4ded0022 lea.l $22(a5), a6
            next = 0x236e6U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x236e6U: { // 41f9000246e8 lea.l $246e8.l, a0
            next = 0x236ecU;
            const auto source_address = 0x246e8U;
            r.address[0] = source_address;
            break;
        }
        case 0x236ecU: { // 7007 moveq #$7, d0
            next = 0x236eeU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x236eeU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x236f0U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x236f0U: { // 51c8fffc dbra d0, $236ee
            next = 0x236f4U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x236eeU;
            break;
        }
        case 0x236f4U: { // 61000136 bsr.w $2382c
            next = 0x236f8U;
            const auto result = m.call(c, 413U, 0x236f4U, 0x2382cU, 0x236f8U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x236f8U: { // 4e75 rts 
            next = 0x236faU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x236f8U, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x236d6U:
        case 0x236daU:
        case 0x236e2U:
        case 0x236e6U:
        case 0x236ecU:
        case 0x236eeU:
        case 0x236f0U:
        case 0x236f4U:
        case 0x236f8U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

FunctionResult cpu_b_stage_0237be(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x237beU: { // 61000022 bsr.w $237e2
            next = 0x237c2U;
            const auto result = m.call(c, 412U, 0x237beU, 0x237e2U, 0x237c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x237c2U: { // 45f9000246c8 lea.l $246c8.l, a2
            next = 0x237c8U;
            const auto source_address = 0x246c8U;
            r.address[2] = source_address;
            break;
        }
        case 0x237c8U: { // 61000160 bsr.w $2392a
            next = 0x237ccU;
            const auto result = m.call(c, 419U, 0x237c8U, 0x2392aU, 0x237ccU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x237ccU: { // 4e75 rts 
            next = 0x237ceU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x237ccU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x237beU:
        case 0x237c2U:
        case 0x237c8U:
        case 0x237ccU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_023752_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x23752U:
    case 0x23756U:
    case 0x2375eU:
    case 0x23762U:
    case 0x23768U:
    case 0x2376aU:
    case 0x2376cU:
    case 0x23770U:
    case 0x23774U:
        return true;
    default: return false;
    }
}
