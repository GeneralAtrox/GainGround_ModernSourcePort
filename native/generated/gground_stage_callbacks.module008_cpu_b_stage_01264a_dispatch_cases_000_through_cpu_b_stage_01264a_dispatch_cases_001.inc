
static bool cpu_b_stage_01264a_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
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
        case 0x1263aU: { // 303c0014 move.w #$14, d0
            next = 0x1263eU;
            const auto value = 0x14U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1264eU: { // 65ea bcs.b $1263a
            next = 0x12650U;
            if ((r.status & 1U) != 0U) { next = 0x1263aU; transfer_kind = 1U; }
            break;
        }
        case 0x12654U: { // 65e4 bcs.b $1263a
            next = 0x12656U;
            if ((r.status & 1U) != 0U) { next = 0x1263aU; transfer_kind = 1U; }
            break;
        }
        case 0x12656U: { // 322d0058 move.w $58(a5), d1
            next = 0x1265aU;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1265aU: { // 0c410400 cmpi.w #$400, d1
            next = 0x1265eU;
            const auto source_value = 0x400U;
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1265eU: { // 652a bcs.b $1268a
            next = 0x12660U;
            if ((r.status & 1U) != 0U) { next = 0x1268aU; transfer_kind = 1U; }
            break;
        }
        case 0x12660U: { // d26d0046 add.w $46(a5), d1
            next = 0x12664U;
            const auto source_address = r.address[5] + 0x46U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x12664U: { // 024107ff andi.w #$7ff, d1
            next = 0x12668U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x12668U: { // 3b410058 move.w d1, $58(a5)
            next = 0x1266cU;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x58U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x12672U: { // 48c0 ext.l d0
            next = 0x12674U;
            r.data[0] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x12674U: { // eb80 asl.l #$5, d0
            next = 0x12676U;
            auto value = r.data[0];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 5U; ++step) {
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
        case 0x12676U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x1267aU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1267aU: { // 48c1 ext.l d1
            next = 0x1267cU;
            r.data[1] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[1])));
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1267cU: { // e581 asl.l #$2, d1
            next = 0x1267eU;
            auto value = r.data[1];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 2U; ++step) {
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
        case 0x1267eU: { // 2001 move.l d1, d0
            next = 0x12680U;
            const auto value = r.data[1];
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x12680U: { // d281 add.l d1, d1
            next = 0x12682U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[1] = value;
            break;
        }
        case 0x12682U: { // d280 add.l d0, d1
            next = 0x12684U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[1] = value;
            break;
        }
        case 0x12684U: { // d3ad001a add.l d1, $1a(a5)
            next = 0x12688U;
            const auto source_value = r.data[1];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x12688U: { // 6010 bra.b $1269a
            next = 0x1268aU;
            if (true) { next = 0x1269aU; transfer_kind = 1U; }
            break;
        }
        case 0x1268aU: { // 202d001e move.l $1e(a5), d0
            next = 0x1268eU;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1268eU: { // d1ad0012 add.l d0, $12(a5)
            next = 0x12692U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x12692U: { // 202d0026 move.l $26(a5), d0
            next = 0x12696U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x12696U: { // d1ad001a add.l d0, $1a(a5)
            next = 0x1269aU;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1269aU: { // 086d0002003d bchg.b #$2, $3d(a5)
            next = 0x126a0U;
            const auto destination_address = r.address[5] + 0x3dU;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old ^ bit_mask);
            break;
        }
        case 0x126a0U: { // 302d0058 move.w $58(a5), d0
            next = 0x126a4U;
            const auto source_address = r.address[5] + 0x58U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_01264a_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x126a4U: { // d07c0080 add.w #$80, d0
            next = 0x126a8U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x126a8U: { // 02400700 andi.w #$700, d0
            next = 0x126acU;
            const auto source_value = 0x700U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x126acU: { // 6000ff62 bra.w $12610
            next = 0x126b0U;
            if (true) { next = 0x12610U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
