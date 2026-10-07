
static bool cpu_b_stage_0185c4_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x185c4U: { // 4a2d0051 tst.b $51(a5)
            next = 0x185c8U;
            const auto destination_address = r.address[5] + 0x51U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x185c8U: { // 66000124 bne.w $186ee
            next = 0x185ccU;
            if ((r.status & 4U) == 0U) { next = 0x186eeU; transfer_kind = 1U; }
            break;
        }
        case 0x185ccU: { // 532d0050 subq.b #$1, $50(a5)
            next = 0x185d0U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x50U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x185d0U: { // 6e000090 bgt.w $18662
            next = 0x185d4U;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18662U; transfer_kind = 1U; }
            break;
        }
        case 0x185d4U: { // 1b780d190050 move.b $d19.w, $50(a5)
            next = 0x185daU;
            const auto source_address = 0xd19U;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x50U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x185daU: { // 302d0052 move.w $52(a5), d0
            next = 0x185deU;
            const auto source_address = r.address[5] + 0x52U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x185deU: { // d07c0080 add.w #$80, d0
            next = 0x185e2U;
            const auto source_value = 0x80U;
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x185e2U: { // 0c400100 cmpi.w #$100, d0
            next = 0x185e6U;
            const auto source_value = 0x100U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x185e6U: { // 6f02 ble.b $185ea
            next = 0x185e8U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x185eaU; transfer_kind = 1U; }
            break;
        }
        case 0x185e8U: { // 7000 moveq #$0, d0
            next = 0x185eaU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x185eaU: { // 3b400052 move.w d0, $52(a5)
            next = 0x185eeU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x52U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x185eeU: { // 4df81480 lea.l $1480.w, a6
            next = 0x185f2U;
            const auto source_address = 0x1480U;
            r.address[6] = source_address;
            break;
        }
        case 0x185f2U: { // dcc0 adda.w d0, a6
            next = 0x185f4U;
            const auto source_value = r.data[0];
            r.address[6] += static_cast<std::int16_t>(source_value);
            break;
        }
        case 0x185f4U: { // 0c6e00050044 cmpi.w #$5, $44(a6)
            next = 0x185faU;
            const auto source_value = 0x5U;
            const auto destination_address = r.address[6] + 0x44U;
            const auto destination_value = m.word(destination_address);
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x185faU: { // 6666 bne.b $18662
            next = 0x185fcU;
            if ((r.status & 4U) == 0U) { next = 0x18662U; transfer_kind = 1U; }
            break;
        }
        case 0x185fcU: { // 302e0012 move.w $12(a6), d0
            next = 0x18600U;
            const auto source_address = r.address[6] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18600U: { // b06d001e cmp.w $1e(a5), d0
            next = 0x18604U;
            const auto source_address = r.address[5] + 0x1eU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18604U: { // 6b5c bmi.b $18662
            next = 0x18606U;
            if ((r.status & 8U) != 0U) { next = 0x18662U; transfer_kind = 1U; }
            break;
        }
        case 0x18606U: { // b06d0020 cmp.w $20(a5), d0
            next = 0x1860aU;
            const auto source_address = r.address[5] + 0x20U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1860aU: { // 6e56 bgt.b $18662
            next = 0x1860cU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18662U; transfer_kind = 1U; }
            break;
        }
        case 0x1860cU: { // 322e001a move.w $1a(a6), d1
            next = 0x18610U;
            const auto source_address = r.address[6] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18610U: { // b26d0022 cmp.w $22(a5), d1
            next = 0x18614U;
            const auto source_address = r.address[5] + 0x22U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x18614U: { // 6b4c bmi.b $18662
            next = 0x18616U;
            if ((r.status & 8U) != 0U) { next = 0x18662U; transfer_kind = 1U; }
            break;
        }
        case 0x18616U: { // b26d0024 cmp.w $24(a5), d1
            next = 0x1861aU;
            const auto source_address = r.address[5] + 0x24U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1861aU: { // 6e46 bgt.b $18662
            next = 0x1861cU;
            if ((r.status & 4U) == 0U && (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x18662U; transfer_kind = 1U; }
            break;
        }
        case 0x1861cU: { // 3b400056 move.w d0, $56(a5)
            next = 0x18620U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x56U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18620U: { // 3b410058 move.w d1, $58(a5)
            next = 0x18624U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x58U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18624U: { // 906d0012 sub.w $12(a5), d0
            next = 0x18628U;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18628U: { // 926d001a sub.w $1a(a5), d1
            next = 0x1862cU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x18638U: { // 3b410054 move.w d1, $54(a5)
            next = 0x1863cU;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x54U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1863cU: { // 4aed0051 tas.b $51(a5)
            next = 0x18640U;
            const auto destination_address = r.address[5] + 0x51U;
            const auto old = m.byte(destination_address);
            m.logic(old, 8U);
            const auto value = old | 0x80U;
            m.byte(destination_address, value);
            break;
        }
        case 0x18640U: { // 302d005a move.w $5a(a5), d0
            next = 0x18644U;
            const auto source_address = r.address[5] + 0x5aU;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x18644U: { // 9041 sub.w d1, d0
            next = 0x18646U;
            const auto source_value = r.data[1];
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x18646U: { // 024007ff andi.w #$7ff, d0
            next = 0x1864aU;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
    default: return false;
    }
    return true;
}
