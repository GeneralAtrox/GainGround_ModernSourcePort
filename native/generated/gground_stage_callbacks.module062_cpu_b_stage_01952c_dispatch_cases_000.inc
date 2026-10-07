
static bool cpu_b_stage_01952c_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1952cU: { // 4a6d0040 tst.w $40(a5)
            next = 0x19530U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x19530U: { // 660001c2 bne.w $196f4
            next = 0x19534U;
            if ((r.status & 4U) == 0U) { next = 0x196f4U; transfer_kind = 1U; }
            break;
        }
        case 0x19534U: { // 302d0048 move.w $48(a5), d0
            next = 0x19538U;
            const auto source_address = r.address[5] + 0x48U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19538U: { // e540 asl.w #$2, d0
            next = 0x1953aU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1953aU: { // 4efb0002 jmp $1953e(pc, d0.w)
            next = 0x1953eU;
            next = (0x1953eU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x1953eU: { // 6000001e bra.w $1955e
            next = 0x19542U;
            if (true) { next = 0x1955eU; transfer_kind = 1U; }
            break;
        }
        case 0x19542U: { // 600000bc bra.w $19600
            next = 0x19546U;
            if (true) { next = 0x19600U; transfer_kind = 1U; }
            break;
        }
        case 0x19546U: { // 600000ce bra.w $19616
            next = 0x1954aU;
            if (true) { next = 0x19616U; transfer_kind = 1U; }
            break;
        }
        case 0x1954aU: { // 600000e2 bra.w $1962e
            next = 0x1954eU;
            if (true) { next = 0x1962eU; transfer_kind = 1U; }
            break;
        }
        case 0x1954eU: { // 600000fc bra.w $1964c
            next = 0x19552U;
            if (true) { next = 0x1964cU; transfer_kind = 1U; }
            break;
        }
        case 0x19552U: { // 60000128 bra.w $1967c
            next = 0x19556U;
            if (true) { next = 0x1967cU; transfer_kind = 1U; }
            break;
        }
        case 0x19556U: { // 60000154 bra.w $196ac
            next = 0x1955aU;
            if (true) { next = 0x196acU; transfer_kind = 1U; }
            break;
        }
        case 0x1955aU: { // 60000178 bra.w $196d4
            next = 0x1955eU;
            if (true) { next = 0x196d4U; transfer_kind = 1U; }
            break;
        }
        case 0x1955eU: { // 536d0054 subq.w #$1, $54(a5)
            next = 0x19562U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19562U: { // 6a28 bpl.b $1958c
            next = 0x19564U;
            if ((r.status & 8U) == 0U) { next = 0x1958cU; transfer_kind = 1U; }
            break;
        }
        case 0x19564U: { // 3b7c00020048 move.w #$2, $48(a5)
            next = 0x1956aU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x48U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1956aU: { // 3b7c00020040 move.w #$2, $40(a5)
            next = 0x19570U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x40U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19570U: { // 426d0064 clr.w $64(a5)
            next = 0x19574U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x19574U: { // 3b7c00040068 move.w #$4, $68(a5)
            next = 0x1957aU;
            const auto value = 0x4U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1957aU: { // 2b7c000360000022 move.l #$36000, $22(a5)
            next = 0x19582U;
            const auto value = 0x36000U;
            const auto destination_address = r.address[5] + 0x22U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x19582U: { // 2b7c000200000026 move.l #$20000, $26(a5)
            next = 0x1958aU;
            const auto value = 0x20000U;
            const auto destination_address = r.address[5] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1958cU: { // 302d0052 move.w $52(a5), d0
            next = 0x19590U;
            const auto source_address = r.address[5] + 0x52U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19590U: { // d07c0080 add.w #$80, d0
            next = 0x19594U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19594U: { // 0c400100 cmpi.w #$100, d0
            next = 0x19598U;
            const auto source_value = 0x100U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19598U: { // 6f02 ble.b $1959c
            next = 0x1959aU;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1959cU; transfer_kind = 1U; }
            break;
        }
        case 0x1959aU: { // 7000 moveq #$0, d0
            next = 0x1959cU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1959cU: { // 3b400052 move.w d0, $52(a5)
            next = 0x195a0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x52U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x195a0U: { // 4df81480 lea.l $1480.w, a6
            next = 0x195a4U;
            const auto source_address = 0x1480U;
            r.address[6] = source_address;
            break;
        }
        case 0x195a4U: { // dcc0 adda.w d0, a6
            next = 0x195a6U;
            const auto source_value = r.data[0];
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x195a6U: { // 0c6e00050044 cmpi.w #$5, $44(a6)
            next = 0x195acU;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[6] + 0x44U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x195acU: { // 6622 bne.b $195d0
            next = 0x195aeU;
            if ((r.status & 4U) == 0U) { next = 0x195d0U; transfer_kind = 1U; }
            break;
        }
        case 0x195aeU: { // 302e0012 move.w $12(a6), d0
            next = 0x195b2U;
            const auto source_address = r.address[6] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x195b2U: { // 322e001a move.w $1a(a6), d1
            next = 0x195b6U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x195b6U: { // 906d0012 sub.w $12(a5), d0
            next = 0x195baU;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x195baU: { // 926d001a sub.w $1a(a5), d1
            next = 0x195beU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x195caU: { // 3b410056 move.w d1, $56(a5)
            next = 0x195ceU;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x195ceU: { // 6018 bra.b $195e8
            next = 0x195d0U;
            if (true) { next = 0x195e8U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
