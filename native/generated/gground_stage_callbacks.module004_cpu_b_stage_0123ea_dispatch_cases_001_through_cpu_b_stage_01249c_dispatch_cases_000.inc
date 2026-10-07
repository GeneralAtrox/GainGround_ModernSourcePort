
static bool cpu_b_stage_0123ea_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1246eU: { // 1b7000000001 move.b (a0, d0.w), $1(a5)
            next = 0x12474U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x12488U: { // 4aed003f tas.b $3f(a5)
            next = 0x1248cU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x1248cU: { // 3b7c01190006 move.w #$119, $6(a5)
            next = 0x12492U;
            const auto value = 0x119U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12492U: { // 3b7c5f5f0010 move.w #$5f5f, $10(a5)
            next = 0x12498U;
            const auto value = 0x5f5fU;
            const auto destination_address = r.address[5] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12498U: { // 4215 clr.b (a5)
            next = 0x1249aU;
            const auto destination_address = r.address[5];
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1249aU: { // 60d8 bra.b $12474
            next = 0x1249cU;
            if (true) { next = 0x12474U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_0123ea(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x123eaU: { // 61000450 bsr.w $1283c
            next = 0x123eeU;
            const auto result = m.call(c, 253U, 0x123eaU, 0x1283cU, 0x123eeU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x123f2U: { // 6100073e bsr.w $12b32
            next = 0x123f6U;
            const auto result = m.call(c, 257U, 0x123f2U, 0x12b32U, 0x123f6U);
            if (result.status == TranslationStatus::complete && result.control == 8U)
                return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1241cU: { // 4eb900016372 jsr $16372.l
            next = 0x12422U;
            const auto result = m.call(c, 306U, 0x1241cU, 0x16372U, 0x12422U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12474U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1247aU;
            const auto result = m.call(c, 280U, 0x12474U, 0x15d24U, 0x1247aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1247aU: { // 4eb900015d3c jsr $15d3c.l
            next = 0x12480U;
            const auto result = m.call(c, 281U, 0x1247aU, 0x15d3cU, 0x12480U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12480U: { // 4eb900015df2 jsr $15df2.l
            next = 0x12486U;
            const auto result = m.call(c, 282U, 0x12480U, 0x15df2U, 0x12486U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12486U: { // 4e75 rts 
            next = 0x12488U;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x12486U, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_0123ea_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_0123ea_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_0123ea_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}

static bool cpu_b_stage_01249c_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x124a0U: { // 65000866 bcs.w $12d08
            next = 0x124a4U;
            if ((r.status & 1U) != 0U) { next = 0x12d08U; transfer_kind = 1U; }
            break;
        }
        case 0x124a8U: { // 6500085e bcs.w $12d08
            next = 0x124acU;
            if ((r.status & 1U) != 0U) { next = 0x12d08U; transfer_kind = 1U; }
            break;
        }
        case 0x124acU: { // 536d0046 subq.w #$1, $46(a5)
            next = 0x124b0U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x46U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x124b0U: { // 6f000856 ble.w $12d08
            next = 0x124b4U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x12d08U; transfer_kind = 1U; }
            break;
        }
        case 0x124b4U: { // 202d001e move.l $1e(a5), d0
            next = 0x124b8U;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x124b8U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x124bcU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x124bcU: { // 202d0026 move.l $26(a5), d0
            next = 0x124c0U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x124c0U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x124c4U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x124c4U: { // 086d0000003c bchg.b #$0, $3c(a5)
            next = 0x124caU;
            const auto destination_address = r.address[5] + 0x3cU;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x124caU: { // 672a beq.b $124f6
            next = 0x124ccU;
            if ((r.status & 4U) != 0U) { next = 0x124f6U; transfer_kind = 1U; }
            break;
        }
        case 0x124ccU: { // 582d003d addq.b #$4, $3d(a5)
            next = 0x124d0U;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x3dU;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x124d0U: { // 0c2d0040003d cmpi.b #$40, $3d(a5)
            next = 0x124d6U;
            const auto source_value = 0x40U;
            const auto destination_address = r.address[5] + 0x3dU;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x124d6U: { // 6b06 bmi.b $124de
            next = 0x124d8U;
            if ((r.status & 8U) != 0U) { next = 0x124deU; transfer_kind = 1U; }
            break;
        }
        case 0x124d8U: { // 1b7c0030003d move.b #$30, $3d(a5)
            next = 0x124deU;
            const auto value = 0x30U;
            const auto destination_address = r.address[5] + 0x3dU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x124deU: { // 41fa0a12 lea.l $12ef2(pc), a0
            next = 0x124e2U;
            const auto source_address = 0x12ef2U;
            r.address[0] = source_address;
            break;
        }
        case 0x124e2U: { // 7000 moveq #$0, d0
            next = 0x124e4U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x124e4U: { // 102d003d move.b $3d(a5), d0
            next = 0x124e8U;
            const auto source_address = r.address[5] + 0x3dU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x124e8U: { // d0c0 adda.w d0, a0
            next = 0x124eaU;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x124eaU: { // 3b580010 move.w (a0)+, $10(a5)
            next = 0x124eeU;
            const auto source_address = r.address[0];
            const auto value = m.word(source_address);
            r.address[0] += 2U;
            const auto destination_address = r.address[5] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x124eeU: { // 1b580001 move.b (a0)+, $1(a5)
            next = 0x124f2U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x124f2U: { // 1b580009 move.b (a0)+, $9(a5)
            next = 0x124f6U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            r.address[0] += 1U;
            const auto destination_address = r.address[5] + 0x9U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
    default: return false;
    }
    return true;
}
