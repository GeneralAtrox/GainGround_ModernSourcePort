
static bool cpu_b_stage_01d920_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1d9a4U: { // d240 add.w d0, d1
            next = 0x1d9a6U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1d9a6U: { // d440 add.w d0, d2
            next = 0x1d9a8U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1d9a8U: { // 302a0002 move.w $2(a2), d0
            next = 0x1d9acU;
            const auto source_address = r.address[2] + 0x2U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d9acU: { // d640 add.w d0, d3
            next = 0x1d9aeU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[3];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(3U, value);
            break;
        }
        case 0x1d9aeU: { // d840 add.w d0, d4
            next = 0x1d9b0U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[4];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(4U, value);
            break;
        }
        case 0x1d9b0U: { // 302e0012 move.w $12(a6), d0
            next = 0x1d9b4U;
            const auto source_address = r.address[6] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d9b4U: { // b042 cmp.w d2, d0
            next = 0x1d9b6U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1d9b6U: { // 6e16 bgt.b $1d9ce
            next = 0x1d9b8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d9b8U: { // b041 cmp.w d1, d0
            next = 0x1d9baU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1d9baU: { // 6b12 bmi.b $1d9ce
            next = 0x1d9bcU;
            if ((r.status & 8U) != 0U) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d9bcU: { // 302e001a move.w $1a(a6), d0
            next = 0x1d9c0U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d9c0U: { // b044 cmp.w d4, d0
            next = 0x1d9c2U;
            const auto source_value = r.data[4];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1d9c2U: { // 6e0a bgt.b $1d9ce
            next = 0x1d9c4U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d9c4U: { // b043 cmp.w d3, d0
            next = 0x1d9c6U;
            const auto source_value = r.data[3];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1d9c6U: { // 6b06 bmi.b $1d9ce
            next = 0x1d9c8U;
            if ((r.status & 8U) != 0U) { next = 0x1d9ceU; transfer_kind = 1U; }
            break;
        }
        case 0x1d9c8U: { // 44fc0001 move.w #$1, ccr
            next = 0x1d9ccU;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x1U & 0x1fU));
            break;
        }
        case 0x1d9ceU: { // 51cfff7c dbra d7, $1d94c
            next = 0x1d9d2U;
            m.dw(7U, r.data[7] - 1U);
            if ((r.data[7] & 0xffffU) != 0xffffU) next = 0x1d94cU;
            break;
        }
        case 0x1d9d2U: { // 44fc0000 move.w #$0, ccr
            next = 0x1d9d6U;
            r.status = static_cast<std::uint16_t>((r.status & 0xffe0U) | (0x0U & 0x1fU));
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01d920(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1d9ccU: { // 4e75 rts 
            next = 0x1d9ceU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1d9ccU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1d9d6U: { // 4e75 rts 
            next = 0x1d9d8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1d9d6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01d920_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01d920_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01d920_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_01bde0_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1bde0U:
    case 0x1bde6U:
    case 0x1bde8U:
    case 0x1bdeeU:
    case 0x1bdf4U:
    case 0x1bdf8U:
    case 0x1bdfeU:
    case 0x1be00U:
    case 0x1be02U:
    case 0x1be08U:
    case 0x1be0eU:
    case 0x1be14U:
    case 0x1be16U:
    case 0x1be1aU:
    case 0x1be1cU:
    case 0x1be20U:
    case 0x1be24U:
    case 0x1be26U:
    case 0x1be2aU:
    case 0x1be2eU:
    case 0x1be34U:
    case 0x1be36U:
    case 0x1be3aU:
    case 0x1be3cU:
    case 0x1be40U:
    case 0x1be42U:
    case 0x1be46U:
    case 0x1be48U:
    case 0x1be4cU:
    case 0x1be52U:
    case 0x1be54U:
    case 0x1be58U:
    case 0x1be5cU:
    case 0x1be5eU:
    case 0x1be62U:
    case 0x1be68U:
    case 0x1be6aU:
    case 0x1be70U:
    case 0x1be72U:
    case 0x1be78U:
    case 0x1be7aU:
    case 0x1be80U:
    case 0x1be82U:
    case 0x1be88U:
    case 0x1be8eU:
    case 0x1be94U:
    case 0x1be9aU:
    case 0x1bea0U:
    case 0x1bea2U:
    case 0x1bea8U:
    case 0x1beacU:
    case 0x1beb0U:
    case 0x1beb4U:
    case 0x1beb6U:
    case 0x1bebaU:
    case 0x1bec0U:
    case 0x1bec2U:
    case 0x1bec6U:
    case 0x1becaU:
    case 0x1beccU:
    case 0x1bed0U:
    case 0x1bed2U:
    case 0x1bed4U:
    case 0x1bed8U:
    case 0x1bedcU:
    case 0x1bedeU:
    case 0x1bee2U:
    case 0x1bee4U:
    case 0x1bee8U:
    case 0x1beecU:
    case 0x1beeeU:
    case 0x1bef0U:
    case 0x1bef4U:
    case 0x1bef8U:
    case 0x1befeU:
    case 0x1bf00U:
    case 0x1bf04U:
    case 0x1bf08U:
    case 0x1bf0eU:
    case 0x1bf14U:
    case 0x1bf1aU:
        return true;
    default: return false;
    }
}
