
static bool cpu_b_stage_0191a8_contains_next_000(std::uint32_t next) noexcept {
    switch (next) {
    case 0x191a8U:
    case 0x191acU:
    case 0x191aeU:
    case 0x191b0U:
    case 0x191b4U:
    case 0x191b8U:
    case 0x191bcU:
    case 0x191c0U:
    case 0x191c2U:
    case 0x191c6U:
    case 0x191caU:
    case 0x191ccU:
    case 0x191d0U:
    case 0x191d2U:
    case 0x191d6U:
    case 0x191d8U:
    case 0x191daU:
    case 0x191dcU:
    case 0x191e0U:
    case 0x191e4U:
    case 0x191e8U:
    case 0x191eaU:
    case 0x191eeU:
    case 0x191f0U:
    case 0x191f2U:
    case 0x191f6U:
    case 0x191f8U:
    case 0x191fcU:
    case 0x19200U:
    case 0x19204U:
    case 0x19208U:
    case 0x1920cU:
    case 0x19210U:
    case 0x19214U:
    case 0x19218U:
    case 0x1921cU:
    case 0x19220U:
    case 0x19224U:
    case 0x19228U:
    case 0x1922cU:
    case 0x19230U:
    case 0x19234U:
    case 0x19238U:
    case 0x1923cU:
    case 0x19240U:
    case 0x19244U:
    case 0x19248U:
    case 0x1924cU:
    case 0x19250U:
    case 0x19254U:
    case 0x19256U:
    case 0x19258U:
    case 0x1925cU:
    case 0x1925eU:
    case 0x19262U:
    case 0x19264U:
    case 0x19268U:
    case 0x1926cU:
    case 0x19270U:
    case 0x19272U:
    case 0x19276U:
    case 0x1927aU:
    case 0x1927eU:
    case 0x19280U:
    case 0x19284U:
        return true;
    default: return false;
    }
}

