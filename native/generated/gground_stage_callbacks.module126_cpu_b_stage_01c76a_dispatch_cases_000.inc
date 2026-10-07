
static bool cpu_b_stage_01c76a_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c76aU: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1c76eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c76eU: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x1c774U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c774U: { // 6666 bne.b $1c7dc
            next = 0x1c776U;
            if ((r.status & 4U) == 0U) { next = 0x1c7dcU; transfer_kind = 1U; }
            break;
        }
        case 0x1c776U: { // 4a6d005a tst.w $5a(a5)
            next = 0x1c77aU;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1c77aU: { // 6720 beq.b $1c79c
            next = 0x1c77cU;
            if ((r.status & 4U) != 0U) { next = 0x1c79cU; transfer_kind = 1U; }
            break;
        }
        case 0x1c77cU: { // 536d005a subq.w #$1, $5a(a5)
            next = 0x1c780U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c780U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c786U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c786U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c78cU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c78cU: { // 422d003f clr.b $3f(a5)
            next = 0x1c790U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c790U: { // 422d003e clr.b $3e(a5)
            next = 0x1c794U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c794U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1c79aU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c79cU: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c7a2U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c7a2U: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1c7a8U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c7a8U: { // 08ed00030041 bset.b #$3, $41(a5)
            next = 0x1c7aeU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c7aeU: { // 1b7c00000058 move.b #$0, $58(a5)
            next = 0x1c7b4U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x58U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c7b4U: { // 08ad00070041 bclr.b #$7, $41(a5)
            next = 0x1c7baU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c7beU: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c7c4U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c7c4U: { // 422d003f clr.b $3f(a5)
            next = 0x1c7c8U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c7c8U: { // 422d003e clr.b $3e(a5)
            next = 0x1c7ccU;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c7ccU: { // 206d006e movea.l $6e(a5), a0
            next = 0x1c7d0U;
            const auto source_address = r.address[5] + 0x6eU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1c7d0U: { // 7000 moveq #$0, d0
            next = 0x1c7d2U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1c7d2U: { // 10280006 move.b $6(a0), d0
            next = 0x1c7d6U;
            const auto source_address = r.address[0] + 0x6U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c7d6U: { // 3b40005a move.w d0, $5a(a5)
            next = 0x1c7daU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c7dcU: { // 4a2d003f tst.b $3f(a5)
            next = 0x1c7e0U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c7e0U: { // 6704 beq.b $1c7e6
            next = 0x1c7e2U;
            if ((r.status & 4U) != 0U) { next = 0x1c7e6U; transfer_kind = 1U; }
            break;
        }
        case 0x1c7e2U: { // 600011f6 bra.w $1d9da
            next = 0x1c7e6U;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1c7e6U: { // 0c2d00100058 cmpi.b #$10, $58(a5)
            next = 0x1c7ecU;
            const auto source_value = 0x10U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1c7ecU: { // 6c16 bge.b $1c804
            next = 0x1c7eeU;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1c804U; transfer_kind = 1U; }
            break;
        }
        case 0x1c7eeU: { // 422d003e clr.b $3e(a5)
            next = 0x1c7f2U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c7f2U: { // 522d0058 addq.b #$1, $58(a5)
            next = 0x1c7f6U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1c7f6U: { // 3b7c00000074 move.w #$0, $74(a5)
            next = 0x1c7fcU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
