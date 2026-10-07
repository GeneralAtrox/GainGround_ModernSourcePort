
static bool cpu_b_stage_01f800_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1f8beU: { // 7000 moveq #$0, d0
            next = 0x1f8c0U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1f8c0U: { // 10280013 move.b $13(a0), d0
            next = 0x1f8c4U;
            const auto source_address = r.address[0] + 0x13U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f8c4U: { // 3b400074 move.w d0, $74(a5)
            next = 0x1f8c8U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f8c8U: { // 08ed00010041 bset.b #$1, $41(a5)
            next = 0x1f8ceU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f8ceU: { // 08ed00000041 bset.b #$0, $41(a5)
            next = 0x1f8d4U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f8d4U: { // 08ad00020041 bclr.b #$2, $41(a5)
            next = 0x1f8daU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1f8e0U: { // 026d03ff005c andi.w #$3ff, $5c(a5)
            next = 0x1f8e6U;
            const auto source_value = 0x3ffU;
            const auto destination_address = r.address[5] + 0x5cU;
            const auto destination_value = m.word(destination_address);
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1f8eaU: { // 4a40 tst.w d0
            next = 0x1f8ecU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x1f8ecU: { // 6b7c bmi.b $1f96a
            next = 0x1f8eeU;
            if ((r.status & 8U) != 0U) { next = 0x1f96aU; transfer_kind = 1U; }
            break;
        }
        case 0x1f8f6U: { // 264e movea.l a6, a3
            next = 0x1f8f8U;
            const auto value = r.address[6];
            r.address[3] = value;
            break;
        }
        case 0x1f8f8U: { // 3cbc0000 move.w #$0, (a6)
            next = 0x1f8fcU;
            const auto value = 0x0U;
            const auto destination_address = r.address[6];
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f8fcU: { // 206d0066 movea.l $66(a5), a0
            next = 0x1f900U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1f900U: { // 3a2d005c move.w $5c(a5), d5
            next = 0x1f904U;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(5U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f904U: { // 3d45003a move.w d5, $3a(a6)
            next = 0x1f908U;
            const auto value = r.data[5];
            const auto destination_address = r.address[6] + 0x3aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f908U: { // 7c00 moveq #$0, d6
            next = 0x1f90aU;
            r.data[6] = 0x0U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x1f90aU: { // 7e00 moveq #$0, d7
            next = 0x1f90cU;
            r.data[7] = 0x0U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x1f90cU: { // 1c280014 move.b $14(a0), d6
            next = 0x1f910U;
            const auto source_address = r.address[0] + 0x14U;
            const auto value = m.byte(source_address);
            m.db(6U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f910U: { // 1e280015 move.b $15(a0), d7
            next = 0x1f914U;
            const auto source_address = r.address[0] + 0x15U;
            const auto value = m.byte(source_address);
            m.db(7U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f914U: { // e94f lsl.w #$4, d7
            next = 0x1f916U;
            m.shift_word(7U, 4U, true, false);
            break;
        }
        case 0x1f916U: { // 5346 subq.w #$1, d6
            next = 0x1f918U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[6];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(6U, value);
            break;
        }
        case 0x1f918U: { // 6710 beq.b $1f92a
            next = 0x1f91aU;
            if ((r.status & 4U) != 0U) { next = 0x1f92aU; transfer_kind = 1U; }
            break;
        }
        case 0x1f91aU: { // 1806 move.b d6, d4
            next = 0x1f91cU;
            const auto value = r.data[6];
            m.db(4U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f91cU: { // e40c lsr.b #$2, d4
            next = 0x1f91eU;
            const auto old = r.data[4] & 0xffU;
            const auto carry = (old >> 1U) & 1U;
            const auto value = old >> 2U;
            m.db(4U, value);
            m.logic(value, 8U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x11U) | (carry ? 0x11U : 0U));
            break;
        }
        case 0x1f91eU: { // 9a47 sub.w d7, d5
            next = 0x1f920U;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[5];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x1f920U: { // 51ccfffc dbra d4, $1f91e
            next = 0x1f924U;
            m.dw(4U, r.data[4] - 1U);
            if ((r.data[4] & 0xffffU) != 0xffffU) next = 0x1f91eU;
            break;
        }
        case 0x1f924U: { // 4846 swap d6
            next = 0x1f926U;
            const auto value = (r.data[6] << 16U) | (r.data[6] >> 16U);
            r.data[6] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1f926U: { // 3c07 move.w d7, d6
            next = 0x1f928U;
            const auto value = r.data[7];
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f928U: { // 4846 swap d6
            next = 0x1f92aU;
            const auto value = (r.data[6] << 16U) | (r.data[6] >> 16U);
            r.data[6] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1f92aU: { // 024507ff andi.w #$7ff, d5
            next = 0x1f92eU;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[5];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x1f92eU: { // 4a3900003437 tst.b $3437.l
            next = 0x1f934U;
            const auto destination_address = 0x3437U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1f934U: { // 6734 beq.b $1f96a
            next = 0x1f936U;
            if ((r.status & 4U) != 0U) { next = 0x1f96aU; transfer_kind = 1U; }
            break;
        }
        case 0x1f93aU: { // 4a40 tst.w d0
            next = 0x1f93cU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x1f93cU: { // 6b2c bmi.b $1f96a
            next = 0x1f93eU;
            if ((r.status & 8U) != 0U) { next = 0x1f96aU; transfer_kind = 1U; }
            break;
        }
        case 0x1f946U: { // 2d6b00220022 move.l $22(a3), $22(a6)
            next = 0x1f94cU;
            const auto source_address = r.address[3] + 0x22U;
            const auto value = m.lng(source_address);
            const auto destination_address = r.address[6] + 0x22U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_01f800_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1f94cU: { // 2d7c000020000066 move.l #$2000, $66(a6)
            next = 0x1f954U;
            const auto value = 0x2000U;
            const auto destination_address = r.address[6] + 0x66U;
            m.lng(destination_address, value);
            m.logic(value, 32U);
            break;
        }
        case 0x1f954U: { // 284d movea.l a5, a4
            next = 0x1f956U;
            const auto value = r.address[5];
            r.address[4] = value;
            break;
        }
        case 0x1f956U: { // 2a4e movea.l a6, a5
            next = 0x1f958U;
            const auto value = r.address[6];
            r.address[5] = value;
            break;
        }
        case 0x1f95eU: { // 2a4c movea.l a4, a5
            next = 0x1f960U;
            const auto value = r.address[4];
            r.address[5] = value;
            break;
        }
        case 0x1f960U: { // 4846 swap d6
            next = 0x1f962U;
            const auto value = (r.data[6] << 16U) | (r.data[6] >> 16U);
            r.data[6] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1f962U: { // da46 add.w d6, d5
            next = 0x1f964U;
            const auto source_value = r.data[6];
            const auto destination_value = r.data[5];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x1f964U: { // 4846 swap d6
            next = 0x1f966U;
            const auto value = (r.data[6] << 16U) | (r.data[6] >> 16U);
            r.data[6] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1f966U: { // 51ceffc2 dbra d6, $1f92a
            next = 0x1f96aU;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x1f92aU;
            break;
        }
    default: return false;
    }
    return true;
}
