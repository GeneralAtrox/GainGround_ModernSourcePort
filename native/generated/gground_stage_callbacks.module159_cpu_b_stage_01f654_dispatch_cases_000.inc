
static bool cpu_b_stage_01f654_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1f56aU: { // 082d00020041 btst.b #$2, $41(a5)
            next = 0x1f570U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1f570U: { // 6626 bne.b $1f598
            next = 0x1f572U;
            if ((r.status & 4U) == 0U) { next = 0x1f598U; transfer_kind = 1U; }
            break;
        }
        case 0x1f572U: { // 4a6d0074 tst.w $74(a5)
            next = 0x1f576U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1f576U: { // 6706 beq.b $1f57e
            next = 0x1f578U;
            if ((r.status & 4U) != 0U) { next = 0x1f57eU; transfer_kind = 1U; }
            break;
        }
        case 0x1f578U: { // 536d0074 subq.w #$1, $74(a5)
            next = 0x1f57cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x74U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1f57eU: { // 08ed00010041 bset.b #$1, $41(a5)
            next = 0x1f584U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f584U: { // 08ad00000041 bclr.b #$0, $41(a5)
            next = 0x1f58aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1f58aU: { // 08ed00020041 bset.b #$2, $41(a5)
            next = 0x1f590U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f590U: { // 3b7c00060074 move.w #$6, $74(a5)
            next = 0x1f596U;
            const auto value = 0x6U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f598U: { // 302d0074 move.w $74(a5), d0
            next = 0x1f59cU;
            const auto source_address = r.address[5] + 0x74U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f59cU: { // 5340 subq.w #$1, d0
            next = 0x1f59eU;
            const auto source_value = 0x1U;
            const auto destination_value = r.data[0];
            const auto value = m.sub(destination_value, source_value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1f59eU: { // 3b400074 move.w d0, $74(a5)
            next = 0x1f5a2U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f5a2U: { // 0c400002 cmpi.w #$2, d0
            next = 0x1f5a6U;
            const auto source_value = 0x2U;
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1f5a6U: { // 6750 beq.b $1f5f8
            next = 0x1f5a8U;
            if ((r.status & 4U) != 0U) { next = 0x1f5f8U; transfer_kind = 1U; }
            break;
        }
        case 0x1f5a8U: { // 4a40 tst.w d0
            next = 0x1f5aaU;
            const auto value = r.data[0];
            m.logic(value, 16U);
            break;
        }
        case 0x1f5aaU: { // 6702 beq.b $1f5ae
            next = 0x1f5acU;
            if ((r.status & 4U) != 0U) { next = 0x1f5aeU; transfer_kind = 1U; }
            break;
        }
        case 0x1f5aeU: { // 4a2d0072 tst.b $72(a5)
            next = 0x1f5b2U;
            const auto destination_address = r.address[5] + 0x72U;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1f5b2U: { // 6706 beq.b $1f5ba
            next = 0x1f5b4U;
            if ((r.status & 4U) != 0U) { next = 0x1f5baU; transfer_kind = 1U; }
            break;
        }
        case 0x1f5b4U: { // 532d0072 subq.b #$1, $72(a5)
            next = 0x1f5b8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x72U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.sub(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1f5b8U: { // 661c bne.b $1f5d6
            next = 0x1f5baU;
            if ((r.status & 4U) == 0U) { next = 0x1f5d6U; transfer_kind = 1U; }
            break;
        }
        case 0x1f5baU: { // 206d0066 movea.l $66(a5), a0
            next = 0x1f5beU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1f5c2U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1f5c8U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1f5c8U: { // 08ed00000041 bset.b #$0, $41(a5)
            next = 0x1f5ceU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f5ceU: { // 08ed00020041 bset.b #$2, $41(a5)
            next = 0x1f5d4U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f5d6U: { // 206d0066 movea.l $66(a5), a0
            next = 0x1f5daU;
            const auto source_address = r.address[5] + 0x66U;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1f5daU: { // 7000 moveq #$0, d0
            next = 0x1f5dcU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1f5dcU: { // 10280013 move.b $13(a0), d0
            next = 0x1f5e0U;
            const auto source_address = r.address[0] + 0x13U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f5e0U: { // 3b400074 move.w d0, $74(a5)
            next = 0x1f5e4U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1f5e4U: { // 08ed00010041 bset.b #$1, $41(a5)
            next = 0x1f5eaU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f5eaU: { // 08ed00000041 bset.b #$0, $41(a5)
            next = 0x1f5f0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x0U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1f5f0U: { // 08ad00020041 bclr.b #$2, $41(a5)
            next = 0x1f5f6U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x2U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1f5f8U: { // 1b6d005f005e move.b $5f(a5), $5e(a5)
            next = 0x1f5feU;
            const auto source_address = r.address[5] + 0x5fU;
            const auto value = m.byte(source_address);
            const auto destination_address = r.address[5] + 0x5eU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1f602U: { // 3b41005c move.w d1, $5c(a5)
            next = 0x1f606U;
            const auto value = r.data[1];
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
