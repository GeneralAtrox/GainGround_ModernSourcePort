
static bool cpu_b_stage_01cd32_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1cdd8U: { // 122d0039 move.b $39(a5), d1
            next = 0x1cddcU;
            const auto source_address = r.address[5] + 0x39U;
            const auto value = m.byte(source_address);
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1cddcU: { // 4881 ext.w d1
            next = 0x1cddeU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[1])));
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cddeU: { // d041 add.w d1, d0
            next = 0x1cde0U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1cde0U: { // 024007ff andi.w #$7ff, d0
            next = 0x1cde4U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1cde4U: { // 3b40005c move.w d0, $5c(a5)
            next = 0x1cde8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cde8U: { // 60c2 bra.b $1cdac
            next = 0x1cdeaU;
            if (true) { next = 0x1cdacU; transfer_kind = 1U; }
            break;
        }
        case 0x1cdeaU: { // 3b7c00010074 move.w #$1, $74(a5)
            next = 0x1cdf0U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1cdf0U: { // 08ed00010041 bset.b #$1, $41(a5)
            next = 0x1cdf6U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cdf6U: { // 08ed00000041 bset.b #$0, $41(a5)
            next = 0x1cdfcU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1cdfcU: { // 08ad00020041 bclr.b #$2, $41(a5)
            next = 0x1ce02U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1ce02U: { // 08ed00010040 bset.b #$1, $40(a5)
            next = 0x1ce08U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1ce08U: { // 609c bra.b $1cda6
            next = 0x1ce0aU;
            if (true) { next = 0x1cda6U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_01cd32(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x1cd52U: { // 610006ae bsr.w $1d402
            next = 0x1cd56U;
            const auto result = m.call(c, 337U, 0x1cd52U, 0x1d402U, 0x1cd56U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1cd6eU: { // 4e75 rts 
            next = 0x1cd70U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cd6eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cd7aU: { // 61000de0 bsr.w $1db5c
            next = 0x1cd7eU;
            const auto result = m.call(c, 351U, 0x1cd7aU, 0x1db5cU, 0x1cd7eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1cd9eU: { // 4e75 rts 
            next = 0x1cda0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cd9eU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cdbeU: { // 4e75 rts 
            next = 0x1cdc0U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cdbeU, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x1cdd6U: { // 4e75 rts 
            next = 0x1cdd8U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1cdd6U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_01cd32_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_01cd32_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_01cd32_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_01d0d6_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1d0d6U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1d0daU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1d0daU: { // 6704 beq.b $1d0e0
            next = 0x1d0dcU;
            if ((r.status & 4U) != 0U) { next = 0x1d0e0U; transfer_kind = 1U; }
            break;
        }
        case 0x1d0dcU: { // 600008fc bra.w $1d9da
            next = 0x1d0e0U;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1d0e0U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1d0e4U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1d0e4U: { // 6704 beq.b $1d0ea
            next = 0x1d0e6U;
            if ((r.status & 4U) != 0U) { next = 0x1d0eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1d0e6U: { // 60000902 bra.w $1d9ea
            next = 0x1d0eaU;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1d0eaU: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1d0f0U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1d0f4U: { // 082d00010040 btst.b #$1, $40(a5)
            next = 0x1d0faU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1d0faU: { // 660a bne.b $1d106
            next = 0x1d0fcU;
            if ((r.status & 4U) == 0U) { next = 0x1d106U; transfer_kind = 1U; }
            break;
        }
        case 0x1d0fcU: { // 082d00060040 btst.b #$6, $40(a5)
            next = 0x1d102U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x6U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1d102U: { // 6602 bne.b $1d106
            next = 0x1d104U;
            if ((r.status & 4U) == 0U) { next = 0x1d106U; transfer_kind = 1U; }
            break;
        }
        case 0x1d104U: { // 6026 bra.b $1d12c
            next = 0x1d106U;
            if (true) { next = 0x1d12cU; transfer_kind = 1U; }
            break;
        }
        case 0x1d106U: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1d10cU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1d10cU: { // 08ad00050040 bclr.b #$5, $40(a5)
            next = 0x1d112U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1d112U: { // 3b7c0200005c move.w #$200, $5c(a5)
            next = 0x1d118U;
            const auto value = 0x200U;
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1d118U: { // 08ad00000040 bclr.b #$0, $40(a5)
            next = 0x1d11eU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1d11eU: { // 08ed00030040 bset.b #$3, $40(a5)
            next = 0x1d124U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1d124U: { // 08ed00020040 bset.b #$2, $40(a5)
            next = 0x1d12aU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1d12cU: { // 08ed00030041 bset.b #$3, $41(a5)
            next = 0x1d132U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1d132U: { // 08ad00010040 bclr.b #$1, $40(a5)
            next = 0x1d138U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1d138U: { // 1b7c0009000b move.b #$9, $b(a5)
            next = 0x1d13eU;
            const auto value = 0x9U;
            const auto destination_address = r.address[5] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1d13eU: { // 6000ffd2 bra.w $1d112
            next = 0x1d142U;
            if (true) { next = 0x1d112U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
