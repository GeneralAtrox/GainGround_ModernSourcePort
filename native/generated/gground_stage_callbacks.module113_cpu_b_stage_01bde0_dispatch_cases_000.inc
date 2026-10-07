
static bool cpu_b_stage_01bde0_dispatch_cases_000([[maybe_unused]] unverified::Machine &m,
                               [[maybe_unused]] FunctionContext &c,
                               [[maybe_unused]] std::uint32_t pc,
                               [[maybe_unused]] std::uint32_t &next,
                               [[maybe_unused]] std::uint8_t &transfer_kind) noexcept {
    [[maybe_unused]] auto &r = c.registers;
    switch (pc) {
        case 0x1bde0U: { // 082d00030041 btst.b #$3, $41(a5)
            next = 0x1bde6U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1bde6U: { // 662e bne.b $1be16
            next = 0x1bde8U;
            if ((r.status & 4U) == 0U) { next = 0x1be16U; transfer_kind = 1U; }
            break;
        }
        case 0x1bde8U: { // 1b7c0008000b move.b #$8, $b(a5)
            next = 0x1bdeeU;
            const auto value = 0x8U;
            const auto destination_address = r.address[5] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1bdeeU: { // 08ed00010041 bset.b #$1, $41(a5)
            next = 0x1bdf4U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1bdf4U: { // 522d0058 addq.b #$1, $58(a5)
            next = 0x1bdf8U;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            const auto value = m.add(destination_value, source_value, 8U);
            m.byte(destination_address, value);
            break;
        }
        case 0x1bdf8U: { // 0c2d007e0058 cmpi.b #$7e, $58(a5)
            next = 0x1bdfeU;
            const auto source_value = 0x7eU;
            const auto destination_address = r.address[5] + 0x58U;
            const auto destination_value = m.byte(destination_address);
            (void)m.sub(destination_value, source_value, 8U, true);
            break;
        }
        case 0x1bdfeU: { // 6702 beq.b $1be02
            next = 0x1be00U;
            if ((r.status & 4U) != 0U) { next = 0x1be02U; transfer_kind = 1U; }
            break;
        }
        case 0x1be02U: { // 08ed00030041 bset.b #$3, $41(a5)
            next = 0x1be08U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x3U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old | bit_mask);
            break;
        }
        case 0x1be08U: { // 08ad00010041 bclr.b #$1, $41(a5)
            next = 0x1be0eU;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x1U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            m.byte(destination_address, old & ~bit_mask);
            break;
        }
        case 0x1be0eU: { // 1b7c0009000b move.b #$9, $b(a5)
            next = 0x1be14U;
            const auto value = 0x9U;
            const auto destination_address = r.address[5] + 0xbU;
            m.byte(destination_address, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1be16U: { // 4a2d003f tst.b $3f(a5)
            next = 0x1be1aU;
            const auto destination_address = r.address[5] + 0x3fU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1be1aU: { // 6704 beq.b $1be20
            next = 0x1be1cU;
            if ((r.status & 4U) != 0U) { next = 0x1be20U; transfer_kind = 1U; }
            break;
        }
        case 0x1be1cU: { // 60001bbc bra.w $1d9da
            next = 0x1be20U;
            if (true) { next = 0x1d9daU; transfer_kind = 1U; }
            break;
        }
        case 0x1be20U: { // 4a2d003e tst.b $3e(a5)
            next = 0x1be24U;
            const auto destination_address = r.address[5] + 0x3eU;
            const auto value = m.byte(destination_address);
            m.logic(value, 8U);
            break;
        }
        case 0x1be24U: { // 6704 beq.b $1be2a
            next = 0x1be26U;
            if ((r.status & 4U) != 0U) { next = 0x1be2aU; transfer_kind = 1U; }
            break;
        }
        case 0x1be26U: { // 60001bc2 bra.w $1d9ea
            next = 0x1be2aU;
            if (true) { next = 0x1d9eaU; transfer_kind = 1U; }
            break;
        }
        case 0x1be2aU: { // 526d0054 addq.w #$1, $54(a5)
            next = 0x1be2eU;
            const auto source_value = 0x1U;
            const auto destination_address = r.address[5] + 0x54U;
            const auto destination_value = m.word(destination_address);
            const auto value = m.add(destination_value, source_value, 16U);
            m.word(destination_address, value);
            break;
        }
        case 0x1be2eU: { // 082d00070040 btst.b #$7, $40(a5)
            next = 0x1be34U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x7U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1be34U: { // 6652 bne.b $1be88
            next = 0x1be36U;
            if ((r.status & 4U) == 0U) { next = 0x1be88U; transfer_kind = 1U; }
            break;
        }
        case 0x1be36U: { // 206d006e movea.l $6e(a5), a0
            next = 0x1be3aU;
            const auto source_address = r.address[5] + 0x6eU;
            const auto value = m.lng(source_address);
            r.address[0] = value;
            break;
        }
        case 0x1be3aU: { // 7000 moveq #$0, d0
            next = 0x1be3cU;
            r.data[0] = 0x0U;
            m.logic(r.data[0], 32U);
            break;
        }
        case 0x1be3cU: { // 10280002 move.b $2(a0), d0
            next = 0x1be40U;
            const auto source_address = r.address[0] + 0x2U;
            const auto value = m.byte(source_address);
            m.db(0U, value);
            m.logic(value, 8U);
            break;
        }
        case 0x1be40U: { // e948 lsl.w #$4, d0
            next = 0x1be42U;
            m.shift_word(0U, 4U, true, false);
            break;
        }
        case 0x1be42U: { // b06d0054 cmp.w $54(a5), d0
            next = 0x1be46U;
            const auto source_address = r.address[5] + 0x54U;
            const auto source_value = m.word(source_address);
            const auto destination_value = r.data[0];
            (void)m.sub(destination_value, source_value, 16U, true);
            break;
        }
        case 0x1be46U: { // 6f68 ble.b $1beb0
            next = 0x1be48U;
            if ((r.status & 4U) != 0U || (((r.status >> 3U) ^ (r.status >> 1U)) & 1U) != 0U) { next = 0x1beb0U; transfer_kind = 1U; }
            break;
        }
        case 0x1be4cU: { // 082d00050041 btst.b #$5, $41(a5)
            next = 0x1be52U;
            const auto destination_address = r.address[5] + 0x41U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1be52U: { // 662e bne.b $1be82
            next = 0x1be54U;
            if ((r.status & 4U) == 0U) { next = 0x1be82U; transfer_kind = 1U; }
            break;
        }
        case 0x1be54U: { // 302d0054 move.w $54(a5), d0
            next = 0x1be58U;
            const auto source_address = r.address[5] + 0x54U;
            const auto value = m.word(source_address);
            m.dw(0U, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1be58U: { // 0240001f andi.w #$1f, d0
            next = 0x1be5cU;
            const auto source_value = 0x1fU;
            const auto destination_value = r.data[0];
            const auto value = destination_value & source_value;
            m.logic(value, 16U);
            m.dw(0U, value);
            break;
        }
        case 0x1be5cU: { // 660c bne.b $1be6a
            next = 0x1be5eU;
            if ((r.status & 4U) == 0U) { next = 0x1be6aU; transfer_kind = 1U; }
            break;
        }
        case 0x1be62U: { // 3b6d003a005c move.w $3a(a5), $5c(a5)
            next = 0x1be68U;
            const auto source_address = r.address[5] + 0x3aU;
            const auto value = m.word(source_address);
            const auto destination_address = r.address[5] + 0x5cU;
            m.word(destination_address, value);
            m.logic(value, 16U);
            break;
        }
        case 0x1be68U: { // 6018 bra.b $1be82
            next = 0x1be6aU;
            if (true) { next = 0x1be82U; transfer_kind = 1U; }
            break;
        }
        case 0x1be6aU: { // 082d00050040 btst.b #$5, $40(a5)
            next = 0x1be70U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x5U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1be70U: { // 6630 bne.b $1bea2
            next = 0x1be72U;
            if ((r.status & 4U) == 0U) { next = 0x1bea2U; transfer_kind = 1U; }
            break;
        }
        case 0x1be72U: { // 082d00060040 btst.b #$6, $40(a5)
            next = 0x1be78U;
            const auto destination_address = r.address[5] + 0x40U;
            const auto old = m.byte(destination_address);
            const auto bit_mask = 1U << (0x6U & 7U);
            r.status = static_cast<std::uint16_t>((r.status & ~4U) | ((old & bit_mask) ? 0U : 4U));
            break;
        }
        case 0x1be78U: { // 6608 bne.b $1be82
            next = 0x1be7aU;
            if ((r.status & 4U) == 0U) { next = 0x1be82U; transfer_kind = 1U; }
            break;
        }
    default: return false;
    }
    return true;
}
