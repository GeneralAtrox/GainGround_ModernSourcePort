
static bool cpu_b_stage_01c8e8_dispatch_cases_001([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1c97cU: { // 3b7c00000074 move.w #$0, $74(a5)
            next = 0x1c982U;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c982U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c988U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c98aU: { // 4a2d003f tst.b $3f(a5)
            next = 0x1c98eU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c98eU: { // 6704 beq.b $1c994
            next = 0x1c990U;
            if ((r.status & 4U) != 0U) { next = 0x1c994U; transfer_kind = 1U; }
            break;
        }
        case 0x1c990U: { // 60001048 bra.w $1d9da
            next = 0x1c994U;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1c994U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1c998U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1c998U: { // 6704 beq.b $1c99e
            next = 0x1c99aU;
            if ((r.status & 4U) != 0U) { next = 0x1c99eU; transfer_kind = 1U; }
            break;
        }
        case 0x1c99aU: { // 6000104e bra.w $1d9ea
            next = 0x1c99eU;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1c99eU: { // 4a6d005a tst.w $5a(a5)
            next = 0x1c9a2U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto value = m.word(destination_address);
            m.logic(value, 16U);
            break;
        }
        case 0x1c9a2U: { // 6706 beq.b $1c9aa
            next = 0x1c9a4U;
            if ((r.status & 4U) != 0U) { next = 0x1c9aaU; transfer_kind = 1U; }
            break;
        }
        case 0x1c9a4U: { // 536d005a subq.w #$1, $5a(a5)
            next = 0x1c9a8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x5aU;
            const auto destination_value = m.word(destination_address);
            const auto value = m.sub(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1c9aaU: { // 082d00010041 btst.b #$1, $41(a5)
            next = 0x1c9b0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1c9b0U: { // 6702 beq.b $1c9b4
            next = 0x1c9b2U;
            if ((r.status & 4U) != 0U) { next = 0x1c9b4U; transfer_kind = 1U; }
            break;
        }
        case 0x1c9b4U: { // 08ed00050041 bset.b #$5, $41(a5)
            next = 0x1c9baU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c9bcU: { // 0c2d00400058 cmpi.b #$40, $58(a5)
            next = 0x1c9c2U;
            const auto source_value = 0x40U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1c9c2U: { // 6c20 bge.b $1c9e4
            next = 0x1c9c4U;
            if ((((r.status >> 3U) ^ (r.status >> 1U)) & 1U) == 0U) { next = 0x1c9e4U; transfer_kind = 1U; }
            break;
        }
        case 0x1c9c4U: { // 522d0058 addq.b #$1, $58(a5)
            next = 0x1c9c8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1c9c8U: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1c9ceU;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c9ceU: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1c9d4U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c9d4U: { // 422d003f clr.b $3f(a5)
            next = 0x1c9d8U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c9d8U: { // 3b6d00760046 move.w $76(a5), $46(a5)
            next = 0x1c9deU;
            const auto source_address = r.address[5] + 0x76U;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x46U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1c9deU: { // 422d003e clr.b $3e(a5)
            next = 0x1c9e2U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1c9e4U: { // 08ad00030041 bclr.b #$3, $41(a5)
            next = 0x1c9eaU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c9eaU: { // 08ad00050041 bclr.b #$5, $41(a5)
            next = 0x1c9f0U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1c9f0U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1c9f6U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1c9f6U: { // 1b7c00000060 move.b #$0, $60(a5)
            next = 0x1c9fcU;
            const auto value = 0x0U;
            const auto destination_address = r.address[5] + 0x60U;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1c9fcU: { // 3b7c00020074 move.w #$2, $74(a5)
            next = 0x1ca02U;
            const auto value = 0x2U;
            const auto destination_address = r.address[5] + 0x74U;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1ca02U: { // 422d003f clr.b $3f(a5)
            next = 0x1ca06U;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            (void)value;
            m.byte(destination_address, 0U);
            m.logic(0U, 8U);
            break;
        }
        case 0x1ca06U: { // 08ed00070041 bset.b #$7, $41(a5)
            next = 0x1ca0cU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1ca0cU: { // 206d006e movea.l $6e(a5), a0
            next = 0x1ca10U;
            const auto source_address = r.address[5] + 0x6eU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1ca10U: { // 7000 moveq #$0, d0
            next = 0x1ca12U;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1ca12U: { // 10280007 move.b $7(a0), d0
            next = 0x1ca16U;
            const auto source_address = r.address[0] + 0x7U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1ca16U: { // 3b40005a move.w d0, $5a(a5)
            next = 0x1ca1aU;
            const auto value = r.data[0];
            const auto destination_address = r.address[5] + 0x5aU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
    default: return false;
    }
    return true;
}
