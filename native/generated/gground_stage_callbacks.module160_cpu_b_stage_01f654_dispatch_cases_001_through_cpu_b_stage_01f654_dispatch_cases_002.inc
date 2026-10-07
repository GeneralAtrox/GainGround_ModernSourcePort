
static bool cpu_b_stage_01f654_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1f606U: { // 206d0066 movea.l $66(a5), a0
            next = 0x1f60aU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1f60aU: { // 3a2d005c move.w $5c(a5), d5
            next = 0x1f60eU;
            const auto source_address = r.address[5] + 0x5cU;
            const auto value = m.word(source_address);
            m.dw(5U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f60eU: { // 3b45003a move.w d5, $3a(a5)
            next = 0x1f612U;
            const auto value = r.data[5];
            const auto destination_address = r.address[5] + 0x3aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f612U: { // 7c00 moveq #$0, d6
            next = 0x1f614U;
            r.data[6] = 0x0U;
            m.logic(r.data[6], 32U);
            break;
        }
        case 0x1f614U: { // 7e00 moveq #$0, d7
            next = 0x1f616U;
            r.data[7] = 0x0U;
            m.logic(r.data[7], 32U);
            break;
        }
        case 0x1f616U: { // 1c280014 move.b $14(a0), d6
            next = 0x1f61aU;
            const auto source_address = r.address[0] + 0x14U;
            const auto value = m.byte(source_address);
            m.db(6U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f61aU: { // 1e280015 move.b $15(a0), d7
            next = 0x1f61eU;
            const auto source_address = r.address[0] + 0x15U;
            const auto value = m.byte(source_address);
            m.db(7U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f61eU: { // e94f lsl.w #$4, d7
            next = 0x1f620U;
            m.shift_word(7U, 4U, true, false);
            break;
        }
        case 0x1f620U: { // 5346 subq.w #$1, d6
            next = 0x1f622U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[6];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(6U, value);
            break;
        }
        case 0x1f622U: { // 6710 beq.b $1f634
            next = 0x1f624U;
            if ((r.status & 4U) != 0U) { next = 0x1f634U; transfer_kind = 1U; }
            break;
        }
        case 0x1f624U: { // 1806 move.b d6, d4
            next = 0x1f626U;
            const auto value = r.data[6];
            m.db(4U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f626U: { // e20c lsr.b #$1, d4
            next = 0x1f628U;
            const auto old = r.data[4] & 0xffU;
            const auto carry = (old >> 0U) & 1U;
            const auto value = old >> 1U;
            m.db(4U, value);
            m.logic(value, 8U);
            r.status = static_cast<std::uint16_t>((r.status & ~0x11U) | (carry ? 0x11U : 0U));
            break;
        }
        case 0x1f628U: { // 9a47 sub.w d7, d5
            next = 0x1f62aU;
            const auto source_value = r.data[7];
            const auto destination_value = r.data[5];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x1f62aU: { // 51ccfffc dbra d4, $1f628
            next = 0x1f62eU;
            m.dw(4U, r.data[4] - 1U);
            if ((r.data[4] & 0xffffU) != 0xffffU) next = 0x1f628U;
            break;
        }
        case 0x1f62eU: { // 4846 swap d6
            next = 0x1f630U;
            const auto value = (r.data[6] << 16U) | (r.data[6] >> 16U);
            r.data[6] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1f630U: { // 3c07 move.w d7, d6
            next = 0x1f632U;
            const auto value = r.data[7];
            m.dw(6U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f632U: { // 4846 swap d6
            next = 0x1f634U;
            const auto value = (r.data[6] << 16U) | (r.data[6] >> 16U);
            r.data[6] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1f634U: { // 024507ff andi.w #$7ff, d5
            next = 0x1f638U;
            const auto source_value = 0x7ffU;
            const auto destination_value = r.data[5];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x1f63cU: { // 4a40 tst.w d0
            next = 0x1f63eU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x1f63eU: { // 6b12 bmi.b $1f652
            next = 0x1f640U;
            if ((r.status & 8U) != 0U) { next = 0x1f652U; transfer_kind = 1U; }
            break;
        }
        case 0x1f648U: { // 4846 swap d6
            next = 0x1f64aU;
            const auto value = (r.data[6] << 16U) | (r.data[6] >> 16U);
            r.data[6] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1f64aU: { // da46 add.w d6, d5
            next = 0x1f64cU;
            const auto source_value = r.data[6];
            const auto destination_value = r.data[5];
            const auto value = m.add(destination_value, source_value, 16U);
            m.dw(5U, value);
            break;
        }
        case 0x1f64cU: { // 4846 swap d6
            next = 0x1f64eU;
            const auto value = (r.data[6] << 16U) | (r.data[6] >> 16U);
            r.data[6] = value;
            m.logic(value, 32U);
            break;
        }
        case 0x1f64eU: { // 51ceffe4 dbra d6, $1f634
            next = 0x1f652U;
            m.dw(6U, r.data[6] - 1U);
            if ((r.data[6] & 0xffffU) != 0xffffU) next = 0x1f634U;
            break;
        }
        case 0x1f654U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1f658U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1f658U: { // 6702 beq.b $1f65c
            next = 0x1f65aU;
            if ((r.status & 4U) != 0U) { next = 0x1f65cU; transfer_kind = 1U; }
            break;
        }
        case 0x1f65cU: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1f662U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1f662U: { // 6600ff06 bne.w $1f56a
            next = 0x1f666U;
            if ((r.status & 4U) == 0U) { next = 0x1f56aU; transfer_kind = 1U; }
            break;
        }
        case 0x1f666U: { // 4a6d0074 tst.w $74(a5)
            next = 0x1f66aU;
            const auto destination_address = r.address[5] + 0x74U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1f66aU: { // 6706 beq.b $1f672
            next = 0x1f66cU;
            if ((r.status & 4U) != 0U) { next = 0x1f672U; transfer_kind = 1U; }
            break;
        }
        case 0x1f66cU: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x1f670U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1f676U: { // 6502 bcs.b $1f67a
            next = 0x1f678U;
            if ((r.status & 1U) != 0U) { next = 0x1f67aU; transfer_kind = 1U; }
            break;
        }
        case 0x1f67aU: { // 206d0066 movea.l $66(a5), a0
            next = 0x1f67eU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1f67eU: { // 1b6800120072 move.b $12(a0), $72(a5)
            next = 0x1f684U;
            const auto source_address = r.address[0] + 0x12U;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x72U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f684U: { // 08ed00010041 bset.b #$1, $41(a5)
            next = 0x1f68aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
    default: return false;
    }
    return true;
}

static bool cpu_b_stage_01f654_dispatch_cases_002([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1f68aU: { // 08ad00000041 bclr.b #$0, $41(a5)
            next = 0x1f690U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1f690U: { // 08ed00020041 bset.b #$2, $41(a5)
            next = 0x1f696U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f696U: { // 3b7c00060074 move.w #$6, $74(a5)
            next = 0x1f69cU;
            const auto value = 0x6U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
