
static bool cpu_b_stage_01c8e8_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c8e8U: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1c8ecU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c8ecU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x1c8f2U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c8f2U: { // 6666 bne.b $1c95a
            next = 0x1c8f4U;
            if ((r.status & 4U) == 0U) { next = 0x1c95aU; transfer_kind = 1U; }
            break;
        }
        case 0x1c8f4U: { // 4a6d005a tst.w $5a(a5)
            next = 0x1c8f8U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1c8f8U: { // 6720 beq.b $1c91a
            next = 0x1c8faU;
            if ((r.status & 4U) != 0U) { next = 0x1c91aU; transfer_kind = 1U; }
            break;
        }
        case 0x1c8faU: { // 536d005a subq.w #$1, $5a(a5)
            next = 0x1c8feU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c8feU: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c904U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c904U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c90aU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c90aU: { // 422d003f clr.b $3f(a5)
            next = 0x1c90eU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c90eU: { // 422d003e clr.b $3e(a5)
            next = 0x1c912U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c912U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1c918U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c91aU: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c920U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c920U: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1c926U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c926U: { // 08ed00030041 bset.b #$3, $41(a5)
            next = 0x1c92cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c92cU: { // 1b7c00000058 move.b #$0, $58(a5)
            next = 0x1c932U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x58U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c932U: { // 08ad00070041 bclr.b #$7, $41(a5)
            next = 0x1c938U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c93cU: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c942U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c942U: { // 422d003f clr.b $3f(a5)
            next = 0x1c946U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c946U: { // 422d003e clr.b $3e(a5)
            next = 0x1c94aU;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c94aU: { // 206d006e movea.l $6e(a5), a0
            next = 0x1c94eU;
            const auto source_address = r.address[5] + 0x6eU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c94eU: { // 7000 moveq #$0, d0
            next = 0x1c950U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1c950U: { // 10280006 move.b $6(a0), d0
            next = 0x1c954U;
            const auto source_address = r.address[0] + 0x6U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c954U: { // 3b40005a move.w d0, $5a(a5)
            next = 0x1c958U;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c95aU: { // 082d00050041 btst.b #$5, $41(a5)
            next = 0x1c960U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c960U: { // 665a bne.b $1c9bc
            next = 0x1c962U;
            if ((r.status & 4U) == 0U) { next = 0x1c9bcU; transfer_kind = 1U; }
            break;
        }
        case 0x1c962U: { // 0c2d00200058 cmpi.b #$20, $58(a5)
            next = 0x1c968U;
            const auto source_value = 0x20U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1c968U: { // 6c20 bge.b $1c98a
            next = 0x1c96aU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1c98aU; transfer_kind = 1U; }
            break;
        }
        case 0x1c96aU: { // 422d003f clr.b $3f(a5)
            next = 0x1c96eU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c96eU: { // 3b6d00760046 move.w $76(a5), $46(a5)
            next = 0x1c974U;
            const auto source_address = r.address[5] + 0x76U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c974U: { // 422d003e clr.b $3e(a5)
            next = 0x1c978U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c978U: { // 522d0058 addq.b #$1, $58(a5)
            next = 0x1c97cU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
    default: return false;
    }
    return true;
}
