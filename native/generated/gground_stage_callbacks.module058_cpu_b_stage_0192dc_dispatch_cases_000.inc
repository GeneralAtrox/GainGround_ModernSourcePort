
static bool cpu_b_stage_0192dc_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x192dcU: { // 202d001e move.l $1e(a5), d0
            next = 0x192e0U;
            const auto source_address = r.address[5] + 0x1eU;
            const auto value = m.lng(source_address);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x192e0U: { // 6a0c bpl.b $192ee
            next = 0x192e2U;
            if ((r.status & 8U) == 0U) { next = 0x192eeU; transfer_kind = 1U; }
            break;
        }
        case 0x192e2U: { // d0ad0012 add.l $12(a5), d0
            next = 0x192e6U;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[0] = value;
            break;
        }
        case 0x192e6U: { // 4840 swap d0
            next = 0x192e8U;
            const auto value = (r.data[0] << 16U) | (r.data[0] >> 16U);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x192e8U: { // b050 cmp.w (a0), d0
            next = 0x192eaU;
            const auto source_address = r.address[0];
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x192eaU: { // 6a16 bpl.b $19302
            next = 0x192ecU;
            if ((r.status & 8U) == 0U) { next = 0x19302U; transfer_kind = 1U; }
            break;
        }
        case 0x192ecU: { // 600c bra.b $192fa
            next = 0x192eeU;
            if (true) { next = 0x192faU; transfer_kind = 1U; }
            break;
        }
        case 0x192eeU: { // d0ad0012 add.l $12(a5), d0
            next = 0x192f2U;
            const auto source_address = r.address[5] + 0x12U;
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[0];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[0] = value;
            break;
        }
        case 0x192f2U: { // 4840 swap d0
            next = 0x192f4U;
            const auto value = (r.data[0] << 16U) | (r.data[0] >> 16U);
            r.data[0] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x192f4U: { // b0680002 cmp.w $2(a0), d0
            next = 0x192f8U;
            const auto source_address = r.address[0] + 0x2U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x192f8U: { // 6f08 ble.b $19302
            next = 0x192faU;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x19302U; transfer_kind = 1U; }
            break;
        }
        case 0x192faU: { // 42ad001e clr.l $1e(a5)
            next = 0x192feU;
            const auto destination_address = r.address[5] + 0x1eU;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0x192feU: { // 302d0012 move.w $12(a5), d0
            next = 0x19302U;
            const auto source_address = r.address[5] + 0x12U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19302U: { // 222d0026 move.l $26(a5), d1
            next = 0x19306U;
            const auto source_address = r.address[5] + 0x26U;
            const auto value = m.lng(source_address);
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x19306U: { // 6a0e bpl.b $19316
            next = 0x19308U;
            if ((r.status & 8U) == 0U) { next = 0x19316U; transfer_kind = 1U; }
            break;
        }
        case 0x19308U: { // d2ad001a add.l $1a(a5), d1
            next = 0x1930cU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[1] = value;
            break;
        }
        case 0x1930cU: { // 4841 swap d1
            next = 0x1930eU;
            const auto value = (r.data[1] << 16U) | (r.data[1] >> 16U);
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1930eU: { // b2680004 cmp.w $4(a0), d1
            next = 0x19312U;
            const auto source_address = r.address[0] + 0x4U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19312U: { // 6a16 bpl.b $1932a
            next = 0x19314U;
            if ((r.status & 8U) == 0U) { next = 0x1932aU; transfer_kind = 1U; }
            break;
        }
        case 0x19314U: { // 600c bra.b $19322
            next = 0x19316U;
            if (true) { next = 0x19322U; transfer_kind = 1U; }
            break;
        }
        case 0x19316U: { // d2ad001a add.l $1a(a5), d1
            next = 0x1931aU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto source_value = m.lng(source_address);
            const auto destination_value = r.data[1];
            const auto value = m.add(destination_value, source_value, 32U);
            r.data[1] = value;
            break;
        }
        case 0x1931aU: { // 4841 swap d1
            next = 0x1931cU;
            const auto value = (r.data[1] << 16U) | (r.data[1] >> 16U);
            r.data[1] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1931cU: { // b2680006 cmp.w $6(a0), d1
            next = 0x19320U;
            const auto source_address = r.address[0] + 0x6U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[1];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x19320U: { // 6f08 ble.b $1932a
            next = 0x19322U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1932aU; transfer_kind = 1U; }
            break;
        }
        case 0x19322U: { // 42ad0026 clr.l $26(a5)
            next = 0x19326U;
            const auto destination_address = r.address[5] + 0x26U;
            const auto value = m.lng(destination_address);
            (void)value;
            m.word(destination_address + 2U, 0U);
            m.word(destination_address, (0U) >> 16U);
            m.logic(0U, 32U);
            break;
        }
        case 0x19326U: { // 322d001a move.w $1a(a5), d1
            next = 0x1932aU;
            const auto source_address = r.address[5] + 0x1aU;
            const auto value = m.word(source_address);
            m.dw(1U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1932aU: { // 1411 move.b (a1), d2
            next = 0x1932cU;
            const auto source_address = r.address[1];
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1932cU: { // 4882 ext.w d2
            next = 0x1932eU;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[2])));
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1932eU: { // 9042 sub.w d2, d0
            next = 0x19330U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x19330U: { // 14290004 move.b $4(a1), d2
            next = 0x19334U;
            const auto source_address = r.address[1] + 0x4U;
            const auto value = m.byte(source_address);
            m.db(2U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x19334U: { // 4882 ext.w d2
            next = 0x19336U;
            const auto value = static_cast<std::uint16_t>(static_cast<std::int16_t>(static_cast<std::int8_t>(r.data[2])));
            m.dw(2U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x19336U: { // 9242 sub.w d2, d1
            next = 0x19338U;
            const auto source_value = r.data[2];
            const auto destination_value = r.data[1];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(1U, value);
            break;
        }
        case 0x1933eU: { // 08100000 btst.b #$0, (a0)
            next = 0x19342U;
            const auto destination_address = r.address[0];
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x19342U: { // 664a bne.b $1938e
            next = 0x19344U;
            if ((r.status & 4U) == 0U) { next = 0x1938eU; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