static bool cpu_b_stage_0191a8_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x191a8U: { // 47f86c00 lea.l $6c00.w, a3
            next = 0x191acU;
            const auto source_address = 0x6c00U;
            r.address[3] = source_address;
            break;
        }
        case 0x191acU: { // 4a5b tst.w (a3)+
            next = 0x191aeU;
            const auto destination_address = r.address[3];
            const auto value = m.word(destination_address);
            r.address[3] += 2U;
            m.logic(value, 16U);
            break;
        }
        case 0x191aeU: { // 6b04 bmi.b $191b4
            next = 0x191b0U;
            if ((r.status & 8U) != 0U) { next = 0x191b4U; transfer_kind = 1U; }
            break;
        }
        case 0x191b0U: { // 47f87002 lea.l $7002.w, a3
            next = 0x191b4U;
            const auto source_address = 0x7002U;
            r.address[3] = source_address;
            break;
        }
        case 0x191b4U: { // 302d001a move.w $1a(a5), d0
            next = 0x191b8U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x191b8U: { // 0c400200 cmpi.w #$200, d0
            next = 0x191bcU;
            const auto source_value = 0x200U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x191bcU: { // 640000c6 bcc.w $19284
            next = 0x191c0U;
            if ((r.status & 1U) == 0U) { next = 0x19284U; transfer_kind = 1U; }
            break;
        }
        case 0x191c0U: { // 3e00 move.w d0, d7
            next = 0x191c2U;
            const auto value = r.data[0];
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x191c2U: { // 06470020 addi.w #$20, d7
            next = 0x191c6U;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[7];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x191c6U: { // 04400020 subi.w #$20, d0
            next = 0x191caU;
            const auto source_value = 0x20U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x191caU: { // 6b10 bmi.b $191dc
            next = 0x191ccU;
            if ((r.status & 8U) != 0U) { next = 0x191dcU; transfer_kind = 1U; }
            break;
        }
        case 0x191ccU: { // 0c4701ff cmpi.w #$1ff, d7
            next = 0x191d0U;
            const auto source_value = 0x1ffU;
            const auto destination_value = r.data[7];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x191d0U: { // 6f04 ble.b $191d6
            next = 0x191d2U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x191d6U; transfer_kind = 1U; }
            break;
        }
        case 0x191d2U: { // 3e3c01ff move.w #$1ff, d7
            next = 0x191d6U;
            const auto value = 0x1ffU;
            m.dw(7U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x191d6U: { // 9e40 sub.w d0, d7
            next = 0x191d8U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[7];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(7U, value);
            break;
        }
        case 0x191d8U: { // d040 add.w d0, d0
            next = 0x191daU;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x191daU: { // d6c0 adda.w d0, a3
            next = 0x191dcU;
            const auto source_value = r.data[0];
            r.address[3] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x191dcU: { // 202d001e move.l $1e(a5), d0
            next = 0x191e0U;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x191e0U: { // 80ad0026 or.l $26(a5), d0
            next = 0x191e4U;
            const auto source_address = r.address[5] + 0x26U;
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[0];
            const auto value = destination_value | source_value;
            m.logic(value, 32U);
            r.data[0] = value;
            break;
        }
        case 0x191e4U: { // 6700009e beq.w $19284
            next = 0x191e8U;
            if ((r.status & 4U) != 0U) { next = 0x19284U; transfer_kind = 1U; }
            break;
        }
        case 0x191e8U: { // 301b move.w (a3)+, d0
            next = 0x191eaU;
            const auto source_address = r.address[3];
            const auto value = m.word(source_address);
            r.address[3] += 2U;
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x191eaU: { // 67000094 beq.w $19280
            next = 0x191eeU;
            if ((r.status & 4U) != 0U) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x191eeU: { // 3c40 movea.w d0, a6
            next = 0x191f0U;
            const auto value = r.data[0];
            r.address[6] = static_cast<std::uint32_t>(static_cast<std::int16_t>(value));
            break;
        }
        case 0x191f0U: { // 7000 moveq #$0, d0
            next = 0x191f2U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x191f2U: { // 102e000b move.b $b(a6), d0
            next = 0x191f6U;
            const auto source_address = r.address[6] + 0xbU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x191f6U: { // e540 asl.w #$2, d0
            next = 0x191f8U;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x191f8U: { // 4efb0002 jmp $191fc(pc, d0.w)
            next = 0x191fcU;
            next = (0x191fcU + static_cast<std::int16_t>(r.data[0])) & 0xffffffU;
            transfer_kind = 1U;
            break;
        }
        case 0x191fcU: { // 60000082 bra.w $19280
            next = 0x19200U;
            if (true) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x19200U: { // 6000007e bra.w $19280
            next = 0x19204U;
            if (true) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x19204U: { // 6000007a bra.w $19280
            next = 0x19208U;
            if (true) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x19208U: { // 60000076 bra.w $19280
            next = 0x1920cU;
            if (true) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x1920cU: { // 6000002a bra.w $19238
            next = 0x19210U;
            if (true) { next = 0x19238U; transfer_kind = 1U; }
            break;
        }
        case 0x19210U: { // 6000006e bra.w $19280
            next = 0x19214U;
            if (true) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x19214U: { // 6000006a bra.w $19280
            next = 0x19218U;
            if (true) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x19218U: { // 60000066 bra.w $19280
            next = 0x1921cU;
            if (true) { next = 0x19280U; transfer_kind = 1U; }
            break;
        }
        case 0x1921cU: { // 6000003a bra.w $19258
            next = 0x19220U;
            if (true) { next = 0x19258U; transfer_kind = 1U; }
            break;
        }
        case 0x19220U: { // 60000036 bra.w $19258
            next = 0x19224U;
            if (true) { next = 0x19258U; transfer_kind = 1U; }
            break;
        }
        case 0x19224U: { // 60000012 bra.w $19238
            next = 0x19228U;
            if (true) { next = 0x19238U; transfer_kind = 1U; }
            break;
        }
        case 0x19228U: { // 60000002 bra.w $1922c
            next = 0x1922cU;
            if (true) { next = 0x1922cU; transfer_kind = 1U; }
            break;
        }
        case 0x1922cU: { // 102d000a move.b $a(a5), d0
            next = 0x19230U;
            const auto source_address = r.address[5] + 0xaU;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
    default: return false;
    }
    return true;
}
