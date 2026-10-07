
static bool cpu_b_stage_01a1f6_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a1f6U: { // 3c6d0036 movea.w $36(a5), a6
            next = 0x1a1faU;
            const auto source_address = r.address[5] + 0x36U;
            const auto value = m.word(source_address);
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x1a1faU: { // 4a16 tst.b (a6)
            next = 0x1a1fcU;
            const auto destination_address = r.address[6];
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1a1fcU: { // 6b04 bmi.b $1a202
            next = 0x1a1feU;
            if ((r.status & 8U) != 0U) { next = 0x1a202U; transfer_kind = 1U; }
            break;
        }
        case 0x1a1feU: { // 4255 clr.w (a5)
            next = 0x1a200U;
            const auto destination_address = r.address[5];
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x1a202U: { // 286e005c movea.l $5c(a6), a4
            next = 0x1a206U;
            const auto source_address = r.address[6] + 0x5cU;
            const auto value = m.lng(source_address);
            r.address[4] = value;
            break;
        }
        case 0x1a206U: { // 5c4c addq.w #$6, a4
            next = 0x1a208U;
            const auto source_value = 0x6U;
            r.address[4] += source_value;
            break;
        }
        case 0x1a208U: { // d8ed003a adda.w $3a(a5), a4
            next = 0x1a20cU;
            const auto source_address = r.address[5] + 0x3aU;
            const auto source_value = m.word(source_address);
            r.address[4] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1a20cU: { // 3b5c0006 move.w (a4)+, $6(a5)
            next = 0x1a210U;
            const auto source_address = r.address[4];
            const auto value = m.word(source_address);
            r.address[4] += 2U;
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a210U: { // 6b0000b2 bmi.w $1a2c4
            next = 0x1a214U;
            if ((r.status & 8U) != 0U) { next = 0x1a2c4U; transfer_kind = 1U; }
            break;
        }
        case 0x1a214U: { // 4a6e0058 tst.w $58(a6)
            next = 0x1a218U;
            const auto destination_address = r.address[6] + 0x58U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1a218U: { // 663a bne.b $1a254
            next = 0x1a21aU;
            if ((r.status & 4U) == 0U) { next = 0x1a254U; transfer_kind = 1U; }
            break;
        }
        case 0x1a21aU: { // 101c move.b (a4)+, d0
            next = 0x1a21cU;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a21cU: { // 4880 ext.w d0
            next = 0x1a21eU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a21eU: { // d06e000c add.w $c(a6), d0
            next = 0x1a222U;
            const auto source_address = r.address[6] + 0xcU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a222U: { // b06d000c cmp.w $c(a5), d0
            next = 0x1a226U;
            const auto source_address = r.address[5] + 0xcU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a226U: { // 6708 beq.b $1a230
            next = 0x1a228U;
            if ((r.status & 4U) != 0U) { next = 0x1a230U; transfer_kind = 1U; }
            break;
        }
        case 0x1a228U: { // 202d0026 move.l $26(a5), d0
            next = 0x1a22cU;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a22cU: { // d1ad0016 add.l d0, $16(a5)
            next = 0x1a230U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x16U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a230U: { // 101c move.b (a4)+, d0
            next = 0x1a232U;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a232U: { // 4880 ext.w d0
            next = 0x1a234U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a234U: { // d06e000e add.w $e(a6), d0
            next = 0x1a238U;
            const auto source_address = r.address[6] + 0xeU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a238U: { // b06d000e cmp.w $e(a5), d0
            next = 0x1a23cU;
            const auto source_address = r.address[5] + 0xeU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1a23cU: { // 6708 beq.b $1a246
            next = 0x1a23eU;
            if ((r.status & 4U) != 0U) { next = 0x1a246U; transfer_kind = 1U; }
            break;
        }
        case 0x1a23eU: { // 202d001e move.l $1e(a5), d0
            next = 0x1a242U;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1a242U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x1a246U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x1a246U: { // 3b6d0012000e move.w $12(a5), $e(a5)
            next = 0x1a24cU;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0xeU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a24cU: { // 3b6d0016000c move.w $16(a5), $c(a5)
            next = 0x1a252U;
            const auto source_address = r.address[5] + 0x16U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a252U: { // 6018 bra.b $1a26c
            next = 0x1a254U;
            if (true) { next = 0x1a26cU; transfer_kind = 1U; }
            break;
        }
        case 0x1a254U: { // 101c move.b (a4)+, d0
            next = 0x1a256U;
            const auto source_address = r.address[4];
            const auto value = m.byte(source_address);
            r.address[4] += 1U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a256U: { // 4880 ext.w d0
            next = 0x1a258U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[0])));
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a258U: { // d06e000c add.w $c(a6), d0
            next = 0x1a25cU;
            const auto source_address = r.address[6] + 0xcU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a25cU: { // 3b40000c move.w d0, $c(a5)
            next = 0x1a260U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0xcU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
