
static bool cpu_b_stage_01f800_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1f800U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1f804U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1f804U: { // 6702 beq.b $1f808
            next = 0x1f806U;
            if ((r.status & 4U) != 0U) { next = 0x1f808U; transfer_kind = 1U; }
            break;
        }
        case 0x1f808U: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1f80eU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1f80eU: { // 6600003e bne.w $1f84e
            next = 0x1f812U;
            if ((r.status & 4U) == 0U) { next = 0x1f84eU; transfer_kind = 1U; }
            break;
        }
        case 0x1f812U: { // 4a6d0074 tst.w $74(a5)
            next = 0x1f816U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1f816U: { // 6706 beq.b $1f81e
            next = 0x1f818U;
            if ((r.status & 4U) != 0U) { next = 0x1f81eU; transfer_kind = 1U; }
            break;
        }
        case 0x1f818U: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x1f81cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1f822U: { // 6502 bcs.b $1f826
            next = 0x1f824U;
            if ((r.status & 1U) != 0U) { next = 0x1f826U; transfer_kind = 1U; }
            break;
        }
        case 0x1f826U: { // 206d0066 movea.l $66(a5), a0
            next = 0x1f82aU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1f82aU: { // 1b6800120072 move.b $12(a0), $72(a5)
            next = 0x1f830U;
            const auto source_address = r.address[0] + 0x12U;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x72U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f830U: { // 08ed00010041 bset.b #$1, $41(a5)
            next = 0x1f836U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f836U: { // 08ad00000041 bclr.b #$0, $41(a5)
            next = 0x1f83cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1f83cU: { // 08ed00020041 bset.b #$2, $41(a5)
            next = 0x1f842U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f842U: { // 3b7c00060074 move.w #$6, $74(a5)
            next = 0x1f848U;
            const auto value = 0x6U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f84eU: { // 082d00020041 btst.b #$2, $41(a5)
            next = 0x1f854U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1f854U: { // 6626 bne.b $1f87c
            next = 0x1f856U;
            if ((r.status & 4U) == 0U) { next = 0x1f87cU; transfer_kind = 1U; }
            break;
        }
        case 0x1f856U: { // 4a6d0074 tst.w $74(a5)
            next = 0x1f85aU;
            const auto destination_address = r.address[5] + 0x74U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1f85aU: { // 67a4 beq.b $1f800
            next = 0x1f85cU;
            if ((r.status & 4U) != 0U) { next = 0x1f800U; transfer_kind = 1U; }
            break;
        }
        case 0x1f85cU: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x1f860U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1f87cU: { // 302d0074 move.w $74(a5), d0
            next = 0x1f880U;
            const auto source_address = r.address[5] + 0x74U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f880U: { // 5340 subq.w #$1, d0
            next = 0x1f882U;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1f882U: { // 3b400074 move.w d0, $74(a5)
            next = 0x1f886U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f886U: { // 0c400002 cmpi.w #$2, d0
            next = 0x1f88aU;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1f88aU: { // 6750 beq.b $1f8dc
            next = 0x1f88cU;
            if ((r.status & 4U) != 0U) { next = 0x1f8dcU; transfer_kind = 1U; }
            break;
        }
        case 0x1f88cU: { // 4a40 tst.w d0
            next = 0x1f88eU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x1f88eU: { // 6702 beq.b $1f892
            next = 0x1f890U;
            if ((r.status & 4U) != 0U) { next = 0x1f892U; transfer_kind = 1U; }
            break;
        }
        case 0x1f892U: { // 4a2d0072 tst.b $72(a5)
            next = 0x1f896U;
            const auto destination_address = r.address[5] + 0x72U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1f896U: { // 6706 beq.b $1f89e
            next = 0x1f898U;
            if ((r.status & 4U) != 0U) { next = 0x1f89eU; transfer_kind = 1U; }
            break;
        }
        case 0x1f898U: { // 532d0072 subq.b #$1, $72(a5)
            next = 0x1f89cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x72U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1f89cU: { // 661c bne.b $1f8ba
            next = 0x1f89eU;
            if ((r.status & 4U) == 0U) { next = 0x1f8baU; transfer_kind = 1U; }
            break;
        }
        case 0x1f89eU: { // 206d0066 movea.l $66(a5), a0
            next = 0x1f8a2U;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1f8a6U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1f8acU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1f8acU: { // 08ed00000041 bset.b #$0, $41(a5)
            next = 0x1f8b2U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f8b2U: { // 08ed00020041 bset.b #$2, $41(a5)
            next = 0x1f8b8U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f8baU: { // 206d0066 movea.l $66(a5), a0
            next = 0x1f8beU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
    default: return false;
    }
    return true;
}
