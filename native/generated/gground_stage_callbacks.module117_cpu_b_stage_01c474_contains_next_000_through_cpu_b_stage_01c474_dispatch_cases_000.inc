
static bool cpu_b_stage_01c474_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x1c474U:
    case 0x1c478U:
    case 0x1c47aU:
    case 0x1c47eU:
    case 0x1c482U:
    case 0x1c484U:
    case 0x1c488U:
    case 0x1c48cU:
    case 0x1c492U:
    case 0x1c494U:
    case 0x1c49aU:
    case 0x1c49cU:
    case 0x1c4a0U:
    case 0x1c4a4U:
    case 0x1c4a6U:
    case 0x1c4a8U:
    case 0x1c4aeU:
    case 0x1c4b0U:
    case 0x1c4b6U:
    case 0x1c4baU:
    case 0x1c4bcU:
    case 0x1c4c0U:
    case 0x1c4c2U:
    case 0x1c4c4U:
    case 0x1c4c6U:
    case 0x1c4c8U:
    case 0x1c4caU:
    case 0x1c4ccU:
    case 0x1c4ceU:
    case 0x1c4d0U:
    case 0x1c4d4U:
    case 0x1c4d6U:
    case 0x1c4d8U:
    case 0x1c4daU:
    case 0x1c4dcU:
    case 0x1c4deU:
    case 0x1c4e2U:
    case 0x1c4e8U:
    case 0x1c4ecU:
    case 0x1c4f0U:
    case 0x1c4f2U:
    case 0x1c4f6U:
    case 0x1c4faU:
    case 0x1c500U:
    case 0x1c502U:
    case 0x1c508U:
    case 0x1c50eU:
    case 0x1c514U:
    case 0x1c51aU:
        return true;
    default: return false;
    }
}

static bool cpu_b_stage_01c474_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c474U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1c478U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c478U: { // 6704 beq.b $1c47e
            next = 0x1c47aU;
            if ((r.status & 4U) != 0U) { next = 0x1c47eU; transfer_kind = 1U; }
            break;
        }
        case 0x1c47aU: { // 6000155e bra.w $1d9da
            next = 0x1c47eU;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1c47eU: { // 4a2d003e tst.b $3e(a5)
            next = 0x1c482U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c482U: { // 6704 beq.b $1c488
            next = 0x1c484U;
            if ((r.status & 4U) != 0U) { next = 0x1c488U; transfer_kind = 1U; }
            break;
        }
        case 0x1c484U: { // 60001564 bra.w $1d9ea
            next = 0x1c488U;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1c488U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1c48cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c48cU: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1c492U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c492U: { // 6708 beq.b $1c49c
            next = 0x1c494U;
            if ((r.status & 4U) != 0U) { next = 0x1c49cU; transfer_kind = 1U; }
            break;
        }
        case 0x1c494U: { // 08ed00010040 bset.b #$1, $40(a5)
            next = 0x1c49aU;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c49cU: { // 206d006e movea.l $6e(a5), a0
            next = 0x1c4a0U;
            const auto source_address = r.address[5] + 0x6eU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c4a0U: { // 30280008 move.w $8(a0), d0
            next = 0x1c4a4U;
            const auto source_address = r.address[0] + 0x8U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c4a4U: { // d040 add.w d0, d0
            next = 0x1c4a6U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c4a6U: { // d040 add.w d0, d0
            next = 0x1c4a8U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1c4a8U: { // 41f900032dec lea.l $32dec.l, a0
            next = 0x1c4aeU;
            const auto source_address = 0x32decU;
            r.address[0] = source_address;
            break;
        }
        case 0x1c4aeU: { // d0c0 adda.w d0, a0
            next = 0x1c4b0U;
            const auto source_value = r.data[0];
            r.address[0] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x1c4b0U: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1c4b6U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c4b6U: { // 302d0012 move.w $12(a5), d0
            next = 0x1c4baU;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c4baU: { // 7200 moveq #$0, d1
            next = 0x1c4bcU;
            r.data[1] = 0x0U;
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x1c4bcU: { // 122d0039 move.b $39(a5), d1
            next = 0x1c4c0U;
            const auto source_address = r.address[5] + 0x39U;
            const auto value = m.byte(source_address);
            m.db(1U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c4c0U: { // 670e beq.b $1c4d0
            next = 0x1c4c2U;
            if ((r.status & 4U) != 0U) { next = 0x1c4d0U; transfer_kind = 1U; }
            break;
        }
        case 0x1c4c2U: { // 1410 move.b (a0), d2
            next = 0x1c4c4U;
            const auto source_address = r.address[0];
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c4c4U: { // 5242 addq.w #$1, d2
            next = 0x1c4c6U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[2];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1c4c6U: { // 4882 ext.w d2
            next = 0x1c4c8U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[2])));
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c4c8U: { // e742 asl.w #$3, d2
            next = 0x1c4caU;
            m.asl_word(2U, 3U);
            break;
        }
        case 0x1c4caU: { // b042 cmp.w d2, d0
            next = 0x1c4ccU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1c4ccU: { // 6e1a bgt.b $1c4e8
            next = 0x1c4ceU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1c4e8U; transfer_kind = 1U; }
            break;
        }
        case 0x1c4ceU: { // 600e bra.b $1c4de
            next = 0x1c4d0U;
            if (true) { next = 0x1c4deU; transfer_kind = 1U; }
            break;
        }
        case 0x1c4d0U: { // 14280001 move.b $1(a0), d2
            next = 0x1c4d4U;
            const auto source_address = r.address[0] + 0x1U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c4d4U: { // 4882 ext.w d2
            next = 0x1c4d6U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[2])));
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c4d6U: { // 5342 subq.w #$1, d2
            next = 0x1c4d8U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[2];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(2U, value);
            break;
        }
        case 0x1c4d8U: { // e742 asl.w #$3, d2
            next = 0x1c4daU;
            m.asl_word(2U, 3U);
            break;
        }
        case 0x1c4daU: { // b042 cmp.w d2, d0
            next = 0x1c4dcU;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1c4dcU: { // 6d0a blt.b $1c4e8
            next = 0x1c4deU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1c4e8U; transfer_kind = 1U; }
            break;
        }
        case 0x1c4deU: { // 06010040 addi.b #$40, d1
            next = 0x1c4e2U;
            const auto source_value = 0x40U;
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 8U);
            m.db(1U, value);
            break;
        }
        case 0x1c4e2U: { // 08ed00050041 bset.b #$5, $41(a5)
            next = 0x1c4e8U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}
