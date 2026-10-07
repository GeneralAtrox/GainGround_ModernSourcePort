
static bool cpu_b_stage_012598_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1259cU: { // 6500009c bcs.w $1263a
            next = 0x125a0U;
            if ((r.status & 1U) != 0U) { next = 0x1263aU; transfer_kind = 1U; }
            break;
        }
        case 0x125a4U: { // 65000094 bcs.w $1263a
            next = 0x125a8U;
            if ((r.status & 1U) != 0U) { next = 0x1263aU; transfer_kind = 1U; }
            break;
        }
        case 0x125a8U: { // 41fa0eb0 lea.l $1345a(pc), a0
            next = 0x125acU;
            const auto source_address = 0x1345aU;
            r.address[0] = source_address;
            break;
        }
        case 0x125acU: { // 30380c02 move.w $c02.w, d0
            next = 0x125b0U;
            const auto source_address = 0xc02U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x125b0U: { // d040 add.w d0, d0
            next = 0x125b2U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x125b2U: { // 30300000 move.w (a0, d0.w), d0
            next = 0x125b6U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x125b6U: { // b06d0016 cmp.w $16(a5), d0
            next = 0x125baU;
            const auto source_address = r.address[5] + 0x16U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x125baU: { // 6e06 bgt.b $125c2
            next = 0x125bcU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x125c2U; transfer_kind = 1U; }
            break;
        }
        case 0x125bcU: { // 42ad0022 clr.l $22(a5)
            next = 0x125c0U;
            const auto destination_address = r.address[5] + 0x22U;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0x125c0U: { // 601a bra.b $125dc
            next = 0x125c2U;
            if (true) { next = 0x125dcU; transfer_kind = 1U; }
            break;
        }
        case 0x125c2U: { // 04ad000020000022 subi.l #$2000, $22(a5)
            next = 0x125caU;
            const auto source_value = 0x2000U;
            const auto destination_address = r.address[5] + 0x22U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.sub(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x125caU: { // 0cad000020000022 cmpi.l #$2000, $22(a5)
            next = 0x125d2U;
            const auto source_value = 0x2000U;
            const auto destination_address = r.address[5] + 0x22U;
            const auto destination_value = m.lng(destination_address);
            (void)m.sub(destination_value, source_value, 32U, true);
            break;
        }
        case 0x125d2U: { // 6a08 bpl.b $125dc
            next = 0x125d4U;
            if ((r.status & 8U) == 0U) { next = 0x125dcU; transfer_kind = 1U; }
            break;
        }
        case 0x125d4U: { // 2b7c000020000022 move.l #$2000, $22(a5)
            next = 0x125dcU;
            const auto value = 0x2000U;
            const auto destination_address = r.address[5] + 0x22U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x125dcU: { // 202d001e move.l $1e(a5), d0
            next = 0x125e0U;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x125e0U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x125e4U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x125e4U: { // 202d0022 move.l $22(a5), d0
            next = 0x125e8U;
            const auto source_address = r.address[5] + 0x22U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x125e8U: { // d1ad0016 add.l d0, $16(a5)
            next = 0x125ecU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x16U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x125ecU: { // 202d0026 move.l $26(a5), d0
            next = 0x125f0U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x125f0U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x125f4U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x125f4U: { // 302d0016 move.w $16(a5), d0
            next = 0x125f8U;
            const auto source_address = r.address[5] + 0x16U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x125f8U: { // e240 asr.w #$1, d0
            next = 0x125faU;
            m.shift_word(0U, 1U, false, true);
            break;
        }
        case 0x125faU: { // d07c003f add.w #$3f, d0
            next = 0x125feU;
            const auto source_value = 0x3fU;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x125feU: { // 1b400010 move.b d0, $10(a5)
            next = 0x12602U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x10U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x12602U: { // 1b400011 move.b d0, $11(a5)
            next = 0x12606U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x11U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x12606U: { // 086d0002003d bchg.b #$2, $3d(a5)
            next = 0x1260cU;
            const auto destination_address = r.address[5] + 0x3dU;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x1260cU: { // 302d0058 move.w $58(a5), d0
            next = 0x12610U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12610U: { // ea48 lsr.w #$5, d0
            next = 0x12612U;
            m.shift_word(0U, 5U, false, false);
            break;
        }
        case 0x12612U: { // d02d003d add.b $3d(a5), d0
            next = 0x12616U;
            const auto source_address = r.address[5] + 0x3dU;
            const auto source_value = m.byte(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(0U, value);
            break;
        }
        case 0x12616U: { // 41fa0e92 lea.l $134aa(pc), a0
            next = 0x1261aU;
            const auto source_address = 0x134aaU;
            r.address[0] = source_address;
            break;
        }
        case 0x1261aU: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x12620U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12620U: { // 1b7000020001 move.b $2(a0, d0.w), $1(a5)
            next = 0x12626U;
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

static bool cpu_b_stage_012598_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1263aU: { // 303c0014 move.w #$14, d0
            next = 0x1263eU;
            const auto value = 0x14U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
