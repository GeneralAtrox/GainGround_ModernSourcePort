
static bool cpu_b_stage_01c73a_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c73aU: { // 206d0066 movea.l $66(a5), a0
            next = 0x1c73eU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c73eU: { // 2050 movea.l (a0), a0
            next = 0x1c740U;
            const auto source_address = r.address[0];
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c740U: { // 302d0054 move.w $54(a5), d0
            next = 0x1c744U;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c744U: { // 7200 moveq #$0, d1
            next = 0x1c746U;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1c746U: { // 1218 move.b (a0)+, d1
            next = 0x1c748U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c748U: { // c041 and.w d1, d0
            next = 0x1c74aU;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c74aU: { // d040 add.w d0, d0
            next = 0x1c74cU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c74cU: { // 3200 move.w d0, d1
            next = 0x1c74eU;
            const auto value = r.data[0];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c74eU: { // d040 add.w d0, d0
            next = 0x1c750U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c750U: { // d240 add.w d0, d1
            next = 0x1c752U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1c752U: { // 206d0066 movea.l $66(a5), a0
            next = 0x1c756U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c756U: { // 2068001a movea.l $1a(a0), a0
            next = 0x1c75aU;
            const auto source_address = r.address[0] + 0x1aU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c75aU: { // d0c1 adda.w d1, a0
            next = 0x1c75cU;
            const auto source_value = r.data[1];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1c75cU: { // 3b580012 move.w (a0)+, $12(a5)
            next = 0x1c760U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c760U: { // 3b580016 move.w (a0)+, $16(a5)
            next = 0x1c764U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c764U: { // 3b58001a move.w (a0)+, $1a(a5)
            next = 0x1c768U;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01c73a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1c768U: { // 4e75 rts 
            next = 0x1c76aU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1c768U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01c73a_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1c73aU:
        case 0x1c73eU:
        case 0x1c740U:
        case 0x1c744U:
        case 0x1c746U:
        case 0x1c748U:
        case 0x1c74aU:
        case 0x1c74cU:
        case 0x1c74eU:
        case 0x1c750U:
        case 0x1c752U:
        case 0x1c756U:
        case 0x1c75aU:
        case 0x1c75cU:
        case 0x1c760U:
        case 0x1c764U:
        case 0x1c768U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01c652_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1c652U:
    case 0x1c656U:
    case 0x1c65cU:
    case 0x1c65eU:
    case 0x1c662U:
    case 0x1c666U:
    case 0x1c668U:
    case 0x1c66cU:
    case 0x1c670U:
    case 0x1c672U:
    case 0x1c678U:
    case 0x1c67eU:
    case 0x1c682U:
    case 0x1c686U:
    case 0x1c68cU:
    case 0x1c68eU:
    case 0x1c694U:
    case 0x1c69aU:
    case 0x1c6a0U:
    case 0x1c6a6U:
    case 0x1c6acU:
    case 0x1c6b0U:
    case 0x1c6b6U:
    case 0x1c6baU:
    case 0x1c6beU:
    case 0x1c6c4U:
    case 0x1c6c6U:
    case 0x1c6caU:
    case 0x1c6ccU:
    case 0x1c6d0U:
    case 0x1c6d6U:
    case 0x1c6d8U:
    case 0x1c6dcU:
    case 0x1c6e0U:
    case 0x1c6e6U:
    case 0x1c6ecU:
    case 0x1c6eeU:
    case 0x1c6f2U:
    case 0x1c6f4U:
    case 0x1c6f8U:
    case 0x1c6fcU:
    case 0x1c6feU:
    case 0x1c702U:
    case 0x1c704U:
    case 0x1c70aU:
    case 0x1c70cU:
    case 0x1c70eU:
    case 0x1c714U:
    case 0x1c71aU:
    case 0x1c720U:
    case 0x1c726U:
    case 0x1c72cU:
    case 0x1c732U:
    case 0x1c738U:
        return true;
    default: return false;
    }
}
