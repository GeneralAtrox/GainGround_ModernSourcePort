
static bool cpu_b_stage_018664_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x18664U: { // 4a2d0051 tst.b $51(a5)
            next = 0x18668U;
            const auto destination_address = r.address[5] + 0x51U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x18668U: { // 66000084 bne.w $186ee
            next = 0x1866cU;
            if ((r.status & 4U) == 0U) { next = 0x186eeU; transfer_kind = 1U; }
            break;
        }
        case 0x1866cU: { // 532d0050 subq.b #$1, $50(a5)
            next = 0x18670U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x50U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x18670U: { // 6e7a bgt.b $186ec
            next = 0x18672U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x186ecU; transfer_kind = 1U; }
            break;
        }
        case 0x18672U: { // 103c0005 move.b #$5, d0
            next = 0x18676U;
            const auto value = 0x5U;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18676U: { // 062d00200038 addi.b #$20, $38(a5)
            next = 0x1867cU;
            const auto source_value = 0x20U;
            const auto destination_address = r.address[5] + 0x38U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1867cU: { // 6404 bcc.b $18682
            next = 0x1867eU;
            if ((r.status & 1U) == 0U) { next = 0x18682U; transfer_kind = 1U; }
            break;
        }
        case 0x1867eU: { // 103c005a move.b #$5a, d0
            next = 0x18682U;
            const auto value = 0x5aU;
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18682U: { // 1b400050 move.b d0, $50(a5)
            next = 0x18686U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x50U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x18686U: { // 302d0052 move.w $52(a5), d0
            next = 0x1868aU;
            const auto source_address = r.address[5] + 0x52U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1868aU: { // d07c0080 add.w #$80, d0
            next = 0x1868eU;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1868eU: { // 0c400100 cmpi.w #$100, d0
            next = 0x18692U;
            const auto source_value = 0x100U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18692U: { // 6f02 ble.b $18696
            next = 0x18694U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x18696U; transfer_kind = 1U; }
            break;
        }
        case 0x18694U: { // 7000 moveq #$0, d0
            next = 0x18696U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x18696U: { // 3b400052 move.w d0, $52(a5)
            next = 0x1869aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x52U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1869aU: { // 4df81480 lea.l $1480.w, a6
            next = 0x1869eU;
            const auto source_address = 0x1480U;
            r.address[6] = source_address;
            break;
        }
        case 0x1869eU: { // dcc0 adda.w d0, a6
            next = 0x186a0U;
            const auto source_value = r.data[0];
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x186a0U: { // 0c6e00050044 cmpi.w #$5, $44(a6)
            next = 0x186a6U;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[6] + 0x44U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x186a6U: { // 6644 bne.b $186ec
            next = 0x186a8U;
            if ((r.status & 4U) == 0U) { next = 0x186ecU; transfer_kind = 1U; }
            break;
        }
        case 0x186a8U: { // 302e0012 move.w $12(a6), d0
            next = 0x186acU;
            const auto source_address = r.address[6] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x186acU: { // b06d001e cmp.w $1e(a5), d0
            next = 0x186b0U;
            const auto source_address = r.address[5] + 0x1eU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x186b0U: { // 6b3a bmi.b $186ec
            next = 0x186b2U;
            if ((r.status & 8U) != 0U) { next = 0x186ecU; transfer_kind = 1U; }
            break;
        }
        case 0x186b2U: { // b06d0020 cmp.w $20(a5), d0
            next = 0x186b6U;
            const auto source_address = r.address[5] + 0x20U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x186b6U: { // 6e34 bgt.b $186ec
            next = 0x186b8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x186ecU; transfer_kind = 1U; }
            break;
        }
        case 0x186b8U: { // 322e001a move.w $1a(a6), d1
            next = 0x186bcU;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x186bcU: { // b26d0022 cmp.w $22(a5), d1
            next = 0x186c0U;
            const auto source_address = r.address[5] + 0x22U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x186c0U: { // 6b2a bmi.b $186ec
            next = 0x186c2U;
            if ((r.status & 8U) != 0U) { next = 0x186ecU; transfer_kind = 1U; }
            break;
        }
        case 0x186c2U: { // b26d0024 cmp.w $24(a5), d1
            next = 0x186c6U;
            const auto source_address = r.address[5] + 0x24U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x186c6U: { // 6e24 bgt.b $186ec
            next = 0x186c8U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x186ecU; transfer_kind = 1U; }
            break;
        }
        case 0x186c8U: { // 3b400056 move.w d0, $56(a5)
            next = 0x186ccU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x186ccU: { // 3b410058 move.w d1, $58(a5)
            next = 0x186d0U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x58U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x186d0U: { // 906d0012 sub.w $12(a5), d0
            next = 0x186d4U;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x186d4U: { // 926d001a sub.w $1a(a5), d1
            next = 0x186d8U;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x186e4U: { // 3b410054 move.w d1, $54(a5)
            next = 0x186e8U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x186e8U: { // 4aed0051 tas.b $51(a5)
            next = 0x186ecU;
            const auto destination_address = r.address[5] + 0x51U;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
    default: return false;
    }
    return true;
}
