
static bool cpu_b_stage_023752_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x23756U: { // 2b7c000237be0002 move.l #$237be, $2(a5)
            next = 0x2375eU;
            const auto value = 0x237beU;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x2375eU: { // 4ded0022 lea.l $22(a5), a6
            next = 0x23762U;
            const auto source_address = r.address[5] + 0x22U;
            r.address[6] = source_address;
            break;
        }
        case 0x23762U: { // 41f9000246e8 lea.l $246e8.l, a0
            next = 0x23768U;
            const auto source_address = 0x246e8U;
            r.address[0] = source_address;
            break;
        }
        case 0x23768U: { // 7007 moveq #$7, d0
            next = 0x2376aU;
            r.data[0] = 0x7U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x2376aU: { // 2cd8 move.l (a0)+, (a6)+
            next = 0x2376cU;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] += 4U;
            const auto destination_address = r.address[6];
            m.lng(destination_address, value);
            r.address[6] += 4U;
            m.logic(value, 32U);
            break;
        }
        case 0x2376cU: { // 51c8fffc dbra d0, $2376a
            next = 0x23770U;
            m.dw(0U, r.data[0] - 1U);
            if ((r.data[0] & 0xffffU) != 0xffffU) next = 0x2376aU;
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_023752(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x23752U: { // 61000174 bsr.w $238c8
            next = 0x23756U;
            const auto result = m.call(c, 416U, 0x23752U, 0x238c8U, 0x23756U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23770U: { // 610000ba bsr.w $2382c
            next = 0x23774U;
            const auto result = m.call(c, 413U, 0x23770U, 0x2382cU, 0x23774U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x23774U: { // 4e75 rts 
            next = 0x23776U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x23774U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_023752_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_023752_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

