
static bool cpu_b_stage_019806_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x19880U: { // 342d001a move.w $1a(a5), d2
            next = 0x19884U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19884U: { // d042 add.w d2, d0
            next = 0x19886U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19886U: { // 044001f4 subi.w #$1f4, d0
            next = 0x1988aU;
            const auto source_value = 0x1f4U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1988aU: { // 3b410074 move.w d1, $74(a5)
            next = 0x1988eU;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1988eU: { // 3b400076 move.w d0, $76(a5)
            next = 0x19892U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x76U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19892U: { // 3b420078 move.w d2, $78(a5)
            next = 0x19896U;
            const auto value = r.data[2];
            const auto destination_address = r.address[5] + 0x78U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19896U: { // 7000 moveq #$0, d0
            next = 0x19898U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x19898U: { // 102d0051 move.b $51(a5), d0
            next = 0x1989cU;
            const auto source_address = r.address[5] + 0x51U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1989cU: { // e540 asl.w #$2, d0
            next = 0x1989eU;
            m.asl_word(0U, 2U);
            break;
        }
        case 0x1989eU: { // 41fa1b74 lea.l $1b414(pc), a0
            next = 0x198a2U;
            const auto source_address = 0x1b414U;
            r.address[0] = source_address;
            break;
        }
        case 0x198a2U: { // 3d7000000060 move.w (a0, d0.w), $60(a6)
            next = 0x198a8U;
            const auto source_address = r.address[0] + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.word(source_address);
            const auto destination_address = r.address[6] + 0x60U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198a8U: { // 1d700003000b move.b $3(a0, d0.w), $b(a6)
            next = 0x198aeU;
            const auto source_address = r.address[0] + 0x3U + static_cast<std::int16_t>(r.data[0]);
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[6] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x198aeU: { // 1cbc0080 move.b #$80, (a6)
            next = 0x198b2U;
            const auto value = 0x80U;
            const auto destination_address = r.address[6];
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x198b2U: { // 1d7c00040001 move.b #$4, $1(a6)
            next = 0x198b8U;
            const auto value = 0x4U;
            const auto destination_address = r.address[6] + 0x1U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x198b8U: { // 2d7c00019b620002 move.l #$19b62, $2(a6)
            next = 0x198c0U;
            const auto value = 0x19b62U;
            const auto destination_address = r.address[6] + 0x2U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x198c0U: { // 3d7c037f0006 move.w #$37f, $6(a6)
            next = 0x198c6U;
            const auto value = 0x37fU;
            const auto destination_address = r.address[6] + 0x6U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198c6U: { // 3d7c0c370008 move.w #$c37, $8(a6)
            next = 0x198ccU;
            const auto value = 0xc37U;
            const auto destination_address = r.address[6] + 0x8U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198ccU: { // 426e0010 clr.w $10(a6)
            next = 0x198d0U;
            const auto destination_address = r.address[6] + 0x10U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x198d0U: { // 302d0074 move.w $74(a5), d0
            next = 0x198d4U;
            const auto source_address = r.address[5] + 0x74U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198d4U: { // 3d400012 move.w d0, $12(a6)
            next = 0x198d8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x12U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198d8U: { // 3d40002a move.w d0, $2a(a6)
            next = 0x198dcU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198dcU: { // 3d40002c move.w d0, $2c(a6)
            next = 0x198e0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198e0U: { // 426e0014 clr.w $14(a6)
            next = 0x198e4U;
            const auto destination_address = r.address[6] + 0x14U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x198e4U: { // 302d0076 move.w $76(a5), d0
            next = 0x198e8U;
            const auto source_address = r.address[5] + 0x76U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198e8U: { // 3d400016 move.w d0, $16(a6)
            next = 0x198ecU;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x16U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198ecU: { // 3d40002e move.w d0, $2e(a6)
            next = 0x198f0U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x2eU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198f0U: { // 3d400030 move.w d0, $30(a6)
            next = 0x198f4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x30U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198f4U: { // 426e0018 clr.w $18(a6)
            next = 0x198f8U;
            const auto destination_address = r.address[6] + 0x18U;
            const auto value = m.word(destination_address);
            (void)value;
            m.word(destination_address, 0U);
            m.logic(0U, 16U);
            break;
        }
        case 0x198f8U: { // 302d0078 move.w $78(a5), d0
            next = 0x198fcU;
            const auto source_address = r.address[5] + 0x78U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x198fcU: { // 3d40001a move.w d0, $1a(a6)
            next = 0x19900U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x1aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19900U: { // 3d400032 move.w d0, $32(a6)
            next = 0x19904U;
            const auto value = r.data[0];
            const auto destination_address = r.address[6] + 0x32U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
