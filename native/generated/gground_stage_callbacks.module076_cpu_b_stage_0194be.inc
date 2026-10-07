
FunctionResult cpu_b_stage_0194be(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x194beU: { // 6100006c bsr.w $1952c
            next = 0x194c2U;
            const auto result = m.call(c, 910U, 0x194beU, 0x1952cU, 0x194c2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x194c2U: { // 61000232 bsr.w $196f6
            next = 0x194c6U;
            const auto result = m.call(c, 911U, 0x194c2U, 0x196f6U, 0x194c6U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x194c6U: { // 6100033e bsr.w $19806
            next = 0x194caU;
            const auto result = m.call(c, 912U, 0x194c6U, 0x19806U, 0x194caU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x194caU: { // 4a2d003f tst.b $3f(a5)
            next = 0x194ceU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x194ceU: { // 674a beq.b $1951a
            next = 0x194d0U;
            if ((r.status & 4U) != 0U) { next = 0x1951aU; transfer_kind = 1U; }
            break;
        }
        case 0x194d0U: { // 61000466 bsr.w $19938
            next = 0x194d4U;
            const auto result = m.call(c, 913U, 0x194d0U, 0x19938U, 0x194d4U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x194d4U: { // 6044 bra.b $1951a
            next = 0x194d6U;
            if (true) { next = 0x1951aU; transfer_kind = 1U; }
            break;
        }
        case 0x1951aU: { // 6100049a bsr.w $199b6
            next = 0x1951eU;
            const auto result = m.call(c, 914U, 0x1951aU, 0x199b6U, 0x1951eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1951eU: { // 610004ca bsr.w $199ea
            next = 0x19522U;
            const auto result = m.call(c, 915U, 0x1951eU, 0x199eaU, 0x19522U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x19522U: { // 41fa1f2c lea.l $1b450(pc), a0
            next = 0x19526U;
            const auto source_address = 0x1b450U;
            r.address[0] = source_address;
            break;
        }
        case 0x19526U: { // 6100fe72 bsr.w $1939a
            next = 0x1952aU;
            const auto result = m.call(c, 904U, 0x19526U, 0x1939aU, 0x1952aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1952aU: { // 4e75 rts 
            next = 0x1952cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1952aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default: return {TranslationStatus::contract_violation, 0U, pc};
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x194beU:
        case 0x194c2U:
        case 0x194c6U:
        case 0x194caU:
        case 0x194ceU:
        case 0x194d0U:
        case 0x194d4U:
        case 0x1951aU:
        case 0x1951eU:
        case 0x19522U:
        case 0x19526U:
        case 0x1952aU:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}
