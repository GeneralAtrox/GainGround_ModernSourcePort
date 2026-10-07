
static bool cpu_b_stage_012742_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x127d0U: { // 0c6e00050044 cmpi.w #$5, $44(a6)
            next = 0x127d6U;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[6] + 0x44U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x127d6U: { // 6608 bne.b $127e0
            next = 0x127d8U;
            if ((r.status & 4U) == 0U) { next = 0x127e0U; transfer_kind = 1U; }
            break;
        }
        case 0x127dcU: { // 6500052a bcs.w $12d08
            next = 0x127e0U;
            if ((r.status & 1U) != 0U) { next = 0x12d08U; transfer_kind = 1U; }
            break;
        }
        case 0x127e0U: { // 322d0048 move.w $48(a5), d1
            next = 0x127e4U;
            const auto source_address = r.address[5] + 0x48U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x127e4U: { // d26d0058 add.w $58(a5), d1
            next = 0x127e8U;
            const auto source_address = r.address[5] + 0x58U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x127e8U: { // 3b410058 move.w d1, $58(a5)
            next = 0x127ecU;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x58U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x127f2U: { // 48c0 ext.l d0
            next = 0x127f4U;
            r.data[0] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x127f4U: { // e980 asl.l #$4, d0
            next = 0x127f6U;
            auto value = r.data[0];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 4U; ++step) {
                const auto before = value;
                carry = (before & 0x80000000U) != 0U;
                value <<= 1U;
                overflow = overflow || ((before ^ value) & 0x80000000U) != 0U;
            }
            r.data[0] = value;
            m.logic(value, 32U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x13U) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U));
            break;
        }
        case 0x127f6U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x127faU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x127faU: { // 48c1 ext.l d1
            next = 0x127fcU;
            r.data[1] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[1])));
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x127fcU: { // e981 asl.l #$4, d1
            next = 0x127feU;
            auto value = r.data[1];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 4U; ++step) {
                const auto before = value;
                carry = (before & 0x80000000U) != 0U;
                value <<= 1U;
                overflow = overflow || ((before ^ value) & 0x80000000U) != 0U;
            }
            r.data[1] = value;
            m.logic(value, 32U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x13U) | (carry ? 0x11U : 0U) | (overflow ? 2U : 0U));
            break;
        }
        case 0x127feU: { // d3ad001a add.l d1, $1a(a5)
            next = 0x12802U;
            const auto source_value = r.data[1];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x12802U: { // 70fc moveq #$fc, d0
            next = 0x12804U;
            r.data[0] = 0xfffffffcU;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x12804U: { // 4a6d0048 tst.w $48(a5)
            next = 0x12808U;
            const auto destination_address = r.address[5] + 0x48U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x12808U: { // 6a02 bpl.b $1280c
            next = 0x1280aU;
            if ((r.status & 8U) == 0U) { next = 0x1280cU; transfer_kind = 1U; }
            break;
        }
        case 0x1280aU: { // 4440 neg.w d0
            next = 0x1280cU;
            const auto old = r.data[0];
            const auto value = m.sub(0U, old, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1280cU: { // d02d003d add.b $3d(a5), d0
            next = 0x12810U;
            const auto source_address = r.address[5] + 0x3dU;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x12810U: { // 0240001c andi.w #$1c, d0
            next = 0x12814U;
            const auto source_value = 0x1cU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x12814U: { // 1b40003d move.b d0, $3d(a5)
            next = 0x12818U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x3dU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x12818U: { // 41fa0dd0 lea.l $135ea(pc), a0
            next = 0x1281cU;
            const auto source_address = 0x135eaU;
            r.address[0] = source_address;
            break;
        }
        case 0x1281cU: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x12822U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12822U: { // 1b7000020001 move.b $2(a0, d0.w), $1(a5)
            next = 0x12828U;
            const auto source_address = r.address[0] + 0x2U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
    default: return false;
    }
    return true;
}

FunctionResult cpu_b_stage_012742(FunctionContext &c) noexcept {
    auto &r = c.registers;
    if (!c.host || c.cpu != 1U || c.state != 0x72U)
        return {TranslationStatus::contract_violation, 0U, r.program_counter};
    unverified::Machine m{*c.host, r, 1U, 0x72U};
    for (;;) {
        const auto pc = r.program_counter;
        auto next = pc;
        std::uint8_t transfer_kind = 0U;
        switch (pc) {
        case 0x12742U: { // 610000f8 bsr.w $1283c
            next = 0x12746U;
            const auto result = m.call(c, 253U, 0x12742U, 0x1283cU, 0x12746U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1274aU: { // 610003e6 bsr.w $12b32
            next = 0x1274eU;
            const auto result = m.call(c, 257U, 0x1274aU, 0x12b32U, 0x1274eU);
            if (result.status == TranslationStatus::complete && result.control == 8U) return FunctionResult::complete(1U, result.exit_program_counter);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x127b8U: { // 4e75 rts
            next = 0x127baU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x127b8U, r.program_counter)) return *event;
            return result;
            break;
        }
        case 0x127d8U: { // 610002f8 bsr.w $12ad2
            next = 0x127dcU;
            const auto result = m.call(c, 959U, 0x127d8U, 0x12ad2U, 0x127dcU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x127ecU: { // 4eb900016372 jsr $16372.l
            next = 0x127f2U;
            const auto result = m.call(c, 306U, 0x127ecU, 0x16372U, 0x127f2U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12828U: { // 4eb900015d24 jsr $15d24.l
            next = 0x1282eU;
            const auto result = m.call(c, 280U, 0x12828U, 0x15d24U, 0x1282eU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1282eU: { // 4eb900015d3c jsr $15d3c.l
            next = 0x12834U;
            const auto result = m.call(c, 281U, 0x1282eU, 0x15d3cU, 0x12834U);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x12834U: { // 4eb900015df2 jsr $15df2.l
            next = 0x1283aU;
            const auto result = m.call(c, 282U, 0x12834U, 0x15df2U, 0x1283aU);
            if (result.status != TranslationStatus::complete || result.control != 1U) return result;
            next = r.program_counter;
            break;
        }
        case 0x1283aU: { // 4e75 rts
            next = 0x1283cU;
            const auto result = m.ret();
            if (auto event = m.interrupt(c, 0x1283aU, r.program_counter)) return *event;
            return result;
            break;
        }
        default:
            if (!(cpu_b_stage_012742_dispatch_cases_000(m, c, pc, next, transfer_kind) || cpu_b_stage_012742_dispatch_cases_001(m, c, pc, next, transfer_kind))) return {TranslationStatus::contract_violation, 0U, pc};
            break;
        }
        if (m.unresolved_bus) return {TranslationStatus::contract_violation, 0U, pc};
        r.program_counter = next;
        if (auto event = m.interrupt(c, pc, next)) return *event;
        if (!cpu_b_stage_012742_contains_next_000(next)) return m.dispatch(c, pc, next, transfer_kind, m.state);
    }
}
