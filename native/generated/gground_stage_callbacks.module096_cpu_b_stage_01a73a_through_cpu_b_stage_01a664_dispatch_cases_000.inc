
FunctionResult cpu_b_stage_01a73a(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1a7baU: { // 4e75 rts 
            next = 0x1a7bcU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a7baU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1a7c4U: { // 4e75 rts 
            next = 0x1a7c6U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1a7c4U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01a73a_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01a73a_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        switch (next) {
        case 0x1a73aU:
        case 0x1a73eU:
        case 0x1a740U:
        case 0x1a742U:
        case 0x1a746U:
        case 0x1a74aU:
        case 0x1a74eU:
        case 0x1a752U:
        case 0x1a754U:
        case 0x1a758U:
        case 0x1a75cU:
        case 0x1a75eU:
        case 0x1a762U:
        case 0x1a764U:
        case 0x1a768U:
        case 0x1a76aU:
        case 0x1a76cU:
        case 0x1a76eU:
        case 0x1a772U:
        case 0x1a776U:
        case 0x1a77aU:
        case 0x1a77eU:
        case 0x1a780U:
        case 0x1a782U:
        case 0x1a784U:
        case 0x1a786U:
        case 0x1a78cU:
        case 0x1a78eU:
        case 0x1a792U:
        case 0x1a794U:
        case 0x1a798U:
        case 0x1a79aU:
        case 0x1a79eU:
        case 0x1a7a0U:
        case 0x1a7a4U:
        case 0x1a7a6U:
        case 0x1a7aaU:
        case 0x1a7acU:
        case 0x1a7b0U:
        case 0x1a7b2U:
        case 0x1a7b6U:
        case 0x1a7baU:
        case 0x1a7bcU:
        case 0x1a7c0U:
        case 0x1a7c4U:
            break;
        default: return m.dispatch(c, pc, next, transfer_kind, m.state);
        }
    }
}

static bool cpu_b_stage_01a664_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a37cU: { // 08ed00020001 bset.b #$2, $1(a5)
            next = 0x1a382U;
            const auto destination_address = r.address[5] + 0x1U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1a382U: { // 2b7c0001a3960002 move.l #$1a396, $2(a5)
            next = 0x1a38aU;
            const auto value = 0x1a396U;
            const auto destination_address = r.address[5] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a38aU: { // 4aed003f tas.b $3f(a5)
            next = 0x1a38eU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x1a38eU: { // 3b7c37370010 move.w #$3737, $10(a5)
            next = 0x1a394U;
            const auto value = 0x3737U;
            const auto destination_address = r.address[5] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a394U: { // 600c bra.b $1a3a2
            next = 0x1a396U;
            if (true) { next = 0x1a3a2U; transfer_kind = 1U; }
            break;
        }
        case 0x1a3a2U: { // 08950000 bclr.b #$0, (a5)
            next = 0x1a3a6U;
            const auto destination_address = r.address[5];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1a664U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1a668U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1a668U: { // 6600fd12 bne.w $1a37c
            next = 0x1a66cU;
            if ((r.status & 4U) == 0U) { next = 0x1a37cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a670U: { // 6500fd0a bcs.w $1a37c
            next = 0x1a674U;
            if ((r.status & 1U) != 0U) { next = 0x1a37cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a67aU: { // 6500fd00 bcs.w $1a37c
            next = 0x1a67eU;
            if ((r.status & 1U) != 0U) { next = 0x1a37cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a67eU: { // 536d0066 subq.w #$1, $66(a5)
            next = 0x1a682U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a682U: { // 6f00fcf8 ble.w $1a37c
            next = 0x1a686U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1a37cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a686U: { // 202d001e move.l $1e(a5), d0
            next = 0x1a68aU;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a68aU: { // d1ad0012 add.l d0, $12(a5)
            next = 0x1a68eU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a68eU: { // 202d0026 move.l $26(a5), d0
            next = 0x1a692U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a692U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x1a696U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a696U: { // 526d0008 addq.w #$1, $8(a5)
            next = 0x1a69aU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a69aU: { // 026d0c930008 andi.w #$c93, $8(a5)
            next = 0x1a6a0U;
            const auto source_value = 0xc93U;
            const auto destination_address = r.address[5] + 0x8U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1a6a6U: { // 41fa1270 lea.l $1b918(pc), a0
            next = 0x1a6aaU;
            const auto source_address = 0x1b918U;
            r.address[0] = source_address;
            break;
        }
    default: return false;
    }
    return true;
}
