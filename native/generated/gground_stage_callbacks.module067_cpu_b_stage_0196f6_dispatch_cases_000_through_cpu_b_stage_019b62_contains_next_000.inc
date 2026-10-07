
static bool cpu_b_stage_0196f6_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x196f6U: { // 302d0040 move.w $40(a5), d0
            next = 0x196faU;
            const auto source_address = r.address[5] + 0x40U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x196faU: { // e540 asl.w #$2, d0
            next = 0x196fcU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x19700U: { // 0c6d00020040 cmpi.w #$2, $40(a5)
            next = 0x19706U;
            const auto source_value = 0x2U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19706U: { // 6b16 bmi.b $1971e
            next = 0x19708U;
            if ((r.status & 8U) != 0U) { next = 0x1971eU; transfer_kind = 1U; }
            break;
        }
        case 0x19708U: { // 206d004c movea.l $4c(a5), a0
            next = 0x1970cU;
            const auto source_address = r.address[5] + 0x4cU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1970cU: { // 302d0040 move.w $40(a5), d0
            next = 0x19710U;
            const auto source_address = r.address[5] + 0x40U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19710U: { // e740 asl.w #$3, d0
            next = 0x19712U;
            m.asl_word(0U, 3U);
            break;
        }
        case 0x19712U: { // d0c0 adda.w d0, a0
            next = 0x19714U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x19714U: { // 302d0064 move.w $64(a5), d0
            next = 0x19718U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19718U: { // 1b7000000059 move.b (a0, d0.w), $59(a5)
            next = 0x1971eU;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x59U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_0196f6(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x196fcU: { // 4ebb0022 jsr $19720(pc, d0.w)
            next = 0x19700U;
            const auto target = (0x19720U + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            const auto result = m.indirect_call(c, 0x196fcU, target, 0x19700U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1971eU: { // 4e75 rts 
            next = 0x19720U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1971eU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_0196f6_dispatch_cases_000(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x196f6U:
        case 0x196faU:
        case 0x196fcU:
        case 0x19700U:
        case 0x19706U:
        case 0x19708U:
        case 0x1970cU:
        case 0x19710U:
        case 0x19712U:
        case 0x19714U:
        case 0x19718U:
        case 0x1971eU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_019b62_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x19b62U:
    case 0x19b66U:
    case 0x19b6cU:
    case 0x19b70U:
    case 0x19b74U:
    case 0x19b78U:
    case 0x19b7cU:
    case 0x19b80U:
    case 0x19b84U:
    case 0x19b88U:
    case 0x19b8eU:
    case 0x19b90U:
    case 0x19b92U:
    case 0x19b94U:
    case 0x19b96U:
    case 0x19b98U:
    case 0x19b9aU:
    case 0x19b9cU:
    case 0x19b9eU:
    case 0x19ba0U:
    case 0x19ba2U:
    case 0x19ba6U:
    case 0x19baaU:
    case 0x19bb2U:
    case 0x19bb4U:
    case 0x19bb8U:
    case 0x19bbcU:
    case 0x19bc0U:
    case 0x19bc2U:
    case 0x19bc6U:
    case 0x19bcaU:
    case 0x19bccU:
    case 0x19bd2U:
    case 0x19bd6U:
    case 0x19bdcU:
    case 0x19be0U:
    case 0x19be4U:
    case 0x19beaU:
    case 0x19bf0U:
    case 0x19bf6U:
    case 0x19bfaU:
    case 0x19bfeU:
    case 0x19c04U:
    case 0x19c06U:
    case 0x19c0aU:
        return true;
    default: return false;
    }
}
