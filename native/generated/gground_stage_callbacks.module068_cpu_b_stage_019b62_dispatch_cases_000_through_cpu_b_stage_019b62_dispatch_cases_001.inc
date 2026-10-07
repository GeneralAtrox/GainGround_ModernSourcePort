
static bool cpu_b_stage_019b62_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19b62U: { // 422d003f clr.b $3f(a5)
            next = 0x19b66U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x19b6cU: { // 65000098 bcs.w $19c06
            next = 0x19b70U;
            if ((r.status & 1U) != 0U) { next = 0x19c06U; transfer_kind = 1U; }
            break;
        }
        case 0x19b70U: { // 536d0066 subq.w #$1, $66(a5)
            next = 0x19b74U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x66U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19b74U: { // 6b000090 bmi.w $19c06
            next = 0x19b78U;
            if ((r.status & 8U) != 0U) { next = 0x19c06U; transfer_kind = 1U; }
            break;
        }
        case 0x19b78U: { // 322d005a move.w $5a(a5), d1
            next = 0x19b7cU;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19b7cU: { // d26d0072 add.w $72(a5), d1
            next = 0x19b80U;
            const auto source_address = r.address[5] + 0x72U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x19b80U: { // 024107ff andi.w #$7ff, d1
            next = 0x19b84U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[1];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x19b84U: { // 3b41005a move.w d1, $5a(a5)
            next = 0x19b88U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19b8eU: { // 48c0 ext.l d0
            next = 0x19b90U;
            r.data[0] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x19b90U: { // e780 asl.l #$3, d0
            next = 0x19b92U;
            auto value = r.data[0];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 3U; ++step) {
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
        case 0x19b92U: { // 2400 move.l d0, d2
            next = 0x19b94U;
            const auto value = r.data[0];
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x19b94U: { // d080 add.l d0, d0
            next = 0x19b96U;
            const auto source_value = r.data[0];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[0] = value;
            break;
        }
        case 0x19b96U: { // d082 add.l d2, d0
            next = 0x19b98U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[0] = value;
            break;
        }
        case 0x19b98U: { // 48c1 ext.l d1
            next = 0x19b9aU;
            r.data[1] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[1])));
            m.logic(r.data[1], 32U);
            break;
        }
        case 0x19b9aU: { // e781 asl.l #$3, d1
            next = 0x19b9cU;
            auto value = r.data[1];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 3U; ++step) {
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
        case 0x19b9cU: { // 2401 move.l d1, d2
            next = 0x19b9eU;
            const auto value = r.data[1];
            r.data[2] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x19b9eU: { // d281 add.l d1, d1
            next = 0x19ba0U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[1] = value;
            break;
        }
        case 0x19ba0U: { // d282 add.l d2, d1
            next = 0x19ba2U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[1] = value;
            break;
        }
        case 0x19ba2U: { // d1ad0012 add.l d0, $12(a5)
            next = 0x19ba6U;
            const auto source_value = r.data[0];
            const auto destination_address = r.address[5] + 0x12U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x19ba6U: { // d3ad001a add.l d1, $1a(a5)
            next = 0x19baaU;
            const auto source_value = r.data[1];
            const auto destination_address = r.address[5] + 0x1aU;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.add(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x19baaU: { // 04ad000080000016 subi.l #$8000, $16(a5)
            next = 0x19bb2U;
            const auto source_value = 0x8000U;
            const auto destination_address = r.address[5] + 0x16U;
            const auto destination_value = m.lng(destination_address);
            const auto value = m.sub(destination_value, source_value, 32U);
            m.word(destination_address + 2U, value);
            m.word(destination_address, (value) >> 16U);
            break;
        }
        case 0x19bb2U: { // 6b52 bmi.b $19c06
            next = 0x19bb4U;
            if ((r.status & 8U) != 0U) { next = 0x19c06U; transfer_kind = 1U; }
            break;
        }
        case 0x19bb4U: { // 302d0010 move.w $10(a5), d0
            next = 0x19bb8U;
            const auto source_address = r.address[5] + 0x10U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19bb8U: { // d07c0303 add.w #$303, d0
            next = 0x19bbcU;
            const auto source_value = 0x303U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19bbcU: { // b06d0060 cmp.w $60(a5), d0
            next = 0x19bc0U;
            const auto source_address = r.address[5] + 0x60U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19bc0U: { // 6e04 bgt.b $19bc6
            next = 0x19bc2U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19bc6U; transfer_kind = 1U; }
            break;
        }
        case 0x19bc2U: { // 3b400010 move.w d0, $10(a5)
            next = 0x19bc6U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x10U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19bc6U: { // 536d0068 subq.w #$1, $68(a5)
            next = 0x19bcaU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19bcaU: { // 6e24 bgt.b $19bf0
            next = 0x19bccU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x19bf0U; transfer_kind = 1U; }
            break;
        }
        case 0x19bccU: { // 3b7c00010068 move.w #$1, $68(a5)
            next = 0x19bd2U;
            const auto value = 0x1U;
            const auto destination_address = r.address[5] + 0x68U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_019b62_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19bd2U: { // 586d0064 addq.w #$4, $64(a5)
            next = 0x19bd6U;
            const auto source_value = 0x4U;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19bd6U: { // 026d000c0064 andi.w #$c, $64(a5)
            next = 0x19bdcU;
            const auto source_value = 0xcU;
            const auto destination_address = r.address[5] + 0x64U;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x19bdcU: { // 302d0064 move.w $64(a5), d0
            next = 0x19be0U;
            const auto source_address = r.address[5] + 0x64U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19be0U: { // 41fa1902 lea.l $1b4e4(pc), a0
            next = 0x19be4U;
            const auto source_address = 0x1b4e4U;
            r.address[0] = source_address;
            break;
        }
        case 0x19be4U: { // 3b7000000006 move.w (a0, d0.w), $6(a5)
            next = 0x19beaU;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19beaU: { // 1b7000030001 move.b $3(a0, d0.w), $1(a5)
            next = 0x19bf0U;
            const auto source_address = r.address[0] + 0x3U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19bf6U: { // 41fa18fc lea.l $1b4f4(pc), a0
            next = 0x19bfaU;
            const auto source_address = 0x1b4f4U;
            r.address[0] = source_address;
            break;
        }
    default: return false;
    }
    return true;
}
