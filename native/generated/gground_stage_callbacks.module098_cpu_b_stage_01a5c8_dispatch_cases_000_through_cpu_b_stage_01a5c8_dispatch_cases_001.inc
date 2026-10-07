
static bool cpu_b_stage_01a5c8_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a5c8U: { // 3a2d005a move.w $5a(a5), d5
            next = 0x1a5ccU;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            m.dw(5U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a5ccU: { // 7c07 moveq #$7, d6
            next = 0x1a5ceU;
            r.data[6] = 0x7U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x1a5ceU: { // 4dd5 lea.l (a5), a6
            next = 0x1a5d0U;
            const auto source_address = r.address[5];
            r.address[6] = source_address;
            break;
        }
        case 0x1a5d0U: { // 600a bra.b $1a5dc
            next = 0x1a5d2U;
            if (true) { next = 0x1a5dcU; transfer_kind = 1U; }
            break;
        }
        case 0x1a5d6U: { // 4a40 tst.w d0
            next = 0x1a5d8U;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x1a5d8U: { // 6b000088 bmi.w $1a662
            next = 0x1a5dcU;
            if ((r.status & 8U) != 0U) { next = 0x1a662U; transfer_kind = 1U; }
            break;
        }
        case 0x1a5dcU: { // 3cbc8000 move.w #$8000, (a6)
            next = 0x1a5e0U;
            const auto value = 0x8000U;
            const auto destination_address = r.address[6];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a5e0U: { // 2d7c0001a6640002 move.l #$1a664, $2(a6)
            next = 0x1a5e8U;
            const auto value = 0x1a664U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a5e8U: { // 3d7c04b20006 move.w #$4b2, $6(a6)
            next = 0x1a5eeU;
            const auto value = 0x4b2U;
            const auto destination_address = r.address[6] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a5eeU: { // 3006 move.w d6, d0
            next = 0x1a5f0U;
            const auto value = r.data[6];
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a5f0U: { // 02400003 andi.w #$3, d0
            next = 0x1a5f4U;
            const auto source_value = 0x3U;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a5f4U: { // d07c0c90 add.w #$c90, d0
            next = 0x1a5f8U;
            const auto source_value = 0xc90U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1a5f8U: { // 3d400008 move.w d0, $8(a6)
            next = 0x1a5fcU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a5fcU: { // 1d7c0007000b move.b #$7, $b(a6)
            next = 0x1a602U;
            const auto value = 0x7U;
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1a602U: { // 422e003f clr.b $3f(a6)
            next = 0x1a606U;
            const auto destination_address = r.address[6] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1a606U: { // 3d7c000a0066 move.w #$a, $66(a6)
            next = 0x1a60cU;
            const auto value = 0xaU;
            const auto destination_address = r.address[6] + 0x66U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a60cU: { // 302d0012 move.w $12(a5), d0
            next = 0x1a610U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a610U: { // 3d400012 move.w d0, $12(a6)
            next = 0x1a614U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a614U: { // 3d40002a move.w d0, $2a(a6)
            next = 0x1a618U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a618U: { // 3d40002c move.w d0, $2c(a6)
            next = 0x1a61cU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a61cU: { // 3d7c00080016 move.w #$8, $16(a6)
            next = 0x1a622U;
            const auto value = 0x8U;
            const auto destination_address = r.address[6] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a622U: { // 3d7c0008002e move.w #$8, $2e(a6)
            next = 0x1a628U;
            const auto value = 0x8U;
            const auto destination_address = r.address[6] + 0x2eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a628U: { // 3d7c00080030 move.w #$8, $30(a6)
            next = 0x1a62eU;
            const auto value = 0x8U;
            const auto destination_address = r.address[6] + 0x30U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a62eU: { // 302d001a move.w $1a(a5), d0
            next = 0x1a632U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a632U: { // 3d40001a move.w d0, $1a(a6)
            next = 0x1a636U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a636U: { // 3d400032 move.w d0, $32(a6)
            next = 0x1a63aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x32U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a63aU: { // 3d400034 move.w d0, $34(a6)
            next = 0x1a63eU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x34U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a63eU: { // 06450300 addi.w #$300, d5
            next = 0x1a642U;
            const auto source_value = 0x300U;
            const auto destination_value = r.data[5];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x1a642U: { // 024507ff andi.w #$7ff, d5
            next = 0x1a646U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[5];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x1a646U: { // 3205 move.w d5, d1
            next = 0x1a648U;
            const auto value = r.data[5];
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1a64eU: { // 48c0 ext.l d0
            next = 0x1a650U;
            r.data[0] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[0])));
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1a650U: { // 48c1 ext.l d1
            next = 0x1a652U;
            r.data[1] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r.data[1])));
            m.logic(r.data[1], 32U);
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_01a5c8_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1a652U: { // eb80 asl.l #$5, d0
            next = 0x1a654U;
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
        case 0x1a654U: { // eb81 asl.l #$5, d1
            next = 0x1a656U;
            auto value = r.data[1];
            bool carry = false;
            bool overflow = false;
            for (unsigned step = 0U; step != 5U; ++step) {
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
        case 0x1a656U: { // 2d40001e move.l d0, $1e(a6)
            next = 0x1a65aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1eU;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a65aU: { // 2d410026 move.l d1, $26(a6)
            next = 0x1a65eU;
            const auto value = r.data[1];
            const auto destination_address = r.address[6] + 0x26U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1a65eU: { // 51ceff72 dbra d6, $1a5d2
            next = 0x1a662U;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x1a5d2U;
            break;
        }
    default: return false;
    }
    return true;
}
